#include "Recorder.h"
using namespace std;
using namespace boost::program_options;
using namespace boost::process;
using namespace boost::asio;

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

awaitable<void> async_record(tcp::socket sock) {
    cout << "async_record" << endl;

    for (;;) {
        char b[1024];
        size_t bytes = co_await sock.async_read_some(buffer(b), use_awaitable);
        cout << "async_record: async_read_some: " << bytes << endl;
    }

    co_return;
}

awaitable<void> async_listen() {
    const auto& exec = co_await this_coro::executor;
    tcp::acceptor acc{ exec, { ip::address::from_string("::1"), 9090 } };
    acc.set_option(socket_base::reuse_address(true));

    for (;;) {
        tcp::socket sock = co_await acc.async_accept(use_awaitable);
        co_spawn(exec, [sock = std::move(sock)]() mutable {
                return async_record(std::move(sock));
            }, detached);
    }

    co_return;
}

// Consider using normal function prototype and boost::nowide.
int wmain(int argc, wchar_t* argv[]) {
    bool do_debug = false, do_cbt = false, do_wndproc = false,
        do_mouse = false, do_keyboard = false;

    options_description desc("Options");
    desc.add_options()
        ("help,h",
            "Help message")
        ("threads,t",
            value<vector<int>>()
            ->multitoken()
            ->default_value(vector<int>{0}, "0"),
            "Process/thread IDs to watch (0 is all)")
        ("debug,d", bool_switch(&do_debug),
            "All hookable events via debug hook")
        ("cbt,c", bool_switch(&do_debug),
            "Window manager message events")
        ("wndproc,w", bool_switch(&do_wndproc),
            "Application message events")
        ("mouse,m", bool_switch(&do_mouse),
            "Application level mouse events")
        ("keyboard,k", bool_switch(&do_keyboard),
            "Application level keyboard events");

    // Drops argv[0] if there is more than one argument, also
    // does not do globbing.
    //int argc;
    //wchar_t **argv = CommandLineToArgvW(lpCmdLine, &argc);

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm); // Calls notification functions *and* sets variables.

    if (vm.count("help")) {
        std::cout << desc << endl;
        exit(0);
    }

    const auto& tids = vm["threads"].as<vector<int>>();
    vector<int> events;
    if (do_debug) events.push_back(WH_DEBUG);
    if (do_cbt) events.push_back(WH_CBT);
    if (do_wndproc) events.push_back(WH_CALLWNDPROC);
    if (do_mouse) events.push_back(WH_MOUSE);
    if (do_keyboard) events.push_back(WH_KEYBOARD);

    if (!events.size()) {
        std::cout << "No events to hook" << endl;
        exit(-1);
    }

    boost::asio::io_context ctx;
    boost::asio::signal_set sigs(ctx, SIGINT, SIGTERM);
    sigs.async_wait([&](auto, auto) {
        ctx.stop();
    });
    co_spawn(ctx, async_listen, detached);
    ctx.run();

    return 0;
}
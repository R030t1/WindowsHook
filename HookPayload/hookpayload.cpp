﻿// HookPayload.cpp : Defines the entry point for the application.
//

#include "HookPayload.h"
using namespace std;
using namespace boost::asio;

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

bool is_setup = false;

boost::thread relay;
boost::condition_variable cv;
boost::lockfree::queue<CWPSTRUCT, boost::lockfree::capacity<50>> q;

boost::thread runner;
boost::mutex mx;

boost::asio::io_context ctx;
tcp::socket serv(ctx);

awaitable<void> async_connect_to_recorder() {
	co_await serv.async_connect({ ip::address::from_string("::1"), 9090 }, use_awaitable);
	//mx.unlock(); // Could remove with better use of co_await?
	// No, just spawn after connected. It's not appropriate to hold up the hook processing
	// until we connect.
	co_return;
}

__declspec(dllexport) BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH: {
		// std::mutex fails because of a deadlock. CreateMutexEx[W] and boost::mutex
		// do not have this issue.
		//mx.lock();

		// std::thread fails because of a deadlock. CreateThread and boost::thread
		// do not have this issue. These issues may be fixed on GCC and clang.
		// Destructor terminates thread, perhaps ensure static storage for thread.

		runner = boost::thread([]() {
			co_spawn(ctx, async_connect_to_recorder, detached);
			ctx.run();
		});

		relay = boost::thread([] {
			while (true) {
				boost::unique_lock<boost::mutex> lock(mx);
				cv.wait(lock);

				CWPSTRUCT cwps;
				assert(q.pop(cwps));

				lock.unlock();
			}
		});
		
		break;
	}
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		// Uncaught exception terminates thread due to interruptible call to
		// condition_variable.wait.
		relay.interrupt();
		break;
	}

	return TRUE;
}

LRESULT DebugProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT CBTProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT CallWndProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (!is_setup) {
		int pid = GetCurrentProcessId();
		wchar_t fname[256];
		wsprintfW(fname, L"C:\\tmp\\%d", pid);
		HANDLE file = CreateFileW(fname,
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
		);
		CloseHandle(file);
		is_setup = true;
	}

	boost::unique_lock<boost::mutex> lock(mx);
	
	// TODO: Should be able to call async send from here.
	q.push(CWPSTRUCT{ *(CWPSTRUCT*)lParam });
	std::vector<CWPSTRUCT> ev{ *(CWPSTRUCT*)lParam };
	//char b[256];
	if (serv.is_open())
		serv.async_send(buffer(ev), [](auto, auto) {});
	//serv.async_send(buffer(ev), [](auto, auto) {});

	lock.unlock();
	cv.notify_one();

	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT MouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT KeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}
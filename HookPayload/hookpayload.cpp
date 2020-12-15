// HookPayload.cpp : Defines the entry point for the application.
//

#include "HookPayload.h"
using namespace std;

bool IsSetup = true;

boost::thread relay;
boost::mutex mx;
boost::condition_variable cv;
boost::lockfree::queue<CWPSTRUCT, boost::lockfree::capacity<50>> q;

void connect() {

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
		mx.lock();

		// std::thread fails because of a deadlock. CreateThread and boost::thread
		// do not have this issue. These issues may be fixed on GCC and clang.
		boost::thread([] {
			IsSetup = false;
			mx.unlock();
		});
		// Destructor terminates thread, perhaps ensure static storage for thread.

		relay = boost::thread([] {
			while (true) {
				boost::unique_lock<boost::mutex> lock(mx);
				cv.wait(lock);

				CWPSTRUCT cwps;
				q.pop(cwps);

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
	if (!IsSetup) {
		int pid = GetCurrentProcessId();
		wchar_t fname[256];
		wsprintfW(fname, L"C:\\tmp\\%d", pid);
		HANDLE file = CreateFileW(fname,
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
		);
		CloseHandle(file);
		IsSetup = true;
	}

	boost::unique_lock<boost::mutex> lock(mx);
	
	q.push(CWPSTRUCT{ *(CWPSTRUCT*)lParam });

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
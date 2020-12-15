// HookPayload.cpp : Defines the entry point for the application.
//

#include "HookPayload.h"
using namespace std;

bool IsSetup = true;

boost::thread relay;
boost::mutex mx;
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
		// do not have this issue.
		boost::thread([] {
			IsSetup = false;
			mx.unlock();
		});

		// Amusing, but do something else
		relay = boost::thread([] {
			while (q.consume_one([] (const CWPSTRUCT cwps) {

			}));
		});
		
		break;
	}
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
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

	q.push(CWPSTRUCT{*(CWPSTRUCT *)lParam});

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
// HookPayload.cpp : Defines the entry point for the application.
//

#include "HookPayload.h"
using namespace std;

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

LRESULT CALLBACK DebugProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT CALLBACK CBTProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

bool IsSetup = false;
LRESULT CALLBACK CallWndProc(
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

	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT CALLBACK MouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}

LRESULT CALLBACK KeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return 0;
}
#include "HookLoader.h"
using namespace std;

HINSTANCE hInstance;
wchar_t szTitle[] = L"hookloader";
wchar_t szMenuName[] = L"hookloader";
wchar_t szWindowClass[] = L"hookloader";

#if defined(_WIN64)
const wchar_t* libname = L"libhookpayload64.dll";
#elif defined(_WIN32)
const wchar_t* libname = L"libhookpayload32.dll";
#endif

HHOOK HookWndProc = NULL;

int set_hooks(HWND hWnd) {
	HMODULE payload = LoadLibraryW(libname);
	if (!payload) {
		cout << "Failed to load library" << endl;
		return 1;
	}

	HOOKPROC CallWndProc = (HOOKPROC)GetProcAddress(payload, "CallWndProc");
	if (!CallWndProc) {
		cout << "Failed to load hookproc" << endl;
		return 1;
	}

	HookWndProc = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, payload, 0);
	if (!HookWndProc) {
		cout << "Failed to set hook" << endl;
		return 1;
	}

	return 0;
}

int APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEXW wcex = {
		.cbSize = sizeof(WNDCLASSEXW),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = LoadIconW(hInstance, IDI_APPLICATION),
		.hCursor = LoadCursorW(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = szMenuName,
		.lpszClassName = szWindowClass,
		.hIconSm = LoadIconW(hInstance, IDI_APPLICATION)
	};
	if (!RegisterClassExW(&wcex)) {
		// TODO.
	}

	HWND hWnd = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);
	if (!hWnd) {
		// TODO.
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (message) {
	case WM_CREATE:
		set_hooks(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}
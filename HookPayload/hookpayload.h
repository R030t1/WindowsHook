// HookPayload.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#define _UNICODE
#define UNICODE

#include <iostream>

#include <Windows.h>

extern "C" {
	__declspec(dllexport) BOOL WINAPI DllMain(
		_In_ HINSTANCE hinstDLL,
		_In_ DWORD     fdwReason,
		_In_ LPVOID    lpvReserved
	);

	__declspec(dllexport) LRESULT DebugProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	__declspec(dllexport) LRESULT CBTProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	__declspec(dllexport) LRESULT CallWndProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	__declspec(dllexport) LRESULT MouseProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	__declspec(dllexport) LRESULT KeyboardProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);
}
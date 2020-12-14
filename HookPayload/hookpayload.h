// HookPayload.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#define _UNICODE
#include <Windows.h>

#include <iostream>

extern "C" {
	BOOL WINAPI DllMain(
		_In_ HINSTANCE hinstDLL,
		_In_ DWORD     fdwReason,
		_In_ LPVOID    lpvReserved
	);

	LRESULT CALLBACK DebugProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	LRESULT CALLBACK CBTProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	LRESULT CALLBACK CallWndProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	LRESULT CALLBACK MouseProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	LRESULT CALLBACK KeyboardProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);
}
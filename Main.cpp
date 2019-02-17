#include <Windows.h>
#include "CheatManager.h"
#include "Globals.h"
#include "System.h"
#include "cexception.h"
#include "StealthOverlay.h"
#include "XorString.hpp"
#include "Menu.h"
#include "EntityUpdator.h"
#include <dwmapi.h>

#pragma comment(lib, "Cheat-Template.lib")

void RunCheat();

#ifdef _MakeAsDLL

DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
) {
	RunCheat();
	TerminateProcess(GetCurrentProcess(), 0);
	return 0;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		auto result = CreateThread(
			NULL,
			0,
			ThreadProc,
			(LPVOID)hinstDLL,
			0,
			NULL
		);
		if (result == NULL) {
			MessageBoxW(NULL, L"Can not Create thread", L"ERROR", 0);
			return false;
		}
	}
	return TRUE;
}

#else
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	RunCheat();
	return 0;
}
#endif

HWND MakeOverlay()
{
	WNDCLASSEXW window_class;
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.style = 0;
	window_class.lpfnWndProc = DefWindowProcW;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandleW(NULL);
	window_class.hIcon = NULL;
	window_class.hCursor = NULL;
	window_class.hbrBackground = NULL; // for testing
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = L"3A09FB91-B7FE-4cf5-91F1-20D3832C6EB6";
	window_class.hIconSm = NULL;

	RegisterClassExW(&window_class);

	HWND hWnd = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE,
		L"3A09FB91-B7FE-4cf5-91F1-20D3832C6EB6",
		NULL,
		WS_POPUP | WS_VISIBLE, 
		Rust::Globals::system_data.GameWindowRect.left,
		Rust::Globals::system_data.GameWindowRect.top,
		Rust::Globals::system_data.width,
		Rust::Globals::system_data.height,
		NULL,
		NULL,
		(HINSTANCE)GetModuleHandleW(NULL),
		NULL
	);

	return hWnd;
}

void RunCheat() {
	try {
		AllocConsole();
		Rust::Globals::Init();

		Rust::Globals::system_data.hOverlay = MakeOverlay();
		MARGINS margin = { 0,0,0,1440 };
		DwmExtendFrameIntoClientArea(Rust::Globals::system_data.hOverlay, &margin);
		SetLayeredWindowAttributes(Rust::Globals::system_data.hOverlay, 0, 0, 0);
		ShowWindow(Rust::Globals::system_data.hOverlay, SW_SHOW);

		Rust::CheatManager manager;

		bool CreateThread = true;

		while (Rust::Globals::CheatRunning) {
			manager.exec();

			MSG msg;
			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					Rust::Globals::CheatRunning = false;

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			if (CreateThread)
			{
				std::thread TaggedObjectAdder(Rust::EntityUpdator::AddNewTaggedObjects);
				std::thread ActiveObjectAdder(Rust::EntityUpdator::AddNewActiveObjects);
				std::thread UpdateThread(Rust::EntityUpdator::UpdateThread);

				TaggedObjectAdder.detach();
				ActiveObjectAdder.detach();
				UpdateThread.detach();

				CreateThread = false;
			}
		}
	}
	catch (std::exception& ex) {
		MessageBoxA(NULL, ex.what(), "Exception", 0);
		return;
	}
}

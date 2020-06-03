#include <Windows.h>
#include <cstdlib>
#include <memory>

#include "game.hpp"


std::unique_ptr<game> G = std::make_unique <game> ();


LRESULT CALLBACK WindowProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		break;

	case WM_QUIT:
		OutputDebugString (L"WM_QUIT\n");

		break;

	case WM_DESTROY:
		PostQuitMessage (0);

		break;

	case WM_CLOSE:
		PostQuitMessage (0);

		break;

	case WM_KEYDOWN:
		G->process_keyboard_input (wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MBUTTONDOWN:
		break;

	case WM_RBUTTONDOWN:

		break;

	case WM_MOUSEMOVE:
		break;

	default:
		break;
	}

	return DefWindowProc (hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE previous_instance, _In_ PWSTR cmd_line, _In_ int cmd_show)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Asteroids";
	wc.hCursor = LoadCursor (hInstance, IDC_ARROW);

	if (!RegisterClass (&wc))
	{
		return EXIT_FAILURE;
	}

	HWND hWnd = CreateWindow (L"Asteroids", L"Asteroids", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return EXIT_FAILURE;
	}

	ShowWindow (hWnd, cmd_show);
	UpdateWindow (hWnd);

<<<<<<< HEAD:Asteroids/main.cpp
<<<<<<< HEAD:Asteroids/Main.cpp
	G = std::make_unique<Game> ();
	G->Start ();
=======
	G->init (hInstance, hWnd);
>>>>>>> 8c62b191fdd96fd9542e8d31773cf2022f7c854b:Asteroids/main.cpp
=======
	G->init ();
>>>>>>> parent of 8c62b19... lower cased files.:Asteroids/Main.cpp

	MSG msg;
	ZeroMemory (&msg, sizeof (msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			G->main_loop ();

			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	G->shutdown ();
	DestroyWindow (hWnd);
    return EXIT_SUCCESS;
}
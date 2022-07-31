#include "pch.h"
#include "windowsWindow.h"

namespace tze {
	unsigned int windowsWindow::width;
	unsigned int windowsWindow::height;
	HWND windowsWindow::hwnd;


	LRESULT CALLBACK windowsWindow::setupMsgHandling(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
	{
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(l_param);
			windowsWindow* const pWnd = static_cast<windowsWindow*>(pCreate->lpCreateParams);

			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&handleMsgsThunk));

			// forward message to window instance handler
			return pWnd->hadnelMsgs(handle, msg, w_param, l_param);
		}
		return DefWindowProc(handle, msg, w_param, l_param);
	}
	LRESULT CALLBACK windowsWindow::handleMsgsThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window instance
		windowsWindow* pWnd = reinterpret_cast<windowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// forward message to window instance handler
		return pWnd->hadnelMsgs(hWnd, msg, wParam, lParam);
	}
	LRESULT windowsWindow::hadnelMsgs(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) noexcept
	{
		switch (msg)
			{
				// check if the window needs to be closed:
				case WM_CLOSE:
				{
					PostQuitMessage(0);
					return 0;
				}

				// keyboard handling:
				
				// clear state when window loses focus
				case WM_KILLFOCUS:
				{
					keyboard::clearState();
					break;
				}

				case WM_KEYDOWN:
				// syskey needs to be abel to track alt key because WM_KEYDOWN doesn't track it
				case WM_SYSKEYDOWN:
				{
					// to check if there is autorepeat:
					if (!(l_param &0x40000000) || keyboard::autorepeatIsEnabled())
						keyboard::onKeyPressed(static_cast<unsigned char>(w_param));
					break;
				}
				case WM_KEYUP:
				case WM_SYSKEYUP:
				{
					keyboard::onKeyReleased(static_cast<unsigned char>(w_param));
					break;
				}
				case WM_CHAR:
				{
					keyboard::onChar(static_cast<unsigned char>(w_param));
					break;
				}

				// mouse handling:
				// when the mouse is moveving:
				case WM_MOUSEMOVE:
				{
					const POINTS point = MAKEPOINTS(l_param);

					// if the client is in the window range
					if ((point.x > -1 && point.x < windowsWindow::width) && (point.y > -1 && point.y < windowsWindow::height))
					{
						mouse::onMouseMove(point.x, point.y);
					
						if (!mouse::isInWindow())
						{
							SetCapture(windowsWindow::hwnd);
							mouse::onMouseEnter;
						}
					}

					// if the client is not in the window range but still pressed to be abel to drag things from outside
					else
					{
						if (w_param&(MK_LBUTTON | MK_RBUTTON))
						{	
							mouse::onMouseMove(point.x, point.y);
						}
						else
						{
							ReleaseCapture();
							mouse::onMouseLeave;
						}
					}
					mouse::onMouseMove(point.x, point.y);
					break;
				}

				// when the left click pressed:
				case WM_LBUTTONDOWN:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onLeftPressed(point.x, point.y);
					break;
				}
				case WM_LBUTTONUP:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onLeftReleased(point.x, point.y);
					break;
				}

				// when the midlle click pressed:
				case WM_MBUTTONDOWN:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onMiddlePressed(point.x, point.y);
					break;
				}
				case WM_MBUTTONUP:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onMiddleRelease(point.x, point.y);
					break;
				}

				// when right click pressed:
				case WM_RBUTTONDOWN:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onRightPressed(point.x, point.y);
					break;
				}
				case WM_RBUTTONUP:
				{
					const POINTS point = MAKEPOINTS(l_param);
					mouse::onRightReleased(point.x, point.y);
					break;
				}

				// when the mouse wheel is moved:
				case WM_MOUSEWHEEL:
				{
					const POINTS point = MAKEPOINTS(l_param);
					int dealta = GET_WHEEL_DELTA_WPARAM(w_param);
					mouse::onWheelDelta(point.x, point.y, dealta);
					break;
				}
			}
		return DefWindowProc(handle, msg, w_param, l_param);

	}
	
	void windowsWindow::initWindow(const unsigned int width, const unsigned int height)
	{
		windowsWindow::width = width;
		windowsWindow::height = height;

		WNDCLASSEX wc;
		HICON hIcon = NULL;
		// HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
		//wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
		//HICON hIcon = static_cast<HICON>(LoadIcon(GetModuleHandle(NULL), (LPSTR)((ULONG_PTR)((WORD)(101))));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = setupMsgHandling;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = hIcon;
		wc.hIconSm = hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_CROSS);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"TRAZEL_ENGINE";
		RegisterClassEx(&wc);

		hwnd = CreateWindowEx(
			NULL,
			L"TRAZEL_ENGINE",
			L"TRAZEL ENGINE",
			/*WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,*/
			WS_OVERLAPPEDWINDOW,
			200, 
			200,
			width, 
			height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL
		);


		// check if the window is created or not
		if (!hwnd)
			throw("Failed to create window with error: " + GetLastError());

		ShowWindow(hwnd, SW_NORMAL);
		UpdateWindow(hwnd);

		// creating graphics object:
		gfx = std::make_unique<graphics>(hwnd);

		// ImGui_ImplWin32_Init(hwnd)
	}
	int windowsWindow::renderingLoop()
	{
		int somethingPressed = 0;
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (true) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					return msg.wParam;
				}
			
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				render();
			}
		}
		return 0;

		// this is incase i want it to be event driven:
		/*
		BOOL gResult;
		while (GetMessage(&msg, NULL, 0, 0)) {
			if (msg.message == WM_QUIT)
				return msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			render();
		}
		return 0;
		*/

		
			/*
			if (keyboard::keyIsPressed(VK_SPACE))
			{
				MessageBox(NULL, L"something happend", L"space key is pressed", NULL);
			}
			// if(keyboard::keyIsPressed(VK_F11))
			// 	MessageBox(NULL, L"something happend", L"f11 key is pressed", NULL);
			// 	// if (!fullscreen_) {
			// 	// 	// Save current window information.  We force the window into restored mode
			// 	// 	// before going fullscreen because Windows doesn't seem to hide the
			// 	// 	// taskbar if the window is in the maximized state.
			// 	// 	saved_window_info_.maximized = !!::IsZoomed(hwnd);
			// 	// 	if (saved_window_info_.maximized)
			// 	// 		::SendMessage(hwnd_, WM_SYSCOMMAND, SC_RESTORE, 0);
			// 	// 	saved_window_info_.style = GetWindowLong(hwnd_, GWL_STYLE);
			// 	// 	saved_window_info_.ex_style = GetWindowLong(hwnd_, GWL_EXSTYLE);
			// 	// 	GetWindowRect(hwnd_, &saved_window_info_.window_rect);
			// 	// }
			if (mouse::isLeftPressed())
			{
				MessageBox(NULL, L"something happend", L"left key is pressed", MB_YESNOCANCEL);
			}
			if (mouse::isRightPressed())
			{
				MessageBox(NULL, L"something happend", L"right key is pressed", NULL);
			}
			if (mouse::isMidlletPressed())
			{
				MessageBox(NULL, L"something happend", L"scroll key is pressed", NULL);
			}
			*/
	}
	void windowsWindow::render()
	{
		gfx->clearBuffer(1,0,0);
		gfx->testDrawing();
		gfx->endFrame();
	}

	graphics& windowsWindow::getGfx()
	{
		return *gfx;
	}
}

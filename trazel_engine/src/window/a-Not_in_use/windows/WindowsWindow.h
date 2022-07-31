#pragma once
#include "../../inputs/keyboard.h"
#include "../../inputs/mouse.h"
#include "graphics/graphics.h"

namespace tze {
	class windowsWindow {
	public:
		windowsWindow(const unsigned int Width = 1280, const unsigned int Height = 720)
		{
			initWindow(Width, Height);
		}
		/*override*/ ~windowsWindow()
		{
			UnregisterClass(L"TRAZEL_ENGINE", GetModuleHandle(NULL));
		}
		
		void initWindow(const unsigned int width, const unsigned int height);
		int renderingLoop();
		void render();

		static LRESULT CALLBACK setupMsgHandling(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);
		static LRESULT CALLBACK handleMsgsThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT hadnelMsgs(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) noexcept;
		
		graphics& getGfx();
	private:
		static HWND hwnd;
		static unsigned int width;
		static unsigned int height;
		std::unique_ptr<graphics> gfx;
	};
}
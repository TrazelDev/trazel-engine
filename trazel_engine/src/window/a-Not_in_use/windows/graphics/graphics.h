#pragma once
#include "pch.h"


namespace tze { class graphics {
public:
	graphics(HWND hwnd);
	graphics(const graphics&) = delete;
	graphics& operator=(const graphics&) = delete;
	~graphics() = default;

	void endFrame();
	void clearBuffer(float red, float green, float blue);
	void testDrawing();

private:
	Microsoft::WRL::ComPtr <ID3D11Device> pDevice = NULL;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap = NULL;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext = NULL;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget = NULL;
};
}

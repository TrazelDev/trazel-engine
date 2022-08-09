#include "pch.h"
#include "graphics.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#ifndef Client_MODE
	#define CHECK_GRAPHICS_ERR(hrcall, sDesc, fDesc) if(FAILED(hrcall)) {TZE_ENGINE_ERR(fDesc);} else if(sDesc == "") {} else {TZE_ENGINE_INFO(sDesc);}
#else
	#define CHECK_GRAPHICS_ERR(hrcall, sDesc, fDesc) hrcall
#endif
namespace tze
{
	graphics::graphics(HWND hwnd)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hwnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		// creating the device and front/back buffers, the swap chains and rendering context
		CHECK_GRAPHICS_ERR(
			D3D11CreateDeviceAndSwapChain
			(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				NULL,
				NULL,
				NULL,
				D3D11_SDK_VERSION,
				&sd,
				&pSwap,
				&pDevice,
				NULL,
				&pContext
			),
			"device was created secsufully",
			"device failed to be created"
		);

		// getting the texture to the subsource in the swap chain (backbuffer)
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackbuffer = NULL;

		CHECK_GRAPHICS_ERR(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackbuffer), "", "error swaping to back buffer");
		CHECK_GRAPHICS_ERR(pDevice->CreateRenderTargetView
		(
			pBackbuffer.Get(),
			NULL,
			&pTarget
		),
			"",
			"error creating render target view"
		);
	}

	void graphics::endFrame()
	{
		HRESULT hr;
		if (FAILED(hr = pSwap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				TZE_ENGINE_ERR(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				CHECK_GRAPHICS_ERR(hr, "something wrong", "error ending the frame");
			}
		}
	}

	void graphics::clearBuffer(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}

	void graphics::testDrawing()
	{
		struct vertex
		{
			float x;
			float y;
		};

		const vertex vertices[] =
		{
			{0.0f, 0.5f},
			{0.5f, -0.5f},
			{-0.5f, -0.5f}
		};

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(vertex);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		CHECK_GRAPHICS_ERR(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer), "", "buffer failed to be created");

		UINT stride = sizeof(vertex);
		UINT offset = 0u;
		pContext->IAGetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// creating pixel shader
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		D3DReadFileToBlob(L"C:/Dev_programmer/trazel_engine/bin/Debug-windows-x86_64/trazel_engine/PixelShader.cso", &pBlob);  // later change that
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader);
		pContext->PSSetShader(pPixelShader.Get(), NULL, 0u); // binding the pixel shader 

		// creating the vertex shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		D3DReadFileToBlob(L"C:/Dev_programmer/trazel_engine/bin/Debug-windows-x86_64/trazel_engine/VertexShader.cso", &pBlob);  // later change that
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader);
		pContext->VSSetShader(pVertexShader.Get(), NULL, 0u); // binding the vertex shader 


		//input (vertex) layout (2s position only)
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] = { {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} };
		pDevice->CreateInputLayout
		(
			ied,
			(UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		);
		pContext->IASetInputLayout(pInputLayout.Get()); // bind the vertex layout


		// bind render target
		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), NULL);

		// set the primitive topology to a triangle list
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 1280;
		vp.Height = 720;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports(1u, &vp);

		pContext->Draw((UINT)std::size(vertices), 0u);
		pSwap->Present(1, 0);
		// C:/Dev_programmer/trazel_engine/bin/Debug-windows-x86_64/trazel_engine
		// C:/Dev_programmer/trazel_engine/trazel_engine/srcw/indow/windows/graphics
		// ../../../../../bin/Debug-windows-x86_64/trazel_engine/VertexShader.cso
	}
}
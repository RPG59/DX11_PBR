#pragma once
#include <d3d11_4.h>
#include <wrl/client.h>
#include <iostream>

using Microsoft::WRL::ComPtr;

struct WindowProperties {
	uint32_t width, height;
	bool fullscreen;
	bool vsync;
};


class D3DContext
{
private:
	IDXGISwapChain4* m_SwapChain;
	ID3D11Device5* m_Device;
	ID3D11DeviceContext4* m_DevContext;
	ComPtr<ID3D11RenderTargetView1> m_BackBuffer;
	ID3D11Texture2D1* m_DepthStencilBuffer;
	ID3D11DepthStencilView* m_DepthStencilView;

public:
	static D3DContext* currentContext;

public:
	D3DContext(void* deviceContext, WindowProperties);
	~D3DContext();
	void Init(HWND, WindowProperties);

	inline static ID3D11Device5* GetDevice() { return D3DContext::currentContext->m_Device; }
	inline static IDXGISwapChain4* GetSwapChain() { return D3DContext::currentContext->m_SwapChain; }
	inline static ID3D11RenderTargetView1* GetBackBuffer() { return D3DContext::currentContext->m_BackBuffer.Get(); }
	inline static ID3D11DeviceContext4* GetDeviceContext() { return D3DContext::currentContext->m_DevContext; }
};


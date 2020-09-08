#include "D3DContext.h"
#include <dxgi1_6.h>
#include <vector>

D3DContext* D3DContext::currentContext = nullptr;

D3DContext::D3DContext(void* deviceContext, WindowProperties windowProperties) {
	Init((HWND)deviceContext, windowProperties);
}

D3DContext::~D3DContext() {

}

void D3DContext::Init(HWND hWnd, WindowProperties windowPropertise) {
	DXGI_SWAP_CHAIN_DESC scd = {};

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = windowPropertise.width;                   // set the back buffer width
	scd.BufferDesc.Height = windowPropertise.height;                 // set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 1;                              // how many multisamples
	scd.SampleDesc.Quality = 0;
	scd.Windowed = !windowPropertise.fullscreen;           // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	ComPtr<ID3D11Device> tempDevice{};
	ComPtr<ID3D11DeviceContext> tempContext{};
	ComPtr<IDXGISwapChain> tempSwapChain{};
#ifdef USE_FACTORY2
	IDXGIFactory7* pFactory = nullptr;
	auto hr_factory = CreateDXGIFactory2(NULL, __uuidof(IDXGIFactory7), (void**)&pFactory);
#else
	IDXGIFactory1* pFactory = nullptr;
	auto hr_factory = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory);
#endif
	std::vector<DXGI_ADAPTER_DESC2> dapters;
	std::vector<IDXGIAdapter3*> adapters;
	IDXGIAdapter* pAdapter = nullptr;

	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		IDXGIAdapter3* pAdapter3 = nullptr;
		pAdapter->QueryInterface(__uuidof(IDXGIAdapter3), (void**)&pAdapter3);

		adapters.push_back(pAdapter3);
		DXGI_ADAPTER_DESC2 desc{};
		pAdapter3->GetDesc2(&desc);
		dapters.push_back(desc);
	}

	auto hr_createdevice = D3D11CreateDevice(
		adapters[0],
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&tempDevice,
		NULL,
		&tempContext);

	UINT levels;

	tempDevice.Get()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 8, &levels);

	auto hr_createSwapChain = pFactory->CreateSwapChain(tempDevice.Get(), &scd, &tempSwapChain);


	auto hr_dxdevice5 = tempDevice->QueryInterface(__uuidof(ID3D11Device5), (void**)&m_Device);
	auto hr_dxdevconterxt4 = tempContext->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)&m_DevContext);
	auto hr_dxswapchain4 = tempSwapChain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&m_SwapChain);

	if (FAILED(hr_dxdevice5) || FAILED(hr_dxdevconterxt4) || FAILED(hr_dxswapchain4)) {
		DebugBreak();
	}

	ComPtr<ID3D11Texture2D1> pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D1), (LPVOID*)&pBackBuffer);

	m_Device->CreateRenderTargetView1(pBackBuffer.Get(), NULL, &m_BackBuffer);

	D3D11_TEXTURE2D_DESC1 depthStencilDesc{};
	depthStencilDesc.Width = windowPropertise.width;
	depthStencilDesc.Height = windowPropertise.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DYNAMIC;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 16;

	m_Device->CreateTexture2D1(&depthStencilDesc, nullptr, &m_DepthStencilBuffer);
	m_Device->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView);

	ID3D11RenderTargetView* const targents[1] = { m_BackBuffer.Get() };

	m_DevContext->OMSetRenderTargets(ARRAYSIZE(targents), targents, m_DepthStencilView);

	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowPropertise.width;
	viewport.Height = windowPropertise.height;
	viewport.MinDepth = .0f;
	viewport.MaxDepth = 1.f;

	m_DevContext->RSSetViewports(1, &viewport);
}

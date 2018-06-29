#include "Graphics.h"

#include <crtdbg.h>

using namespace DirectX11;

Graphics* Graphics::m_Graphics = nullptr;

Graphics::Graphics() {
}


Graphics::~Graphics() {
	if (m_Graphics == this) {
		m_Graphics = nullptr;
	}
	m_Swapchain->Release();
	m_Backbuffer->Release();
	m_Device->Release();
	m_DeviceContext->Release();
}


void Graphics::createWindow(UINT a_Width, UINT a_Height, WNDPROC a_WndProc) {
	m_Graphics = this;

	m_Width = a_Width;
	m_Height = a_Height;
	HINSTANCE hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window


	WNDCLASSEX wc;
	RECT wr = { 0,0,m_Width,m_Height };

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = a_WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "DirectXWindow";

	RegisterClassEx(&wc);

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL, "DirectXWindow", "First DirectX Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
}

void Graphics::startDirectX() {
	if (hWnd == nullptr) {
		_ASSERT_EXPR(false, L"No Window to create DirectX context");
		return;
	}
	//DX11 Init
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.BufferDesc.Width = m_Width;							// set the back buffer width
	scd.BufferDesc.Height = m_Height;						// set the back buffer height
	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // allow full-screen switching
	//^ sorta works, when using a smaller res then window size it will move all other windows from my right screen to my left 
	//allows alt+enter to change fullscreen

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,//IDXGIAdapter adapter 
								  D3D_DRIVER_TYPE_HARDWARE, //D3D_DRIVER_TYPE DriverType 
								  NULL, //HMODULE software (handle to dll that implements software rasterizer)
								  createDeviceFlags, //UINT flags
								  NULL, //D3D_FEATURE_LEVEL FeatureLevels
								  NULL, //UINT FeatureLevels (number of feature levels)
								  D3D11_SDK_VERSION, //UINT SDKversion (USE D3D11_SDK_VERSION)
								  &scd, //DXGI_SWAP_CHAIN_DESC SwapChainDesc
								  &m_Swapchain, //IDXGISwapChain swapChain
								  &m_Device, //ID3D11Device Device
								  NULL, //D3D_FEATURE_LEVEL FeatureLevel
								  &m_ImmediateContext); //ID3D11DeviceContext ImmediateContext/DeviceContext
	m_DeviceContext = m_ImmediateContext;

	//DX11 Get Backbuffer/render target
	ID3D11Texture2D* pBackBuffer;
	m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);// Get a pointer to the back buffer

	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_Backbuffer);

	m_ImmediateContext->OMSetRenderTargets(1, &m_Backbuffer, NULL);

	pBackBuffer->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = m_Width;
	vp.Height = m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);
}

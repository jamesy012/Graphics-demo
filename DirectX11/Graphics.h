#pragma once

#include <Windows.h>

#include <D3D11.h>
#include <D3Dx10.h>

namespace DirectX11 {
	//handles the window and the graphics
	class Graphics {
	public:
		static Graphics* m_Graphics;

		Graphics();
		~Graphics();

		void createWindow(UINT a_Width, UINT a_Height, WNDPROC a_WndProc);
		void startDirectX();

		ID3D11Device* getDevice() {
			return m_Device;
		}

		ID3D11DeviceContext* getDeviceContext() {
			return m_DeviceContext;
		}

		ID3D11RenderTargetView* getBackbuffer() {
			return m_Backbuffer;
		}

		IDXGISwapChain* getSwapChain() {
			return m_Swapchain;
		}

	private:
		HWND hWnd = nullptr;

		IDXGISwapChain* m_Swapchain;             // the pointer to the swap chain interface
		ID3D11Device* m_Device;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext* m_ImmediateContext;           // the pointer to our Direct3D device context
		ID3D11DeviceContext* m_DeviceContext;           // Same as ImmediateContext (just a different name for it, since both are common)

		ID3D11RenderTargetView* m_Backbuffer;

		UINT m_Width, m_Height;
	};
}

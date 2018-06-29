
#include <Windows.h>
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "Graphics.h"
#include "Mesh.h"
#include "Shader.h"

//http://www.directxtutorial.com/LessonList.aspx?listid=11

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace DirectX11;


// function prototypes
void CleanD3D(void);         // closes Direct3D and releases memory
void RenderFrame(void);

Graphics* m_Graphics;
Mesh* m_Triangle;
Shader* m_Shader;

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam);

#if _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
				   HINSTANCE   hPrevInstance,              // Previous Instance
				   LPSTR       lpCmdLine,              // Command Line Parameters
				   int     nCmdShow)               // Window Show State
{
#endif


	m_Graphics = new Graphics();
	m_Graphics->createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WindowProc);
	m_Graphics->startDirectX();

	(m_Triangle = new Mesh())->createTriangle();

	m_Shader = new Shader();
	m_Shader->load("Shaders.shader");
	m_Shader->use();

	//main loop
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		} else {
			//run Game code!

			RenderFrame();
		}
	}

	CleanD3D();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;

}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// sort through and find what code to run for the message given
	switch (message) {
		// this message is read when the window is closed
		case WM_DESTROY:
			{
				// close the application entirely
				PostQuitMessage(0);
				return 0;
			} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void RenderFrame(void) {
	static int value = 0;
	value++;
	value = value % 255;
	float percent = value / 255.0f;

	m_Graphics->getDeviceContext()->ClearRenderTargetView(m_Graphics->getBackbuffer(), D3DXCOLOR(percent, percent, percent, 1.0f));

	//render here
	m_Triangle->draw();


	//swap
	m_Graphics->getSwapChain()->Present(1, 0);

}

// this is the function that cleans up Direct3D and COM
void CleanD3D() {

	m_Graphics->getSwapChain()->SetFullscreenState(FALSE, NULL);

	delete m_Shader;

	delete m_Triangle;
	delete m_Graphics;

}


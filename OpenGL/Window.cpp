#include "Window.h"

#include <GL/glew.h>
#include <Windows.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc

struct _WindowHandle : public WindowHandle {
public:
	 HGLRC           hRC = NULL;                           // Permanent Rendering Context
	 HDC             hDC = NULL;                           // Private GDI Device Context
	 HWND            hWnd = NULL;                          // Holds Our Window Handle
	 HINSTANCE       hInstance;                          // Holds The Instance Of The Application
};

WindowHandle* createWindow(const char * title, int width, int height, int bits, bool fullscreenflag) {
	_WindowHandle* ourWindow = new _WindowHandle();


	GLuint      PixelFormat;                        // Holds The Results After Searching For A Match
	WNDCLASS    wc;                         // Windows Class Structure
	DWORD       dwExStyle;                      // Window Extended Style
	DWORD       dwStyle;                        // Window Style

	RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;                        // Set Left Value To 0
	WindowRect.right = (long)width;                       // Set Right Value To Requested Width
	WindowRect.top = (long)0;                         // Set Top Value To 0
	WindowRect.bottom = (long)height;                     // Set Bottom Value To Requested Height

	ourWindow->fullscreen = fullscreenflag;                      // Set The Global Fullscreen Flag

	ourWindow->hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = (WNDPROC)WndProc;                // WndProc Handles Messages
	wc.cbClsExtra = 0;                        // No Extra Window Data
	wc.cbWndExtra = 0;                        // No Extra Window Data
	wc.hInstance = ourWindow->hInstance;                    // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
	wc.hbrBackground = NULL;                     // No Background Required For GL
	wc.lpszMenuName = NULL;                     // We Don't Want A Menu
	wc.lpszClassName = "OpenGL";                 // Set The Class Name

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Exit And Return FALSE
	}

	if (ourWindow->fullscreen) {
		DEVMODE dmScreenSettings; // Device Mode
		memset(&dmScreenSettings, 0, sizeof(DEVMODE)); // make sure memory is cleared

		dmScreenSettings.dmSize = sizeof(DEVMODE); // size of structure
		dmScreenSettings.dmPelsWidth = width; // width of screen
		dmScreenSettings.dmPelsHeight = height; // height of screen
		dmScreenSettings.dmBitsPerPel = bits; // bits
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
				ourWindow->fullscreen = false;
			} else {
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;                   // Exit And Return FALSE
			}
		}
	}

	if (ourWindow->fullscreen) { // are we still there?
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;
		//dwStyle = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	//SetPropW(ourWindow->hWnd, L"OpenGLWindow", ourWindow);

	ourWindow->hWnd = CreateWindowEx(dwExStyle, "OpenGL", title, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, ourWindow->hInstance, NULL);

	if (!ourWindow->hWnd) {
		//DestroyOpenGLWindow();
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}



	static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		bits,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		16,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                         // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                             // Layer Masks Ignored
	};

	ourWindow->hDC = GetDC(ourWindow->hWnd);
	if (!ourWindow->hDC) {
		//DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	PixelFormat = ChoosePixelFormat(ourWindow->hDC, &pfd);
	if (!PixelFormat)             // Did Windows Find A Matching Pixel Format?
	{
		//DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!SetPixelFormat(ourWindow->hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
	{
		//DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	ourWindow->hRC = wglCreateContext(ourWindow->hDC);
	if (!ourWindow->hRC) {
		//DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(ourWindow->hDC, ourWindow->hRC))                        // Try To Activate The Rendering Context
	{
		//DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	SetWindowLongPtr(ourWindow->hWnd, GWLP_USERDATA, (LONG_PTR)ourWindow);

	ShowWindow(ourWindow->hWnd, SW_SHOW);
	SetForegroundWindow(ourWindow->hWnd);
	SetFocus(ourWindow->hWnd);

	//resizeOpenGLWindow(width, height);
	//initOpenGL();


	return (WindowHandle*)ourWindow;
}

void swapBuffers(WindowHandle * a_Window) {
	_WindowHandle* ourWindow = (_WindowHandle*)a_Window;

	SwapBuffers(ourWindow->hDC);           // Swap Buffers (Double Buffering)
}

void destroyWindow(WindowHandle * a_Window) {
	_WindowHandle* ourWindow = (_WindowHandle*)a_Window;

	if (ourWindow->fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	if (ourWindow->hRC) {
		if (!wglMakeCurrent(NULL, NULL)) { //make context null
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(ourWindow->hRC)) { //delete context
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		ourWindow->hRC = NULL;
	}
	if (ourWindow->hDC && !ReleaseDC(ourWindow->hWnd, ourWindow->hDC)) {
		//failed
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ourWindow->hDC = NULL;
	}
	if (ourWindow->hWnd && !DestroyWindow(ourWindow->hWnd)) {
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ourWindow->hWnd = NULL;                          // Set hWnd To NULL
	}
	if (!UnregisterClass("OpenGL", ourWindow->hInstance))               // Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ourWindow->hInstance = NULL;                         // Set hInstance To NULL
	}
}

LRESULT CALLBACK WndProc(HWND    hWnd,                   // Handle For This Window
						 UINT    uMsg,                   // Message For This Window
						 WPARAM  wParam,                 // Additional Message Information
						 LPARAM  lParam)                 // Additional Message Information
{
	//_WindowHandle* ourWindow = (_WindowHandle*)GetPropW(hWnd, L"OpenGLWindow");
	_WindowHandle* ourWindow = (_WindowHandle*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	printf("Msg: %i\n", uMsg);
	switch (uMsg)                               // Check For Windows Messages
	{
		case WM_ACTIVATE:                       // Watch For Window Activate Message
			{
				if (!HIWORD(wParam))                    // Check Minimization State
				{
					ourWindow->active = TRUE;                    // Program Is Active
				} else {
					ourWindow->active = FALSE;                   // Program Is No Longer Active
				}

				return 0;                       // Return To The Message Loop
			}
		case WM_SYSCOMMAND:                     // Intercept System Commands
			{
				switch (wParam)                     // Check System Calls
				{
					case SC_SCREENSAVE:             // Screensaver Trying To Start?
					case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
						return 0;                   // Prevent From Happening
				}
				break;                          // Exit
			}
		case WM_CLOSE:                          // Did We Receive A Close Message?
			{
				PostQuitMessage(0);                 // Send A Quit Message
				return 0;                       // Jump Back
			}
		case WM_KEYDOWN:                        // Is A Key Being Held Down?
			{
				ourWindow->keys[wParam] = TRUE;                    // If So, Mark It As TRUE
				return 0;                       // Jump Back
			}
		case WM_KEYUP:                          // Has A Key Been Released?
			{
				ourWindow->keys[wParam] = FALSE;                   // If So, Mark It As FALSE
				return 0;                       // Jump Back
			}
		case WM_SIZE:                           // Resize The OpenGL Window
			{
				//resizeOpenGLWindow(LOWORD(lParam), HIWORD(lParam));       // LoWord=Width, HiWord=Height
				glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));

				return 0;                       // Jump Back
			}
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
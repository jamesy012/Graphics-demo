
#include <Windows.h>
#include <iostream>

#include <GL/glew.h>

#include "Window.h"

/*
HGLRC           hRC = NULL;                           // Permanent Rendering Context
HDC             hDC = NULL;                           // Private GDI Device Context
HWND            hWnd = NULL;                          // Holds Our Window Handle
HINSTANCE       hInstance;                          // Holds The Instance Of The Application

bool    keys[256];                              // Array Used For The Keyboard Routine
bool    active = TRUE;                                // Window Active Flag Set To TRUE By Default
bool    fullscreen = TRUE;                            // Fullscreen Flag Set To Fullscreen Mode By Default
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc
*/
int value;


GLvoid resizeOpenGLWindow(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
}

GLvoid initOpenGL() {
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

}

GLvoid drawOpenGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	value++;
	value = value % 255;
	float percent = value / 255.0f;
	glClearColor(percent, percent, percent, 0);
}
/*
void DestroyOpenGLWindow() {
	if (fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	if (hRC) {
		if (!wglMakeCurrent(NULL, NULL)) { //make context null
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC)) { //delete context
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) {
		//failed
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;                          // Set hWnd To NULL
	}
	if (!UnregisterClass("OpenGL", hInstance))               // Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;                         // Set hInstance To NULL
	}
}


BOOL CreateGLWindow(const char* title, int width, int height, int bits, bool fullscreenflag) {

	GLuint      PixelFormat;                        // Holds The Results After Searching For A Match
	WNDCLASS    wc;                         // Windows Class Structure
	DWORD       dwExStyle;                      // Window Extended Style
	DWORD       dwStyle;                        // Window Style

	RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;                        // Set Left Value To 0
	WindowRect.right = (long)width;                       // Set Right Value To Requested Width
	WindowRect.top = (long)0;                         // Set Top Value To 0
	WindowRect.bottom = (long)height;                     // Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;                      // Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);            // Grab An Instance For Our Window

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = (WNDPROC)WndProc;                // WndProc Handles Messages
	wc.cbClsExtra = 0;                        // No Extra Window Data
	wc.cbWndExtra = 0;                        // No Extra Window Data
	wc.hInstance = hInstance;                    // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);          // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // Load The Arrow Pointer
	wc.hbrBackground = NULL;                     // No Background Required For GL
	wc.lpszMenuName = NULL;                     // We Don't Want A Menu
	wc.lpszClassName = "OpenGL";                 // Set The Class Name

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Exit And Return FALSE
	}

	if (fullscreen) {
		DEVMODE dmScreenSettings; // Device Mode
		memset(&dmScreenSettings, 0, sizeof(DEVMODE)); // make sure memory is cleared

		dmScreenSettings.dmSize = sizeof(DEVMODE); // size of structure
		dmScreenSettings.dmPelsWidth = width; // width of screen
		dmScreenSettings.dmPelsHeight = height; // height of screen
		dmScreenSettings.dmBitsPerPel = bits; // bits
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
				fullscreen = false;
			} else {
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;                   // Exit And Return FALSE
			}
		}
	}

	if (fullscreen) { // are we still there?
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;
		//dwStyle = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	hWnd = CreateWindowEx(dwExStyle, "OpenGL", title, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		DestroyOpenGLWindow();
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

	hDC = GetDC(hWnd);
	if (!hDC) {
		DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!PixelFormat)             // Did Windows Find A Matching Pixel Format?
	{
		DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))               // Are We Able To Set The Pixel Format?
	{
		DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	hRC = wglCreateContext(hDC);
	if (!hRC) {
		DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC))                        // Try To Activate The Rendering Context
	{
		DestroyOpenGLWindow();                         // Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	resizeOpenGLWindow(width,height);
	initOpenGL();

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND    hWnd,                   // Handle For This Window
						 UINT    uMsg,                   // Message For This Window
						 WPARAM  wParam,                 // Additional Message Information
						 LPARAM  lParam)                 // Additional Message Information
{
	switch (uMsg)                               // Check For Windows Messages
	{
		case WM_ACTIVATE:                       // Watch For Window Activate Message
			{
				if (!HIWORD(wParam))                    // Check Minimization State
				{
					active = TRUE;                    // Program Is Active
				} else {
					active = FALSE;                   // Program Is No Longer Active
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
				keys[wParam] = TRUE;                    // If So, Mark It As TRUE
				return 0;                       // Jump Back
			}
		case WM_KEYUP:                          // Has A Key Been Released?
			{
				keys[wParam] = FALSE;                   // If So, Mark It As FALSE
				return 0;                       // Jump Back
			}
		case WM_SIZE:                           // Resize The OpenGL Window
			{
				resizeOpenGLWindow(LOWORD(lParam), HIWORD(lParam));       // LoWord=Width, HiWord=Height
				return 0;                       // Jump Back
			}
	}
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
*/
int main() {
//int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
//				   HINSTANCE   hPrevInstance,              // Previous Instance
//				   LPSTR       lpCmdLine,              // Command Line Parameters
//				   int     nCmdShow)               // Window Show State
//{
	MSG msg;                                // Windows Message Structure
	BOOL    done = FALSE;                         // Bool Variable To Exit Loop

	WindowHandle* wh = createWindow("Window", 640, 480, 16, false);


	//if (!CreateGLWindow("OpenGL!", 640, 480, 16, false)) {
	//	return 0;
	//}

	{
		int glMajor, glMinor;
		const  GLubyte* glVersion;
		const  GLubyte* glVender;
		const  GLubyte* glRenderer;
		glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
		glGetIntegerv(GL_MINOR_VERSION, &glMinor);
		glVersion = glGetString(GL_VERSION);
		glVender = glGetString(GL_VENDOR);
		glRenderer = glGetString(GL_RENDERER);
		printf("OpenGL - %s\n(%i.%i) Vender: %s, Renderer: %s\n\n", glVersion, glMajor, glMinor, glVender, glRenderer);
	}

	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)               // Have We Received A Quit Message?
			{
				done = TRUE;                  // If So done=TRUE
			} else {                        // If Not, Deal With Window Messages
				TranslateMessage(&msg);             // Translate The Message
				DispatchMessage(&msg);              // Dispatch The Message
			}
		} else {
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (wh->active)                     // Program Active?
			{
				if (wh->keys[VK_ESCAPE])                // Was ESC Pressed?
				{
					done = TRUE;              // ESC Signalled A Quit
				} else                        // Not Time To Quit, Update Screen
				{
					drawOpenGL();              // Draw The Scene
					swapBuffers(wh);
				}
			}
			//if (keys[VK_F1])                    // Is F1 Being Pressed?
			//
			//	keys[VK_F1] = FALSE;              // If So Make Key FALSE
			//	DestroyOpenGLWindow();                 // Kill Our Current Window
			//	fullscreen = !fullscreen;             // Toggle Fullscreen / Windowed Mode
			//										  // Recreate Our OpenGL Window
			//	if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen)) {
			//		return 0;               // Quit If Window Was Not Created
			//	}

		}
	}

//DestroyOpenGLWindow();
	destroyWindow(wh);
return(msg.wParam);
}

/*
int main(){
	std::cout << "Hello" << std::endl;

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
	}

	system("pause");
	return 0;
}
*/
#pragma once

struct WindowHandle {
	bool fullscreen;
	bool active = false;
	int value = 5;
	bool    keys[256];                              // Array Used For The Keyboard Routine

};


WindowHandle* createWindow(const char* title, int width, int height, int bits, bool fullscreenflag);
void swapBuffers(WindowHandle* a_Window);
void destroyWindow(WindowHandle* a_Window);

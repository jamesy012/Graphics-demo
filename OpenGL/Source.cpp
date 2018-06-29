
#include <Windows.h>
#include <iostream>

#include <GL/glew.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

using namespace OpenGL;

WindowHandle* m_Window;
Shader* m_Shader;
Mesh* m_Mesh;

const char* FragmentShader = R"(
#version 460 core
out vec4 FragColor;  
in vec4 vertColor;
  
void main() {
    FragColor = vertColor;
}
)";

const char* VertexShader = R"(
#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;
  
out vec4 vertColor;

void main() {
    gl_Position = vec4(vPos, 1.0);
    vertColor = vColor;
}    
)";

GLvoid resizeOpenGLWindow(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
}

GLvoid initOpenGL() {
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

}

GLvoid drawOpenGL() {
	static int value = 0;
	value++;
	value = value % 255;
	float percent = value / 255.0f;
	glClearColor(percent, percent, percent, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw
	m_Mesh->draw();

	swapBuffers(m_Window);	
}

#if _DEBUG
int main() {
#else
int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
				   HINSTANCE   hPrevInstance,              // Previous Instance
				   LPSTR       lpCmdLine,              // Command Line Parameters
				   int     nCmdShow)               // Window Show State
{
#endif

	MSG msg;                                // Windows Message Structure
	BOOL    done = FALSE;                         // Bool Variable To Exit Loop

	m_Window = createWindow("Window", 640, 480, 16, false);
	m_Shader = new Shader();
	m_Mesh = new Mesh();
	
	m_Shader->load(VertexShader, FragmentShader);
	m_Shader->use();
	
	m_Mesh->createTriangle();

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
			if (m_Window->active)                     // Program Active?
			{
				if (m_Window->keys[VK_ESCAPE])                // Was ESC Pressed?
				{
					done = TRUE;              // ESC Signalled A Quit
				} else                        // Not Time To Quit, Update Screen
				{
					drawOpenGL();              // Draw The Scene
				}
			}
		}
	}

//DestroyOpenGLWindow();
	destroyWindow(m_Window);
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
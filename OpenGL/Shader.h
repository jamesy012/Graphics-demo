#pragma once

#include <GL/glew.h>

namespace OpenGL {


	class Shader {
	public:
		Shader();
		~Shader();

		void use();
		void load(const char* a_VertexText, const char* a_FragmentText);
	private:
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_ShaderProgram = 0;


	};
}

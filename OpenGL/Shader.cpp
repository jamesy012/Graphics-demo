#include "Shader.h"

#include <stdio.h>


using namespace OpenGL;

Shader::Shader() {
}

Shader::~Shader() {

}

void Shader::use() {
	glUseProgram(m_ShaderProgram);
}


void Shader::load(const char * a_VertexText, const char * a_FragmentText) {
	GLint success;

	//create id's
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	m_ShaderProgram = glCreateProgram();

	//get shader code into objects
	glShaderSource(m_VertexShader, 1, &a_VertexText, nullptr);
	glShaderSource(m_FragmentShader, 1, &a_FragmentText, nullptr);

	//compile those objects
	glCompileShader(m_VertexShader);
	glCompileShader(m_FragmentShader);

	//check for errors with compiles
	glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(m_VertexShader, 512, nullptr, infoLog);
		printf("error compiling vertex Shader: %s\n", infoLog);
	}
	glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(m_FragmentShader, 512, nullptr, infoLog);
		printf("error compiling fragment Shader: %s\n", infoLog);
	}

	//attach them to the program
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);

	//link everything togeather into the program
	glLinkProgram(m_ShaderProgram);

	//check for errors with linking
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
		printf("error compiling shader program: %s\n", infoLog);
	}

	//detach from program
	glDetachShader(m_ShaderProgram, m_VertexShader);
	glDetachShader(m_ShaderProgram, m_FragmentShader);

	//they arnt needed now, can delete
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);

}

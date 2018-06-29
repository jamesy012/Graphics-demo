#pragma once

#include <GL/glew.h>

namespace OpenGL {
	struct Vec3 {
		GLfloat x, y, z;
	};
	struct Vec4 {
		GLfloat x, y, z, w;
	};
	struct Vertex {
		Vec3 m_Position;
		Vec4 m_Color;
	};
	class Mesh {
	public:
		Mesh();
		~Mesh();

		void createTriangle();
		void draw();
	private:

		void bind();

		GLuint m_Vao = 0, m_Vbo = 0;//, m_Ebo;
		unsigned int m_NumOfVerts = 0;
	};
}
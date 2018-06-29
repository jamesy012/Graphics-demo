#include "Mesh.h"

#include <cstddef>

using namespace OpenGL;

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::createTriangle() {
	const Vertex OurVertices[] = {
		{ { 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.45f, -0.5, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.45f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	{ { 0.45f, -0.5, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.0f, -0.8f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { -0.45f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	};
	m_NumOfVerts = sizeof(OurVertices) / sizeof(Vertex);

	glGenVertexArrays(1, &m_Vao);
	glGenBuffers(1, &m_Vbo);

	//bind all future data to the vao
	glBindVertexArray(m_Vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(OurVertices), &OurVertices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);

	//Vertex Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, m_Color));

	glBindVertexArray(0);

}

void Mesh::draw() {
	glBindVertexArray(m_Vao);

	//glDrawElements
	glDrawArrays(GL_TRIANGLES, 0, m_NumOfVerts);

	glBindVertexArray(0);
}

void Mesh::bind() {
	//todo set bufferData here too
	glGenVertexArrays(1, &m_Vao);
	glGenBuffers(1, &m_Vbo);

	//bind all future data to the vao
	glBindVertexArray(m_Vao);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);

	//Vertex Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, m_Color));


	glBindVertexArray(0);
}

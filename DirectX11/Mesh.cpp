#include "Mesh.h"

#include "Graphics.h"

using namespace DirectX11;

Mesh::Mesh() {
}

Mesh::~Mesh() {
	m_VBuffer->Release();
}

void Mesh::createTriangle() {
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] = {
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{0.0f, -0.8f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
	};

	//todo move most of this out
	m_NumOfVerts = sizeof(OurVertices) / sizeof(VERTEX);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * m_NumOfVerts; //same as sizeof(OurVertices) (optimisation?)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Graphics::m_Graphics->getDevice()->CreateBuffer(&bd, NULL, &m_VBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	Graphics::m_Graphics->getDeviceContext()->Map(m_VBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	Graphics::m_Graphics->getDeviceContext()->Unmap(m_VBuffer, NULL);

}

void Mesh::draw() {
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Graphics::m_Graphics->getDeviceContext()->IASetVertexBuffers(0, 1, &m_VBuffer, &stride, &offset);

	Graphics::m_Graphics->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Graphics::m_Graphics->getDeviceContext()->Draw(m_NumOfVerts, 0);
}

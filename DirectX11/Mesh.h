#pragma once

#include <D3D11.h>
#include <D3Dx10.h>

namespace DirectX11 {
	struct VERTEX {
		FLOAT X, Y, Z;
		D3DXCOLOR Color;
	};

	class Mesh {
	public:
		Mesh();
		~Mesh();

		void createTriangle();

		void draw();

	private:
		unsigned int m_NumOfVerts;

		ID3D11Buffer* m_VBuffer;
	};

}
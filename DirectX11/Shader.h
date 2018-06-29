#pragma once

#include <D3D11.h>

namespace DirectX11 {

	class Shader {
	public:
		Shader();
		~Shader();

		void load(const char* a_FilePath);

		void use();

	private:
		ID3D11VertexShader * pVS;    // the vertex shader
		ID3D11PixelShader *pPS;     // the pixel shader

		ID3D11InputLayout *pLayout;
	};
}

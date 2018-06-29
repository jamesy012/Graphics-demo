#include "Shader.h"

#include <d3dx11.h>

#include "Graphics.h"

using namespace DirectX11;

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::load(const char * a_FilePath) {
	ID3D10Blob *VS, *PS;

	D3DX11CompileFromFile(a_FilePath, 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(a_FilePath, 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);
	//D3D10CompileShader() //<- loading from const char*

	Graphics::m_Graphics->getDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	Graphics::m_Graphics->getDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	Graphics::m_Graphics->getDeviceContext()->VSSetShader(pVS, 0, 0);
	Graphics::m_Graphics->getDeviceContext()->PSSetShader(pPS, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Graphics::m_Graphics->getDevice()->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
}

void Shader::use() {
	Graphics::m_Graphics->getDeviceContext()->IASetInputLayout(pLayout);
}


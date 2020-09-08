#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Shader* shader, UINT32 size, const void* data, LPCSTR name) {
	m_BufferDesc = {};
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.ByteWidth = size;

	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3DContext::GetDevice()->CreateInputLayout(ied, ARRAYSIZE(ied), shader->GetVsBlob()->GetBufferPointer(), shader->GetVsBlob()->GetBufferSize(), &m_InputLayout);

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = data;

	auto hr = D3DContext::GetDevice()->CreateBuffer(&m_BufferDesc, &sd, &m_BufferHandle);

	if (FAILED(hr)) {
		DebugBreak();
	}
}

void VertexBuffer::Bind(UINT32 val) {
	UINT32 stride = val;
	UINT32 offset = 0;

	D3DContext::GetDeviceContext()->IASetInputLayout(m_InputLayout);
	D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
}




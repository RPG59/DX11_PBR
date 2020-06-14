#pragma once
#include <d3d11_4.h>
#include "D3DContext.h"
#include "Shader.h"

class VertexBuffer {
private:
	D3D11_BUFFER_DESC m_BufferDesc;
	D3D11_MAPPED_SUBRESOURCE m_MappedSubserource;
	ID3D11Buffer* m_BufferHandle;
	ID3D11InputLayout* m_InputLayout;
public:
	VertexBuffer(Shader*, UINT32, const void*, LPCSTR);
public:
	void Bind(UINT32);
};
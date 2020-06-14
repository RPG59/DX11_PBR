#pragma once
#include <d3d11_4.h>

#include "Shader.h"
#include "D3DContext.h"

class ConstantBuffer
{
private:
	D3D11_BUFFER_DESC m_BufferDesc;
	D3D11_MAPPED_SUBRESOURCE m_MappedSubserource;
	ID3D11Buffer* m_BufferHandle;
	ID3D11InputLayout* m_InputLayout;
	UINT32 m_Slot;
public:
	ConstantBuffer(UINT32, UINT32);
	void Map(UINT32, const void*);
	void BindVS();
	void BindPS();
};


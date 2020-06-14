#pragma once
#include <d3d11_4.h>

#include "D3DContext.h"

class IndexBuffer
{
private:
	ID3D11Buffer* m_BufferHandle;
	SIZE_T m_Count;
public:
	IndexBuffer(UINT32* data, SIZE_T size);
	inline SIZE_T GetCount() const { return m_Count; }
};


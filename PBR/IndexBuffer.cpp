#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(UINT32* data, SIZE_T count) :m_Count(count) {
	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT32) * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = data;

	auto hr = D3DContext::GetDevice()->CreateBuffer(&ibd, &InitData, &m_BufferHandle);
	if (FAILED(hr)) {
		DebugBreak();
	}

	D3DContext::GetDeviceContext()->IASetIndexBuffer(m_BufferHandle, DXGI_FORMAT_R32_UINT, 0);
}

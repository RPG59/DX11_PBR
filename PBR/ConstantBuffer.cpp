#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(UINT32 slot, UINT32 size) :m_Slot(slot) {
	m_BufferDesc = {};
	m_BufferDesc.ByteWidth = size;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = new byte(m_BufferDesc.ByteWidth);

	auto buffer_hr = D3DContext::GetDevice()->CreateBuffer(&m_BufferDesc, &InitData, &m_BufferHandle);
	if (FAILED(buffer_hr)) {
		DebugBreak();
	}
}

void ConstantBuffer::Map(UINT32 size, const void* pData) {
	D3D11_MAPPED_SUBRESOURCE msr{};

	D3DContext::GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
	memcpy(msr.pData, pData, size);
	D3DContext::GetDeviceContext()->Unmap(m_BufferHandle, NULL);
}

void ConstantBuffer::BindVS() {
	D3DContext::GetDeviceContext()->VSSetConstantBuffers(m_Slot, 1, &m_BufferHandle);
}

void ConstantBuffer::BindPS() {
	D3DContext::GetDeviceContext()->PSSetConstantBuffers(m_Slot, 1, &m_BufferHandle);
}

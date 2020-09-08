#include "Mesh.h"

Mesh::Mesh(UINT32 size, void* data, UINT32 itemSize, UINT32 indexSize, UINT32* indices, Shader* pShader, Material* material, std::string name) :
	m_Shader(pShader), m_Material(material), m_ItemSize(itemSize), m_Name(name) {
	m_VBO = new VertexBuffer(pShader, size * itemSize, data, "SPHERE_DATA");
	m_EBO = new IndexBuffer(indices, indexSize);

	XMMATRIX modelMatirx = XMMatrixTranslation(0, 0, 0);
	XMStoreFloat4x4(&m_ModelMatirx, modelMatirx);

	UpdateMaterialConstantBuffer();
}

Mesh::~Mesh() {
	if (m_VBO != nullptr)
		delete m_VBO;
	if (m_EBO != nullptr)
		delete m_EBO;
	if (m_Material != nullptr)
		delete m_Material;
}

void Mesh::SetPosition(float x, float y, float z) {
	XMMATRIX modelMatirx = XMMatrixTranslation(x, y, z);
}

void Mesh::Render() {
	if (m_NeedUpdate) {
		Update();
	}
	ConstantBuffer* constantBuffer = (ConstantBuffer*)m_Shader->GetUniformMap()["ModelMatrix"];

	constantBuffer->Map(sizeof(XMFLOAT4X4), &m_ModelMatirx);
	constantBuffer->BindVS();
	m_VBO->Bind(m_ItemSize);
	D3DContext::GetDeviceContext()->DrawIndexed(m_EBO->GetCount(), 0, 0);
}

void Mesh::Update() {
	auto translation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	auto scaling = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	auto result = XMMatrixMultiply(scaling, translation);
	XMStoreFloat4x4(&m_ModelMatirx, result);
}

void Mesh::SetPosition(XMFLOAT3 position) {
	m_Position = position;
	m_NeedUpdate = true;
}

void Mesh::SetScale(XMFLOAT3 scale) {
	m_Scale = scale;
	m_NeedUpdate = true;
}

void Mesh::SetMaterial(Material* pMaterial) {
	if (m_Material != nullptr) {
		delete m_Material;
	}
	m_Material = pMaterial;
	UpdateMaterialConstantBuffer();
}

void Mesh::UpdateMaterialConstantBuffer() {
	auto materilBuffer = (ConstantBuffer*)m_Shader->GetUniformMap()["Material"];

	materilBuffer->Map(sizeof(Material), m_Material);
	materilBuffer->BindPS();
}



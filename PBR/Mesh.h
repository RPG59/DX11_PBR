#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "D3DContext.h"
#include "ConstantBuffer.h"
#include "Material.h"

using namespace DirectX;

class Mesh
{
private:
	VertexBuffer* m_VBO = nullptr;
	IndexBuffer* m_EBO = nullptr;
	Shader* m_Shader;
	Material* m_Material = nullptr;
	XMFLOAT4X4 m_ModelMatirx;
	XMFLOAT3 m_Position{ 0, 0, 0 };
	XMFLOAT3 m_Scale{ 1,1,1 };
	std::string m_Name;
	UINT32 m_ItemSize;
	bool m_NeedUpdate = true;
	bool m_Drawable = true;

public:
	Mesh(UINT32, void*, UINT32, UINT32, UINT32*, Shader*, Material*, std::string = "defualt_mesh_name");
	~Mesh();
	void Render();
	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT3);
	void SetScale(XMFLOAT3);
	void Update();
	void SetMaterial(Material*);
	void UpdateMaterialConstantBuffer();

	void ChangeDrawable(bool drawable) {
		m_Drawable = drawable;
	}

	inline std::string& getName() { return m_Name; }
	inline bool isDrawable() { return m_Drawable; }
};


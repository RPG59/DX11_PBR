#pragma once
#include <vector>

#include "Mesh.h"
#include "Light.h"
#include "Shader.h"

class Scene {
private:
	std::vector<Mesh*> m_Meshes;
	Light* m_Lights = nullptr;
	bool m_needUpdateLights = true;
	Shader* m_Shader;
	ConstantBuffer* m_LightUniformBuffer = nullptr;


private:
	void UpdateLights();


public:
	Scene(Shader* pShader);
	~Scene();
	void Draw();
	void AddLight(Light* pLight);
	void AddMesh(Mesh* pMesh);
};



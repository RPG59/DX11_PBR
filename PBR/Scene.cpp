#include "Scene.h"

Scene::Scene(Shader* pShader) :m_Shader(pShader) {
	m_LightUniformBuffer = new ConstantBuffer(3, sizeof(Light));
}

Scene::~Scene() {
	delete m_LightUniformBuffer;
}

void Scene::Draw() {
	if (m_needUpdateLights) {
		UpdateLights();
	}

	for (auto pMesh : m_Meshes) {
		pMesh->Render();
	}
}

void Scene::UpdateLights() {
	if (m_Lights != nullptr) {
		m_LightUniformBuffer->Map(sizeof(Light), m_Lights);
		m_LightUniformBuffer->BindPS();
	}
}

void Scene::AddLight(Light* pLight) {
	// TOOD: lights array
	m_needUpdateLights = true;
	m_Lights = pLight;
}

void Scene::AddMesh(Mesh* pMesh) {
	m_Meshes.push_back(pMesh);
}
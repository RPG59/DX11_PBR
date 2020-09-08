#include "Model.h"
#include "Sphere.h"
#include "Material.h"

Model::Model(std::string const& path, Shader* shader) : m_Shader(shader) {
	loadModel(path);
}

void Model::loadModel(std::string const& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		DebugBreak();
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (UINT32 i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (UINT32 i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<SphereData> geometry;
	std::vector<UINT32> indices;

	for (UINT32 i = 0; i < mesh->mNumVertices; ++i) {
		SphereData sd{};
		sd.position.x = mesh->mVertices[i].x;
		sd.position.y = mesh->mVertices[i].y;
		sd.position.z = mesh->mVertices[i].z;

		//sd.normal.x = mesh->mNormals[i].x;
		//sd.normal.y = mesh->mNormals[i].y;

		geometry.push_back(sd);
	}

	for (UINT32 i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (UINT32 j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Material* material = new Material();

	return new Mesh(
		geometry.size(),
		&geometry[0],
		sizeof(SphereData),
		indices.size(),
		&indices[0],
		m_Shader,
		material,
		std::string(mesh->mName.C_Str()));

}

void Model::Draw() {

}
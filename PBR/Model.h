#include <string>
#include <vector>
#undef min

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model {
private:
	std::vector<Mesh*> m_Meshes;
	Shader* m_Shader;

public:
	Model(std::string const& path, Shader*);
	void Draw();
	std::vector<Mesh*>& getMeshes() { return m_Meshes; }
private:
	void loadModel(std::string const& path);
	void processNode(aiNode*, const aiScene*);
	Mesh* processMesh(aiMesh*, const aiScene*);
};
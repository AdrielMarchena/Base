#pragma once

#include "Base/Base.h"
#include "render/gl/Shader.h"
#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
namespace Base
{
	class Model {
	private:
		std::vector<MeshTexture> texture_loaded;
	public:
		Model(const std::string& path)
			:m_Directory(path) 
		{ 
			m_Meshes = MakeRef< std::vector<Mesh>>();
			LoadModel(path); 
		}

		/* Will Override the Textures in the Meshes with the same name */
		void OverrideTexture(Ref<render::Texture> texture);

		/* Will Override the Textures with the target name in the Meshes*/
		void OverrideTexture(Ref<render::Texture> texture, const std::string& target);

		const Ref<std::vector<Mesh>>& GetMeshes() const { return m_Meshes; }
		Ref<std::vector<Mesh>>& GetMeshes() { return m_Meshes; }

		void Draw(Ref<render::Shader>& shader);

		static Ref<Model> CreateModel(const std::string& path);
	private:

		Ref<std::vector<Mesh>> m_Meshes;
		std::string m_Directory;

		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

		
	};
}
#include "Model.h"
#include "utils/base_assert.h"

#include "utils/gl_error_macro_db.h"
#include "gl/glew.h"

namespace Base
{

	void Model::OverrideTexture(Ref<render::Texture> texture)
	{
		for (auto& mesh : *m_Meshes)
		{
			for (auto& mesh_textures : mesh.Textures)
			{
				if (texture->GetName() == mesh_textures.Texture->GetName())
				{
					mesh_textures.Texture = texture;
				}
			}
		}
	}

	void Model::OverrideTexture(Ref<render::Texture> texture, const std::string& target)
	{
		for (auto& mesh : *m_Meshes)
		{
			for (auto& mesh_textures : mesh.Textures)
			{
				if (mesh_textures.Texture->GetName() == target)
				{
					mesh_textures.Texture = texture;
				}
			}
		}
	}

	void Model::Draw(Ref<render::Shader>& shader)
	{
		for (auto& m : *m_Meshes)
			m.Draw(shader);
	}

	Ref<Model> Model::CreateModel(const std::string& path)
	{
		return MakeRef<Model>(path);
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			const std::string error = "Assimp Error: " + std::string(importer.GetErrorString());
			BASE_CORE_ASSERT(false, error);
		}

		m_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
		texture_loaded.clear();
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		m_Meshes->reserve(node->mNumMeshes);
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes->push_back(ProcessMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{

		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;
		//Vertex
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			MeshVertex vertex{};

			vertex.Position =
			{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			if (mesh->HasNormals())
			{
				vertex.Normal =
				{
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				};
			}

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				vertex.TexCoords =
				{
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		//Indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		//Textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<MeshTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<MeshTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
	{
		std::vector<MeshTexture> textures;
		textures.reserve(mat->GetTextureCount(type));
		for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;

			for (uint32_t j = 0; j < texture_loaded.size(); j++)
			{
				if (std::strcmp(texture_loaded[j].Path.data(), str.C_Str()) == 0)
				{
					textures.push_back(texture_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				MeshTexture texture;
				const std::string filename = m_Directory + '/' + str.C_Str();
				texture.Texture = render::Texture::CreateTexture(m_Directory + '/' + str.C_Str());
				texture.Type = typeName;
				texture.Path = str.C_Str();
				textures.push_back(texture);
				texture_loaded.push_back(texture);
			}
			
		}
		return textures;
	}
}
#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "render/gl/Shader.h"
#include "render/gl/VertexArray.h"
#include "render/gl/VertexBuffer.h"
#include "render/gl/IndexBuffer.h"
#include "render/gl/VertexAttribute.h"
#include "render/gl/Texture.h"
namespace Base
{
#define BASE_MAX_BONE_INFLUENCE 4
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;
		//int m_BoneIDs[BASE_MAX_BONE_INFLUENCE];
		//float m_Weights[BASE_MAX_BONE_INFLUENCE];
	};

	struct MeshTexture
	{
		Ref<render::Texture> Texture;
		std::string Type;
		std::string Path;
	};

	class Mesh
	{
	public:
		std::vector<MeshVertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<MeshTexture> Textures;

		Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshTexture>& textures);
		//Temp
		void Draw(Ref<render::Shader> shader);

	private:
		Ref<render::VertexArray> m_VAO;
		Ref<render::VertexBuffer> m_VBO;
		Ref<render::IndexBuffer> m_EBO;

		void SetupMesh();
	};
}



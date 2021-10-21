#include "Mesh.h"

#include "gl/glew.h"
#include "utils/gl_error_macro_db.h"
namespace Base
{
	Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshTexture>& textures)
		:Vertices(vertices),Indices(indices),Textures(textures)
	{
		SetupMesh();
	}

	void Mesh::Draw(Ref<render::Shader> shader)
	{
		//TODO: Set texture limit here
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < Textures.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i)); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = Textures[i].Type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			// now set the sampler to the correct texture unit
			shader->SetUniform1i((name + number).c_str(), i);
			// and finally bind the texture
			GLCall(glBindTexture(GL_TEXTURE_2D, Textures[i].Texture->GetId()));
		}

		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		m_VAO->Unbind();
		GLCall(glActiveTexture(GL_TEXTURE0));
	}

	void Mesh::SetupMesh()
	{
		m_VAO = render::VertexArray::CreateVertexArray();
		m_VAO->Bind();


		size_t vboSize = Vertices.size() * sizeof(MeshVertex);
		m_VBO = render::VertexBuffer::CreateVertexBuffer(vboSize,GL_Usage::STATIC);

		m_VBO->SubData(vboSize, &Vertices[0], 0);


		size_t eboSize = Indices.size() * sizeof(unsigned int);
		m_EBO = render::IndexBuffer::CreateIndexBuffer(eboSize,&Indices[0],GL_Usage::STATIC);

		render::VertexAttribute layout(m_VBO);
		layout.AddLayoutFloat(3, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Position));
		layout.AddLayoutFloat(3, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Normal));
		layout.AddLayoutFloat(2, sizeof(MeshVertex), (void*)offsetof(MeshVertex, TexCoords));
		//layout.AddLayoutFloat(3, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Tangent));
		//layout.AddLayoutFloat(3, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Bitangent));
		//layout.AddLayoutFloat(4, sizeof(MeshVertex), (void*)offsetof(MeshVertex, m_BoneIDs));
		//layout.AddLayoutFloat(4, sizeof(MeshVertex), (void*)offsetof(MeshVertex, m_Weights));

		m_VAO->Unbind();

		//Vertices.clear();
		//Indices.clear();
	}
}
#include "SkyBox.h"

#include "utils/gl_error_macro_db.h"
#include "gl/glew.h"

#include "stb_image.h"

namespace Base
{
	SkyBox::SkyBox(const std::vector<std::string>& paths, Ref<render::Shader> shader)
		:m_CubeTextureId(LoadCubeMap(paths)), m_Shader(shader), m_ProjView(glm::mat4(1.0f))
	{
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_VA = render::VertexArray::CreateVertexArray();
        m_VA->Bind();

        m_VB = render::VertexBuffer::CreateVertexBuffer(sizeof(skyboxVertices),GL_Usage::STATIC);
        m_VB->SubData(sizeof(skyboxVertices), skyboxVertices, 0);

        render::VertexAttribute layout(m_VB);
        layout.AddLayoutFloat(3, 3 * sizeof(float), (const void*)0);

        m_Shader->Bind();
        m_Shader->SetUniform1i("skybox", 0);

	}

    void SkyBox::SetViewProj(const glm::mat4& proj, const glm::mat4& view)
    {
        glm::mat4 new_view = glm::mat4(glm::mat3(view));
        m_ProjView = proj * new_view;
    }

    void SkyBox::Draw()
    {
        GLCall(glDepthFunc(GL_LEQUAL));
        //GLCall(glDepthMask(GL_FALSE););
        m_Shader->Bind();

        m_Shader->SetUniformMat4f("u_ViewProj", m_ProjView);

        m_VA->Bind();
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTextureId));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        m_VA->Unbind();
        GLCall(glDepthFunc(GL_LESS));
        //GLCall(glDepthMask(GL_TRUE););

    }

    uint32_t SkyBox::LoadCubeMap(const std::vector<std::string>& paths)
	{
		uint32_t textureId = 0;
		GLCall(glGenTextures(1, &textureId));
		BASE_CORE_ASSERT(textureId, "Could not generate Texture");
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureId));

		int w = 0, h = 0, c = 0;
		for (uint32_t i = 0; i < paths.size(); i++)
		{
			stbi_set_flip_vertically_on_load(false);
			unsigned char* data = stbi_load(paths[i].c_str(), &w, &h, &c, 0);
			
			if (!data) //Failed
			{
				stbi_image_free(data);
				BASE_CORE_ASSERT(false, "Could not load cube face image {0}", paths[i]);
			}
			else
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
				stbi_image_free(data);
			}

			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		}
			return textureId;
	}

    SkyBox::~SkyBox()
    {
        if (m_CubeTextureId)
        {
            GLCall(glDeleteTextures(1, &m_CubeTextureId));
            m_CubeTextureId = NULL;
        }
    }

    Ref<SkyBox> SkyBox::CreateSkyBox(const std::vector<std::string>& paths, Ref<render::Shader> shader)
    {
        return MakeRef<SkyBox>(paths,shader);
    }

    Ref<SkyBox> SkyBox::CreateSkyBox(const std::vector<std::string>& paths, const std::string& shader_path)
    {
        Ref<render::Shader> shader = render::Shader::CreateShader(shader_path);
        return MakeRef<SkyBox>(paths, shader);
    }
}

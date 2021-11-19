/*
*	@file QuadRender2D.cpp
*
*	The render it self it's here (cpp)
*
*	Implementation File
*/

#include "render2D.h"
#include "utils/gl_error_macro_db.h"
#include "utils/Instrumentor.h"
#include "render/gl/base_gl.h"
#include "gl/GLContext.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
namespace Base
{
	namespace utils
	{
		inline static void rotate(glm::vec3 vertices[4], float rotation, const glm::vec3& rotationCenter, const glm::vec3& axis)
		{
			const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), -rotationCenter);
			const glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), rotation, axis);
			const glm::mat4 reverseTranslationMatrix = glm::translate(glm::identity<glm::mat4>(), rotationCenter);

			for (size_t i = 0; i < 4; i++) {
				vertices[i] = glm::vec3(
					reverseTranslationMatrix * rotationMatrix * translationMatrix * glm::vec4(vertices[i], 1.0f));
			}
		}

		inline static void SampleTextureOnShader(Ref<render::Shader>& shader, int32_t max_textures, std::vector<uint32_t>& slots)
		{
			int32_t* samplers = new int32_t[max_textures];
			for (int i = 0; i < max_textures; i++)
				samplers[i] = i;
			shader->SetUniform1iv("u_Textures", max_textures, samplers);
			delete[] samplers;

			slots = std::vector<uint32_t>(max_textures);
			slots[0] = render::Texture::GetWhiteTexture()->GetId();
			for (size_t i = 1; i < max_textures; i++)
				slots[i] = 0;
		}

		inline static int32_t MaxTexturesSlots()
		{
			static int32_t MaxT = 8;
			MaxT = GLContext::GetFragmetShaderMaxTextureUnits();
			return MaxT;
		}

		inline static glm::mat4 pos_trans(const glm::vec3& pos, const glm::vec2& size)
		{
			return glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		//Find this here
		//https://www.programmersought.com/article/74894584653/
		static inline glm::vec3 bezier_3order_mix(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float_t t)
		{
			const glm::vec3 q0 = glm::mix(p0, p1, t);
			const glm::vec3 q1 = glm::mix(p1, p2, t);
			const glm::vec3 q2 = glm::mix(p2, p3, t);

			const glm::vec3 r0 = glm::mix(q0, q1, t);
			const glm::vec3 r1 = glm::mix(q1, q2, t);

			return glm::mix(r0, r1, t);
		}

		static inline glm::vec3 bezier_2order_mix(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float_t t)
		{
			const glm::vec3 q0 = glm::mix(p0, p1, t);
			const glm::vec3 q1 = glm::mix(p1, p2, t);

			return glm::mix(q0, q1, t);
		}

	}

	using namespace render;
	static glm::vec3 m_default_axis = { 0.0f,0.0f,1.0f };
	static glm::vec2 m_default_tex_coords[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f}
	};

	static glm::vec4 QuadVertexPositions[4] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	static Scope<render::ShaderLib> m_Shaders;

	static const size_t MaxQuadCount = 500;
	static const size_t MaxQuadVertexCount = MaxQuadCount * 4;
	static const size_t MaxQuadIndexCount = MaxQuadCount * 6;

	static const size_t MaxCircleCount = 100;
	static const size_t MaxCircleVertexCount = MaxCircleCount * 4;
	static const size_t MaxCircleIndexCount = MaxCircleCount * 6;

	static const size_t MaxLineCount = 100;
	static const size_t MaxLineVertexCount = MaxLineCount * 2;
	static const size_t MaxLineIndexCount = MaxLineCount * 2;

	static int32_t MaxTexture = utils::MaxTexturesSlots();

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int32_t entityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int32_t entityID;
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float_t TexIndex;

		int32_t entityID;
	};

	struct CircleVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float_t TexIndex;

		glm::vec3 LocalPosition;
		float_t Thickness;
		float_t Fade;

		int32_t entityID;
	};

	struct RenderData
	{
		//Quads
		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<IndexBuffer> QuadIB;

		QuadVertex* QuadBuffer = nullptr;
		QuadVertex* QuadBufferPtr = nullptr;
		uint32_t QuadIndexCount = 0;
		uint8_t QuadTextureSlotIndex = 1;
		std::vector<uint32_t> QuadTextureSlots;
		RenderStats QuadRenderStats;

		//Circles
		Ref<VertexArray> CircleVA;
		Ref<VertexBuffer> CircleVB;
		Ref<IndexBuffer> CircleIB; //Same as the quad one

		CircleVertex* CircleBuffer = nullptr;
		CircleVertex* CircleBufferPtr = nullptr;
		uint32_t CircleIndexCount = 0;
		uint8_t CircleTextureSlotIndex = 1;
		std::vector<uint32_t> CircleTextureSlots;
		RenderStats CircleRenderStats;

		//Lines
		Ref<VertexArray> LineVA;
		Ref<VertexBuffer> LineVB;
		uint32_t LineCount = 0;
		RenderStats LineRenderStats;

		LineVertex* LineBuffer = nullptr;
		LineVertex* LineBufferPtr = nullptr;

		//Global
		uint8_t g_WhiteTextureSlot = 0;
		
	} m_Data;

	void Render2D::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void Render2D::ClearColor(GL_ClearCommand clear)
	{
		render::GLCommands::GL_Clear(GL_ClearCommand::ClearDepthColor);
	}

	void Render2D::Init()
	{
		BASE_PROFILE_FUNCTION();

		m_Shaders = MakeScope<render::ShaderLib>();

		m_Shaders->Load("shaders/Quad.glsl");
		m_Shaders->Load("Triangle", "shaders/Quad.glsl");
		m_Shaders->Load("shaders/Circle.glsl");
		m_Shaders->Load("shaders/Line.glsl");
		m_Shaders->Load("shaders/Text.glsl");
		
		//Do Render stuff
		MaxTexture = utils::MaxTexturesSlots();

		//Quad preparation
		m_Shaders->Get("Quad")->Bind();

		m_Data.QuadBuffer = new QuadVertex[MaxQuadVertexCount];

		m_Data.QuadVA = VertexArray::CreateVertexArray();

		m_Data.QuadVB = VertexBuffer::CreateVertexBuffer(MaxQuadVertexCount * sizeof(QuadVertex));

		VertexAttribute quad_layout(m_Data.QuadVB);

		quad_layout.AddLayoutFloat(3, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));

		quad_layout.AddLayoutFloat(4, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));

		quad_layout.AddLayoutFloat(2, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords));

		quad_layout.AddLayoutFloat(1, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

		quad_layout.AddLayoutInt(1, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, entityID));

		uint32_t* indices = new uint32_t[MaxQuadIndexCount]{};
		uint32_t offset = 0;
		for (int i = 0; i < MaxQuadIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		m_Data.QuadIB = IndexBuffer::CreateIndexBuffer(_msize(indices), indices);
		delete[] indices;

		utils::SampleTextureOnShader(m_Shaders->Get("Quad"), MaxTexture, m_Data.QuadTextureSlots);

		//Circles
		m_Shaders->Get("Circle")->Bind();

		m_Data.CircleBuffer = new CircleVertex[MaxCircleVertexCount];

		m_Data.CircleVA = VertexArray::CreateVertexArray();

		m_Data.CircleVB = VertexBuffer::CreateVertexBuffer(MaxCircleVertexCount * sizeof(CircleVertex));

		VertexAttribute circle_layout(m_Data.CircleVB);
		circle_layout.AddLayoutFloat(3, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Position));

		circle_layout.AddLayoutFloat(4, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Color));

		circle_layout.AddLayoutFloat(2, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexCoords));

		circle_layout.AddLayoutFloat(1, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, TexIndex));

		circle_layout.AddLayoutFloat(3, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, LocalPosition));

		circle_layout.AddLayoutFloat(1, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Thickness));

		circle_layout.AddLayoutFloat(1, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Fade));

		circle_layout.AddLayoutInt(1, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, entityID));

		m_Data.CircleIB = m_Data.QuadIB;
		m_Data.CircleIB->Bind();
		
		utils::SampleTextureOnShader(m_Shaders->Get("Circle"), MaxTexture, m_Data.CircleTextureSlots);

		//Lines
		m_Shaders->Get("Line")->Bind();
		m_Data.LineBuffer = new LineVertex[MaxLineVertexCount];

		m_Data.LineVA = VertexArray::CreateVertexArray();

		m_Data.LineVB = VertexBuffer::CreateVertexBuffer(MaxLineVertexCount * sizeof(LineVertex));

		VertexAttribute line_layout(m_Data.LineVB);
		line_layout.AddLayoutFloat(3, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position));

		line_layout.AddLayoutFloat(4, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color));

		line_layout.AddLayoutInt(1, sizeof(LineVertex), (const void*)offsetof(LineVertex, entityID));

		m_Data.LineVA->Unbind();

		//GLOBAL GL CONFIGS
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_LINE_SMOOTH));
		//GLCall(glEnable(GL_MULTISAMPLE));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void Render2D::AddShader(const std::string& path)
	{
		m_Shaders->Load(path);
	}

	void Render2D::BeginBatch()
	{
		m_Data.QuadBufferPtr = m_Data.QuadBuffer;
		m_Data.QuadIndexCount = 0;
		m_Data.QuadTextureSlotIndex = 1;
		m_Data.QuadRenderStats.DrawCount = 0;

		m_Data.CircleBufferPtr = m_Data.CircleBuffer;
		m_Data.CircleIndexCount = 0;
		m_Data.CircleTextureSlotIndex = 1;
		m_Data.CircleRenderStats.DrawCount = 0;

		m_Data.LineBufferPtr = m_Data.LineBuffer;
		m_Data.LineCount = 0;
		m_Data.LineRenderStats.DrawCount = 0;
	}

	void Render2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		//BASE_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		m_Shaders->Get("Quad")->Bind();
		m_Shaders->Get("Quad")->SetUniformMat4f("u_ViewProj", viewProj);

		m_Shaders->Get("Circle")->Bind();
		m_Shaders->Get("Circle")->SetUniformMat4f("u_ViewProj", viewProj);

		m_Shaders->Get("Line")->Bind();
		m_Shaders->Get("Line")->SetUniformMat4f("u_ViewProj", viewProj);
	}

	void Render2D::BeginScene(const EditorCamera& camera)
	{
		auto viewProj = camera.GetViewProjection();

		m_Shaders->Get("Quad")->Bind();
		m_Shaders->Get("Quad")->SetUniformMat4f("u_ViewProj", viewProj);

		m_Shaders->Get("Circle")->Bind();
		m_Shaders->Get("Circle")->SetUniformMat4f("u_ViewProj", viewProj);

		m_Shaders->Get("Line")->Bind();
		m_Shaders->Get("Line")->SetUniformMat4f("u_ViewProj", viewProj);
	}

	void Render2D::EndBatch()
	{
		if (m_Data.QuadIndexCount)
		{
			//End Quads
			GLsizeiptr size = (uint8_t*)m_Data.QuadBufferPtr - (uint8_t*)m_Data.QuadBuffer;
			m_Data.QuadVB->Bind();
			m_Data.QuadVB->SubData(size,m_Data.QuadBuffer);
		}

		if (m_Data.CircleIndexCount)
		{
			//End Circles
			GLsizeiptr size = (uint8_t*)m_Data.CircleBufferPtr - (uint8_t*)m_Data.CircleBuffer;
			m_Data.CircleVB->Bind();
			m_Data.CircleVB->SubData(size, m_Data.CircleBuffer);
		}

		if (m_Data.LineCount)
		{
			//End Lines
			GLsizeiptr size = (uint8_t*)m_Data.LineBufferPtr - (uint8_t*)m_Data.LineBuffer;
			m_Data.LineVB->Bind();
			m_Data.LineVB->SubData(size, m_Data.LineBuffer);
		}
	}

	void Render2D::Flush()
	{
		if (m_Data.QuadIndexCount)
		{
			//Draw Quads
			m_Shaders->Get("Quad")->Bind();

			for (size_t i = 0; i < m_Data.QuadTextureSlotIndex; i++)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + i));
				GLCall(glBindTexture(GL_TEXTURE_2D, m_Data.QuadTextureSlots[i]));
			}

			m_Data.QuadVA->Bind();

			GLCall(glDrawElements(GL_TRIANGLES, m_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr));
			
			m_Data.QuadTextureSlotIndex = 1;
			m_Data.QuadIndexCount = 0;
			m_Data.QuadRenderStats.DrawCount++;
		}

		if (m_Data.CircleIndexCount)
		{
			//Draw Circles
			m_Shaders->Get("Circle")->Bind();

			for (size_t i = 0; i < m_Data.CircleTextureSlotIndex; i++)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + i));
				GLCall(glBindTexture(GL_TEXTURE_2D, m_Data.CircleTextureSlots[i]));
			}

			m_Data.CircleVA->Bind();

			GLCall(glDrawElements(GL_TRIANGLES, m_Data.CircleIndexCount, GL_UNSIGNED_INT, nullptr));

			m_Data.CircleTextureSlotIndex = 1;
			m_Data.CircleIndexCount = 0;
			m_Data.CircleRenderStats.DrawCount++;
		}

		if (m_Data.LineCount)
		{
			//Draw Lines
			m_Shaders->Get("Line")->Bind();

			m_Data.LineVA->Bind();

			GLCall(glDrawArrays(GL_LINES, 0 , m_Data.LineCount));

			m_Data.LineCount = 0;
			m_Data.LineRenderStats.DrawCount++;
		}
	}

	void Render2D::Dispose()
	{
		delete m_Shaders.release();

		m_Data.QuadVA.reset();
		m_Data.CircleVA.reset();
		m_Data.LineVA.reset();

		m_Data.QuadVB.reset();
		m_Data.CircleVB.reset();
		m_Data.LineVB.reset();

		m_Data.QuadIB.reset();
		m_Data.CircleIB.reset();

		if (m_Data.QuadBuffer)
			delete m_Data.QuadBuffer;
		if (m_Data.CircleBuffer)
			delete m_Data.CircleBuffer;
		if (m_Data.LineBuffer)
			delete m_Data.LineBuffer;

		Texture::DeleteWhiteTexture();
	}

	const Ref<Shader> Render2D::GetQuadShader()
	{
		return m_Shaders->Get("Quad");
	}

	const Ref<Shader> Render2D::GetLineShader()
	{
		return m_Shaders->Get("Line");
	}

	RenderStats Render2D::GetQuadStats()
	{
		return m_Data.QuadRenderStats;
	}

	RenderStats Render2D::GetCircleStats()
	{
		return m_Data.CircleRenderStats;
	}

	RenderStats Render2D::GetLineStats()
	{
		return m_Data.LineRenderStats;
	}

	const Ref<Shader> Render2D::GetCircleShader()
	{
		return m_Shaders->Get("Circle");
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int32_t entityID, float_t rotation, const glm::vec3& axis)
	{
		DrawQuad(utils::pos_trans(position, size), color, entityID);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4], int32_t entityID,float_t rotation, const glm::vec3& axis)
	{
		DrawQuad(utils::pos_trans(position, size), color,entityID);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture, int32_t entityID,const glm::vec4& color,float_t rotation, const glm::vec3& axis)
	{
		DrawQuad(utils::pos_trans(position, size), texture, entityID ,color);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture, int32_t entityID,const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		DrawQuad(utils::pos_trans(position, size), sub_texture, entityID ,color);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,int32_t entityID)
	{
		if (m_Data.QuadIndexCount >= MaxQuadIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		for (size_t i = 0; i < 4; i++)
		{
			m_Data.QuadBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.QuadBufferPtr->Color = color;
			m_Data.QuadBufferPtr->TexCoords = m_default_tex_coords[i];
			m_Data.QuadBufferPtr->TexIndex = m_Data.g_WhiteTextureSlot;
			m_Data.QuadBufferPtr->entityID = entityID;
			m_Data.QuadBufferPtr++;
		}
		m_Data.QuadIndexCount += 6;
		m_Data.QuadRenderStats.DrawCount++;
		m_Data.QuadRenderStats.TotalCount++;
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color[4], int32_t entityID)
	{
		if (m_Data.QuadIndexCount >= MaxQuadIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		for (size_t i = 0; i < 4 ; i++)
		{
			m_Data.QuadBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.QuadBufferPtr->Color = color[i];
			m_Data.QuadBufferPtr->TexCoords = m_default_tex_coords[i];
			m_Data.QuadBufferPtr->TexIndex = m_Data.g_WhiteTextureSlot;
			m_Data.QuadBufferPtr->entityID = entityID;
			m_Data.QuadBufferPtr++;
		}
		m_Data.QuadIndexCount += 6;
		m_Data.QuadRenderStats.DrawCount++;
		m_Data.QuadRenderStats.TotalCount++;
	}

	void Render2D::DrawQuad(const glm::mat4& transform, Ref<Texture> texture, int32_t entityID, const glm::vec4& color)
	{
		if (m_Data.QuadIndexCount >= MaxQuadIndexCount || m_Data.QuadTextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		int8_t texture_index = 0;
		if (texture->GetId())
			for (int8_t i = 1; i < m_Data.QuadTextureSlotIndex; i++)
			{
				if (m_Data.QuadTextureSlots[i] == texture->GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (texture->GetId())
			if (!texture_index)
			{
				texture_index = m_Data.QuadTextureSlotIndex;
				m_Data.QuadTextureSlots[m_Data.QuadTextureSlotIndex] = texture->GetId();
				m_Data.QuadTextureSlotIndex++;
			}

		for (size_t i = 0; i < 4; i++)
		{
			m_Data.QuadBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.QuadBufferPtr->Color = color;
			m_Data.QuadBufferPtr->TexCoords = m_default_tex_coords[i];
			m_Data.QuadBufferPtr->TexIndex = texture_index;
			m_Data.QuadBufferPtr->entityID = entityID;
			m_Data.QuadBufferPtr++;
		}

		m_Data.QuadIndexCount += 6;
		m_Data.QuadRenderStats.DrawCount++;
		m_Data.QuadRenderStats.TotalCount++;
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, int32_t entityID, const glm::vec4& color)
	{
		if (m_Data.QuadIndexCount >= MaxQuadIndexCount || m_Data.QuadTextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		auto coords = sub_texture.GetTexCoords();

		int8_t texture_index = 0;
		if (sub_texture.GetId())
			for (int8_t i = 1; i < m_Data.QuadTextureSlotIndex; i++)
			{
				if (m_Data.QuadTextureSlots[i] == sub_texture.GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (sub_texture.GetId())
			if (!texture_index)
			{
				texture_index = m_Data.QuadTextureSlotIndex;
				m_Data.QuadTextureSlots[m_Data.QuadTextureSlotIndex] = sub_texture.GetId();
				m_Data.QuadTextureSlotIndex++;
			}

		for (size_t i = 0; i < 4; i++)
		{
			m_Data.QuadBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.QuadBufferPtr->Color = color;
			m_Data.QuadBufferPtr->TexCoords = coords[i];
			m_Data.QuadBufferPtr->TexIndex = texture_index;
			m_Data.QuadBufferPtr->entityID = entityID;
			m_Data.QuadBufferPtr++;
		}

		m_Data.QuadIndexCount += 6;
		m_Data.QuadRenderStats.DrawCount++;
		m_Data.QuadRenderStats.TotalCount++;
	}

	//LINE RENDERING --------------------------------------------------------------------------------------------------------------------------------------------------

	void Render2D::DrawOutLineQuad(const glm::mat4& transform, const glm::vec4& color, int32_t entityID)
	{
		glm::vec3 quadLines[4];
		for (int i = 0; i < 4; i++)
			quadLines[i] = transform * QuadVertexPositions[i];

		DrawLine(quadLines[0], quadLines[1], color,entityID);
		DrawLine(quadLines[1], quadLines[2], color,entityID);
		DrawLine(quadLines[2], quadLines[3], color,entityID);
		DrawLine(quadLines[3], quadLines[0], color,entityID);
	}

	void Render2D::DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color, int32_t entityID)
	{
		if (m_Data.LineCount >= MaxLineIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		m_Data.LineBufferPtr->Position = origin;
		m_Data.LineBufferPtr->Color = color;
		m_Data.LineBufferPtr->entityID = entityID;
		m_Data.LineBufferPtr++;

		m_Data.LineBufferPtr->Position = dest;
		m_Data.LineBufferPtr->Color = color;
		m_Data.LineBufferPtr->entityID = entityID;
		m_Data.LineBufferPtr++;

		m_Data.LineCount += 2;
		m_Data.LineRenderStats.DrawCount++;
		m_Data.LineRenderStats.TotalCount++;
	}

	void Render2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, int32_t entityID, float_t precision)
	{
		glm::vec3* acting_origin = &const_cast<glm::vec3&>(origin);
		glm::vec3 acting_interpol{};
		for (float i = precision; i < 1; i += precision)
		{
			acting_interpol = utils::bezier_3order_mix(origin, p1, p2, dest, i);
			DrawLine(*acting_origin, acting_interpol, color,entityID);
			acting_origin = &acting_interpol;
		}
		DrawLine(*acting_origin, dest, color,entityID); // Connect the last line with the destination
	}

	void Render2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, int32_t entityID, float_t precision)
	{
		glm::vec3* acting_origin = &const_cast<glm::vec3&>(origin);
		glm::vec3 acting_interpol{};
		for (float i = precision; i < 1; i += precision)
		{
			acting_interpol = utils::bezier_2order_mix(origin, p1, dest, i);
			DrawLine(*acting_origin, acting_interpol, color, entityID);
			acting_origin = &acting_interpol;
		}
		DrawLine(*acting_origin, dest, color, entityID); // Connect the last line with the destination
	}

	//CIRCLE RENDERING OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const glm::vec4& color, int32_t entityID, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(utils::pos_trans(position, quad_size), radius, fade, thick, color, entityID);
	}

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, Ref<Texture> texture, int32_t entityID, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(utils::pos_trans(position, quad_size), radius, fade, thick, texture, entityID, color);
	}

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, float_t fade, float_t thick, const SubTexture& sub_texture, int32_t entityID, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quad_size = { radius, radius,position.z };
		DrawCircle(utils::pos_trans(position, quad_size), radius, fade, thick, sub_texture, entityID, color);
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const glm::vec4& color, int32_t entityID)
	{
		if (m_Data.CircleIndexCount >= MaxCircleIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}
		static constexpr float tex_index = 0;

		for (uint8_t i = 0; i < 4; i++)
		{
			m_Data.CircleBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.CircleBufferPtr->Color = color;
			m_Data.CircleBufferPtr->TexCoords = m_default_tex_coords[i];
			m_Data.CircleBufferPtr->TexIndex = tex_index;
			m_Data.CircleBufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
			m_Data.CircleBufferPtr->Thickness = thick;
			m_Data.CircleBufferPtr->Fade = fade;
			m_Data.CircleBufferPtr->entityID = entityID;
			m_Data.CircleBufferPtr++;
		}

		m_Data.CircleIndexCount += 6;
		m_Data.CircleRenderStats.DrawCount++;
		m_Data.CircleRenderStats.TotalCount++;
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, Ref<Texture> texture, int32_t entityID, const glm::vec4& color)
	{
		if (m_Data.CircleIndexCount >= MaxCircleIndexCount || m_Data.CircleTextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		int8_t texture_index = 0;
		if (texture->GetId())
			for (int8_t i = 1; i < m_Data.CircleTextureSlotIndex; i++)
			{
				if (m_Data.CircleTextureSlots[i] == texture->GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (texture->GetId())
			if (!texture_index)
			{
				texture_index = m_Data.CircleTextureSlotIndex;
				m_Data.CircleTextureSlots[m_Data.CircleTextureSlotIndex] = texture->GetId();
				m_Data.CircleTextureSlotIndex++;
			}

		for (size_t i = 0; i < 4; i++)
		{
			m_Data.CircleBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.CircleBufferPtr->Color = color;
			m_Data.CircleBufferPtr->TexCoords = m_default_tex_coords[i];
			m_Data.CircleBufferPtr->TexIndex = texture_index;
			m_Data.CircleBufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
			m_Data.CircleBufferPtr->Thickness = thick;
			m_Data.CircleBufferPtr->Fade = fade;
			m_Data.CircleBufferPtr->entityID = entityID;
			m_Data.CircleBufferPtr++;
		}

		m_Data.CircleIndexCount += 6;
		m_Data.CircleRenderStats.DrawCount++;
		m_Data.CircleRenderStats.TotalCount++;
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, float_t fade, float_t thick, const SubTexture& sub_texture, int32_t entityID, const glm::vec4& color)
	{
		if (m_Data.CircleIndexCount >= MaxCircleIndexCount || m_Data.CircleTextureSlotIndex > MaxTexture - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		const glm::vec2* coords = sub_texture.GetTexCoords();
		BASE_CORE_ASSERT(coords, "GetTexCoords return nullptr");

		int8_t texture_index = 0;
		if (sub_texture.GetId())
			for (int8_t i = 1; i < m_Data.CircleTextureSlotIndex; i++)
			{
				if (m_Data.CircleTextureSlots[i] == sub_texture.GetId())
				{
					texture_index = i;
					break;
				}
			}

		if (sub_texture.GetId())
			if (!texture_index)
			{
				texture_index = m_Data.CircleTextureSlotIndex;
				m_Data.CircleTextureSlots[m_Data.CircleTextureSlotIndex] = sub_texture.GetId();
				m_Data.CircleTextureSlotIndex++;
			}

		for (size_t i = 0; i < 4; i++)
		{
			m_Data.CircleBufferPtr->Position = transform * QuadVertexPositions[i];
			m_Data.CircleBufferPtr->Color = color;
			m_Data.CircleBufferPtr->TexCoords = coords[i];
			m_Data.CircleBufferPtr->TexIndex = texture_index;
			m_Data.CircleBufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
			m_Data.CircleBufferPtr->Thickness = thick;
			m_Data.CircleBufferPtr->Fade = fade;
			m_Data.CircleBufferPtr->entityID = entityID;
			m_Data.CircleBufferPtr++;
		}
		m_Data.CircleIndexCount += 6;
		m_Data.CircleRenderStats.DrawCount++;
		m_Data.CircleRenderStats.TotalCount++;
	}

	void Render2D::SetLineWidth(float_t thickness)
	{
		GLCall(glLineWidth(thickness));
	}
}
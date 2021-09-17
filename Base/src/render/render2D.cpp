/*
*	@file QuadRender2D.cpp
*	
*	The render it self it's here (cpp)
*
*	Implementation File
*/
#include "render2D.h"
#include "geo_renders/RenderTemplate.h"
#include "glm/gtc/matrix_transform.hpp"
#include "gl/glew.h"

namespace Base
{
namespace render
{
	glm::vec3 Render2D::m_default_axis = { 0.0f,0.0f,1.0f };
	glm::mat4 Render2D::m_Transform = glm::ortho(0, 800, 0, 600,1,-10); //TODO: remove
	Ref<QuadRender2D> Render2D::m_QuadRender;
	Ref<CircleRender> Render2D::m_CircleRender;
	Ref<LineRender2D> Render2D::m_LineRender;
	Ref<QuadRender2D> Render2D::m_TextRender;
	Ref<TriRender> Render2D::m_TriRender;
	ShaderLib Render2D::m_Shaders;

	void Render2D::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r,color.g,color.b,color.a));
	}

	void Render2D::ClearColor(GLbitfield clear)
	{
		GLCall(glClear(clear));
	}

	void Render2D::Init()
	{

		m_Shaders.Load("shaders/Quad.glsl");
		//m_Shaders.Load("Triangle", "shaders/Quad.glsl");
		m_Shaders.Load("shaders/Circle.glsl");
		m_Shaders.Load("shaders/Line.glsl");
		m_Shaders.Load("shaders/Text.glsl");

		m_QuadRender	= std::make_shared<QuadRender2D>(m_Shaders.Get("Quad"));
		m_LineRender	= std::make_shared<LineRender2D>(m_Shaders.Get("Line"));
		m_CircleRender	= std::make_shared<CircleRender>(m_Shaders.Get("Circle"));
		m_TextRender	= std::make_shared<QuadRender2D>(m_Shaders.Get("Text"));
		m_TriRender		= std::make_shared<TriRender>(m_Shaders.Get("Quad")); //TODO: Test to se if works with the same shader

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		//GLCall(glEnable(GL_MULTISAMPLE));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void Render2D::AddShader(const std::string& path)
	{
		m_Shaders.Load(path);
	}
	
	void Render2D::BeginBatch()
	{
		m_LineRender->BeginBatch();
		m_QuadRender->BeginBatch();
		m_CircleRender->BeginBatch();
		m_TextRender->BeginBatch();
		m_TriRender->BeginBatch();
	}

	void Render2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() 
			* glm::inverse(transform);
			
		m_LineRender->BeginScene(viewProj, m_Transform);
		m_QuadRender->BeginScene(viewProj, m_Transform);
		m_CircleRender->BeginScene(viewProj, m_Transform);
		m_TextRender->BeginScene(viewProj, m_Transform);
		m_TriRender	->BeginScene(viewProj, m_Transform);
	}

	void Render2D::EndBatch()
	{
		m_LineRender->EndBatch();
		m_QuadRender->EndBatch();
		m_CircleRender->EndBatch();
		m_TextRender->EndBatch();
		m_TriRender->EndBatch();
	}

	void Render2D::Flush()
	{
		m_LineRender->Flush();
		m_QuadRender->Flush();
		m_CircleRender->Flush();
		m_TextRender->Flush();
		m_TriRender->Flush();
	}

	void Render2D::Dispose()
	{
		m_QuadRender->Dispose();
		m_CircleRender->Dispose();
		m_LineRender->Dispose();
		m_TextRender->Dispose();
		m_TriRender->Dispose();
	}

	void Render2D::Sort()
	{
		//TODO: implement something
	}

	const Ref<Shader> Render2D::GetQuadShader()
	{
		return m_QuadRender->GetShader();
	}

	const Ref<Shader> Render2D::GetLineShader()
	{
		return m_LineRender->GetShader();
	}

	const Ref<Shader> Render2D::GetCircleShader()
	{
		return m_CircleRender->GetShader();
	}

	const Ref<Shader> Render2D::GetTextShader()
	{
		return m_TextRender->GetShader();
	}

	const Ref<Shader> Render2D::GetTriShader()
	{
		return m_TriRender->GetShader();
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(position, size, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color[4], float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(position, size, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture, const glm::vec4& color,float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(position, size, texture, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SubTexture& sub_texture, const glm::vec4& color,float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(position, size, sub_texture, color,rotation, axis);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(transform, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color[4], float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(transform, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(transform, texture, color, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::mat4& transform, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender->DrawQuad(transform, sub_texture, color, rotation, axis);
	}

	void Render2D::DrawText(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_TextRender->DrawQuad(position, size, texture, color, rotation, axis);
	}

	void Render2D::DrawOutLineQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
				   float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quads[4] = {
			{position.x,position.y, position.z},
			{position.x + size.x, position.y, position.z},
			{position.x + size.x,position.y + size.y, position.z},
			{position.x,position.y + size.y, position.z}
		};

		if (rotation)
			rotate(quads, rotation, { position.x + (size.x / 2), position.y + (size.y / 2), position.z }, axis);

		const glm::vec3& left_bottom  = quads[0];
		const glm::vec3& right_bottom = quads[1];
		const glm::vec3& right_top	 =  quads[2];
		const glm::vec3& left_top	 =  quads[3];

		//Bottom Line
		DrawLine(left_bottom, right_bottom, color);
		//Left Line
		DrawLine(left_bottom, left_top, color);
		//Top Line
		DrawLine(left_top, right_top, color);
		//Right Line
		DrawLine(right_bottom, right_top, color);
	}

	void Render2D::DrawLine(const glm::vec3& origin, const glm::vec3& dest, const glm::vec4& color)
	{
		m_LineRender->DrawLine(origin, dest, color);
	}

	void Render2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& dest, const glm::vec4& color, float_t precision)
	{
		m_LineRender->DrawCurveLine(origin, p1, p2, dest,color,precision);
	}

	void Render2D::DrawCurveLine(const glm::vec3& origin, const glm::vec3& p1, const glm::vec3& dest, const glm::vec4& color, float_t precision)
	{
		m_LineRender->DrawCurveLine(origin, p1, dest, color, precision);
	}

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, bool fill, float thick, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(position, radius, fill, thick, color, rotation, axis);
	}

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(position, radius, fill, thick, texture ,color, rotation, axis);
	}

	void Render2D::DrawCircle(const glm::vec3& position, float_t radius, float fill, float thick, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(position, radius, fill, thick, sub_texture, color, rotation, axis);
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, bool fill, float thick, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(transform, radius, fill, thick, color, rotation, axis);
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, Ref<Texture> texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(transform, radius, fill, thick, texture, color, rotation, axis);
	}

	void Render2D::DrawCircle(const glm::mat4& transform, float_t radius, float fill, float thick, const SubTexture& sub_texture, const glm::vec4& color, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender->DrawCircle(transform, radius, fill, thick, sub_texture, color, rotation, axis);
	}

	void Render2D::DrawTriangle(const glm::vec3 points[3], const glm::vec4& color)
	{
		m_TriRender->DrawTriangle(points, color);
	}

	void Render2D::DrawTriangle(const glm::vec3 points[3], const glm::vec4 color[3])
	{
		m_TriRender->DrawTriangle(points, color);
	}

	void Render2D::LineBeginBatch()
	{
		m_LineRender->BeginBatch();
	}

	void Render2D::LineEndBatch()
	{
		m_LineRender->EndBatch();
	}

	void Render2D::LineFlush()
	{
		m_LineRender->Flush();
	}

	void Render2D::QuadBeginBatch()
	{
		m_QuadRender->BeginBatch();
	}

	void Render2D::QuadEndBatch()
	{
		m_QuadRender->EndBatch();
	}

	void Render2D::QuadFlush()
	{
		m_QuadRender->Flush();
	}

	void Render2D::CircleBeginBatch()
	{
		m_CircleRender->BeginBatch();
	}

	void Render2D::CircleEndBatch()
	{
		m_CircleRender->EndBatch();
	}

	void Render2D::CircleFlush()
	{
		m_CircleRender->Flush();
	}

	void Render2D::TextBeginBatch()
	{
		m_TextRender->BeginBatch();
	}

	void Render2D::TextEndBatch()
	{
		m_TextRender->EndBatch();
	}

	void Render2D::TextFlush()
	{
		m_TextRender->Flush();
	}

	void Render2D::TriBeginBatch()
	{
		m_TriRender->BeginBatch();
	}

	void Render2D::TriEndBatch()
	{
		m_TriRender->EndBatch();
	}

	void Render2D::TriFlush()
	{
		m_TriRender->Flush();
	}

	//Deprecated
	void Render2D::DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick, float_t layer)
	{
		m_QuadRender->DrawQuadLine(origin, dest, color, thick, layer);
	}
}
}
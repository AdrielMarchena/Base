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
#include <iostream>
namespace en
{
namespace render
{
	glm::vec3 Render2D::m_default_axis = { 0.0f,0.0f,1.0f };
	

	Render2D::Render2D(const char* quad_vs, const char* quad_fs,
		const char* line_vs, const char* line_fs,
		const char* circle_vs, const char* circle_fs)
		:m_QuadRender(quad_vs,quad_fs), 
		m_LineRender(line_vs,line_fs),
		m_CircleRender(circle_vs,circle_fs)
	{
	}

	Render2D::~Render2D()
	{
	}
	
	void Render2D::BeginBatch()
	{
		m_LineRender.BeginBatch();
		m_QuadRender.BeginBatch();
		m_CircleRender.BeginBatch();
	}

	void Render2D::EndBatch()
	{
		m_LineRender.EndBatch();
		m_QuadRender.EndBatch();
		m_CircleRender.EndBatch();
	}

	void Render2D::Flush()
	{
		m_LineRender.Flush();
		m_QuadRender.Flush();
		m_CircleRender.Flush();
	}

	const Shader& Render2D::GetQuadShader()
	{
		return m_QuadRender.GetShader();
	}

	const Shader& Render2D::GetLineShader()
	{
		return m_LineRender.GetShader();
	}

	const Shader& Render2D::GetCircleShader()
	{
		return m_CircleRender.GetShader();
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender.DrawQuad(position, size, color, layer, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color[4], float_t layer, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender.DrawQuad(position, size, color, layer, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender.DrawQuad(position, size, texture, layer, rotation, axis);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& sub_texture, float_t layer, float_t rotation, const glm::vec3& axis)
	{
		m_QuadRender.DrawQuad(position, size, sub_texture, layer, rotation, axis);
	}

	void Render2D::DrawOutLineQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
				   float_t layer, float_t rotation, const glm::vec3& axis)
	{
		glm::vec3 quads[4] = {
			{position.x,position.y,-layer},
			{position.x + size.x, position.y,-layer},
			{position.x + size.x,position.y + size.y,-layer},
			{position.x,position.y + size.y,-layer}
		};

		if (rotation)
			Render::rotate(quads, rotation, { position.x + (size.x / 2), position.y + (size.y / 2), layer }, axis);

		const glm::vec2 left_bottom  = { quads[0].x,quads[0].y };
		const glm::vec2 right_bottom = { quads[1].x,quads[1].y };
		const glm::vec2 right_top	 = { quads[2].x,quads[2].y };
		const glm::vec2 left_top	 = { quads[3].x,quads[3].y };

		//Bottom Line
		DrawLine(left_bottom, right_bottom, color, layer);
		//Left Line
		DrawLine(left_bottom, left_top, color ,layer);
		//Top Line
		DrawLine(left_top, right_top, color, layer);
		//Right Line
		DrawLine(right_bottom, right_top, color, layer);
	}

	void Render2D::DrawLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t layer)
	{
		m_LineRender.DrawLine(origin, dest, color, layer);
	}

	void Render2D::DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& dest, const glm::vec4& color, float_t precision ,float_t layer)
	{
		m_LineRender.DrawCurveLine(origin, p1, p2, dest,color,precision,layer);
	}

	void Render2D::DrawCurveLine(const glm::vec2& origin, const glm::vec2& p1, const glm::vec2& dest, const glm::vec4& color, float_t precision, float_t layer)
	{
		m_LineRender.DrawCurveLine(origin, p1, dest, color, precision, layer);
	}

	void Render2D::DrawCircle(const glm::vec2& position, float_t radius, const glm::vec4& color, bool fill, float thick,float_t layer, float_t rotation, const glm::vec3& axis)
	{
		m_CircleRender.DrawCircle(position, radius, fill, thick ,color, layer, rotation, axis);
	}

	void Render2D::LineBeginBatch()
	{
		m_LineRender.BeginBatch();
	}

	void Render2D::LineEndBatch()
	{
		m_LineRender.EndBatch();
	}

	void Render2D::LineFlush()
	{
		m_LineRender.Flush();
	}

	void Render2D::QuadBeginBatch()
	{
		m_QuadRender.BeginBatch();
	}

	void Render2D::QuadEndBatch()
	{
		m_QuadRender.EndBatch();
	}

	void Render2D::QuadFlush()
	{
		m_QuadRender.Flush();
	}

	void Render2D::CircleBeginBatch()
	{
		m_CircleRender.BeginBatch();
	}

	void Render2D::CircleEndBatch()
	{
		m_CircleRender.EndBatch();
	}

	void Render2D::CircleFlush()
	{
		m_CircleRender.Flush();
	}

	//Deprecated
	void Render2D::DrawQuadLine(const glm::vec2& origin, const glm::vec2& dest, const glm::vec4& color, float_t thick, float_t layer)
	{
		m_QuadRender.DrawQuadLine(origin, dest, color, thick, layer);
	}
}
}
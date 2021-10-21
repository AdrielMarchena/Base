#include "Maze.h"

#include "scene/Components.h"
#include "imgui.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "utils/Generic.h"
#include "Log.h"
#include "utils/Instrumentor.h"

#include "Base/Base.h"
void Maze::OnAttach()
{
	BASE_PROFILE_FUNCTION();
	
	APP_TRACE("Attach Maze");
	m_Scene = Base::MakeScope<Base::Scene>();
	m_Camera = m_Scene->CreateEntity("Main_Camera");

	auto& cam = m_Camera.AddComponent<Base::CameraComponent>();
	cam.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);

	m_TexQuad = m_Scene->CreateEntity("Quad");
	auto& spr = m_TexQuad.AddComponent<Base::SpriteComponent>();
	spr.Color = Color::Base_Color;
}

void Maze::OnUpdate(const Base::UpdateArgs& args)
{
	BASE_PROFILE_FUNCTION();
	if (!Base::WindowProps().minimized)
		m_Scene->OnUpdate(args);
}

void Maze::OnRender()
{
}

void Maze::OnImGui()
{
}

void Maze::Dispose()
{
}

void Maze::OnResize(const Base::ResizeArgs& args)
{
	auto& cam = m_Camera.GetComponent<Base::CameraComponent>();
	cam.Camera.SetViewportSize(args.new_w, args.new_h);
}

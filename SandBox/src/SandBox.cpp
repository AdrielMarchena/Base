#include "SandBox.h"

#include "scene/Components.h"
#include "utils/RandomList.h"
#include "utils/Files.h"
//Make sense?

using Mouse = Base::input::Mouse;
using Keyboard = Base::input::Keyboard;
static bool m_click = false;

inline static float Distance(const glm::vec2& point_a, const glm::vec2& point_b)
{
	return float(std::sqrt(std::pow(point_b.x - point_a.x, 2) + std::pow(point_b.y - point_a.y, 2)));
}

static Base::Entity CreateBall(Base::Scene* scene, Base::Ref<Base::render::Texture> texture);

class ScriptExample : public Base::ScriptableEntity
{
public:
	void OnCreate()
	{
	}

	void OnUpdate(const Base::UpdateArgs& args)
	{
		auto& comp = GetComponent<Base::CircleComponent>();
		auto& pos = GetComponent<Base::TransformComponent>().Transform;
		auto& vel = GetComponent<Base::VelocityComponent>().Velocity;
		auto& rad = comp.Radius;

		if (Distance({ BASE_XPOS(pos),BASE_YPOS(pos) }, Base::input::Mouse::m_pos(600.0f)) <= rad)
		{
			if (m_click)
			{
				auto& tag = GetComponent<Base::TagComponent>().Tag;
				APP_INFO("{0}:\n\t   Position X:{1} Y:{2}\n\t   Radius {3}\n\t Velocity VX:{4} VY:{5}\n",
					tag, BASE_XPOS(pos), BASE_YPOS(pos),rad,vel.x,vel.y);
			}
		}

		BASE_XPOS(pos) += vel.x * args.dt;
		BASE_YPOS(pos) += vel.y * args.dt;

		if (BASE_XPOS(pos) + rad > 800.0f)
			vel.x = -fabs(vel.x);
		if (BASE_XPOS(pos) - rad < 0.0f)
			vel.x = fabs(vel.x);
		if (BASE_YPOS(pos) + rad > 600.0f)
			vel.y = -fabs(vel.y);
		if (BASE_YPOS(pos) - rad < 0.0f)
			vel.y = fabs(vel.y);
	}
};

static Base::Entity CreateBall(Base::Scene* scene,Base::Ref<Base::render::Texture> texture)
{
	static unsigned int i = 0;
	static float norm = 1.0f / 256.0f;
	auto ball = scene->CreateEntity("Ball_" + std::to_string(i));
	auto& Circle = ball.AddComponent<Base::CircleComponent>();
	auto& Trans = ball.GetComponent<Base::TransformComponent>().Transform;
	Circle.Radius = ((P_random() + 10)) % 60;
	BASE_XPOS(Trans) = ((P_random() + int(Circle.Radius)) << 2) % 800 - Circle.Radius;
	BASE_YPOS(Trans) = ((P_random() + int(Circle.Radius)) << 2) % 600 - Circle.Radius;
	BASE_ZPOS(Trans) = 0.0f;
	Trans = glm::scale(Trans, glm::vec3(Circle.Radius*2, Circle.Radius * 2,0.0f));
	ball.AddComponent<Base::TextureComponent>(texture);
	auto& vel = ball.AddComponent<Base::VelocityComponent>().Velocity;
	vel.x = vel.y = ((P_random() << 2));
	vel.x = vel.x - ((int(Circle.Radius) << 3));
	vel.y = vel.y - ((int(Circle.Radius) << 3));
	vel.z = 1.0f;
	ball.AddComponent<Base::NativeScriptComponent>().Bind<ScriptExample>();
	i++;
	return ball;
}

SandBox::SandBox()
	:Base::windowing::Window()
{
}

SandBox::~SandBox()
{
}

void SandBox::OnAttach()
{
	m_Textures = Base::render::Texture::LoadAsyncTexture(Base::utils::Files::GetPairText("images/"));

	//m_Balls.reserve(1000);
	m_Scene = std::make_unique<Base::Scene>();
	float norm = 1.0f / 256.0f;
	
	m_Balls.push_back(CreateBall(m_Scene.get(),m_Textures["test"]));
	m_Balls.push_back(CreateBall(m_Scene.get(),m_Textures["test"]));

	m_TexQuad = m_Scene->CreateEntity("Thing");
	m_TexQuad.AddComponent<Base::TextureComponent>(m_Textures["MuriloDude"]);
	auto& pos = m_TexQuad.GetComponent<Base::TransformComponent>().Transform;
	BASE_XPOS(pos) = 400;
	BASE_YPOS(pos) = 300;
	BASE_ZPOS(pos) = 0.0f;
	pos = glm::scale(pos, glm::vec3(50.0f, -50.0f, 0.0f)); //Negative flip in some way

	//Camera
	glm::mat4 camera_transform = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f }) *
		glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));
	
	m_Camera = m_Scene->CreateEntity("Main_Camera");
	m_Camera.AddComponent<Base::CameraComponent>(camera_transform);
	
	m_Scene->SceneBegin();
	Window::OnAttach();
}

void SandBox::OnUpdate(const Base::UpdateArgs& args)
{
	if (Base::input::Mouse::isClicked(BASE_MOUSE_BUTTON_2))
		m_click = true;
	if (Base::input::Mouse::isClicked(BASE_MOUSE_BUTTON_1))
	{
		//size_t s = m_Balls.size();
		//for (int i = 0; i < s; i++)
		{
			auto ball = CreateBall(m_Scene.get(), m_Textures["test"]);
			m_Balls.push_back(ball);
			m_Scene->StartNativeScript(ball);
		}
	}
	m_Scene->OnUpdate(args);
	m_click = false;
}

void SandBox::OnRender()
{
}

void SandBox::OnImGui()
{
}

void SandBox::Dispose()
{
	m_Scene->SceneEnd();
}

#include "SandBox.h"

#include "scene/Components.h"
#include "utils/RandomList.h"
#include "utils/Files.h"
//Make sense?
#define XPOS(x) x[3][0]
#define YPOS(x) x[3][1]
#define ZPOS(x) x[3][2]

inline static float Distance(const glm::vec2& point_a, const glm::vec2& point_b)
{
	return float(std::sqrt(std::pow(point_b.x - point_a.x, 2) + std::pow(point_b.y - point_a.y, 2)));
}

static Base::Entity CreateBall(Base::Scene* scene);

class ScriptExample : public Base::ScriptableEntity
{
public:
	void OnCreate()
	{
	}

	void OnUpdate(const Base::UpdateArgs& args)
	{
		auto& comp = GetComponent<Base::CircleComponent>();
		auto& vel = GetComponent<Base::VelocityComponent>().Velocity;

		auto& rad = comp.Radius;
		auto& pos = comp.Position;

		pos.x += vel.x * args.dt;
		pos.y += vel.y * args.dt;

		if (pos.x + rad > 800.0f)
			vel.x = -fabs(vel.x);
		if (pos.x - rad < 0.0f)
			vel.x = fabs(vel.x);
		if (pos.y + rad > 600.0f)
			vel.y = -fabs(vel.y);
		if (pos.y - rad < 0.0f)
			vel.y = fabs(vel.y);
	}
};

static Base::Entity CreateBall(Base::Scene* scene)
{
	static unsigned int i = 0;
	static float norm = 1.0f / 256.0f;
	auto ball = scene->CreateEntity("Ball_" + i);
	auto& Circle = ball.AddComponent<Base::CircleComponent>();
	Circle.Radius = ((P_random() + 10)) % 60;
	Circle.Position = { ((P_random() + int(Circle.Radius)) << 2)%800 - Circle.Radius,((P_random() + int(Circle.Radius)) << 2) % 600 - Circle.Radius,1.0f };
	Circle.Color = { norm * P_random(),norm * P_random(),norm * P_random() ,1.0f };
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

	m_Balls.reserve(1000);
	m_Scene = std::make_unique<Base::Scene>();
	float norm = 1.0f / 256.0f;
	
	m_Balls.push_back(CreateBall(m_Scene.get()));
	m_Balls.push_back(CreateBall(m_Scene.get()));

	m_TexQuad = m_Scene->CreateEntity("Thing");
	m_TexQuad.AddComponent<Base::TextureComponent>(m_Textures["test"]);
	auto& pos = m_TexQuad.GetComponent<Base::TransformComponent>().Transform;
	XPOS(pos) = 400;
	YPOS(pos) = 300;
	ZPOS(pos) = 1.0f;
	pos = glm::scale(pos, glm::vec3(-50.0f, 50.0f, 1.0f));//Negative flip in some way
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
	if (Base::input::Mouse::isClicked(BASE_MOUSE_BUTTON_1))
	{
		auto ball = CreateBall(m_Scene.get());
		m_Balls.push_back(ball);
		m_Scene->StartNativeScript(ball);
	}
	m_Scene->OnUpdate(args);
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

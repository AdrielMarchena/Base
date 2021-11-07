#include "Example1.h"

#include "utils/Files.h"
#include "scene/Components.h"
#include "scene/CameraScript.h"
#include "imgui.h"
#include "utils/RandomList.h"
#include "render/gl/Gl_Commands.h"

//#define USING_3DCAMERA_EXAMPLE

float Fov = 45.0f;
glm::vec3 Pos;
glm::vec3 Scale;
static inline bool Preprare3DCamera(Base::Scope<Base::Scene>& scene,Base::Entity& camera)
{
#if defined BASE_USING_3D
	camera = scene->CreateEntity("Main3D_Camera");

	auto& Camera_Transform = camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = camera.AddComponent<Base::CameraComponent>();
	auto& Camera_Script = camera.AddComponent<Base::NativeScriptComponent>();

	Camera_Script.Bind<Base::PerspectiveScript>();

	Camera_comp.Camera.SetPerspective(glm::radians(45.0f), -10.0f, 10.0f);
	Camera_comp.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height);
	scene->StartNativeScript(camera);
	scene->AwakeNativeScript(camera);

	return true;
#endif
	return false;
}

Example1::Example1()
	:Base::windowing::Window()
{
}

Example1::Example1(const char* title, float_t w, float_t h, bool resizeble, bool fullscreen)
	:Base::windowing::Window(title,w,h,resizeble,fullscreen)
{
}

Example1::Example1(const Base::windowing::WindowSpecification& specs)
	: Base::windowing::Window(specs)
{
}

Example1::~Example1()
{
}

void Example1::OnAttach() //Called before the game loop starts
{
	m_Scene = Base::MakeScope<Base::Scene>(); //Create scene

	auto texture = Base::render::Texture::CreateTexture("images/test.PNG"); //Create a texture from file
	if (texture->GetId()) APP_INFO("texture using 'test.PNG' created!"); //Log if texture is all good

	//All entitys have two default components, Base::TagComponent, is just a name, 'Circle in this case'
	//and Base::TransformComponent, is the position(translation), scale and rotation of this entity
	m_Entitys["Circle"] = m_Scene->CreateEntity("Circle"); //Create the circle entity

	m_Entitys["Circle"].AddComponent<Base::CircleComponent>(); // This makes the scene render this component as a circle
	m_Entitys["Circle"].AddComponent<Base::TextureComponent>(texture); // Add texture

	m_Entitys["Quad"] = m_Scene->CreateEntity("Quad"); //Create the Quad entity
	m_Entitys["Quad"].AddComponent<Base::SpriteComponent>(Color::Green); //Add sprite (solid color)
	
	//Get the transform of this entity
	auto& Quad_Transform = m_Entitys["Quad"].GetComponent<Base::TransformComponent>(); 
	Quad_Transform.Scale = {1.0f,1.0f,1.0f}; // X Y Z
	Quad_Transform.Translation.z = -0.5f; 
	
	m_Camera = m_Scene->CreateEntity("Main2D_Camera"); //Create camera entity
	
	auto& Camera_Transform = m_Camera.GetComponent<Base::TransformComponent>();
	auto& Camera_comp = m_Camera.AddComponent<Base::CameraComponent>(); // Add a camera component, this is what make this entity a camera

	Camera_comp.Camera.SetViewportSize(Base::WindowProps().width, Base::WindowProps().height); //Set the viewport of the camera to the screen size
#if defined BASE_USING_3D
	if (Preprare3DCamera(m_Scene, m_Camera)) //If USING_3DCAMERA_EXAMPLE is defined the func returns true (see the function for more details)
	{
		APP_INFO("3D Camera script enabled");
		const char* model_path = "resources/models/cube/cube.obj";
		auto sphere = Base::Model::CreateModel("model_path"); //Load and create a model from a path
		if (sphere) APP_INFO("model using '{0}' created!", model_path);

		m_Entitys["Sphere"] = m_Scene->CreateEntity("Sphere"); 
		m_Entitys["Sphere"].AddComponent<Base::ModelComponent>(sphere); //Add the model component and give that sphere model to it

		this->HideCursor(); //Hide the cursor, in other words, the window will capture the cursor so it does not get out of the window
	}
	else // Only bind script on 2DCamera if there is no 3D camera
#endif
	{
		auto& Camera_Script = m_Camera.AddComponent<Base::NativeScriptComponent>(); // Add the component that will have the script for this camera
		Camera_Script.Bind<Base::OrthoCameraScript>(); //Bind the Base::OrthoCameraScript to this entity, this need to be started
		m_Scene->StartNativeScript(m_Camera); //In this case, it is started here, this call the OnCreate funtion of this script
		m_Scene->AwakeNativeScript(m_Camera); //This one calls the OnAwake
		//The OnUpdate function will be called each frame, as long as this script is instanciated on the Base::NativeScriptComponent
	}
}

void Example1::OnUpdate(const Base::UpdateArgs& args) //Called each frame
{
	using kb = Base::input::Keyboard;
	using ms = Base::input::Mouse;

#if defined USING_3DCAMERA_EXAMPLE
	if (kb::isClicked(BASE_KEY_H))
	{
		auto& c = m_Camera3D.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)&c.Instance)->mouse_is_hide = true;
		this->HideCursor();
	}
	if (kb::isClicked(BASE_KEY_U))
	{
		auto& c = m_Camera3D.GetComponent<Base::NativeScriptComponent>();
		((Base::PerspectiveScript*)&c.Instance)->mouse_is_hide = false;
		this->UnhideCursor();
	}
#endif
	if(!Base::WindowProps().minimized) //don't update the scene then the window is minimized
		m_Scene->OnUpdate(args);
}

void Example1::OnRender() // Do nothing
{
	//This function do nothing, will be removed or changed to something else, like a PreRender or something
}

void Example1::OnImGui() // Called after init a new ImGui frame, that is terminated after this function
{
	auto& circ = m_Entitys["Circle"].GetComponent<Base::CircleComponent>();
	auto& quad = m_Entitys["Quad"].GetComponent<Base::TransformComponent>();
	ImGui::Begin("Circle"); //Init a ImGui window

	//Add Slider whith float on the ImGui window
	if(ImGui::SliderFloat("Radius", &circ.Radius,0.0f,10.0f))
		quad.Scale = glm::vec3(circ.Radius);
	ImGui::SliderFloat("Thickness", &circ.Thickness, 0.0f, 1.0f);
	ImGui::SliderFloat("Fade", &circ.Fade, 0.0f, 10.0f);

	ImGui::SliderFloat3("Quad Pos", &quad.Translation.x, -10.0f, 10.0f);

	ImGui::End();//End a ImGui window

}

void Example1::Dispose() // Called in the class destructor, before terminate important things
{
	//Dispose things here if you need,
	//Textures that are Ref or Scope, such as other resources will be deleted when the
	//game is over, if for some reason you are using a resource that is not wrapped in
	//Ref in Scope, delete it here
}

void Example1::OnResize(const Base::ResizeArgs& args) //Called when the window is resized
{
	if (Base::WindowProps().minimized)
		return;

	//Updates the viewport of the camera when the window is resized
	auto& Camera_comp = m_Camera.GetComponent<Base::CameraComponent>();
	Camera_comp.Camera.SetViewportSize(args.new_w, args.new_h);
#if defined BASE_USING_3D
	auto& Camera3D = m_Camera3D.GetComponent<Base::CameraComponent>();
	Camera3D.Camera.SetViewportSize(args.new_w, args.new_h);
#endif
}

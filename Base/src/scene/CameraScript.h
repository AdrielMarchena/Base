#pragma once

#include "Components.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "ScriptableEntity.h"
#include "event/MouseEvent.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Base
{
	/* Simple control script for orthographic cameras */
	class OrthoCameraScript : public ScriptableEntity
	{
	public:
		float Velocity = 5.5f;
		float Zoom = 10.0f;
		bool zoom_enable = true;
		bool movement_enable = true;
	protected:
		void ProcessMouseScroll(float yoffset)
		{
			Zoom -= (float)yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 45.0f)
				Zoom = 45.0f;

			auto& camera = GetComponent<CameraComponent>();
			camera.Camera.SetOrthographicSize(Zoom);
		}

		void ProcessMovement(float dt)
		{
			auto& T = GetComponent<TransformComponent>().Translation;
			using kb = Base::input::Keyboard;

			if (kb::isPress(BASE_KEY_D))
				T.x += Velocity * dt;
			if (kb::isPress(BASE_KEY_A))
				T.x -= Velocity * dt;
			if (kb::isPress(BASE_KEY_W))
				T.y += Velocity * dt;
			if (kb::isPress(BASE_KEY_S))
				T.y -= Velocity * dt;
		}

		virtual void OnUpdate(const UpdateArgs& args) override
		{
			auto& T = GetComponent<TransformComponent>().Translation;
			using kb = Base::input::Keyboard;
			using ms = Base::input::Mouse;

			if (movement_enable)
				ProcessMovement(args.dt);
		}

		virtual void OnEvent(Event& e)
		{
			EventDispatcher dispatch(e);

			dispatch.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(OrthoCameraScript::OnMouseScroll));
		}

		bool OnMouseScroll(MouseScrollEvent& e)
		{
			if(zoom_enable)
				ProcessMouseScroll(e.GetYOffset());
			return true;
		}
	};

	class PerspectiveScript : public ScriptableEntity
	{
	public:
		enum class Camera_Movement : uint8_t 
		{
			NONE,
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};
		float default_yaw = -90.0f;
		float default_pitch = 0.0f;
		float default_speed = 3.0f;
		float default_sensitivity = 0.1f;
		float default_zoom = 45.0f;
		bool mouse_is_hide = true;

	private:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		glm::vec2 last_mouse;
		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
	public:

		void SyncSpeed()
		{
			MovementSpeed = default_speed;
			MouseSensitivity = default_sensitivity;
		}

		void ProcessKeyboard(Camera_Movement direction,float dt)
		{
			float velocity = MovementSpeed * dt;
			if (direction == Camera_Movement::FORWARD)
				Position += Front * velocity;
			if (direction == Camera_Movement::BACKWARD)
				Position -= Front * velocity;
			if (direction == Camera_Movement::LEFT)
				Position -= Right * velocity;
			if (direction == Camera_Movement::RIGHT)
				Position += Right * velocity;
		}

		void ProcessMouseMovement(float xoffset, float yoffset,bool constrain_pitch = true)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrain_pitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			// update Front, Right and Up Vectors using the updated Euler angles
			//UpdateCameraVectors();
		}

		void ProcessMouseScroll(float yoffset)
		{
			Zoom -= (float)yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 90.0f)
				Zoom = 90.0f;

			auto& camera = GetComponent<CameraComponent>();
			camera.Camera.SetPerspectiveVerticalFOV(glm::radians(Zoom));
		}

		glm::mat4 ViewMatrix() const
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

	protected:
		virtual void OnCreate() override
		{
			Position = {0.0f,0.0f,0.0f};
			Up = {0.0f,1.0f,0.0f};
			WorldUp = Up;
			Front = {0.0f,0.0f,-1.0f};
			Right = {0.0f,0.0f,0.0f};
			
			using ms = input::Mouse;
			last_mouse = { (ms::gpos().x - ms::gprevPos().x), (ms::gprevPos().y - ms::gpos().y) };

			Yaw = default_yaw;
			Pitch = default_pitch;
			MovementSpeed = default_speed;
			MouseSensitivity = default_sensitivity;
			Zoom = default_zoom;

			auto& Transform = GetComponent<TransformComponent>();
			UpdateCameraVectors();
			Transform.Manual_Transform = ViewMatrix();
		}
		virtual void OnAwake() override
		{
			Position = { 0.0f,0.0f,0.0f };
			Up = { 0.0f,1.0f,0.0f };
			WorldUp = Up;
			Front = { 0.0f,0.0f,-1.0f };
			Right = { 0.0f,0.0f,0.0f };
			
			using ms = input::Mouse;
			last_mouse = { (ms::gpos().x - ms::gprevPos().x), (ms::gprevPos().y - ms::gpos().y) };

			Yaw = default_yaw;
			Pitch = default_pitch;
			MovementSpeed = default_speed;
			MouseSensitivity = default_sensitivity;
			Zoom = default_zoom;

			auto& Transform = GetComponent<TransformComponent>();
			UpdateCameraVectors();
			Transform.Manual_Transform = ViewMatrix();
		}

		virtual void OnUpdate(const UpdateArgs& args) override
		{
			using ky = input::Keyboard;
			using ms = input::Mouse;
			auto& Transform = GetComponent<TransformComponent>();

			//Process Movement
			Camera_Movement direction = Camera_Movement::NONE;
			if (ky::isPress(BASE_KEY_W))
				ProcessKeyboard(Camera_Movement::FORWARD,args.dt);
			if (ky::isPress(BASE_KEY_A))
				ProcessKeyboard(Camera_Movement::LEFT,args.dt);
			if (ky::isPress(BASE_KEY_S))
				ProcessKeyboard(Camera_Movement::BACKWARD, args.dt);
			if (ky::isPress(BASE_KEY_D))
				ProcessKeyboard(Camera_Movement::RIGHT, args.dt);

			ProcessKeyboard(direction, args.dt);

			float offvalue = ms::goffValue().y;
			if (offvalue)
				ProcessMouseScroll(offvalue);

			glm::vec2 m_offset = { (ms::gpos().x - ms::gprevPos().x), (ms::gprevPos().y - ms::gpos().y) };

			if(m_offset != last_mouse && mouse_is_hide)
				ProcessMouseMovement(m_offset.x,m_offset.y);

			last_mouse = m_offset;
			UpdateCameraVectors();
			Transform.Manual_Transform = ViewMatrix();
		}
		private:
			void UpdateCameraVectors()
			{
				glm::vec3 front{};

				front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
				front.y = sin(glm::radians(Pitch));
				front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
				Front = glm::normalize(front);
				// also re-calculate the Right and Up vector
				Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
				Up = glm::normalize(glm::cross(Right, Front));
			}
	};
}
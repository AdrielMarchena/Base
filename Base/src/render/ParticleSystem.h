/*
* @file ParticleSystem.h
*
* Particle System Stuff
*/

#pragma once

#include "glm/glm.hpp"
#include "camera/OrthographicCamera.h"
#include "args/VirtualArgs.h"
#include <vector>
namespace en{
	namespace render {
		struct ParticleProps
		{
			glm::vec2 Position = {0.0f,0.0f};
			glm::vec2 Velocity, VelocityVariation = { 0.0f,0.0f };
			glm::vec4 ColorBegin = { 0.0f,0.0f, 0.0f, 0.0f }, ColorEnd = { 0.0f, 0.0f, 0.0f, 0.0f };
			Texture* TexturePtr = nullptr;
			float SizeBegin = 0.0f, SizeEnd = 0.0f, SizeVariation = 0.0f;
			float LifeTime = 1.0f;
			float Gravity = 0.0f;

			static ParticleProps Effects[];
		};

		class ParticleSystem
		{
		public:
			ParticleSystem(uint32_t count = 200);

			void OnUpdate(const UpdateArgs& args);
			void OnUpdate(float dt);
			void OnRender(const RenderArgs& args);

			void Emit(const ParticleProps& particleProps);
		private:
			struct Particle
			{
				glm::vec2 Position;
				glm::vec2 Velocity;
				glm::vec4 ColorBegin, ColorEnd;
				float Rotation = 0.0f;
				float SizeBegin, SizeEnd;

				Texture* TexturePtr = nullptr;

				float LifeTime = 1.0f;
				float LifeRemaining = 0.0f;

				bool Active = false;
				float Gravity = 0.0f;
			};
			std::vector<Particle> m_ParticlePool;
			uint32_t m_PoolIndex = 199;
		};
	}
}
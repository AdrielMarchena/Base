/*
*	@file ParticleSystem.cpp
*
*	Particle System Stuff
*
*	Implementation File
*/

#include "ParticleSystem.h"
#include "utils/RandomList.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include "render2D.h"
#include <iostream>

#include <future>

namespace Base {
	namespace render {

		ParticleProps ParticleProps::Effects[] =
		{
			{ // Effect 0
				{0,0}, //Position
				{10,10}, //Velocity
				{200,200}, //Velocity Variation
				{1.0f, 1.0f, 1.0f, 1.0f}, //Color Begin
				{0.0f, 0.0f, 0.0f, 1.0f}, //Color End
				nullptr,// Texture pointer
				 50.0f, // Size Begin
				 2.0f,  // Size End
				 3.0f,	// Size Variation
				 1.0f,  // Life Time
				 0.0f,  // Gravity
			}  // Effect 0
		};

		ParticleSystem::ParticleSystem(uint32_t count)
		{
			m_ParticlePool.resize(count);
			m_PoolIndex = count-1;
		}

		void ParticleSystem::OnUpdate(const UpdateArgs& args)
		{
			OnUpdate(args.dt);
		}

		void ParticleSystem::OnUpdate(float dt)
		{
			for (auto& particle : m_ParticlePool)
			{
				if (!particle.Active)
					continue;

				if (particle.LifeRemaining <= 0.0f)
				{
					particle.Active = false;
					continue;
				}

				particle.LifeRemaining -= dt;
				particle.Position += particle.Velocity * dt;
				particle.Position.y -= particle.Gravity * dt;
				particle.Rotation += 1.0f * dt;
			}
		}

		void ParticleSystem::OnRender(const RenderArgs& args)
		{
			for (auto& particle : m_ParticlePool)
			{	
				if (!particle.Active)
					continue;

				particle.Position.y -= particle.Gravity;

				// Fade away particles
				float life = particle.LifeRemaining / particle.LifeTime;
				float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

				if (particle.TexturePtr == nullptr)
				{
					glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
					color.a = color.a * life;
					args.render.DrawQuad(glm::vec3(particle.Position, 2.0f), { size,size }, color, particle.Rotation, { 0.0f,1.0f,1.0f });
				}
				else
					args.render.DrawQuad(glm::vec3(particle.Position, 2.0f), { size,size }, *particle.TexturePtr, Color::White ,particle.Rotation, { 0.0f,1.0f,1.0f });
			}
		}

		void ParticleSystem::Emit(const ParticleProps& particleProps)
		{
			Particle& particle = m_ParticlePool[m_PoolIndex];
			particle.Active = true;
			particle.Position = particleProps.Position;
			particle.Rotation = Random::Float() * 5.0f * glm::pi<float>();
			particle.Gravity = particleProps.Gravity;
			// Velocity
			particle.Velocity = particleProps.Velocity;
			particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
			particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

			// Color
			particle.ColorBegin = particleProps.ColorBegin;
			particle.ColorEnd = particleProps.ColorEnd;

			particle.LifeTime = particleProps.LifeTime;
			particle.LifeRemaining = particleProps.LifeTime;
			particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
			particle.SizeEnd = particleProps.SizeEnd;

			particle.TexturePtr = particleProps.TexturePtr;

			m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
		}
	}
}
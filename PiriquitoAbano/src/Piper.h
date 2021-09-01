#pragma once
#include "Pipe.h"
#include "utils/RandomList.h"
#include <cstdlib>

#include <functional>

static inline float RandFloat(float a)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / a));
}

class Piper
{
private:
	std::vector<Pipe> Pipes;
	float dec_velocity = 200.0f;
	Random rando;
	
public:
	float threshold = 565.0f;
	float pipe_velocity = 300.0f;
	float PipeGap = 225.0f;

	Piper(){}
	~Piper(){}

	void Reset()
	{
		threshold = 565.0f;
		pipe_velocity = 300.0f;
		PipeGap = 225.0f;
		MapMe([&](Pipe& pipe) 
		{
				pipe.Despawn();
		});
	}

	void OnAttach(const en::AttachArgs& args)
	{
		Pipes.resize(10);
		rando.Init();
	}

	std::vector<Pipe>& GetPipes()
	{
		return Pipes;
	}

	inline void MapMe(const std::function<void(Pipe&)>& function)
	{
		for (auto& pipe : Pipes)
		{
			function(pipe);
		}
	}

	void SpawnPairPipe(float dt)
	{
		static float ac_threshold = threshold;
		if (ac_threshold >= 0.0f)
		{
			ac_threshold -= dec_velocity * dt;
			return;
		}

		constexpr float bottom_min = 50.0f;
		constexpr float up_min = 50.0f;

		float a_h = std::clamp(Random::Float() * 600.0f, up_min, 600.0f - (bottom_min + PipeGap) );
		float b_h = std::fabs(a_h - 600.0f) - PipeGap;
		//float b_h = a_h - PipeGap;

   		SpawnUpPipe(a_h);
		PointPipe();
		SpawnDownPipe(b_h);
		ac_threshold = threshold;
	}

	void UpdatePipes(const en::UpdateArgs& args)
	{
		MapMe([&](Pipe& pipe) {
			pipe.OnUpdate(args);
		});
	}

	void RenderPipes(const en::RenderArgs& args)
	{
		MapMe([&](Pipe& pipe) {
			pipe.OnRender(args);
		});
	}

	void SetPipesTexture(std::shared_ptr<en::render::Texture> texture)
	{
		MapMe([=](Pipe& pipe) {
			pipe.SetTexture(texture);
		});
	}

	void SetPipesBodyTexture(std::shared_ptr<en::render::Texture> texture)
	{
		MapMe([=](Pipe& pipe) {
			pipe.SetBodyTexture(texture);
		});
	}

	void OnImGui(const en::ImGuiArgs& args)
	{
		MapMe([&](Pipe& pipe) {
			pipe.OnImGui(args);
		});

		ImGui::Text("Piper constrol");
		ImGui::SliderFloat("Pipe Gap", &PipeGap, 50.0f, 300.0f);
		ImGui::SliderFloat("Pipe velocity", &pipe_velocity, 50.0f, 1000.0f);
		ImGui::SliderFloat("Threshold", &threshold, 200.0f, 1500.0f);
	}

private:
	void SpawnDownPipe(float h)
	{
		glm::vec2 new_pos = {900.0f,0.0f};
		glm::vec2 new_size = {64.0f,h };

		for (auto& pipe : Pipes)
		{
			if (pipe.IsAlive())
				continue;
 			pipe.Spawn({ new_pos,new_size,{pipe_velocity,0.0f} });
			break;
		}
	}

	void SpawnUpPipe(float h)
	{
		glm::vec2 new_pos = { 900.0f,600.0f - h};
		glm::vec2 new_size = { 64.0f, h };

		for (auto& pipe : Pipes)
		{
			if (pipe.IsAlive())
				continue;
			pipe.Spawn({ new_pos,new_size,{pipe_velocity,0.0f} });
			pipe.invert = true;
			break;
		}
	}

	void PointPipe()
	{
		glm::vec2 new_pos = { 900.0f + 32.0f,0.0f };
		glm::vec2 new_size = { 64.0f/4, 600.0f };

		for (auto& pipe : Pipes)
		{
			if (pipe.IsAlive())
				continue;
			pipe.Spawn({ new_pos,new_size,{pipe_velocity,0.0f} });
			pipe.SetRender(false);
			pipe.pointPipe = true;
			break;
		}
	}
};
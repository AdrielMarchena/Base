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
	float PipeGap = 225.0f;
	Random rando;
	en::render::Texture& DownPipe;
	en::render::Texture& UpPipe;
public:
	Piper():DownPipe(en::render::Texture::GetBlanckTexture()),UpPipe(en::render::Texture::GetBlanckTexture()) {}
	~Piper(){}

	void OnAttach(const en::AttachArgs& args)
	{
		//Blank Texture
		static en::render::Texture blank;
		Pipes.reserve(15);
		for (int i = 0; i < 15; i++)
		{
			Pipes.emplace_back(blank);
		}
		rando.Init();
	}

	std::vector<Pipe>& GetPipes()
	{
		return Pipes;
	}

	void MapMe(const std::function<void(Pipe&)>& function)
	{
		for (auto& pipe : Pipes)
		{
			function(pipe);
		}
	}

	void SpawnPairPipe(float dt)
	{
		static float threshold = 900.0f;
		if (threshold >= 400.0f)
		{
			threshold -= 200.0f * dt;
			return;
		}

		constexpr float bottom_min = 50.0f;
		constexpr float up_min = 50.0f;

		float a_h = std::clamp(RandFloat(600.0f), up_min, 600.0f - (bottom_min + PipeGap) );
		float b_h = std::fabs(a_h - 600.0f) - PipeGap;
		//float b_h = a_h - PipeGap;

   		SpawnUpPipe(a_h);
		PointPipe();
		SpawnDownPipe(b_h);
		threshold = 900.0f;
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

	void SetPipesTexture(en::render::Texture& texture)
	{
		MapMe([&](Pipe& pipe) {
			pipe.SetTexture(texture);
		});
	}

	void SetDownPipesTexture(en::render::Texture& texture)
	{
		DownPipe = texture;
	}

	void SetUpPipesTexture(en::render::Texture& texture)
	{
		UpPipe = texture;
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
			pipe.Spawn({ new_pos,new_size,{0.0f,0.0f} });
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
			pipe.Spawn({ new_pos,new_size,{0.0f,0.0f} });
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
			pipe.Spawn({ new_pos,new_size,{0.0f,0.0f} });
			pipe.SetRender(false);
			pipe.pointPipe = true;
			break;
		}
	}
};
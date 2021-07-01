#pragma once
#include "Pipe.h"
#include "utils/RandomList.h"
#include <cstdlib>

static inline float RandFloat(float a)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / a));
}

class Piper
{
private:
	std::vector<Pipe> Pipes;
	float PipeGap = 200.0f;
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
		Pipes.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			Pipes.emplace_back(blank);
		}
		rando.Init();
	}

	std::vector<Pipe>& GetPipes()
	{
		return Pipes;
	}

	void SpawnPairPipe(float dt)
	{
		static float threshold = 900.0f;
		if (threshold >= 400.0f)
		{
			threshold -= 200.0f * dt;
			return;
		}

		float a_h = RandFloat(600.0f - PipeGap) + 50.0f;
		float b_h = ((600.0f - 64.0f) - (a_h + PipeGap));

		SpawnUpPipe(a_h);
		SpawnDownPipe(b_h);
		threshold = 900.0f;
	}

	void UpdatePipes(const en::UpdateArgs& args)
	{
		for (auto& pipe : Pipes)
		{
			pipe.OnUpdate(args);
		}
	}

	void RenderPipes(const en::RenderArgs& args)
	{
		for (auto& pipe : Pipes)
		{
			pipe.OnRender(args);
		}
	}

	void SetPipesTexture(en::render::Texture& texture)
	{
		for (auto& pipe : Pipes)
		{
			pipe.SetTexture(texture);
		}
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
			if (pipe.IsRender())
				continue;
			pipe.Spawn({ new_pos,new_size,{0.0f,0.0f} });
			pipe.invert = true;
			//pipe.SetTexture(DownPipe);
			break;
		}
	}

	void SpawnUpPipe(float h)
	{
		glm::vec2 new_pos = { 900.0f,600.0f - h };
		glm::vec2 new_size = { 64.0f,h };

		for (auto& pipe : Pipes)
		{
			if (pipe.IsRender())
				continue;
			pipe.Spawn({ new_pos,new_size,{0.0f,0.0f} });
			//pipe.SetTexture(UpPipe);
			break;
		}
	}
};
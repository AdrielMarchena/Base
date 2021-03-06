#pragma once
#include <stdint.h>

#include "render/gl/Texture.h"

namespace Base
{
	namespace utils
	{

	}

	class PerlinNoise1D
	{
	private:
		float* m_NoiseSeed1D = nullptr;
		float* m_PerlinNoise1D = nullptr;
		uint32_t m_OutputSize;
		bool m_Generated = false;
		float m_Bias = 2.0f;
	public:

		PerlinNoise1D(uint32_t size = 256);
		PerlinNoise1D(const PerlinNoise1D&) = delete; //TODO: implement copy contructor to memcopy arrays
		~PerlinNoise1D();

		void GenerateNoise(int octaves);

		Ref<render::Texture> GenerateNoiseTexture();
	};

	class PerlinNoise2D
	{
	private:
		float* m_NoiseSeed2D = nullptr;
		float* m_PerlinNoise2D = nullptr;
		uint32_t m_OutputWidth;
		uint32_t m_OutputHeight;
		bool m_Generated = false;
		float m_Bias = 2.0f;
	public:

		PerlinNoise2D(uint32_t w = 256, uint32_t h = 256);
		PerlinNoise2D(const PerlinNoise2D&) = delete; //TODO: implement copy contructor to memcopy arrays
		~PerlinNoise2D();

		void GenerateNoise(int octaves);

		Ref<render::Texture> GenerateNoiseTexture();
		Ref<render::Texture> GenerateSeedTexture();

	private:
		static Ref<render::Texture> GenerateTexture(int w, int h,float* data);
	};


}
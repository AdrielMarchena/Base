#pragma once
#include <stdint.h>

#include "render/gl/Texture.h"

namespace Base {
	namespace utils {

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
		float m_Bias;
		int m_Octaves;
		float m_ColorInterpolationPrecision = 0.033f;
		glm::vec3 m_MinimumColor = { 75.0f, 75.0f, 75.0f };
		glm::vec3 m_MaxColor = { 225.0f, 225.0f, 225.0f };
		glm::vec3 m_MediumColor = (m_MinimumColor + m_MaxColor) * 0.5f;

	public:

		PerlinNoise2D(uint32_t w = 256, uint32_t h = 256, int octaves = 8, float bias = 1.50f);
		PerlinNoise2D(const PerlinNoise2D&) = delete; //TODO: implement copy contructor to memcopy arrays
		~PerlinNoise2D();

		void GenerateNoise();

		void SetBias(float bias) { m_Bias = bias; }
		void SetOctaves(float octaves) { m_Octaves = octaves; }
		void SetColorInterpolationPrecision(float colorInterpolationPrecision) { m_ColorInterpolationPrecision = colorInterpolationPrecision; }

		void SetMinumumColor(const glm::vec3& minimum) { m_MinimumColor = minimum; UpdateMedium(); }
		void SetMaxColor(const glm::vec3& max) { m_MaxColor = max; UpdateMedium(); }

		Ref<render::Texture> GenerateNoiseTexture();
		Ref<render::Texture> GenerateSeedTexture();

	private:
		glm::vec3 DecideColorMtb(float data);
		void UpdateMedium() { (m_MinimumColor + m_MaxColor) * 0.5f; }
		static Ref<render::Texture> GenerateTexture(int w, int h, float* data, float precision, std::function<glm::vec3(float)> decideColor);
	};


}
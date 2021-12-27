#include "pch.h"
#include "PerlinNoise.h"

#include <random>
namespace Base
{

	
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	//UUID::UUID()
		//:m_UUID(s_UniformDistribution(s_Engine))
	

	PerlinNoise1D::PerlinNoise1D(uint32_t size)
		:m_OutputSize(size)
	{
		m_NoiseSeed1D = new float[m_OutputSize];
		m_PerlinNoise1D = new float[m_OutputSize];

		for(uint32_t i = 0; i < m_OutputSize; i++)
			m_NoiseSeed1D[i] = (float)std::rand() / (float)RAND_MAX;

	}

	PerlinNoise1D::~PerlinNoise1D()
	{
		if(m_NoiseSeed1D)
			delete[] m_NoiseSeed1D;
		if(m_PerlinNoise1D)
			delete[] m_PerlinNoise1D;
	}

	void PerlinNoise1D::GenerateNoise(int octaves)
	{
		m_Bias = m_Bias < 0.2 ? 0.2 : m_Bias;
		for (uint32_t x = 0; x < m_OutputSize; x++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = m_OutputSize >> o;
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % m_OutputSize;

				float fBlend = (float)(x - nSample1) / (float)nPitch;
				float fSample = (1.0f - fBlend) * m_NoiseSeed1D[nSample1] + fBlend * m_NoiseSeed1D[nSample2];
				fNoise += fSample * fScale;

				fScaleAcc += fScale;
				fScale = fScale / m_Bias;
			}
			m_PerlinNoise1D[x] = (fNoise / fScaleAcc);

		}

		m_Generated = true;
	}

	Ref<render::Texture> PerlinNoise1D::GenerateNoiseTexture()
	{
		if (!m_Generated)
		{
			BASE_CORE_ASSERT(false, "Could not generate Texture because Noise is not generated, try using 'GenerateNoise()' method first");
			return MakeRef<render::Texture>();
		}

		TextureBufferType* Buffer = render::Texture::CreateTextureBuffer(m_OutputSize, 1, 3);

		for (unsigned int ix = 0; ix < m_OutputSize; ++ix)
		{
			for (unsigned int iy = 0; iy < 1; ++iy)
			{
				int r = (m_PerlinNoise1D[ix] * 255);
				int g = (m_PerlinNoise1D[ix] * 255);
				int b = (m_PerlinNoise1D[ix] * 255);
				Buffer[ix * 1 * 3 + iy * 3 + 0] = r; //red
				Buffer[ix * 1 * 3 + iy * 3 + 1] = g; //green
				Buffer[ix * 1 * 3 + iy * 3 + 2] = b; //blue
			}
		}

		TextureSpecifications texture_specs;
		texture_specs.Width = m_OutputSize;
		texture_specs.Height = 1;
		texture_specs.Channels = 3;
		texture_specs.Buffer = reinterpret_cast<TextureBufferType*>(m_PerlinNoise1D);
		//texture_specs.Type = GL_TexType::FLOAT;
		texture_specs.KeepSourceBuffer = false;

		auto tex = MakeRef<render::Texture>(texture_specs);

		render::Texture::DeleteTextureBuffer(Buffer);

		return tex;
	}

	//______________________________________________________________________________________________________________________________________

	PerlinNoise2D::PerlinNoise2D(uint32_t w, uint32_t h)
		:m_OutputWidth(w),m_OutputHeight(h)
	{
		m_NoiseSeed2D = new float[m_OutputWidth * m_OutputHeight];
		m_PerlinNoise2D = new float[m_OutputWidth * m_OutputHeight];


		for (uint32_t i = 0; i < m_OutputWidth * m_OutputHeight; i++)
		{
			auto ran = 1.0 / (double)s_UniformDistribution(s_Engine) + 1.0;
			m_NoiseSeed2D[i] = ((float)std::rand() / (float)RAND_MAX / (float)ran);
		}

	}

	PerlinNoise2D::~PerlinNoise2D()
	{
		if (m_NoiseSeed2D)
			delete[] m_NoiseSeed2D;
		if (m_PerlinNoise2D)
			delete[] m_PerlinNoise2D;
	}

	Ref<render::Texture> PerlinNoise2D::GenerateTexture(int w, int h, float* data)
	{
		TextureBufferType* Buffer = render::Texture::CreateTextureBuffer(w, h, 3);

		for (unsigned int ix = 0; ix < h; ix++)
		{
			for (unsigned int iy = 0; iy < w; iy++)
			{
				int c = (*(data + iy * w + ix) * 255);
				*(Buffer + ix * w * 3 + iy * 3 + 0) = c; // r
				*(Buffer + ix * w * 3 + iy * 3 + 1) = c; // g
				*(Buffer + ix * w * 3 + iy * 3 + 2) = c; // b
			}
		}

		TextureSpecifications texture_specs;
		texture_specs.Width = w;
		texture_specs.Height = h;
		texture_specs.Channels = 3;
		texture_specs.Buffer = Buffer;
		texture_specs.Type = GL_TexType::UNSIGNED_BYTE;
		texture_specs.KeepSourceBuffer = false;

		auto tex = MakeRef<render::Texture>(texture_specs);

		render::Texture::DeleteTextureBuffer(Buffer);

		return tex;
	}

	void PerlinNoise2D::GenerateNoise(int octaves)
	{
		m_Bias = m_Bias < 0.2 ? 0.2 : m_Bias;
		for (uint32_t x = 0; x < m_OutputWidth; x++)
		for (uint32_t y = 0; y < m_OutputHeight; y++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = m_OutputWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % m_OutputWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % m_OutputWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * m_NoiseSeed2D[nSampleY1 * m_OutputWidth + nSampleX1] + fBlendX * m_NoiseSeed2D[nSampleY1 * m_OutputWidth + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * m_NoiseSeed2D[nSampleY2 * m_OutputWidth + nSampleX1] + fBlendX * m_NoiseSeed2D[nSampleY2 * m_OutputWidth + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / m_Bias;
			}
			m_PerlinNoise2D[y * m_OutputWidth + x] = (fNoise / fScaleAcc);

		}

		m_Generated = true;
	}

	Ref<render::Texture> PerlinNoise2D::GenerateNoiseTexture()
	{
		if (!m_Generated)
		{
			BASE_CORE_ASSERT(false, "Could not generate Texture because Noise is not generated, try using 'GenerateNoise()' method first");
			return MakeRef<render::Texture>();
		}

		return GenerateTexture(m_OutputWidth,m_OutputHeight,m_PerlinNoise2D);
	}

	Ref<render::Texture> PerlinNoise2D::GenerateSeedTexture()
	{
		if (!m_NoiseSeed2D)
		{
			BASE_CORE_ASSERT(false, "Could not generate Texture because Noise Seed is null");
			return MakeRef<render::Texture>();
		}

		return GenerateTexture(m_OutputWidth, m_OutputHeight, m_NoiseSeed2D);
	}

}
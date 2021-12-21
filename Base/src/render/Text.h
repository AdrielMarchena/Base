#pragma once

#include "Base/Base.h"

#include <unordered_map>
#include <string>
#include "render/gl/Texture.h"
#include "render/SubTexture.h"
namespace msdfgen
{
	class FontHandle;
	class FreetypeHandle;
}

namespace msdf_atlas
{
	class FontGeometry;
}

namespace Base
{

	struct AsyncGlyphInfo
	{
		TextureSpecifications TextureSpec;
		double Advance;
		unsigned char character;
		float* pixels = nullptr;
	};

	struct GRect
	{
		float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
		
		GRect() = default;
		GRect(const GRect&) = default;
		GRect(float parm_x,float parm_y,float parm_w,float parm_h)
			:x(parm_x),y(parm_y),w(parm_w),h(parm_h)
		{}

		void Normilize(float value = 1.0f)
		{
			if (!w && !h)
				return;
			float normx = value / (w);
			float normy = value / (h);

			x *= normx;
			w *= normx;
			y *= normy;
			h *= normy;
		}
		
	};

	struct FontSpecifications
	{
		int32_t width = 32;
		int32_t height = 32;
		double angleThreshold = 3.0;
		double range = 4.0;
		glm::vec2 scale{ 1.0,1.0 };
		glm::vec2 translate{ 4.0,4.0 };

		//Misc
		bool LoadAsync = true; //This option uses std::async to load Glyphs
	};

	struct VertexGlyph
	{
		glm::vec2 coord;
		glm::vec2 texCoords;
	};

	struct GlyphTextInfo
	{
		const char* Text;
		glm::vec2 TextSize; //textWidth,-y
		VertexGlyph Vertex;
	};

	struct Glyph
	{
		Ref<render::SubTexture> Texture;
		GRect Rect;
		double Advance;

		operator bool() const { return Texture->operator bool(); }
	};

	class Font
	{
	private:
		static uint32_t s_FreetypeDeinitializeCounter;
		static msdfgen::FreetypeHandle* m_FreeTypeHandler;
		msdfgen::FontHandle* m_FontHandler = nullptr;
		FontSpecifications m_Specs;
		std::string m_Name;
		std::string m_Path;
		std::unordered_map<unsigned char,Glyph> m_Glyphs;
		Ref<render::Texture> m_Atlas;
		msdf_atlas::FontGeometry* m_FontGeo = nullptr;
	public:
		
		Font() = default;
		Font(const Font&) = default;
		~Font();
		Font(const std::string& path,const FontSpecifications& specs, const std::string& name = std::string());

		void ContructGlyphs();
		void ContructGlyphsAsync();
		/* Get the specified glyph and return a */
		Glyph GetGlyph(unsigned char character);
		AsyncGlyphInfo GetGlyphAsync(unsigned char character);

		void ConstructAtlas();
		void aConstructAtlas();

		GlyphTextInfo GetVertexDataFromText(const std::string& text);

		const Ref<render::Texture>& GetAtlas() const { return m_Atlas; }
		Ref<render::Texture> GetAtlas() { return m_Atlas; }

		const std::unordered_map<unsigned char, Glyph>& GetGlyphsList() const { return m_Glyphs; }
		std::unordered_map<unsigned char, Glyph>& GetGlyphsList() { return m_Glyphs; }
	};

	class FontLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Font>> m_Texts;
	public:
		FontLibrary() = default;
		FontLibrary(const FontLibrary&) = default;
		~FontLibrary() = default;

		/* Store a Ref Font */
		void Add(Ref<Font> text, const std::string& name = std::string());

		bool Exists(const std::string& name);

		/* Create, store and return new Ref Font if does not exist */
		Ref<Font> Create(const std::string& path, int32_t size ,const std::string& name = std::string());

		/* Return a Ref font if exists */
		Ref<Font> Get(const std::string& name);
		
		/* Delete the Ref Font from this library */
		void Delete(const std::string& name);

		/* Delete the Ref Font from this library and return it */
		Ref<Font> Pop(const std::string& name);

	};
}
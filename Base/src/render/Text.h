#pragma once

#include "Base/Base.h"

#include <unordered_map>
#include <string>
#include "render/gl/Texture.h"
namespace msdfgen
{
class FontHandle;
}
namespace Base
{

	struct FontSpecifications
	{
		int32_t width = 32;
		int32_t height = 32;
	};

	struct Glyph
	{
		double Advance;
	};

	class Font
	{
	private:
		msdfgen::FontHandle* m_FontHandler = nullptr;
		FontSpecifications m_Specs;
		std::string m_Name;
		std::string m_Path;
	public:
		
		Font() = default;
		Font(const Font&) = default;
		~Font();
		Font(const std::string& path,const FontSpecifications& specs, const std::string& name = std::string());

		Ref<render::Texture> GetGlyph(char character,int wid, int hei);
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
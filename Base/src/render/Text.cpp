
/* Include this first because some Windows lib define INFINITE and you know what happens after */
#include "pch.h"
#undef INFINITE
#include "msdfgen.h"
#include "msdfgen-ext.h"

#include "Text.h"
#include "utils/base_assert.h"
#include "render/gl/TextureSpecifications.h"
namespace msdf = msdfgen; //TODO: Annoyin todo so you remember this id thing here

namespace Base
{
	Font::Font(const std::string& path, const FontSpecifications& specs, const std::string& name)
		:m_Path(path),m_Specs(specs),m_Name(name)
	{
		msdf::FreetypeHandle* ft = msdf::initializeFreetype();

		if (ft)
			m_FontHandler = msdf::loadFont(ft, path.c_str());

		msdf::deinitializeFreetype(ft);

		if (m_Name == "" || m_Name.empty())
		{
			if (m_Path == "" || m_Path.empty())
				m_Name = "Unamed Font";
			else
			{
				auto lastSlash = m_Path.find_last_of("/\\");
				lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
				auto lastDot = m_Path.rfind('.');
				auto count = lastDot == std::string::npos ? m_Path.size() - lastSlash : lastDot - lastSlash;
				m_Name = path.substr(lastSlash, count);
			}
		}
	}

	Font::~Font()
	{
		if(m_FontHandler)
			msdf::destroyFont(m_FontHandler);
	}

	Ref<render::Texture> Font::GetGlyph(char character, int wid, int hei)
	{
		BASE_CORE_ASSERT(false, "Not implemented");
		if (m_FontHandler)
		{
			msdf::Shape shape;
			if (msdf::loadGlyph(shape, m_FontHandler, character))
			{
				shape.normalize();

				msdf::edgeColoringSimple(shape, 3.0);

				msdf::Bitmap<float, 3> msdf(wid, hei);

				msdf::generateMSDF(msdf, shape, 4.0, 1.0, msdf::Vector2(4.0, 4.0));

				uint8_t* pixels = new uint8_t[wid * hei * 4]{ 0 };

				//msdf::pixelFloatToByte();

				TextureSpecifications spec;
				spec.Width = wid;
				spec.Height = hei;
				spec.Channels = 3;
				spec.Buffer;

			}
		}
		return Ref<render::Texture>();
	}

	//-----------------------------------------------------------------

	void FontLibrary::Add(Ref<Font> text, const std::string& name)
	{
		if (!Exists(name))
		{
			if (name != "" && !name.empty())
			{
				m_Texts[name] = text;
			}
			else
			{
				//TODO: implement
				BASE_CORE_ASSERT(false, "Not implemented!");
			}
		}
	}

	bool FontLibrary::Exists(const std::string& name)
	{
		return m_Texts.find(name) != m_Texts.end();
	}

	Ref<Font> FontLibrary::Create(const std::string& path, int32_t size, const std::string& name)
	{
		//TODO: implement
		BASE_CORE_ASSERT(false, "Not implemented!");

		return Ref<Font>();
	}

	Ref<Font> FontLibrary::Get(const std::string& name)
	{
		BASE_CORE_ASSERT(Exists(name), "Font {0} does not exist!", name);
		return m_Texts[name];
	}

	void FontLibrary::Delete(const std::string& name)
	{
		BASE_CORE_ASSERT(Exists(name), "Font {0} does not exist!", name);
		m_Texts.erase(name);
	}

	[[nodiscard]]
	Ref<Font> FontLibrary::Pop(const std::string& name)
	{
		BASE_CORE_ASSERT(Exists(name), "Font {0} does not exist!", name);
		auto ref = m_Texts[name];
		Delete(name);
		return ref;
	}
}
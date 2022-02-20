

#include "pch.h"
/* Remove define from some microsoft header, the msdfgen have some variable with the same name */
#undef INFINITE
#include "msdfgen.h"
#include "msdfgen-ext.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"

#include "Text.h"
#include "utils/base_assert.h"
#include "render/gl/TextureSpecifications.h"

#include <future>

namespace msdf = msdfgen; //TODO: Annoying todo so you remember this thing is here

static std::mutex s_Mutex;

namespace Base
{

	namespace utils
	{
		template<typename T>
		msdf::Vector2 convert(const T& t)
		{
			BASE_CORE_ASSERT(false, "Template 'convert' was not specialized for this type {0}",BASE_GET_PARSE_TYPE_NAME(T));
			return t;
		}

		template<>
		msdf::Vector2 convert<glm::vec2>(const glm::vec2& other)
		{
			return msdf::Vector2(other.x, other.y);
		}

		/* Got it here 'https://raymii.org/s/tutorials/Cpp_std_async_with_a_concurrency_limit.html' */
		class Semafoor {
		public:
			explicit Semafoor(size_t count) : count(count) {}
			size_t getCount() const { return count; };
			void lock() { // call before critical section
				std::unique_lock<std::mutex> lock(mutex);
				condition_variable.wait(lock, [this] {
					if (count != 0) // written out for clarity, could just be return (count != 0);
						return true;
					else
						return false;
					});
				--count;
			}
			void unlock() {  // call after critical section
				std::unique_lock<std::mutex> lock(mutex);
				++count;
				condition_variable.notify_one();
			}

		private:
			std::mutex mutex;
			std::condition_variable condition_variable;
			size_t count;
		};

	}

	uint32_t Font::s_FreetypeDeinitializeCounter = 0;
	msdfgen::FreetypeHandle* Font::m_FreeTypeHandler = nullptr;

	Font::Font(const std::string& path, const FontSpecifications& specs, const std::string& name)
		:m_Path(path),m_Specs(specs),m_Name(name)
	{
		if(s_FreetypeDeinitializeCounter <= 0)
			m_FreeTypeHandler = msdf::initializeFreetype();

		if (m_FreeTypeHandler)
			m_FontHandler = msdf::loadFont(m_FreeTypeHandler, path.c_str());

		aConstructAtlas();

		//if (specs.LoadAsync)
		//	ContructGlyphsAsync();
		//else
		//	ContructGlyphs();

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

	void Font::ContructGlyphs()
	{
		constexpr uint8_t n_glyph = 126;
		m_Glyphs.reserve(n_glyph);

		for (uint8_t i = 48; i < n_glyph; i++)
		{
			m_Glyphs[i] = GetGlyph(i);
		}

	}

	void Font::ContructGlyphsAsync()
	{
		/*utils::Semafoor maxThreads(5);
		constexpr uint8_t n_glyph = 126;
		m_Glyphs.reserve(n_glyph);
		std::vector<std::future<AsyncGlyphInfo>> futures;
		futures.reserve(n_glyph);
		auto lamb = [&](unsigned char c, utils::Semafoor& maxJoobs)
		{
			std::scoped_lock<utils::Semafoor&> lock(maxJoobs);
			return GetGlyphAsync(c);
		};

		for (uint8_t i = 48; i < n_glyph; i++)
			futures.emplace_back(std::async(std::launch::async, lamb, i, std::ref(maxThreads)));

		bool allComplete = false;

		while (!allComplete)
		{
			allComplete = true;

			for (auto& f : futures)
			{
				if (f._Is_ready())
				{
					auto aglyph = f.get();
					if(aglyph.pixels)
						aglyph.TextureSpec.Buffer = reinterpret_cast<TextureBufferType*>(aglyph.pixels);
					Glyph g{ render::Texture::CreateTexture(aglyph.TextureSpec) , aglyph.Advance };//TODO: Get advance as well latter on
					m_Glyphs[aglyph.character] = g;
					if(aglyph.pixels)
						delete[] aglyph.pixels;
				}
				else
					if (f.valid())
						allComplete = false;
			}

		}*/
	}

	Font::~Font()
	{
		if (m_FontHandler)
			msdf::destroyFont(m_FontHandler);

		if (s_FreetypeDeinitializeCounter <= 0)
		{
			if (m_FreeTypeHandler)
			{
				msdf::deinitializeFreetype(m_FreeTypeHandler);
				m_FreeTypeHandler = nullptr;
			}
		}
		else
			s_FreetypeDeinitializeCounter--;

		if (m_FontGeo)
			delete m_FontGeo;
	}

	Glyph Font::GetGlyph(unsigned char character)
	{
		Glyph g;
		if (m_FontHandler)
		{
			msdf::Shape shape;
			if (msdf::loadGlyph(shape, m_FontHandler, character,&g.Advance))
			{
				shape.normalize();

				msdf::edgeColoringSimple(shape, m_Specs.angleThreshold);

				msdf::Bitmap<float, 3> msdf(m_Specs.width, m_Specs.height);

				auto scale = utils::convert(m_Specs.scale);
				auto translate = utils::convert(m_Specs.translate);

				msdf::generateMSDF(msdf, shape, m_Specs.range, scale, translate);

				//msdf::pixelFloatToByte();

				TextureSpecifications spec;
				spec.Width = m_Specs.width;
				spec.Height = m_Specs.height;
				spec.Channels = 3;
				spec.Buffer = reinterpret_cast<TextureBufferType*>(msdf.operator float *());
				spec.GenerateMipMap = false;
				spec.UnpackAligment = 1;
				spec.MagFilter = GL_TextureFilter::LINEAR;
				spec.MinFilter = GL_TextureFilter::LINEAR;
				spec.Type = GL_TexType::FLOAT;
				spec.TextureLodBias = 0.f;
				spec.GenerateMipMap = true;
				spec.KeepSourceBuffer = false;
				spec.CopySourceBuffer = false;
				
				//g.Texture = render::Texture::CreateTexture(spec);

			}
		}
		return g;
	}

	AsyncGlyphInfo Font::GetGlyphAsync(unsigned char character)
	{
		AsyncGlyphInfo aglyph;
		if (m_FontHandler)
		{
			msdf::Shape shape;
			bool res = false;
			{
				std::lock_guard<std::mutex> lock(s_Mutex);
				res = msdf::loadGlyph(shape, m_FontHandler, character,&aglyph.Advance);
			}
			if (res)
			{
				shape.normalize();

				msdf::edgeColoringSimple(shape, m_Specs.angleThreshold);

				msdf::Bitmap<float, 3> msdf(m_Specs.width, m_Specs.height);

				auto scale = utils::convert(m_Specs.scale);
				auto translate = utils::convert(m_Specs.translate);

				msdf::generateMSDF(msdf, shape, m_Specs.range, scale, translate);

				//msdf::pixelFloatToByte();
				TextureSpecifications spec;
				spec.Width = m_Specs.width;
				spec.Height = m_Specs.height;
				spec.Channels = 3;
				spec.GenerateMipMap = false;
				spec.UnpackAligment = 1;
				spec.MagFilter = GL_TextureFilter::LINEAR;
				spec.MinFilter = GL_TextureFilter::LINEAR;
				spec.Type = GL_TexType::FLOAT;
				spec.TextureLodBias = 0.f;
				spec.GenerateMipMap = true;
				spec.KeepSourceBuffer = false;
				spec.CopySourceBuffer = false;

				aglyph.TextureSpec = spec;
				aglyph.character = character;
				size_t s = m_Specs.width * m_Specs.height * 3;
				aglyph.pixels = new float[s];
				//memcpy_s(aglyph.pixels, s * sizeof(float), msdf.operator float* (), s * sizeof(float));
				memcpy(aglyph.pixels,msdf.operator float* (),s * sizeof(float));
			}
			else
				BASE_TRACE("Coluld not load glyph {0}", character);
		}
		return aglyph;
	}

	void Font::ConstructAtlas()
	{
		using namespace msdf_atlas;
		std::vector<GlyphGeometry> glyphs;

		FontGeometry fontGeo(&glyphs);

		fontGeo.loadCharset(m_FontHandler, 1, Charset::ASCII);
		for (GlyphGeometry& glyph : glyphs)
		{
			glyph.wrapBox(32., 4 / 32., 1.);
			glyph.edgeColoring(msdfgen::edgeColoringSimple, 3, 0);
		}
		DynamicAtlas<ImmediateAtlasGenerator<float, 3, msdfGenerator, BitmapAtlasStorage<byte, 3>>> dynamicAtlas;
		dynamicAtlas.add(glyphs.data(), glyphs.size());

		//Atlas Texture
		TextureSpecifications spec;
		msdfgen::BitmapConstRef<msdfgen::byte,3> m = dynamicAtlas.atlasGenerator().atlasStorage();
		spec.Width = m.width;
		spec.Height = m.height;
		spec.Channels = 3;
		spec.GenerateMipMap = false;
		spec.UnpackAligment = 1;
		spec.MagFilter = GL_TextureFilter::LINEAR;
		spec.MinFilter = GL_TextureFilter::LINEAR;
		spec.Type = GL_TexType::BYTE;
		spec.TextureLodBias = 0.f;
		spec.GenerateMipMap = true;
		spec.KeepSourceBuffer = false;
		spec.CopySourceBuffer = false;
		spec.DeleteSourceBuffer = false;

		//TODO: Remove copy
		size_t s = m.width * m.height * 3;
		float* buffer = new float[s];
		//memcpy_s(buffer, s * sizeof(float), m.pixels, s * sizeof(float));
		memcpy(buffer,m.pixels,s * sizeof(float));

		spec.Buffer = reinterpret_cast<TextureBufferType*>(buffer);

		m_Atlas = render::Texture::CreateTexture(spec);
		delete[] buffer;

		m_Glyphs.reserve(fontGeo.getGlyphs().size());
		for (auto& glyph : fontGeo.getGlyphs())
		{
			int x, y, w, h;
			glyph.getBoxRect(x, y, w, h);
			Glyph g;
			g.Advance = glyph.getAdvance();
			g.Texture = MakeRef<render::SubTexture>(render::SubTexture::CreateFromAtlas(m_Atlas, glm::vec2(w, h), glm::vec2(x, y), m_Atlas->GetSize()));
			//auto rect = glyph.getBoxRect()
			unsigned char c = glyph.getCodepoint();
			m_Glyphs[c] = g;
		}

	}

	void Font::aConstructAtlas()
	{
		using namespace msdf_atlas;

		int w = 0, h = 0;

		std::vector<GlyphGeometry> glyphs;

		m_FontGeo = new FontGeometry(&glyphs);

		int glyphsLoaded = m_FontGeo->loadCharset(m_FontHandler, 1, Charset::ASCII);
		for (GlyphGeometry& glyph : glyphs)
		{
			glyph.wrapBox(32., 4 / 32., 1.);
			glyph.edgeColoring(msdfgen::edgeColoringSimple, 3, 0);
		}

		TightAtlasPacker::DimensionsConstraint atlasSizeContraint = TightAtlasPacker::DimensionsConstraint::EVEN_SQUARE;
		TightAtlasPacker atlasPacker;
		atlasPacker.setDimensionsConstraint(atlasSizeContraint);
		atlasPacker.setPadding(0);
		atlasPacker.setMinimumScale(MSDF_ATLAS_DEFAULT_EM_SIZE);
		
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setUnitRange(0.0);
		atlasPacker.setMiterLimit(1.0);

		if (int remaining = atlasPacker.pack(glyphs.data(), glyphs.size())) {
			if (remaining < 0) {
				BASE_CORE_ASSERT(false, "Failed to pack glyphs into atlas.");
			}
			else {
				//printf("Error: Could not fit %d out of %d glyphs into the atlas.\n", remaining, (int)glyphs.size());
				BASE_CORE_ASSERT(false, "Error: Could not fit {0} out of {1} glyphs into the atlas.", remaining, (int)glyphs.size());
			}
		}
		atlasPacker.getDimensions(w, h);

		ImmediateAtlasGenerator<float, 3, msdfGenerator, BitmapAtlasStorage<float, 3>> generator(w, h);
		GeneratorAttributes att;
		generator.setThreadCount(std::max((int)std::thread::hardware_concurrency(), 1));
		generator.generate(glyphs.data(), glyphs.size());
		msdfgen::BitmapConstRef<float, 3> bitmap = (msdfgen::BitmapConstRef<float, 3>) generator.atlasStorage();

		//Atlas Texture
		TextureSpecifications spec;
		//msdfgen::BitmapConstRef<msdfgen::byte,3> m = bitmap.width;
		spec.Width = bitmap.width;
		spec.Height = bitmap.height;
		spec.Channels = 3;
		spec.GenerateMipMap = false;
		spec.UnpackAligment = 1;
		spec.MagFilter = GL_TextureFilter::LINEAR;
		spec.MinFilter = GL_TextureFilter::LINEAR;
		spec.Type = GL_TexType::FLOAT;
		spec.TextureLodBias = 0.f;
		spec.GenerateMipMap = false;
		spec.KeepSourceBuffer = false;
		spec.CopySourceBuffer = false;
		spec.DeleteSourceBuffer = false;

		//TODO: Remove copy
		size_t s = bitmap.width * bitmap.height * 3;
		float* buffer = new float[s];
		//memcpy_s(buffer, s * sizeof(float), bitmap.pixels, s * sizeof(float));
		memcpy(buffer,bitmap.pixels,s * sizeof(float));

		spec.Buffer = reinterpret_cast<TextureBufferType*>(buffer);
		m_Atlas = render::Texture::CreateTexture(spec);

		delete[] buffer;


		m_Glyphs.reserve(m_FontGeo->getGlyphs().size());
		for (auto& glyph : m_FontGeo->getGlyphs())
		{
			int x, y, w, h;
			glyph.getBoxRect(x, y, w, h);

			GRect r( x,y,w,h );
			r.Normilize(1.0f);

			Glyph g;
			g.Advance = glyph.getAdvance();
			g.Texture = MakeRef<render::SubTexture>(render::SubTexture::CreateFromAtlas(m_Atlas, glm::vec2(w, h), glm::vec2(x, y), m_Atlas->GetSize()));
			//auto rect = glyph.getBoxRect()
			unsigned char c = glyph.getCodepoint();
			m_Glyphs[c] = g;
		}

		//auto test = GetVertexDataFromText("Adriel");
	}

	GlyphTextInfo Font::GetVertexDataFromText(const std::string& text)
	{
		using namespace msdf_atlas;
		double texelWidth = 1. / m_Atlas->GetSize().x;
		double texelHeight = 1. / m_Atlas->GetSize().y;
		double textWidth = 0;
		msdfgen::FontMetrics FontMet;
		GlyphTextInfo info;
		if(!msdfgen::getFontMetrics(FontMet,m_FontHandler)) return info;

		double fsScale = 1 / (FontMet.ascenderY - FontMet.descenderY);
		double x = 0, y = -fsScale * FontMet.ascenderY;
		bool anyGlyphs = false;

		info.Text = text.c_str();

		for (int i = 0; i < text.size()-1;i++)
		{
			char c = text[i];
			if (c == '\r')
				continue;
			if (c == '\n') 
			{
				textWidth = std::max(textWidth, x);
				x = 0;
				y -= fsScale * FontMet.lineHeight;
				continue;
			}

			const GlyphGeometry* glyph = m_FontGeo->getGlyph(c);
			if (glyph) {
				if (!glyph->isWhitespace()) {
					double pl, pb, pr, pt;
					double il, ib, ir, it;
					glyph->getQuadPlaneBounds(pl, pb, pr, pt);
					glyph->getQuadAtlasBounds(il, ib, ir, it);
					pl *= fsScale, pb *= fsScale, pr *= fsScale, pt *= fsScale;
					pl += x, pb += y, pr += x, pt += y;
					il *= texelWidth, ib *= texelHeight, ir *= texelWidth, it *= texelHeight;

					BASE_DEBUG_LOG("{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}",
						pl, pb, il, ib,
						pr, pb, ir, ib,
						pl, pt, il, it,
						pr, pt, ir, it,
						pl, pt, il, it,
						pr, pb, ir, ib);

				}
				double advance = glyph->getAdvance();
				m_FontGeo->getAdvance(advance, text[i], text[i+1]);
				x += fsScale * advance;
				anyGlyphs = true;
			}
			

		}
	
		info.TextSize = { textWidth,-y };
		return info;
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
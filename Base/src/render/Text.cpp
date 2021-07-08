#include "Text.h"

#include <iostream>

#include <future>
#include "utils/threading.h"
#include "utils/Generic.h"
#include "utils/Logs.h"
namespace en
{
namespace render
{

    static FT_Library fft;
    static FT_Face face;
    static std::mutex* mt;

    Text::Text(const char* font)
    {
        if (FT_Init_FreeType(&fft))
            throw std::exception("ERROR::FREETYPE: Could not init FreeType Library");

        if (FT_New_Face(fft, font, 0, &face))
            throw std::exception("ERROR::FREETYPE: Failed to load font");

        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        Character charater{};

        for (unsigned char c = 0; c < 255; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture = 0;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // now store character for later use
            Character character(texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            );
            m_Characters[c] = charater;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(fft);
    }

    Text::Text(loadCharacter face)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned char c = 0; c < 255; c++)
        {
            FT_Set_Pixel_Sizes(face.face, 0, 48);
            FT_Error er = FT_Load_Char(face.face, c, FT_LOAD_RENDER);
            if (er)
            {
                const std::string msg = FT_Error_String(er);
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph: "<< msg << std::endl;
                continue;
            }
            
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
           
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Wid, m_Hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.m_Pixels);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face.face->glyph->bitmap.width,
                face.face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face.face->glyph->bitmap.buffer
            );  
            
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face.face->glyph->bitmap.width, face.face->glyph->bitmap.rows),
                glm::ivec2(face.face->glyph->bitmap_left, face.face->glyph->bitmap_top),
                static_cast<unsigned int>(face.face->glyph->advance.x)
            };
            m_Characters[c] = character;         
        }
        FT_Done_Face(face.face);
        glBindTexture(GL_TEXTURE_2D, 0);
        //TODO: I dont know how to delete all faces
    }

    Text::~Text()
    {
        for (auto& c : m_Characters)
            glDeleteTextures(1, &c.second.TextureID);
    }

    void Text::RenderText(const en::RenderArgs& args, const std::string& text, float x, float y, float scale, glm::vec3 color)
    {
        static Texture TmpTexture;

        for (auto c : text)
        {
            Character& ch = m_Characters[c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            //Draw here
            TmpTexture.SetID(ch.TextureID);
            args.render.DrawText({ xpos,ypos + h }, { w,-h }, TmpTexture,5.0f,glm::vec4(color,1.0f));
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }

    static inline void TryCreate(std::unordered_map<std::string, Text>& map, utils::ResourceLoads<std::string, loadCharacter>& info, const utils::NameCaps& nameCaps)
    {
        for (auto& inf : info.resources)
        {
            if (info.futures[inf.first]._Is_ready())
            {
                std::string name = inf.first;
                switch (nameCaps)
                {
                case utils::NameCaps::NONE: break;
                case utils::NameCaps::ALL_LOWER: name = utils::ToLower(name); break;
                case utils::NameCaps::ALL_UPPER: name = utils::ToUpper(name); break;
                default: break;
                }
                try
                {
                    map.emplace(name, inf.second);
                    D_LOG("FONT CREATED: '" << inf.first << "'");
                    info.futures.erase(inf.first);
                    info.resources.erase(inf.first);
                    break;
                }
                catch (const std::exception& ex)
                {
                    info.futures.erase(inf.first);
                    info.resources.erase(inf.first);
                    std::cout << "Can't Create Font '" << inf.first << "' , error: " << ex.what() << std::endl;
                    break;
                }
            }
        }
    }

    static inline void CreateFont(std::unordered_map<std::string, Text>& map, utils::ResourceLoads<std::string, loadCharacter>& info, const utils::NameCaps& nameCaps)
    {
        using namespace utils;
        while (!info.isAllLoad())
        {
            std::lock_guard<std::mutex> lock(info.mutex);
            TryCreate(map, info, nameCaps);
        }
        TryCreate(map, info, nameCaps);
        std::lock_guard<std::mutex> lock(info.mutex);
        info.futures.clear();
        info.resources.clear();
    }

    std::unordered_map<std::string, Text> Text::LoadFontsAsync(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps, uint8_t batchLimit)
    {
        if (FT_Init_FreeType(&fft))
            throw std::exception("ERROR::FREETYPE: Could not init FreeType Library");

        utils::ResourceLoads<std::string, loadCharacter> loads;
        mt = &loads.mutex;
        auto lamb = [&](const std::string& name, const std::string& path)
        {
            try
            {
                auto load = GetText(path.c_str(), loads.mutex);
                std::lock_guard<std::mutex> lock(loads.mutex);
                loads.resources[name] = load;
                D_LOG("Font Loaded!:  '" << name << "'");
            }
            catch (const std::exception& e)
            {
                std::lock_guard<std::mutex> lock(loads.mutex);
                std::cout << "Error trying to create Font '" << name << "', error:\n\t" << e.what() << std::endl;
            }
        };

        uint8_t count = 0;
        std::unordered_map<std::string, Text> mm;
        for (auto& name : names)
        {
            if (count > batchLimit)
            {
                CreateFont(mm, loads, nameCaps);
                loads.resources.clear();
                loads.futures.clear();
                count = 0;
            }
            loads.futures[name.first] = std::async(std::launch::async, lamb, name.first, name.second);
            count++;
        }

        CreateFont(mm, loads, nameCaps);
        FT_Done_FreeType(fft);
        return mm;
    }

    std::unordered_map<std::string, Text> Text::LoadFonts(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps)
    {
        std::unordered_map<std::string, Text>mm;
        for (auto& te : names)
        {
            std::string name = te.first;
            switch (nameCaps)
            {
            case utils::NameCaps::NONE: break;
            case utils::NameCaps::ALL_LOWER: name = utils::ToLower(name); break;
            case utils::NameCaps::ALL_UPPER: name = utils::ToUpper(name); break;
            default: break;
            }
            try
            {
                mm.emplace(name, te.second.c_str());
                D_LOG("FONT CREATED: '" << te.first << "'");
            }
            catch (const std::exception& ex)
            {
                std::cout << "Can't Create Font '" << te.first << "' , error: " << ex.what() << std::endl;
                break;
            }
        }
        return mm;
    }

    loadCharacter Text::GetText(const char* font,std::mutex& mt)
    {
        FT_Face l_face;
        {
            std::lock_guard<std::mutex> lock(mt);
            if (FT_New_Face(fft, font, 0, &l_face))
                throw std::exception("ERROR::FREETYPE: Failed to load font");
            FT_Set_Pixel_Sizes(face, 0, 48);
        }
        //FT_Done_Face(face);
        return { l_face };
    }


}
}
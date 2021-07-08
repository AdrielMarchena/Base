#pragma once
#include <string>
#include <mutex>
#include "gl/glew.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include "ft2build.h"
#include "args/RenderArgs.h"
#include FT_FREETYPE_H
namespace en
{
	namespace render
	{
		struct loadCharacter
		{
			FT_Face face;
		};
		struct Character
		{
			public:
			GLuint TextureID = 0;
			glm::ivec2 Size = {0,0};
			glm::ivec2 Bearing = {0,0};
			GLuint Advance = 0;
			Character() = default;
			Character(GLuint texID, glm::ivec2 size, glm::ivec2 bearing, GLuint advance)
			:TextureID(texID),Size(size),Bearing(bearing),Advance(advance)
			{}
		};

		class Text
		{
		private:
			std::unordered_map<char, Character> m_Characters;
			//FT_Library ft;
			//FT_Face face;
		public:
			Text() = default;
			Text(const char* font);
			Text(loadCharacter faces);
			~Text();
			void RenderText(const en::RenderArgs& args, const std::string& text, float x, float y, float scale, glm::vec3 color);

			static std::unordered_map<std::string, Text> LoadFontsAsync(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps = utils::NameCaps::NONE, uint8_t batchLimit = 10);
			static std::unordered_map<std::string, Text> LoadFonts(const std::vector<std::pair<std::string, std::string>>& names, const utils::NameCaps& nameCaps = utils::NameCaps::NONE);

		private:
			static loadCharacter GetText(const char* font, std::mutex& mt);
		};
	}
}




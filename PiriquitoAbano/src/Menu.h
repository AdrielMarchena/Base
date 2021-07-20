#pragma once

#include "colision/Colisions.h"
#include "glm/glm.hpp"
#include <functional>
#include "args/VirtualArgs.h"
#include "glm/gtx/compatibility.hpp"
#include "render/Text.h"

namespace en
{
	namespace GUI
	{
		struct Menu
		{
			Rect out_box;
			Rect button;
			std::string out_text;
			std::string button_text; 

			glm::vec3 out_text_color;
			glm::vec3 button_text_color;
			glm::vec3 hover_button_text_color;
			std::function<void(const en::UpdateArgs& args)> button_action;
			glm::vec3* actual_button_text_color = &button_text_color;

		private:
			glm::vec2 txt_out_pos;
			glm::vec2 txt_button_pos;

		public:

			void OnUpdate(const UpdateArgs& args);
			void OnRender(const RenderArgs& args, const render::Text& font);
			
			void AdjustPositions(const render::Text& font);

			//Return position so you draw a box in the middle of other box in a nice way
			static glm::vec2 middle_box_in_box(const Rect& outer_box, const Rect& inter_box);
		};
	}
}




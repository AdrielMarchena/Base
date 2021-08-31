#include "Menu.h"

namespace en
{
	namespace GUI
	{
		void Menu::OnUpdate(const en::UpdateArgs& args)
		{
			if (en::colision::Colide::PointVsRect(args.m_pos, button))
			{
				actual_button_text_color = &hover_button_text_color;
				if (args.mouse.isPress(GLFW_MOUSE_BUTTON_1))
				{
					button_action(args);
				}
			}
			else
			{
				actual_button_text_color = &button_text_color;
			}
		}
		void Menu::OnRender(const en::RenderArgs& args, std::shared_ptr<en::render::Text> font)
		{
			//AdjustPositions(font);

			args.render.DrawQuad(out_box.pos, out_box.size, { 0.2f,0.5f,0.9f,0.5f }, 3.0f);
			font->RenderText(args, out_text, txt_out_pos.x, txt_out_pos.y, 0.50f, out_text_color);

			args.render.DrawQuad(button.pos, button.size, { 0.2f,0.6f,0.2f,0.5f }, 3.0f);
			font->RenderText(args, button_text, txt_button_pos.x, txt_button_pos.y, 0.45f, *actual_button_text_color);
		}

		void Menu::AdjustPositions(std::shared_ptr<en::render::Text> font)
		{
			button.pos.x = middle_box_in_box(out_box, button).x;
			button.pos.y = glm::lerp(out_box.pos.y, out_box.pos.y + out_box.size.y, 0.15f);
			txt_out_pos = (out_box.pos + (out_box.size / 2.0f)) - (font->PreviewWid(out_text,0.50f) / 2.0f);
			txt_out_pos.y = glm::lerp(out_box.pos.y, out_box.pos.y + out_box.size.y, 0.5f);
			txt_button_pos = (button.pos + (button.size / 2.0f)) - (font->PreviewWid(button_text,0.50f) / 2.0f);
			txt_button_pos.y = glm::lerp(button.pos.y,button.pos.y + button.size.y,0.33f);
		}

		glm::vec2 Menu::middle_box_in_box(const Rect& outer_box, const Rect& inter_box)
		{
			glm::vec2 half_out = outer_box.pos + (outer_box.size / 2.0f);
			glm::vec2 half_in = inter_box.pos + (inter_box.size / 2.0f);
			return half_out - half_in;
		}
	}
}

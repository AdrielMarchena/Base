#include "Cell.h"

#include "utils/Instrumentor.h"
#include "utils/RandomList.h"
#include "render/Colors.h"
#include "render/render2D.h"
#include <functional>

void MapScript::OnCreate()
{
	auto& Transform = GetComponent<Base::TransformComponent>();
	Transform.Scale = glm::vec3(9.0f, 9.0f, 1.0f);
}

void MapScript::OnAwake()
{
	auto& Texture = GetComponent<Base::TextureComponent>().Texture;

	columns = p_Columns;
	rows = p_Rows;

	//3D array
	texture_buffer = new unsigned char[rows * columns * 4];
	for (unsigned int ix = 0; ix < rows; ++ix)
	{
		for (unsigned int iy = 0; iy < columns; ++iy)
		{
			texture_buffer[ix * columns * 4 + iy * 4 + 0] = 255 * 0.3f;   //red
			texture_buffer[ix * columns * 4 + iy * 4 + 1] = 255 * 0.3f;   //green
			texture_buffer[ix * columns * 4 + iy * 4 + 2] = 255 * 0.3f;   //blue
			texture_buffer[ix * columns * 4 + iy * 4 + 3] = 0xff; //alpha
		}
	}
	Random::Init();
	current_map.create(rows, columns);
	new_map.create(rows, columns);
	int count = int(columns * rows * 0.5f), x = 0, y = 0;
	do
	{
		x = int((P_random() << 5 ) * Random::Float()) % columns;
		y = int((P_random() << 5 ) * Random::Float()) % rows;
		new_map.set_cell(x, y);
		texture_buffer[x * columns * 4 + y * 4 + 0] = 255 * Color::Base_Color.r;   //red
		texture_buffer[x * columns * 4 + y * 4 + 1] = 255 * Color::Base_Color.g;   //green
		texture_buffer[x * columns * 4 + y * 4 + 2] = 255 * Color::Base_Color.b;   //blue
		texture_buffer[x * columns * 4 + y * 4 + 3] = 0xff; //alpha

		count--;
	} while (count >= 0);

	Texture = Base::render::Texture::CreateTexture(columns, rows, texture_buffer);
	//delete[] texture_buffer;

	current_map.copy_cells(new_map);
}

void MapScript::OnUpdate(const Base::UpdateArgs& args)
{
	using mouse = Base::input::Mouse;
	using key = Base::input::Keyboard;

	/*if (mouse::isPress(BASE_MOUSE_BUTTON_1))
	{
		const glm::vec2 mouse_pos = mouse::m_pos() * m_Size;
		//Cells[(int)col_clamp(mouse_pos.x)][(int)row_clamp(mouse_pos.y)] = true;
		//Cells[(int)CLAMP_MATRIX((float)columns, mouse_pos.x)][(int)CLAMP_MATRIX((float)rows, mouse_pos.y)] = true;
	}

	if (mouse::isPress(BASE_MOUSE_BUTTON_2))
	{
		const glm::vec2 mouse_pos = mouse::m_pos() * m_Size;
		//Cells[(int)CLAMP_MATRIX((float)columns, mouse_pos.x)][(int)CLAMP_MATRIX((float)rows, mouse_pos.y)] = false;
	}*/

	if (key::isClicked(BASE_KEY_P))
		m_CellPause = !m_CellPause;

	if (key::isClicked(BASE_KEY_I))
		m_IgnoreTimer = !m_IgnoreTimer;

	if (m_CellPause)
		return;

	if (m_CurrentCount >= 0.0f && !m_IgnoreTimer)
	{
		m_CurrentCount -= (m_CountInit) * args.dt;
		return;
	}

	auto& Texture = GetComponent<Base::TextureComponent>().Texture;
	current_map.next_generation(new_map, Texture, texture_buffer);

	current_map.copy_cells(new_map);

	m_CurrentCount = m_CountInit;
}

void MapScript::OnDestroy()
{
	current_map.destroy();
	new_map.destroy();
	delete[] texture_buffer;
}

void cell_map::create(unsigned int h, unsigned int w)
{
	width = w;
	width_in_bytes = (w + 7) / 8;
	height = h;
	length_in_bytes = width_in_bytes * h;
	cells = new cell_type[length_in_bytes];
	memset(cells, 0, length_in_bytes);
}

void cell_map::destroy()
{
	delete[] cells;
}

void cell_map::clear_cell(unsigned int x, unsigned int y)
{
	cell_type* cell_ptr =
		cells + (y * width_in_bytes) + (x / 8);

	*(cell_ptr) &= ~(0x80 >> (x & 0x07));
}

void cell_map::set_cell(unsigned int x, unsigned int y)
{
	cell_type* cell_ptr =
		cells + (y * width_in_bytes) + (x / 8);

	*(cell_ptr) |= 0x80 >> (x & 0x07);
}

void cell_map::copy_cells(cell_map& sourcemap)
{
	memcpy(cells, sourcemap.cells, length_in_bytes);
}

int cell_map::cell_state(int x, int y)
{
	cell_type* cell_ptr;
#if WRAP_EDGES
	while (x < 0) x += width;     // wrap, if necessary
	while (x >= width) x -= width;
	while (y < 0) y += height;
	while (y >= height) y -= height;
#else
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return 0;   // return 0 for off edges if no wrapping
#endif
	cell_ptr = cells + (y * width_in_bytes) + (x / 8);
	return (*cell_ptr & (0x80 >> (x & 0x07))) ? 1 : 0;
}

void cell_map::next_generation(cell_map& next_map, Base::Ref<Base::render::Texture>& texture, unsigned char* texture_buffer)
{

	unsigned int x, y, neighbor_count;

	//3D array
	BASE_ASSERT(texture_buffer != nullptr);
	//delete[] texture_buffer; //Needed?
	//texture_buffer = nullptr;
	//texture_buffer = new unsigned char[height * width * 4];

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			// Figure out how many neighbors this cell has
			neighbor_count = cell_state(x - 1, y - 1) + cell_state(x, y - 1) +
				cell_state(x + 1, y - 1) + cell_state(x - 1, y) +
				cell_state(x + 1, y) + cell_state(x - 1, y + 1) +
				cell_state(x, y + 1) + cell_state(x + 1, y + 1);
			if (cell_state(x, y) == 1) {
				// The cell is on; does it stay on?
				if ((neighbor_count != 2) && (neighbor_count != 3)) {
					next_map.clear_cell(x, y);    // turn it off
					texture_buffer[x * width * 4 + y * 4 + 0] = 255 * 0.3f; //R
					texture_buffer[x * width * 4 + y * 4 + 1] = 255 * 0.3f; //G
					texture_buffer[x * width * 4 + y * 4 + 2] = 255 * 0.3f; //B
					texture_buffer[x * width * 4 + y * 4 + 3] = 255; //A
				}
				else
				{
					texture_buffer[x * width * 4 + y * 4 + 0] = Color::Base_Color.r * 255; //R
					texture_buffer[x * width * 4 + y * 4 + 1] = Color::Base_Color.g * 255; //G
					texture_buffer[x * width * 4 + y * 4 + 2] = Color::Base_Color.b * 255; //B
					texture_buffer[x * width * 4 + y * 4 + 3] = 255; //A
				}
			}
			else {
				// The cell is off; does it turn on?
				if (neighbor_count == 3) {
					next_map.set_cell(x, y);      // turn it on
					texture_buffer[x * width * 4 + y * 4 + 0] = Color::Base_Color.r * 255; //R
					texture_buffer[x * width * 4 + y * 4 + 1] = Color::Base_Color.g * 255; //G
					texture_buffer[x * width * 4 + y * 4 + 2] = Color::Base_Color.b * 255; //B
					texture_buffer[x * width * 4 + y * 4 + 3] = 255; //A
				}
				else
				{
					texture_buffer[x * width * 4 + y * 4 + 0] = 255 * 0.3f; //R
					texture_buffer[x * width * 4 + y * 4 + 1] = 255 * 0.3f; //G
					texture_buffer[x * width * 4 + y * 4 + 2] = 255 * 0.3f; //B
					texture_buffer[x * width * 4 + y * 4 + 3] = 255; //A
				}
			}
		}
	}

	texture->Dispose();
	texture = Base::render::Texture::CreateTexture(width, height, texture_buffer);

	//delete[] texture_buffer;

}


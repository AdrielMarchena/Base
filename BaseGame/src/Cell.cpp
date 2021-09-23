#include "Cell.h"

#include "utils/Instrumentor.h"
#include "utils/RandomList.h"
#include "render/Colors.h"
#include "render/render2D.h"
#include <functional>
#define ITERATE_MATRIX(x) {for(int col = 0; col < columns; col++){\
							for (int row = 0; row < rows; row++){\
								x;}}}

#define CLAMP_MATRIX(m,n) std::clamp(n, 0.0f, m-1)

void MapScript::OnCreate()
{
	double ratio = Base::WindowProps().aspect_ratio;
	transform.Scale = glm::vec3(m_Size, m_Size, 0.0f);
	start = glm::vec3(-5 * ratio, -5.0f, 1.0f) + (transform.Scale * 0.5f);

	columns = 96;
	rows = 96;

	current_map.create(rows, columns);
	new_map.create(rows, columns);
	int count = 25 , x = 0, y = 0;
	do
	{
		x = P_random() << 2 % columns;
		y = P_random() << 2 % rows;
		new_map.set_cell(x, y);
		//new_map.set_cell(count, count);
		count--;
	} 	while (count >= 0);
	current_map.copy_cells(new_map);
}

void MapScript::ExtraRender()
{
	using render = Base::render::Render2D;

	transform.Translation = start;

	for (int y = 0; y < new_map.height; y++) {
		for (int x = 0; x < new_map.width; x++) {
			bool alive = new_map.cell_state(x, y);
			transform.Translation = start + (glm::vec3(x * transform.Scale.x, y * transform.Scale.y, 0.0f) * 1.1f);
			render::DrawQuad(transform.GetTransform(), alive ? Color::Base_Color : glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		}
	}

}

void MapScript::OnUpdate(const Base::UpdateArgs& args)
{
	using mouse = Base::input::Mouse;
	using key = Base::input::Keyboard;

	if (mouse::isPress(BASE_MOUSE_BUTTON_1))
	{
		const glm::vec2 mouse_pos = mouse::m_pos() * m_Size;
		//Cells[(int)col_clamp(mouse_pos.x)][(int)row_clamp(mouse_pos.y)] = true;
		//Cells[(int)CLAMP_MATRIX((float)columns, mouse_pos.x)][(int)CLAMP_MATRIX((float)rows, mouse_pos.y)] = true;
	}

	if (mouse::isPress(BASE_MOUSE_BUTTON_2))
	{
		const glm::vec2 mouse_pos = mouse::m_pos() * m_Size;
		//Cells[(int)CLAMP_MATRIX((float)columns, mouse_pos.x)][(int)CLAMP_MATRIX((float)rows, mouse_pos.y)] = false;
	}

	if (key::isClicked(BASE_KEY_P))
		m_CellPause = !m_CellPause;

	if (m_CellPause)
		return;

	if (m_CurrentCount >= 0.0f)
	{
		m_CurrentCount -= (m_CountInit * 0.5f) * args.dt;
		return;
	}

	current_map.next_generation(new_map);

	current_map.copy_cells(new_map);

	m_CurrentCount = m_CountInit;
}

void MapScript::OnDestroy()
{
	current_map.destroy();
	new_map.destroy();
}

void cell_map::create(unsigned int h, unsigned int w)
{
	width = w;
	width_in_bytes = (w + 7) / 8;
	height = h;
	length_in_bytes = width_in_bytes * h;
	cells = new unsigned char[length_in_bytes];
	memset(cells, 0, length_in_bytes);
}

void cell_map::destroy()
{
	delete[] cells;
}

void cell_map::clear_cell(unsigned int x, unsigned int y)
{
	unsigned char* cell_ptr =
		cells + (y * width_in_bytes) + (x / 8);

	*(cell_ptr) &= ~(0x80 >> (x & 0x07));
}

void cell_map::set_cell(unsigned int x, unsigned int y)
{
	unsigned char* cell_ptr =
		cells + (y * width_in_bytes) + (x / 8);

	*(cell_ptr) |= 0x80 >> (x & 0x07);
}

void cell_map::copy_cells(cell_map& sourcemap)
{
	memcpy(cells, sourcemap.cells, length_in_bytes);
}

int cell_map::cell_state(int x, int y)
{
	unsigned char* cell_ptr;
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

void cell_map::next_generation(cell_map& next_map)
{

	unsigned int x, y, neighbor_count;

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
					//transform.Translation = start + (glm::vec3(x * transform.Scale.x, y * transform.Scale.y, 0.0f) * 1.1f);
					//render::DrawQuad(transform.GetTransform(), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
				}
			}
			else {
				// The cell is off; does it turn on?
				if (neighbor_count == 3) {
					next_map.set_cell(x, y);      // turn it on
					//transform.Translation = start + (glm::vec3(x * transform.Scale.x, y * transform.Scale.y, 0.0f) * 1.1f);
					//render::DrawQuad(transform.GetTransform(), Color::Base_Color);
				}
			}
		}
	}
}


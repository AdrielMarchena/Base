#include "Cell.h"

#include <iostream>
#include <algorithm>
#include "imgui.h"
const glm::vec2 Size = { 25.0f,25.0f };
const glm::vec4 Global_Live_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
const glm::vec4 Global_Dead_Color = { 0.1f, 0.1f, 0.1f, 1.0f };

static void IterateMap(const std::function<void(int col, int row)>& func)
{
	for (int col = 0; col < TOTAL_COLUMNS; col++)
	{
		for (int row = 0; row < TOTAL_COLUMNS; row++)
		{
			func(col, row);
		}
	}
}

bool Map::CheckNeighbours(int p_col, int p_row, float dt)
{
	//Rules
	// < 3 livin neighbours = Die
	// = 3 livin neighbours = Live
	// > 3 livin neighbours = Die
	// if dead and = 3 livin neighbours = revive
	if (p_col < 0 || p_row < 0)
		return false;
	
	//    #0 #1 #2
	// #0  N  N	 N	 N = Neighbour
	// #1  N  O	 N   O = Actual Cell
	// #2  N  N	 N
	constexpr char middle = 1;
	int living_neighbours = 0;
	bool alive = false;
	int col_start = p_col == 0 ? 0 : p_col - 1;
	int col_end = p_col == TOTAL_COLUMNS ? p_col - 1 : p_col + 1;
	int row_start = p_row == 0 ? 0 : p_row - 1;
	int row_end = p_row == TOTAL_ROWS ? p_row - 1 : p_row + 1;


	for (int col = col_start; col <= col_end; col++)
	{
		if (col < 0 || col >= TOTAL_COLUMNS)
			continue; // Skip out of bounds
		for (int row = row_start; row <= row_end; row++)
		{
			if (row < 0 || row >= TOTAL_COLUMNS)
				continue; // Skip out of bounds
			if (col == p_col && row == p_row)
			{
				alive = OldCells[col][row];
				continue; // don't check with yourself
			}
			bool populated = OldCells[col][row];
			if (populated)
				living_neighbours++;
		}
	}

	//Dead
	if (!alive)
	{
		if (living_neighbours == 3)
			return true;
		return false;
	}
	//Alive
	if (living_neighbours == 2 || living_neighbours == 3)
		return true;
	return false;

}

static void CopyHere(bool origin[TOTAL_COLUMNS][TOTAL_ROWS], bool dest[TOTAL_COLUMNS][TOTAL_ROWS])
{
	std::copy(&origin[0][0], &origin[0][0] + TOTAL_COLUMNS * TOTAL_ROWS, &dest[0][0]);
}

void Map::UpdateCells(const en::UpdateArgs& args)
{
	//Mouse test
	if (args.mouse.isPress(GLFW_MOUSE_BUTTON_1))
	{
		auto mouse_pos = (args.m_pos / Size + glm::vec2(400/25, 300/25));
		OldCells[(int)mouse_pos.x][(int)mouse_pos.y] = true;
	}

	if (args.mouse.isPress(GLFW_MOUSE_BUTTON_2))
	{
		auto mouse_pos = (args.m_pos / Size + glm::vec2(400/25, 300/25));
		OldCells[(int)mouse_pos.x][(int)mouse_pos.y] = false;
	}

	if (args.keyboard.isClicked(GLFW_KEY_Q))
	{
		IterateMap([&](int col, int row)
		{
			OldCells[col][row] = false;
		});
	}

	if (args.keyboard.isClicked(GLFW_KEY_P))
	{
		pause = !pause;
	}

	if (pause) // Do nothing other than check input if paused
		return;


	static float timestamp = init_timestamp;
	if (timestamp >= 0)
	{
		timestamp -= decay_timestamp * args.dt;
		return;
	}

	CopyHere(OldCells,NewCells);

	IterateMap([&](int col, int row)
	{
		NewCells[col][row] = CheckNeighbours(col, row);
	});
	
	CopyHere(NewCells,OldCells);
	timestamp = init_timestamp;
}

void Map::DrawCells(const en::RenderArgs& args)
{
	for (int col = 0; col < TOTAL_COLUMNS; col++)
	{
		for (int row = 0; row < TOTAL_ROWS; row++)
		{
			const glm::vec4& quad_color = OldCells[col][row] ? Global_Live_Color : Global_Dead_Color;
			const glm::vec4& grid_color = !OldCells[col][row] ? Global_Live_Color : Global_Dead_Color;
			float cm = args.camera_ctr.GetZoomLevel();
			args.render.DrawOutLineQuad(glm::vec2(col, row) * Size - glm::vec2(400, 300), Size, grid_color, 0.5f);
			args.render.DrawQuad(glm::vec2(col, row) * Size - glm::vec2(400,300) , Size , quad_color);
		}
	}
	for (auto& lamb : m_RenderThisPlease)
		lamb(args);
	m_RenderThisPlease.clear();
}

void Map::OnImGui(const en::ImGuiArgs& args)
{
	ImGui::Text("Configs");
	ImGui::SliderFloat("Init timestamp velocity", &init_timestamp, -1.0f, 5.0f);
	ImGui::SliderFloat("Decay timestamp velocity", &decay_timestamp, 0.0001f, 5.0f);
	if(ImGui::Button("Pause"))
	{
		pause = !pause;
	}
}

void Map::OnAttach(const std::vector<InitActiveCell>& actives)
{

	IterateMap([&](int col, int row)
	{ 
		OldCells[col][row] = false;
	});

	for (auto& ac : actives)
	{
		if (ac.Column >= TOTAL_COLUMNS || ac.Row >= TOTAL_ROWS)
			continue;
		OldCells[ac.Column][ac.Row] = true;
	}
}

Map::Map(const std::vector<InitActiveCell>& actives)
{
	OnAttach(actives);
}
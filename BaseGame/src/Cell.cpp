#include "Cell.h"

const glm::vec2 Size = { 5.0f,5.0f };
const glm::vec3 Global_Live_Color = { 1.0f, 1.0f, 1.0f };
const glm::vec3 Global_Dead_Color = { 0.1f, 0.1f, 0.1f };

static bool CheckNeighbours(int col, int row, bool map[TOTAL_COLUMNS][TOTAL_ROWS])
{
	//Rules
	// < 3 livin neighbours = Die
	// = 3 livin neighbours = Live
	// > 3 livin neighbours = Die
	// if dead and = 3 livin neighbours = revive
	if (col < 0 || row < 0)
		return false;
	
	//    #0 #1 #2
	// #0  N  N	 N	 N = Neighbour
	// #1  N  O	 N   O = Actual Cell
	// #2  N  N	 N
	constexpr char middle = 1;
	int living_neighbours = 0;

	int col_start = col - 1;
	int col_end = col + 1;
	int row_start = row - 1;
	int row_end = row + 1;

	for (int col = col_start; col < col_end; col++)
	{
		if (col < 0 || col >= TOTAL_COLUMNS)
			continue; // Skip out of bounds
		for (int row = row_start; row < row_end; row++)
		{
			if (row < 0 || row >= TOTAL_COLUMNS)
				continue; // Skip out of bounds
			if (col == middle && row == middle)
				continue; // don't check with yourself
			if (map[col, row])
				living_neighbours++;
		}
	}

	if (living_neighbours == 3)
		return true;
	else
		return false;

}

static void CopyHere(bool origin[TOTAL_COLUMNS][TOTAL_ROWS], bool dest[TOTAL_COLUMNS][TOTAL_ROWS])
{
	std::copy(&origin[0][0], &origin[0][0] + TOTAL_COLUMNS * TOTAL_ROWS, &dest[0][0]);
}

void Map::UpdateCells(const en::UpdateArgs& args)
{
	CopyHere(OldCells,NewCells);

	for (int col = 0; col < TOTAL_COLUMNS; col++)
	{
		for (int row = 0; row < TOTAL_ROWS; row++)
		{
			NewCells[col][row] = CheckNeighbours(col,row,NewCells);
		}
	}
	CopyHere(NewCells,OldCells);
}

void Map::DrawCells(const en::RenderArgs& args)
{
	for (int col = 0; col < TOTAL_COLUMNS; col++)
	{
		for (int row = 0; row < TOTAL_ROWS; row++)
		{
			const glm::vec3& color = OldCells[col][row] ? Global_Live_Color : Global_Dead_Color;
			args.render.DrawQuad(glm::vec2(col, row) * Size, Size, glm::vec4(color, 1.0f));
		}
	}
}

void Map::OnAttach(const std::vector<InitActiveCell>& actives)
{
	for (int col = 0; col < TOTAL_COLUMNS; col++)
	{
		for (int row = 0; row < TOTAL_COLUMNS; row++)
		{
			OldCells[col][row] = false;
		}
	}
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
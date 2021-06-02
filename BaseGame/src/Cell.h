#pragma once
#include "glm/glm.hpp"
#include "args/UpdateArgs.h"
#include "args/RenderArgs.h"
#include <functional>

#define TOTAL_COLUMNS 800 / 25
#define TOTAL_ROWS 600 / 25

struct InitActiveCell
{
	int Column = 0;
	int Row = 0;
};

class Map
{
private:
	std::vector<std::function<void(const en::RenderArgs& r_args)>> m_RenderThisPlease;
	bool OldCells[TOTAL_COLUMNS][TOTAL_ROWS];
	bool NewCells[TOTAL_COLUMNS][TOTAL_ROWS];
	bool pause = false;
public:
	Map(const std::vector<InitActiveCell>& actives);
	Map() = default;
	void OnAttach(const std::vector<InitActiveCell>& actives);
	void UpdateCells(const en::UpdateArgs& args);
	void DrawCells(const en::RenderArgs& args);
private:
	bool CheckNeighbours(int p_col, int p_row,float dt = 1);
};
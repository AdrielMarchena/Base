#pragma once
#include "glm/glm.hpp"
#include "args/UpdateArgs.h"
#include "args/RenderArgs.h"

#define TOTAL_COLUMNS 100
#define TOTAL_ROWS 100

struct InitActiveCell
{
	int Column = 0;
	int Row = 0;
};

class Map
{
private:
	bool OldCells[TOTAL_COLUMNS][TOTAL_ROWS];
	bool NewCells[TOTAL_COLUMNS][TOTAL_ROWS];
public:
	Map(const std::vector<InitActiveCell>& actives);
	Map() = default;
	void OnAttach(const std::vector<InitActiveCell>& actives);
	void UpdateCells(const en::UpdateArgs& args);
	void DrawCells(const en::RenderArgs& args);
};
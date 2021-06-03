#pragma once
#include "glm/glm.hpp"
#include "args/UpdateArgs.h"
#include "args/RenderArgs.h"
#include "args/ImGuiArgs.h"
#include <functional>

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
	std::vector<std::function<void(const en::RenderArgs& r_args)>> m_RenderThisPlease;
	//TODO: Pass this matrix to Heap memory
	bool** OldCells;
	bool** NewCells;
	bool pause = false;
	float init_timestamp = 1.0f;
	float decay_timestamp = 1.0f;
public:
	Map(const std::vector<InitActiveCell>& actives);
	Map();
	~Map();
	void OnAttach(const std::vector<InitActiveCell>& actives);
	void UpdateCells(const en::UpdateArgs& args);
	void DrawCells(const en::RenderArgs& args);
	void OnImGui(const en::ImGuiArgs& args);
private:
	bool CheckNeighbours(int p_col, int p_row,float dt = 1);
	void InitMatrix();
};
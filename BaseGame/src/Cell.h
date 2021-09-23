#pragma once
#include "glm/glm.hpp"
#include <functional>
#include "scene/Components.h"

#include "scene/ScriptableEntity.h"

struct CellState
{
	int x = 0;
	int y = 0;
	bool alive;
};

class cell_map
{
public:

	unsigned char* cells;
	unsigned int width;
	unsigned int width_in_bytes;
	unsigned int height;
	unsigned int length_in_bytes;

	void create(unsigned int h, unsigned int w);
	void destroy();

	void copy_cells(cell_map& sourcemap);
	void set_cell(unsigned int x, unsigned int y);
	void clear_cell(unsigned int x, unsigned int y);
	int cell_state(int x, int y);
	void next_generation(cell_map& next_map);

};

class MapScript : public Base::ScriptableEntity
{
private:
	//bool** OldCells;
	//bool** NewCells;
	cell_map current_map;
	cell_map new_map;
	Base::TransformComponent transform;
	glm::vec3 start = {0.0f,0.0f,0.0f};
	bool m_CellPause = true;
	float m_CountInit = 5.0f;
	float m_CurrentCount = 1.0f;
	uint8_t columns = 50;
	uint8_t rows = 50;
	float m_Size = 0.35f;

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void ExtraRender() override;
	virtual void OnDestroy() override;

};
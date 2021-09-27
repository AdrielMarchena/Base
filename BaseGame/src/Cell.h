#pragma once
#include "glm/glm.hpp"
#include <functional>
#include "scene/Components.h"

#include "scene/ScriptableEntity.h"
#include "Base/Base.h"
#include "render/gl/Texture.h"

typedef unsigned char cell_type;

class cell_map
{
public:

	cell_type* cells;
	unsigned int width;
	unsigned int width_in_bytes;
	unsigned int height;
	unsigned int length_in_bytes;

	unsigned int to_be_alive1 = 2;
	unsigned int to_be_alive2 = 3;
	unsigned int to_revive = 3;

	void create(unsigned int h, unsigned int w);
	void destroy();

	void copy_cells(cell_map& sourcemap);
	void set_cell(unsigned int x, unsigned int y);
	void clear_cell(unsigned int x, unsigned int y);
	int cell_state(int x, int y);
	void next_generation(cell_map& next_map, Base::Ref<Base::render::Texture>& texture);

};

class MapScript : public Base::ScriptableEntity
{
private:
	cell_map current_map;
	cell_map new_map;
	bool m_CellPause = true;
	bool m_IgnoreTimer = false;
	float m_CountInit = 5.0f;
	float m_CurrentCount = 1.0f;
	uint32_t columns = 50;
	uint32_t rows = 50;

protected:

	virtual void OnCreate() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(const Base::UpdateArgs& args) override;
	virtual void OnDestroy() override;
public:
	//unsigned char* GetTexBuffer() { return texture_buffer; }
	uint32_t p_Columns = 24;
	uint32_t p_Rows = 24;
	unsigned int ToBeAlive1 = 2;
	unsigned int ToBeAlive2 = 3;
	unsigned int ToRevive = 3;
};
/*
*	@file ImGuiArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once
#include "render/render2D.h"

namespace en
{
	struct ImGuiArgs
	{
		render::render2D& render;
	};
}
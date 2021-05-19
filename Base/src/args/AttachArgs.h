/*
*	@file AttachArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/
#pragma once

#include "render/render2D.h"
#include "camera/OrthographicCameraController.h"
namespace en
{
	struct AttachArgs
	{
		render::render2D& render;
		OrthographicCameraController& camera;
	};
}
/*
*	@file RenderArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once
#include "camera/OrthographicCameraController.h"
#include "render/render2D.h"
namespace en{
	struct RenderArgs
	{
		float dt;
		render::Render2D& render;
		OrthographicCameraController& camera_ctr;
		OrthographicCamera& camera;
	}; 
}
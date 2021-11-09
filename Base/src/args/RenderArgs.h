/*
*	@file RenderArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once
#include "camera/OrthographicCameraController.h"
#include "render/render2D.h"
namespace Base{
	struct RenderArgs
	{
		float dt;
		OrthographicCameraController& camera_ctr;
		OrthographicCamera& camera;
		float width;
		float height;
		float res_w;
		float res_h;
	}; 
}
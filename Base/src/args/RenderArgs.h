#pragma once
#include "camera/OrthographicCameraController.h"
#include "render/render2D.h"
namespace en{
	struct RenderArgs
	{
		float dt;
		render::render2D& render;
		OrthographicCameraController& camera_ctr;
		OrthographicCamera& camera;
	}; 
}
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
		render::Render2D& render; //Why this is here?
		OrthographicCameraController& camera;
		float width;
		float height;
		float res_w;
		float res_h;
	};
}
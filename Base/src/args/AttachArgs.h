/*
*	@file AttachArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/
#pragma once

#include "render/render2D.h"
#include "camera/OrthographicCameraController.h"
namespace Base
{
	struct AttachArgs
	{
		OrthographicCameraController& camera;
		float width;
		float height;
		float res_w;
		float res_h;
	};
}
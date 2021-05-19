/*
*	@file ResizeArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method or event
*/

#pragma once

namespace en
{
	struct ResizeArgs
	{
		float new_w, new_h;
		float old_w, old_h;
	};
}
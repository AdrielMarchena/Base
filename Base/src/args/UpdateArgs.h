/*
*	@file UpdateArgs.h
*	@author Adriel Marchena Santos
*	
*	Args passed to a Window method
*/

#pragma once

struct UpdateArgs
{
	float dt;

	UpdateArgs() = default;
	UpdateArgs(float deltaTime)
		:dt(deltaTime){}
	UpdateArgs(const UpdateArgs&) = default;
};

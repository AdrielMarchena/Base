/*
* 	@file RandomList.cpp
*
* 	File handlind things here
* 	Part a grab from The Cherno
*	Other part from Doom source code

*/

#pragma once
#include <random>

int P_random();
int M_random();
void ResetPointers();

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};
#pragma once
#include <SDL2/SDL.h>
#define _USE_MATH_DEFINES
#include <cmath> 
const float screenScale = 4.00f;


namespace utils
{
	inline float hireTimeInSeconds()
	{
		float t = SDL_GetTicks();
		t *= 0.001f;
		return t;
	}
	inline float sinWave(float t, float AMPLITUDE = 1.0f, float FREQUENCY = 1.0f, float ZERO_OFFSET=1.0f) 
	{
		return AMPLITUDE * sin(2 * FREQUENCY * M_PI * 0.15 * t + 0) + ZERO_OFFSET;
	}
	
}	
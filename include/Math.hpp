#pragma once

#include <cmath>
#include <iostream>

struct Vector2f
{
	Vector2f()
	:x(0.0f) ,y(0.0f)
	{}

	Vector2f(float p_x,float p_y)
	:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << x << ", " << y << std::endl;
	}

	void add(float p_mx, float p_my)
	{
		x += p_mx;
		y += p_my;
	}
	void addVec(Vector2f p_vec)
	{
		x += p_vec.x;
		y += p_vec.y;
	}
	void decrease(float p_decel_x,float p_decel_y)
	{
		x += ((round(x*acc)/acc) > 0? -p_decel_x: (round(x*acc)/acc) < 0? p_decel_x : 0);
		y += ((round(y*acc)/acc) > 0? -p_decel_y: (round(y*acc)/acc) < 0? p_decel_y : 0);
		if((round(x*acc2)/acc2) == 0)
			x=0;		
		if((round(y*acc2)/acc2) == 0)
			y=0;
	}
	
	float x,y;	
	float acc = 100.0f;
	float acc2 = 10.0f;
};

namespace MathUtils{
	float clip(float n, float lower, float upper);
}

#pragma once	
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Utils.hpp"

class Entity
{
public:
	Entity(Vector2f p_pos, SDL_Texture* p_tex ,float p_scale = 1);
	Vector2f& getPos()
	{
		return pos;
	}	
	void setPos(Vector2f p_pos)
	{
		pos.x = p_pos.x / screenScale;
		pos.y = p_pos.y / screenScale;
	}
	void setTruePos(Vector2f p_pos)
	{
		pos = p_pos;
	}
	float getScale();
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
	void updateDVD();
	void update();
	void move(Vector2f p_translate);
	void addVel(Vector2f p_vel);
	void setVel(Vector2f p_vel);
	void setVelX(float p_velx);
	void setVelY(float p_vely);
	void addVelX(float p_velx);
	void addVelY(float p_vely);	
	void setPosX(float p_posx);
	void setPosY(float p_posy);



	int distanceFromWallLeft(Entity p_ent);
	int distanceFromWallRight(Entity p_ent);
	int distanceFromWallDown(Entity p_ent);
	int distanceFromWallUp(Entity p_ent);
	int distanceFromWallX(Entity p_ent);
	int distanceFromWallY(Entity p_ent);
	int collidesX(Entity p_ent);
	int collidesY(Entity p_ent);

    bool canGoNegX = true;
    bool canGoNegY = true;
    bool canGoPosX = true;
    bool canGoPosY = true;
	
	Vector2f getVel();
	float getVelX();
	float getVelY();
	bool beingHeld = false;
private:
	Vector2f pos;
	Vector2f vel;
	float gravity = 0.25f;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
	bool movx = true,movy = true;;
	float scale;
};
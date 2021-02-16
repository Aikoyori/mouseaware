#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"

const float collisionAccuracy = 0.0f;//screenScale;//2.0f;
const float cornerAccuracy = 0.1f;
Vector2f originalPos,originalVel,tempPos;

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex, float p_scale)
:pos(p_pos), tex(p_tex), scale(p_scale)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 16;
	currentFrame.h = 16;
}
	
float Entity::getScale()
{
	return scale;
}
SDL_Texture* Entity::getTex()
{
	return tex;
}
SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}
void Entity::updateDVD()
{

	if(pos.x >= 1280 / 4.0f - currentFrame.w)
	{
		movx = false;
	}
	if(pos.y >= 720 / 4.0f - currentFrame.h)
	{
		movy = false;
	}
	if(pos.x <= 0)
	{
		movx = true;
	}
	if(pos.y <= 0)
	{
		movy = true;
	}
	if(movx)
	{
		if(movy)
		{

			pos.addVec(Vector2f(1.0f,1.0f));
		}
		else
		{
			pos.addVec(Vector2f(1.0f,-1.0f));
		}
	}
	else
	{

	{
		if(movy)
		{

			pos.addVec(Vector2f(-1.0f,1.0f));
		}
		else
		{
			pos.addVec(Vector2f(-1.0f,-1.0f));
		}
	}
	}
}
void Entity::update()
{
	// Boundaries

	originalPos = pos;
	tempPos = pos;
	originalVel = vel;
	vel.decrease(0.10f,0.0f);
	tempPos.addVec(vel);
	if(tempPos.x<0)
	{

		setVelX(0.0f);
		tempPos.x=0;
	}
	else if(tempPos.x + currentFrame.w * scale >= 1280/screenScale)
	{
		setVelX(0.0f);
		tempPos.x=1280/screenScale - (currentFrame.w * scale) ;
			
	}
	else
	{

		if(!canGoNegX && (((vel.x>0)-(vel.x<0)) == -1)) {setVelX(0.0f);tempPos.x = originalPos.x; std::cout << "COLLIDE WITH LEFT" << std::endl;}
		if(!canGoPosX && (((vel.x>0)-(vel.x<0)) == 1)) {setVelX(0.0f);tempPos.x = originalPos.x; std::cout << "COLLIDE WITH RIGHT" << std::endl;}
	}

	// GRAVITY
	if(tempPos.y<0)
	{

		setVelY(0.0f);
		tempPos.y=0;
	}
	else if(tempPos.y + currentFrame.h * scale >= 720/screenScale)
	{
		setVelY(0.0f);
		tempPos.y=(720/screenScale - (currentFrame.h * scale));
	}
	else
	{
		
		if(!canGoNegY && (((vel.y>0)-(vel.y<0)) == -1)) {setVelY(0.0f); tempPos.y = originalPos.y; std::cout << "COLLIDE WITH CEIL" << std::endl;}
		if(!canGoPosY && (((vel.y>0)-(vel.y<0)) == 1)) {setVelY(0.0f);tempPos.y = originalPos.y;std::cout << "COLLIDE WITH FLOOR" << std::endl;}
		if(canGoPosY && !beingHeld) vel.addVec(Vector2f(0.0f,gravity));

	}

	if (!beingHeld)pos = tempPos;
	else if(!canGoNegX || !canGoPosX||!canGoNegY || !canGoPosY){
		pos = tempPos; 	
	}

}


void Entity::move(Vector2f p_translate)
{
	pos.addVec(p_translate);
}
void Entity::addVel(Vector2f p_vel)
{
	vel.addVec(p_vel);
}
void Entity::setVel(Vector2f p_vel)
{
	vel=p_vel;
}
void Entity::setVelX(float p_velx)
{
	vel.x=p_velx;
}
void Entity::setVelY(float p_vely)
{
	vel.y=p_vely;
}
void Entity::addVelX(float p_velx)
{
	vel.x+=p_velx;
}
void Entity::addVelY(float p_vely)
{
	vel.y+=p_vely;
}
void Entity::setPosX(float p_posx)
{
	pos.x=p_posx;
}
void Entity::setPosY(float p_posy)
{
	pos.y=p_posy;
}
int Entity::distanceFromWallLeft(Entity p_ent)
{
	if((((pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x>=(p_ent.getPos().x)))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return p_ent.getPos().x - pos.x;
	return -1;
}
int Entity::distanceFromWallRight(Entity p_ent)
{
	if((((pos.x+currentFrame.w)<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x+currentFrame.w>=(p_ent.getPos().x))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return p_ent.getPos().x - pos.x;
	return -1;
}
int Entity::distanceFromWallDown(Entity p_ent)
{
	if((((pos.y+currentFrame.h)<(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y+currentFrame.h>=(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return p_ent.getPos().y - pos.y;
	return -1;
}

int Entity::distanceFromWallUp(Entity p_ent)
{
	if(((pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y>(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return p_ent.getPos().y - pos.y;
	return -1;
}

int Entity::distanceFromWallX(Entity p_ent)
{
	if((((pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x>=(p_ent.getPos().x)))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return p_ent.getPos().x - pos.x;
	if((((pos.x+currentFrame.w)<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x+currentFrame.w>=(p_ent.getPos().x))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return p_ent.getPos().x - pos.x;
	return 0;
}
int Entity::distanceFromWallY(Entity p_ent)
{
	if(((pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y>(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return p_ent.getPos().y - pos.y;
	if((((pos.y+currentFrame.h)<(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y+currentFrame.h>=(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return p_ent.getPos().y - pos.y;
	return -1;
}

int Entity::collidesX(Entity p_ent)
{
	if((((pos.x+currentFrame.w)<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x+currentFrame.w>=(p_ent.getPos().x))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)) && (abs(distanceFromWallX(p_ent)) > abs(distanceFromWallY(p_ent))))
		return 1;
	if((((pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x>=(p_ent.getPos().x)))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy))&& (abs(distanceFromWallX(p_ent)) > abs(distanceFromWallY(p_ent))))
		return -1;
	return 0;
}
int Entity::collidesY(Entity p_ent)
{
	if((((pos.y+currentFrame.h)<(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y+currentFrame.h>=(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy))&& (abs(distanceFromWallX(p_ent)) < abs(distanceFromWallY(p_ent))))
		return 1;
	if(((pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y>(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy))&& (abs(distanceFromWallX(p_ent)) < abs(distanceFromWallY(p_ent))))
		return -1;
	return 0;
}

/*

int Entity::collidesX(Entity p_ent)
{
	if((((pos.x+currentFrame.w)<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x+currentFrame.w>(p_ent.getPos().x))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return 1;
	if((((pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w)) && (pos.x>=(p_ent.getPos().x)))) && ((pos.y+currentFrame.h-collisionAccuracy)>=(p_ent.getPos().y)) && (pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h-collisionAccuracy)))
		return -1;
	return 0;
}
int Entity::collidesY(Entity p_ent)
{
	if((((pos.y+currentFrame.h)<(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y+currentFrame.h>=(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return 1;
	if(((pos.y<=(p_ent.getPos().y+p_ent.getCurrentFrame().h)) && (pos.y>(p_ent.getPos().y))) && ((pos.x+currentFrame.w-collisionAccuracy)>=(p_ent.getPos().x)) && (pos.x<=(p_ent.getPos().x+p_ent.getCurrentFrame().w-collisionAccuracy)))
		return -1;
	return 0;
}
*/

Vector2f Entity::getVel(){
	return vel;
}
float Entity::getVelX(){
	return vel.x;
}
float Entity::getVelY(){
	return vel.y;
}
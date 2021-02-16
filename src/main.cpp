#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Mouse.hpp"
#include "PerlinNoise.h"



int main(int argc, char* args[])
{

	//float screenScale = 4.0f;

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL INTIALIZATION FAILED - SDL ERROR : " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "SDL_IMG INTIALIZATION FAILED - SDL_IMG ERROR : " << SDL_GetError() << std::endl;

	RenderWindow window("Mouseaware", 1280 ,720, screenScale);

	float windowRefreshRate = window.getRefreshRate();
 	std::cout << windowRefreshRate << std::endl;
	enum KeyPressSurfaces
	{
	    KEY_PRESS_SURFACE_DEFAULT,
	    KEY_PRESS_SURFACE_UP,
	    KEY_PRESS_SURFACE_DOWN,
	    KEY_PRESS_SURFACE_LEFT,
	    KEY_PRESS_SURFACE_RIGHT,
	    KEY_PRESS_SURFACE_TOTAL
	};


	SDL_Texture* grassTexture = window.loadTexture("res/gfx/ground_grass.png");
	SDL_Texture* dirtTexture = window.loadTexture("res/gfx/ground_dirt.png");
	SDL_Texture* playerTexture = window.loadTexture("res/gfx/crate.png");
    SDL_Texture* skyTexture = window.loadTexture("res/gfx/sky.png");
    SDL_Texture* stoneTexture = window.loadTexture("res/gfx/stone.png");
    unsigned int noiseSeed = 45;//24242420;//15755;//8928;
	PerlinNoise pn(noiseSeed);

	// Entity entities[6] = 
	// {
	// 	Entity(0,150, grassTexture,4.0f),
	// 	Entity(64,150, grassTexture,4.0f),
	// 	Entity(128,150, grassTexture,4.0f),
	// 	Entity(192,150, grassTexture,4.0f),
	// 	Entity(256,150, grassTexture,4.0f),
	// 	Entity(320,150, grassTexture,4.0f),
	// };

	std::vector<Entity> entities = 
	{

	};

	float startY = 32.0f;
	float duration = 10.0f;
	float mouseOffsetX;
	float mouseOffsetY;
	bool holdLeft = false;
	bool holdRight = false;
	const float tileSnap = 16.0f;
	for(int i = 0; i < 32; i++)
	{
		float yPos = round(170*pn.noise((double)200, (double)i/5.0, (double)7.5)/tileSnap)*tileSnap;
		//Entity grass_terrain(Vector2f(16 * i ,round(utils::sinWave(i,16.0,0.25f)/16.0)*16.0 + startY), grassTexture, 1.0f);
		Entity grass_terrain(Vector2f(16 * i ,yPos + startY), grassTexture, 1.0f);
		entities.push_back(grass_terrain);	
		for (int j = 1; j <= 3; j++)
		{
			Entity dirt_terrain(Vector2f(16 * i ,yPos + startY + grass_terrain.getCurrentFrame().h*j), dirtTexture, 1.0f);
			entities.push_back(dirt_terrain);	
		}
		for (int j = 1; j <= 10; j++)
		{
			Entity stone_terrain(Vector2f(16 * i ,yPos + startY + grass_terrain.getCurrentFrame().h*(j+3)), stoneTexture, 1.0f);
			entities.push_back(stone_terrain);	
		}
	}

	Entity player(Vector2f(0,0), playerTexture, 1.0f);
	player.addVel(Vector2f(0.0f,0.0f));
	bool gameRunning = true;

	SDL_Event event;
	Entity* downCollideWith;
	Entity* upCollideWith;
	Entity* leftCollideWith;
	Entity* rightCollideWith;
	bool mouseBeingHeldOnBox = false;
	const float timeStep = 0.0133333f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();
	Vector2f beforeMove, velBeforeMove;
	while(gameRunning)
	{

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
		beforeMove = player.getPos();
		velBeforeMove = player.getVel();

		while(accumulator>=timeStep)
		{
			SDL_PumpEvents();
			while (SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)	
				{
					gameRunning = false;
				}				
				if( event.type == SDL_KEYDOWN )
                    {
                    	if(event.key.state == SDL_PRESSED)
                    	{

			                if( event.key.keysym.scancode== SDL_SCANCODE_UP && !player.canGoPosY )player.setVelY((-4.0f));

			                if( event.key.keysym.scancode== SDL_SCANCODE_LEFT ) holdLeft = true;

			                if( event.key.keysym.scancode== SDL_SCANCODE_RIGHT ) holdRight = true;
                    	}
                    }

				if( event.type == SDL_KEYUP )
                    {
                    	if(event.key.state == SDL_RELEASED)
                    	{

			                if( event.key.keysym.scancode== SDL_SCANCODE_LEFT) holdLeft = false;

			                if( event.key.keysym.scancode== SDL_SCANCODE_RIGHT ) holdRight = false;
			            }
                    
                    }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                	if(event.button.button == (SDL_BUTTON_LEFT) && event.button.state == SDL_PRESSED)
                    {
                    	int x, y;
			        	SDL_GetMouseState( &x, &y);
			        	x = x/screenScale;
			        	y = y/screenScale;
			        	mouseOffsetX = x - player.getPos().x;
			        	mouseOffsetY = y - player.getPos().y;
			        	if((x>player.getPos().x ) && (x<player.getPos().x+player.getCurrentFrame().w) && (y>player.getPos().y) && (y<player.getPos().y+player.getCurrentFrame().h))
			        	{
			        		std::cout << "HELD ON BOX" << std::endl;
	                		mouseBeingHeldOnBox = true;
			        	}
	                }
                }
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                	
                	if(event.button.button == (SDL_BUTTON_LEFT) && event.button.state == SDL_RELEASED)
	                {
	                	mouseBeingHeldOnBox = false;
	                }
                }
                if(event.type == SDL_MOUSEMOTION)
                {
			        int x, y, xrel,yrel;
			        SDL_GetMouseState( &x, &y);
			        SDL_GetRelativeMouseState( &xrel, &yrel );
                	if(mouseBeingHeldOnBox)
                	{
                		player.setPosX(x/screenScale-mouseOffsetX);
                		player.setPosY(y/screenScale-mouseOffsetY);
                	//player.addVel(Vector2f((float)xrel/200.0f,(float)yrel/200.0f));
                		player.setVel(Vector2f((float)xrel/duration,(float)yrel/duration));
                	}
                	else
                	{

                	}

                }



			}
			        player.canGoNegX = true;
			        player.canGoNegY = true;
			        player.canGoPosX = true;
			        player.canGoPosY = true;

					for(Entity& ent : entities)
					{
						switch(player.collidesX(ent))
						{
							case -1: 
							player.canGoNegX=false;
							leftCollideWith = &ent;
							
							//player.setVelX(0.0f);
							break;
							case 1: 
							player.canGoPosX=false;
							rightCollideWith = &ent;
							//player.setPosX(beforeMove.x);
							//player.setVelX(0.0f);
							break;
						}
						switch(player.collidesY(ent))
						{
							case -1: 
							player.canGoNegY=false;
							upCollideWith = &ent;
							//.setPosY(beforeMove.y);
							//player.setVelY(0.0f);
							break;
							case 1: 
							player.canGoPosY=false;
							downCollideWith = &ent;
							//.setPosY(beforeMove.y);
							//player.setVelY(0.0f);
							break;
						}
					}

                   if(!mouseBeingHeldOnBox){
                   	player.beingHeld = false;
                   if(holdLeft){ player.addVelX((-0.40f)); std::cout << "LEFT IS HELD" << std::endl;}
                   if(holdRight) {player.addVelX((0.40f)); std::cout << "RIGHT IS HELD" << std::endl;}}
                   else{
                   	player.beingHeld = true;
                   }
                   if(holdLeft || holdRight)player.setVelX(MathUtils::clip(player.getVelX(),-2.0f,2.0f));	
					if(!player.canGoNegX && player.getVel().x<0) {player.setPosX(leftCollideWith->getPos().x+leftCollideWith->getCurrentFrame().w);player.setVelX(velBeforeMove.x);}
					if(!player.canGoPosX && player.getVel().x>0) {player.setPosX(rightCollideWith->getPos().x-player.getCurrentFrame().w);player.setVelX(velBeforeMove.x);}
					if(!player.canGoNegY && player.getVel().y<0) {player.setPosY(upCollideWith->getPos().y+upCollideWith->getCurrentFrame().h);player.setVelY(velBeforeMove.y);}
					if(!player.canGoPosY && player.getVel().y>0) {player.setPosY(downCollideWith->getPos().y-player.getCurrentFrame().h);player.setVelY(velBeforeMove.y);}
			accumulator -= timeStep;
			player.update();
		

		}

		const float alpha = accumulator / timeStep; 
		window.clear();

		// RENDERING 

        window.renderBackgroundTexture(skyTexture);

		for(Entity& ent : entities)
		{
			window.render(ent);

		}
		window.render(player);

		// std::cout << utils::hireTimeInSeconds() << std::endl;


		window.display();
	}

	window.cleanUp();

	SDL_Quit();
	return 0;
}
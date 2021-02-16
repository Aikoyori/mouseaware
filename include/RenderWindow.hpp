#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

class RenderWindow
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h, float p_screenScale = 1.0f);
	SDL_Texture* loadTexture(const char* p_filePath);

	void cleanUp();
	void clear();
	void render(Entity&	p_entity);
	void display();
	float getScreenScale();
	SDL_Renderer* getRenderer();
	void renderBackgroundTexture(SDL_Texture* p_tex);
	float getRefreshRate();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	float screenScale;
};
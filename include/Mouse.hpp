#pragma once

#include <SDL2/SDL.h>
#include "Mouse.hpp"
#include "Math.hpp"
#include "Utils.hpp"


class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( int x, int y );
		Vector2f getPosition();
		Vector2f getScaledPosition();

        //Handles mouse event
        void handleEvent( SDL_Event* e );
    
        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
};
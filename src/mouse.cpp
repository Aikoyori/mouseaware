
#include <SDL2/SDL.h>
#include "Mouse.hpp"
#include "Math.hpp"
#include "Utils.hpp"

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}
Vector2f LButton::getPosition()
{
    return Vector2f(mPosition.x,mPosition.y);
}
Vector2f LButton::getScaledPosition()
{
    return Vector2f(mPosition.x/screenScale,mPosition.y/screenScale);
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
              //Check if mouse is in button
    }
}
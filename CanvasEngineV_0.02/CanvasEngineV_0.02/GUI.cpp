#include "GUI.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>

Cint::Cint()
{

}

Cint::Cint(int value)
{
	v = value;
}

void Cint::animate(int value, int time)
{
	intAnimation newAnimation(&this->v, value, time);
	animations.push_back(newAnimation);
}

void Cint::delay(int time)
{
	intAnimation newAnimation(&this->v, 0, time);
	animations.push_back(newAnimation);
}

void Cint::play(int timepassed)
{

}

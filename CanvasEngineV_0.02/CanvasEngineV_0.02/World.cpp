#include "World.h"
#include <SDL.h>
#include <SDL_thread.h>
#include "GUI.h"

ex

World::World()
{
}

void World::calibrate()
{
}

void World::getEvents(SDL_Event e)
{
	eventBuffer.push_back(e);
}

void World::render()
{
	for(std::list<renderData>::iterator it = renderBuffer.begin, et = renderBuffer.end();it != et; it++)
	{
		(*it).render();
	}
}

bool World::simulate()
{
	return false;
}

bool World::initialize()
{
	return true;
}
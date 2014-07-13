#ifndef WORLD_H
#define WORLD_H
#include "Engine.h"
#include "physicsEngine.h"
#include <SDL.h>
#include <SDL_thread.h>

class World;
class Layer;
class Obj;

struct Key
{
	int keyCode;
	bool on;
};

class Layer{
	std::list<Layer*> sharedLayers;
	std::list<Obj*> contained;
};

class Obj
{
public:
	std::list<Obj*> children;
	World* containerWorld;
	renderData renderable;
	Obj();
	Obj(double x, double y, double w, double h);
	virtual void calibrateRenderable();
	virtual void setWorld(World* world);
	virtual void behave();
	virtual void render(std::list<renderData>* data, physicsBox camera);
};

class collidableObj:public Obj
{
public:
	physicsObject* obj;
	collidableObj();
	collidableObj(double x, double y, double w, double h);
	virtual void setWorld(World* world);
	virtual void calibratePhysics();
	virtual void move(double x, double y);
};

class World:public CanvasSystem
{
public:
	SDL_sem* renderSem;
	SDL_sem* eventSem;
	SDL_sem* positionSem;
	physicsBox camera;
	std::list<renderData> renderBuffer;
	std::list<SDL_Event> eventBuffer;
	physicsEngine* physics;
	World();
	virtual bool simulate();
	virtual bool initialize();
	void calibrate();
	void getEvents(SDL_Event e);
	void render();
};


#endif
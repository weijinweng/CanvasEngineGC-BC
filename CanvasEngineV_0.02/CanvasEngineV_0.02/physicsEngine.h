#ifndef PHYSICS_H
#define PHYSICS_H

class physicsEngine;
class dynamicPhysicsObject;
class boxCollider;
class staticPhysicsObject;

class point{
public:
	point(){}
	point(int x, int y){
		this->x = x;
		this->y = y;
	}
	double x,y;
	~point();
};

class Vector2D{
public:
	double x,y;
	Vector2D(){};
	Vector2D(double x, double y);
	static Vector2D dot(Vector2D x1, Vector2D x2);
	void scale(double scale);
};

class rayTrace{
public:
};

class physicsBox{
public:
	double x, y, h, w;
};

class physicsObject{
public:
	double mass;
	physicsObject(){}
	void setEngine(physicsEngine* world){}
	virtual void collide(physicsObject* obj){}
	virtual void collide(staticPhysicsObject* sta){}
	virtual void collide(dynamicPhysicsObject* dyn){}
	virtual void setWorld(physicsEngine* world);
	~physicsObject();
};

class dynamicPhysicsObject:public physicsObject{
};

class boxCollider:public dynamicPhysicsObject{
};

class staticPhysicsObject{
public:
};

class Terrain:public staticPhysicsObject{
public:
};	

class physicsEngine:public CanvasSystem
{
public:
	physicsEngine();
	~physicsEngine();
	virtual void simulate(int timepassed);
	virtual void render();
};

#endif
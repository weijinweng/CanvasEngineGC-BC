#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>

enum DIV_VARIABLES
{
	TOP,
	LEFT,
	WIDT,
	HEIGHT,
	OPACITY,
};
//Animations a int value based on destination value and time.
class intAnimation
{
public:
	intAnimation(int* edit, int coordinate, int time);
	int coordinate;
	int* v;
	int time;
	bool play(int timepassed);
};

//CanvasInt, used for int values that are animatable.
class Cint{
public:
	int v;
	Cint();
	Cint(int value);
	void play(int timpassed);
	void animate(int value, int time);
	void delay(int time);
	std::list<intAnimation> animations;
};



class div
{
	SDL_Rect renderRect;
public:
	div(int top, int left, int height, int width);
	~div();
	bool setImage(std::string img);
	void handleMouse(SDL_Event* e);
	void onMouseDown();
	void onMouseUp();
	void onHover();
	void offHover();
	void animate(int variable, int destination, int timepassed);
	void render();
	SDL_Texture* background;
	SDL_Color backgroundColor;
	SDL_Texture* textTexture;
	int fontHeight;
	Cint opacity;
	std::string text;
	std::string id;
	std::string Class;
	Cint top, left, width, height;
	std::list<div*> children;
};

#endif
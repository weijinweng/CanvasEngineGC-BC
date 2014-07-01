#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>



class CanvasTexture{
public:
	SDL_Texture* texture;

	void* pixel;

	int pitch;

	int width;
	int height;

	CanvasTexture();
	CanvasTexture(std::string img);
	bool lockTexture();
	bool unlockTexture();
	bool loadImage(std::string img);
	bool loadText(std::string text);
	bool colorKey(SDL_Color key, SDL_Color to);
};

enum DIV_VARIABLES
{
	TOP,
	LEFT,
	WIDTH,
	HEIGHT,
	OPACITY,
	RELATIVE,
	ABSOLUTE,
	FIXED,
	RELATIVE_LEFT
};

enum TEXT_TYPE
{
	CENTERED_TEXT,
	RIGHT_TEXT,
	LEFT_TEXT
};
//Animations a int value based on destination value and time.
class intAnimation
{
public:
	intAnimation(int* edit, int coordinate, int time);
	int coordinate;
	int origin;
	int* v;
	int time;
	int tpassed;
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
	bool operator= (int other);
	std::list<intAnimation> animations;
};



class gObj
{
	SDL_Rect renderRect;
public:
	gObj(int top, int left, int width, int height);
	~gObj();
	bool setImage(std::string img);
	void setColor(int r, int g, int b, int a);
	bool setText(std::string text);
	virtual void handleMouse(SDL_Event* e);
	virtual void childHandleMouse(int x, int y, SDL_Event* e);
	virtual void onMouseDown();
	virtual void onMouseUp();
	virtual void onHover();
	virtual void offHover();
	void addGObj( gObj* obj);
	void play(int timepassed);
	gObj* animate(int variable, int destination, int timepassed);
	void render(int x, int y);
	SDL_Texture* background;
	CanvasTexture backgroundTexture;
	SDL_Color backgroundColor;
	SDL_Texture* textTexture;
	CanvasTexture ctextTexture;
	SDL_Color textColor;
	int fontHeight;
	int textHeight;
	int textWidth;
	int textOrientation;
	int position;
	Cint opacity;
	std::string text;
	std::string id;
	std::string Class;
	Cint top, left, width, height;
	std::list<gObj*> children;
	bool mousedown;
	bool hover;
	bool show;
};


#endif
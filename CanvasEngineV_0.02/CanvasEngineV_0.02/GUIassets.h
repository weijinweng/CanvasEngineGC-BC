#ifndef GUIASSETS_H
#define GUIASSETS_H

#include "GUI.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>

class editorGUI;



class tab:public gObj
{
public:
	editorGUI* container;
	tab(int left, int top, int width, int height, editorGUI* gui);
	tab* showTab();
};

class tabButton:public gObj
{
public:
	tab* target;
	tabButton(int top, int left, int width, int height, tab* tabTarget, std::string text);
	virtual void onHover();
	virtual void offHover();
	virtual void onMouseDown();
	virtual void onMouseUp();
};

class boolToggler:public gObj
{
public:
	bool* target;
	boolToggler(int x, int y, int width, int height, bool* target, std::string text);
	virtual void onMouseUp();
	virtual void onMouseDown();
};

class quitButton:public boolToggler
{
public:
	quitButton();
	virtual void onHover();
	virtual void offHover();
	virtual void onMouseDown();
	virtual void onMouseUp();
};

class editorGUI:public gObj
{
public:
	editorGUI();
	void initialize();
	void hideAll(std::string cls);
};

#endif
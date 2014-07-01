#include "GUI.h"
#include "GUIassets.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_X;
extern int SCREEN_Y;
extern SDL_Window* mainWindow;
extern bool quit;

draggableWindow::draggableWindow(int left, int top, int width, int height):gObj(top, left, width, height)
{
	setColor(255,255,255,0);
	Class = "draggable";
	drag = false;
}

void draggableWindow::onMouseDown()
{
	drag = true;
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	xOffset = x;
	yOffset = y;
}

void draggableWindow::onHover()
{
	if(drag)
	{
		int velX;
		int velY;
		SDL_GetMouseState(&velX, &velY);
		int xChange = velX - xOffset;
		int yChange = velY - yOffset;
		SDL_SetWindowPosition(mainWindow, SCREEN_X + xChange, SCREEN_Y + yChange);
		SDL_GetWindowPosition(mainWindow, &SCREEN_X, &SCREEN_Y);
	}
}

void draggableWindow::offHover()
{
	if(drag)
	{
		int velX;
		int velY;
		SDL_GetRelativeMouseState(&velX, &velY);
		SDL_SetWindowPosition(mainWindow, SCREEN_X +velX, SCREEN_Y + velY);
		SDL_GetWindowPosition(mainWindow, &SCREEN_X, &SCREEN_Y);
	}
}

void draggableWindow::onMouseUp()
{
	drag = false;
}

tab::tab(int left, int top, int width, int height, editorGUI* gui):gObj(top, left, width, height)
{
	container = gui;
	Class = "tab";
	this->setColor(40, 40, 40, 255);
	position = ABSOLUTE;
}

tab* tab::showTab()
{
	container->hideAll(Class);
	this->show = true;
	return this;
}

tabButton::tabButton(int top, int left, int width, int height, tab* tabTarget, std::string text):gObj(top, left, width, height)
{
	target = tabTarget;
	fontHeight = 30;
	this->setColor(191, 191, 191, 255);
	setText(text);
	position = RELATIVE_LEFT;
	Class = "tabButton";

	
}

void tabButton::offHover()
{
	if(target->show)
	{
		this->setColor(242, 246, 172, 255);
	} else {
		this->setColor(191, 191, 191, 255);
	}
}

void tabButton::onHover()
{
	this->setColor(248, 252, 176, 255);
}
void tabButton::onMouseDown()
{
	this->setColor(242, 246, 172, 255);
	target->showTab();
}

void tabButton::onMouseUp()
{
	setColor(248, 252, 176, 255);
}

boolToggler::boolToggler(int top, int left, int width, int height, bool* target, std::string text):gObj(top, left, width, height)
{
	this->target = target;
	setText(text);
}

void boolToggler::onMouseUp()
{
	if(mousedown)
	{
		*target = !*target;

	}

}

void boolToggler::onMouseDown()
{

}

quitButton::quitButton():boolToggler(0, SCREEN_WIDTH-50, 50, 30, &quit, "X")
{
	setColor(14, 65, 194, 255);
	textColor.a = 255;
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	fontHeight = 25;
}

void quitButton::offHover()
{

		this->setColor(14, 65, 194, 255);

}

void quitButton::onHover()
{
	this->setColor(18, 80, 237, 255);
}
void quitButton::onMouseDown()
{
	this->setColor(14, 65, 194, 255);
}

void quitButton::onMouseUp()
{
	setColor(18, 80, 237, 255);
	if(mousedown)
	{
		*target = !*target;
	}
}

editorGUI::editorGUI():gObj(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
{
	draggableWindow* drag = new draggableWindow(0,0,SCREEN_WIDTH,40);
	addGObj(drag);
	gObj* logo = new gObj(0, 0, 300, 40);
	logo->setImage("img/logo/logo.png");
	addGObj(logo);
	quitButton* qButton = new quitButton();
	addGObj(qButton);
	tab* firstTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addGObj(firstTab);
	tabButton* firstTabButton = new tabButton(180, 0, 100, 30, firstTab, "First");
	addGObj(firstTabButton);
	tab* secondTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addGObj(secondTab);
	tabButton* secondTabButton = new tabButton(180, 0, 100, 30, secondTab, "Second");
	addGObj(secondTabButton);
	firstTab->showTab();
	gObj* partitioner = new gObj(178, 0, SCREEN_WIDTH, 2);
	partitioner->setColor(242, 246, 172, 255);
	addGObj(partitioner);
	gObj* partitioner2 = new gObj(39, 0, SCREEN_WIDTH, 2);
	partitioner2->setColor(242, 246, 172, 255);
	addGObj(partitioner2);

}

void editorGUI::initialize()
{

}

void editorGUI::hideAll(std::string cls)
{

	for(std::list<gObj*>::iterator it = children.begin(); it != children.end(); it++)
	{

		if((*it)->Class == cls)
		{
			(*it)->show = false;
		}
	}

}
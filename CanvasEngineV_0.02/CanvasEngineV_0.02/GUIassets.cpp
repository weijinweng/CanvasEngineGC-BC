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
extern bool quit;

tab::tab(int left, int top, int width, int height, editorGUI* gui):gObj(top, left, width, height)
{
	container = gui;
	Class = "tab";
	this->setColor(14, 65, 194, 255);
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
	position = RELATIVE_LEFT;
	Class = "tabButton";
	setText(text);
}

void tabButton::offHover()
{
	if(target->show)
	{
		this->setColor(14, 65, 194, 255);
	} else {
		this->setColor(191, 191, 191, 255);
	}
}

void tabButton::onHover()
{
	this->setColor(18, 80, 237, 255);
}
void tabButton::onMouseDown()
{
	this->setColor(14, 65, 194, 255);
	target->showTab();
}

void tabButton::onMouseUp()
{
	backgroundColor.r = 18;
	backgroundColor.b = 237;
	backgroundColor.g = 80;
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
	gObj* logo = new gObj(0, 0, 300, 40);
	logo->setImage("img/logo/logo.png");
	addGObj(logo);
	quitButton* qButton = new quitButton();
	addGObj(qButton);
	tab* firstTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addGObj(firstTab);
	tabButton* firstTabButton = new tabButton(180, 200, 100, 30, firstTab, "First");
	addGObj(firstTabButton);
	tab* secondTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addGObj(secondTab);
	tabButton* secondTabButton = new tabButton(180, 200, 100, 30, secondTab, "Second");
	addGObj(secondTabButton);
	firstTab->showTab();
}

void editorGUI::initialize()
{

}

void editorGUI::hideAll(std::string cls)
{

	for(std::list<gObj*>::iterator it = children.begin(); it != children.end(); it++)
	{
		printf("happens\n");
		if((*it)->Class == cls)
		{
			(*it)->show = false;
		}
	}

}
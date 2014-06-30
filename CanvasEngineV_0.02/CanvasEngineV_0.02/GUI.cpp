#include "GUI.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

extern SDL_Renderer* mainRenderer;
extern TTF_Font* CanvasFont;

intAnimation::intAnimation(int* value, int destination, int time)
{
	v = value;
	this->coordinate = destination;
	this->time = time;
	tpassed = 0;
	origin = *value;
}

bool intAnimation::play(int timepassed)
{
	if(!time)
		return false;
	tpassed += timepassed;
	double timeFraction =((double)tpassed/(double)time);
	*v = origin +(timeFraction*(coordinate - origin));
	if(tpassed >= time)
		time = 0;
	return true;
}

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
	if(animations.size()!= 0)
	{
		if(!animations.front().play(timepassed))
			animations.pop_front();
	}
}

bool Cint::operator=(int other)
{
	this->v = other;
	return true;
}

gObj::gObj(int top, int left, int width, int height)
{
	this->top = top;
	this->left = left;
	this->width = width;
	this->height = height;
	this->setColor(255,255,255,0);
	background = NULL;
	textTexture = NULL;
	text = "";
	id = "";
	Class = "joe";
	opacity.v = 0;
	fontHeight = 40;
	textColor.r = 255;
	textColor.b = 255;
	textColor.g = 255;
	textColor.a = 255;
	textOrientation = CENTERED_TEXT;
	show = true;
}

void gObj::handleMouse(SDL_Event* e)
{
	if(show)
	{
		if((*e).type == SDL_MOUSEBUTTONDOWN||(*e).type == SDL_MOUSEBUTTONUP || (*e).type == SDL_MOUSEMOTION)
		{

			int x, y;
			SDL_GetMouseState(&x, &y);
			if((*e).type == SDL_MOUSEBUTTONDOWN)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{

					if(x > (left.v+width.v)
						|| x < left.v
						|| y > (top.v + height.v)
						|| y < top.v)
					{

						offHover();
						hover = false;
					} else
					{

						onMouseDown();
						mousedown = true;
					}
				}

			}
			if((*e).type == SDL_MOUSEBUTTONUP)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v)
						|| x < left.v
						|| y > (top.v + height.v)
						|| y < top.v)
					{
						offHover();
						hover = false;
					} else 
					{
						onMouseUp();
						mousedown = false;
					}
				}
			}
			if((*e).type == SDL_MOUSEMOTION)
			{
				if(x > (left.v+width.v)
					|| x < left.v
					|| y > (top.v + height.v)
					|| y < top.v)
				{
					offHover();
					hover = true;
				}else 
				{
					onHover();
					hover = false;
				}
			}
		}
		int relativeY = 0;
		int relativeX = 0;
		for(std::list<gObj*>::iterator it = children.begin(); it!=children.end(); it++)
		{
			if((*it)->position == RELATIVE)
			{
				(*it)->childHandleMouse(left.v , relativeY + top.v, e);
				relativeY += (*it)->height.v;
			}
			else if((*it)->position == RELATIVE_LEFT)
			{
				(*it)->childHandleMouse(relativeX + left.v, top.v, e);
				relativeX += (*it)->width.v;
			} else (*it)->childHandleMouse(left.v, top.v, e);
		}
	}

}

void gObj::childHandleMouse(int ix, int iy, SDL_Event* e)
{
	if(show)
	{
		if((*e).type == SDL_MOUSEBUTTONDOWN||(*e).type == SDL_MOUSEBUTTONUP || (*e).type == SDL_MOUSEMOTION)
		{

			int x, y;
			SDL_GetMouseState(&x, &y);
			if((*e).type == SDL_MOUSEBUTTONDOWN)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v + ix)
						|| x < left.v + ix
						|| y > (top.v + height.v + iy)
						|| y < top.v + iy)
					{

						offHover();
						hover = false;
					} else
					{
						onMouseDown();
						mousedown = true;
					}
				}

			}
			if((*e).type == SDL_MOUSEBUTTONUP)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v + ix)
						|| x < left.v + ix
						|| y > (top.v + height.v + iy)
						|| y < top.v + iy)
					{
						offHover();
						hover = false;
					} else 
					{
						onMouseUp();
						mousedown = false;
					}
				}
			}
			if((*e).type == SDL_MOUSEMOTION)
			{
				if(x > (left.v+width.v + ix)
					|| x < left.v + ix
					|| y > (top.v + height.v + iy)
					|| y < top.v + iy)
				{
					offHover();
					hover = false;
				}else 
				{
					onHover();
					hover = true;
				}
			}
		}
		int relativeY = 0;
		int relativeX = 0;
		for(std::list<gObj*>::iterator it = children.begin(); it!=children.end(); it++)
		{
			if((*it)->position == RELATIVE)
			{
				(*it)->childHandleMouse(left.v + ix, relativeY + top.v + iy, e);
				relativeY += (*it)->height.v;
			}
			else if((*it)->position == RELATIVE_LEFT)
			{
				(*it)->childHandleMouse(relativeX + left.v + ix, top.v + iy, e);
				relativeX += (*it)->width.v;
			} else (*it)->childHandleMouse(left.v + ix, top.v + iy, e);
		}
	}
}

void gObj::play(int timepassed)
{
	top.play(timepassed);
	left.play(timepassed);
	width.play(timepassed);
	height.play(timepassed);
	opacity.play(timepassed);
	for(std::list<gObj*>::iterator it = children.begin(); it != children.end(); it++)
	{

		(*it)->play(timepassed);
	}
}

bool gObj::setText(std::string text)
{
	SDL_Surface* textSurface = TTF_RenderText_Blended(CanvasFont, text.c_str(), textColor);
	if(textSurface == NULL)
	{
		printf("TT_RenderText_Blended error %s\n", TTF_GetError());
		return false;
	} else {
		if(textTexture != NULL)
		{
			SDL_DestroyTexture(textTexture);
			textTexture = NULL;
		}
		textTexture = SDL_CreateTextureFromSurface(mainRenderer, textSurface);
		if(textTexture == NULL)
		{
			printf("SDL_CreateTextureFromSurface error %s\n", SDL_GetError());
			return false;
		} else {
			this->text = text;
			textHeight = textSurface->h;
			textWidth = textSurface->w;
		}
		SDL_FreeSurface(textSurface);
	}
	return true;
}

bool gObj::setImage(std::string img)
{
	SDL_Surface* imgSurface = IMG_Load(img.c_str());
	if(imgSurface == NULL)
	{
		printf("IMG_Load failed %s\n", IMG_GetError());
		return false;
	} else {
		if(background != NULL)
		{
			SDL_DestroyTexture(background);
			background = NULL;
		}
		background = SDL_CreateTextureFromSurface(mainRenderer, imgSurface);
		if(background == NULL)
		{
			printf("SDL_CreateTexture failed %s\n", SDL_GetError());
			return false;
		}
		SDL_FreeSurface(imgSurface);
	}
	return true;
}

void gObj::setColor(int r, int g, int b, int opacity)
{
	backgroundColor.r = r;
	backgroundColor.g = g;
	backgroundColor.b = b;
	this->opacity.v = opacity;
	backgroundColor.a = opacity;
}

void gObj::onMouseDown()
{
}

void gObj::onMouseUp()
{
}

void gObj::onHover()
{
}

void gObj::offHover()
{
}

void gObj::addGObj(gObj* obj)
{
	children.push_back(obj);
}

gObj* gObj::animate(int variable, int destination, int time)
{
	switch(variable)
	{
	case TOP:
		{
			top.animate(destination, time);
		}
		break;
	case LEFT:
		{
			left.animate(destination, time);
		}
		break;
	case WIDTH:
		{
			width.animate(destination, time);
		}
		break;
	case HEIGHT:
		{
			height.animate(destination, time);
		}
		break;
	case OPACITY:
		{
			opacity.animate(destination, time);
		}
		break;
	}
	return this;
}

void gObj::render(int x, int y)
{
	if(show)
	{
		renderRect.x = left.v + x;
		renderRect.y = top.v + y;
		renderRect.w = width.v;
		renderRect.h = height.v;
		if(background == NULL)
		{
			SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(mainRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			SDL_RenderFillRect(mainRenderer, &renderRect);
		} else {
			SDL_RenderCopy(mainRenderer, background, NULL, &renderRect);
		}
		if(textTexture != NULL)
		{
			renderRect.h = fontHeight;
			renderRect.w = ((double)textWidth)*((double)fontHeight/(double)textHeight);
			if(textOrientation == RIGHT_TEXT)
			{
				renderRect.x = height.v - renderRect.w + left.v;
			} else if (textOrientation == CENTERED_TEXT)
			{
				renderRect.x = left.v + ((double)(width.v - renderRect.w))/2;
			} else {
				renderRect.x = left.v;
			}
			renderRect.x += x;
			SDL_RenderCopy(mainRenderer, textTexture, NULL, &renderRect);
		}
		int relativeY = 0;
		int relativeX = 0;
		for(std::list<gObj*>::iterator it = children.begin(); it!=children.end(); it++)
		{

			if((*it)->position == RELATIVE)
			{
				(*it)->render(left.v + x, relativeY + top.v+ y);
				relativeY += (*it)->height.v;
			}
			else if((*it)->position == RELATIVE_LEFT)
			{
				(*it)->render(relativeX + x + left.v, top.v + y);
				relativeX += (*it)->width.v;
			} else (*it)->render(left.v + x, top.v + y);
		}
	}
}

gObj::~gObj()
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(textTexture);
}
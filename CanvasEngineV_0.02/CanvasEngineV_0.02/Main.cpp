#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GUI.h"
#include "GUIassets.h"
#include <string>

SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;
TTF_Font* CanvasFont = NULL;

int SCREEN_WIDTH = 1680;
int SCREEN_HEIGHT = 1020;

int lastFrameTime = 0;
int deltaTime = 0;

bool quit = false;
bool CanvasDebug = true;

static int GetDeltaTime()
{
	int DeltaTime = SDL_GetTicks() - lastFrameTime;
	if (DeltaTime > 33)
		DeltaTime = 33;
	deltaTime = DeltaTime;
	return deltaTime;
}

static void setFrame()
{
	GetDeltaTime();
	lastFrameTime = SDL_GetTicks();
}

bool Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL init error %s\n", SDL_GetError());
		return false;
	}
	else
	{
		mainWindow = SDL_CreateWindow("Canvas Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(mainWindow == NULL)
		{
			printf("SDL create window error %s\n", SDL_GetError());
			return false;
		}
		else{
			SDL_GetWindowSize(mainWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
			mainRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED);
			if(mainRenderer == NULL)
			{
				printf("SDL_CreateRenderer error %s\n", SDL_GetError());
				return false;
			}
				else{
					SDL_SetRenderDrawColor( mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					int imgflags = IMG_INIT_PNG;
					if(!(IMG_Init(imgflags) & imgflags))
					{
						printf("IMG_Init faild %s\n", IMG_GetError());
						return false;
					}

					if(TTF_Init() == -1)
					{
						printf("SDL_ttf initialization error %s\n", TTF_GetError());
						return false;
					}
					else
					{
						CanvasFont = TTF_OpenFont("font/Tahoma.ttf", 72);
						if(CanvasFont == NULL)
						{
							printf("TTF_OpenFont failed %s\n", TTF_GetError());
							return false;
						}
						else
						{
							printf("init success\n");
						}
					}
				}
			}
	}
	return true;
}

void close()
{
	SDL_DestroyRenderer(mainRenderer);
	mainRenderer == NULL;
	SDL_DestroyWindow(mainWindow);
	mainWindow == NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{

	if(Init())
	{
		gObj* firstDiv = new gObj(100, 100, 400, 400);
		editorGUI* editor = new editorGUI();
		firstDiv->setColor(0, 0, 0, 255);
		printf("%d\n",deltaTime);
		firstDiv->animate(TOP, 200, 1000);
		editor->initialize();
		while(!quit)
		{

			SDL_Event e;
			while(SDL_PollEvent(&e)>0)
			{
				if(e.type == SDL_QUIT)
				{
					quit = true;
				}
				editor->handleMouse(&e);
			}

			SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
			SDL_RenderClear(mainRenderer);
			setFrame();
			std::string text = std::to_string( 1000.0/(deltaTime>0?deltaTime:1));
			firstDiv->setText(text);
			firstDiv->play(deltaTime);
			firstDiv->render(0,0);
			editor->play(deltaTime);
			editor->render(0,0);
			SDL_RenderPresent(mainRenderer);

		}
	}
	close();
	return 1;
}
#include "ModuleWindow.h"

#include <SDL.h>
#include "Utils.h"
#include <stdio.h>

ModuleWindow::ModuleWindow() : Module()
{}

ModuleWindow::~ModuleWindow()
{}

bool ModuleWindow::Init()
{
	printf("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		m_Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (m_Window == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			m_ScreenSurface = SDL_GetWindowSurface(m_Window);
		}
	}

	return ret;
}

bool ModuleWindow::CleanUp()
{
	printf("Destroying SDL window and quitting all SDL systems");

	if (m_Window != nullptr)
	{
		SDL_DestroyWindow(m_Window);
	}

	SDL_Quit();
	return true;
}

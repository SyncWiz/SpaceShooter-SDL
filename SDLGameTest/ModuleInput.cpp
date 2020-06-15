#include "ModuleInput.h"

#include "Engine.h"
#include <SDL.h>

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module()
{
	m_Keyboard = new KeyState[MAX_KEYS];
	memset(m_Keyboard, (int)KeyState::KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(m_Keyboard);
}

bool ModuleInput::Init()
{
	printf("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		printf("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleInput::Start()
{
	return true;
}

UpdateStatus ModuleInput::PreUpdate()
{
	static SDL_Event event;

	memset(m_WindowEvents, false, (int)EventWindow::WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (m_Keyboard[i] == KeyState::KEY_IDLE)
				m_Keyboard[i] = KeyState::KEY_DOWN;
			else
				m_Keyboard[i] = KeyState::KEY_REPEAT;
		}
		else
		{
			if (m_Keyboard[i] == KeyState::KEY_REPEAT || m_Keyboard[i] == KeyState::KEY_DOWN)
				m_Keyboard[i] = KeyState::KEY_UP;
			else
				m_Keyboard[i] = KeyState::KEY_IDLE;
		}
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_WindowEvents[EventWindow::WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_WindowEvents[EventWindow::WE_HIDE] = true;
				break;

			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				m_WindowEvents[EventWindow::WE_SHOW] = true;
				break;
			}
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
		return UpdateStatus::UPDATE_STOP;

	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return m_WindowEvents[ev];
}
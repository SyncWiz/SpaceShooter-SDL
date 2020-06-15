#pragma once

#include "Module.h"

#include <list>

class Scene : public Module
{
public:

	Scene(const char* background);
	virtual ~Scene();

	/*bool Init() override;
	UpdateStatus Update() override;
	bool CleanUp() override;

	void Instantiate(Entity* entityToAdd);

private:
	void DrawBackground();

private:
	std::list<Entity*> m_Entities;;*/
	//SDL_Texture* m_Background = nullptr;
};
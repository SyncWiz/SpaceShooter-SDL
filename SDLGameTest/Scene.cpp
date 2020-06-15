#include "Scene.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include <SDL_image.h>


Scene::Scene(const char* background) : Module()
{
	//m_Background = engine->m_Textures->LoadOrGet(background);
}

Scene::~Scene()
{}

/*
bool Scene::Init()
{
	for (Entity* entity : m_Entities)
	{
		//entity->Start();
	}
}

UpdateStatus Scene::Update()
{
	DrawBackground();

	bool toDelete = false;
	for (Entity* entity : m_Entities)
	{
		toDelete = false;
		//toDelete = entity->Update();
		if (toDelete)
		{
			//entity->Stop();
			RELEASE(entity);
			m_Entities.remove(entity);
		}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
	for (Entity* entity : m_Entities)
	{
		RELEASE(entity);
	}
	m_Entities.clear();
}

void Scene::Instantiate(Entity* entityToAdd)
{
	entityToAdd->Start();
	m_Entities.push_back(entityToAdd);
}

void Scene::DrawBackground()
{
	if (m_Background)
	{
		engine->m_Renderer->Blit(m_Background, 0, 0, NULL);
	}
}*/


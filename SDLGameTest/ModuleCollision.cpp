#include "ModuleCollision.h"

#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include <SDL_scancode.h>

using namespace std;

ModuleCollision::ModuleCollision()
{
	m_CollisionMatrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	
	m_CollisionMatrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
}

ModuleCollision::~ModuleCollision()
{}

UpdateStatus ModuleCollision::PreUpdate()
{
	for (list<Collider*>::iterator it = m_Colliders.begin(); it != m_Colliders.end();)
	{
		if ((*it)->m_Delete == true)
		{
			RELEASE(*it);
			it = m_Colliders.erase(it);
		}
		else
			++it;
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleCollision::Update()
{
	list<Collider*>::iterator iter;
	for (list<Collider*>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
	{
		Collider* previousCollider = *it;
		iter = it;
		for (++iter; iter != m_Colliders.end(); ++iter)
		{
			Collider* currentCollider = *iter;
			if (previousCollider->CheckCollision(currentCollider->m_Rect) == true)
			{
				if (m_CollisionMatrix[previousCollider->m_Type][currentCollider->m_Type] && previousCollider->m_CallbackEntity)
					previousCollider->m_CallbackEntity->OnCollision(previousCollider, currentCollider);

				if (m_CollisionMatrix[currentCollider->m_Type][previousCollider->m_Type] && currentCollider->m_CallbackEntity)
					currentCollider->m_CallbackEntity->OnCollision(currentCollider, previousCollider);
			}
		}
	}

#ifdef _DEBUG
	if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
		m_ShowDebug = !m_ShowDebug;

	if (m_ShowDebug == true)
		DebugDraw();
#endif

	return UpdateStatus::UPDATE_CONTINUE;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, ColliderType type, Module* callback)
{
	Collider* ret = new Collider(rect, type, callback);
	m_Colliders.push_back(ret);
	return ret;
}

#ifdef _DEBUG
void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
	{
		switch ((*it)->m_Type)
		{
		case ColliderType::COLLIDER_PLAYER:
			Engine::Instance()->m_Renderer->DrawQuad((*it)->m_Rect, 255, 255, 255, 80);
			break;

		case ColliderType::COLLIDER_ENEMY:
			Engine::Instance()->m_Renderer->DrawQuad((*it)->m_Rect, 255, 0, 0, 80);
			break;
		}
	}
}
#endif

bool ModuleCollision::CleanUp()
{
	printf("Freeing all colliders");

	for (list<Collider*>::iterator it = m_Colliders.begin(); it != m_Colliders.end(); ++it)
		RELEASE(*it);

	m_Colliders.clear();

	return true;
}

// Collider methods
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (m_Rect.x < r.x + r.w &&
		m_Rect.x + m_Rect.w > r.x &&
		m_Rect.y < r.y + r.h &&
		m_Rect.h + m_Rect.y > r.y);
}
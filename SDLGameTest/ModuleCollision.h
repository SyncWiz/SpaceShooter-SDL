#pragma once

#include "Module.h"
#include "SDL_rect.h"

#include<list>

enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,

	COLLIDER_MAX
};


struct Collider
{
	SDL_Rect m_Rect = { 0,0,0,0 };
	bool m_Delete = false;
	ColliderType m_Type;
	
	//TODO: DCERVERA ADD ENTITIES HERE INSTEAD OF MODULES
	Module* m_CallbackEntity;

	Collider(SDL_Rect rectangle, ColliderType type, Module* callback = NULL) :
		m_Rect(rectangle), m_Type(type), m_CallbackEntity(callback)
	{}

	void SetPos(int x, int y)
	{
		m_Rect.x = x;
		m_Rect.y = y;
	}
	void SetSize(int x, int y)
	{
		m_Rect.w = x;
		m_Rect.h = y;
	}

	void SetType(ColliderType type)
	{
		m_Type = type;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;

	bool CleanUp() override;

	Collider* AddCollider(const SDL_Rect& rect, ColliderType type, Module* callback);
#ifdef _DEBUG
	void DebugDraw();
#endif
	std::list<Collider*> m_Colliders;

private:
#ifdef _DEBUG
	bool m_ShowDebug = false;
#endif
	bool m_CollisionMatrix[COLLIDER_MAX][COLLIDER_MAX];
};
#pragma once

#include "Utils.h"
#include "Point.h"
#include <stdlib.h>
#include "Animation.h"

struct Collider;
struct SDL_Texture;

class Scene;

class Entity
{
public:

    Entity(const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : m_TexturePath(texturePath)
        , m_TextureScale(textureScale)
        , m_Position(initialPosition)
        , m_Scene(scene)
        , m_Active(active)
    {}

    virtual ~Entity()
    {}

    virtual void Init();
    virtual void Update();
    virtual void CleanUp();

    void SetScale(float x, float y)
    {
        m_TextureScale.x = x;
        m_TextureScale.y = y;
    }

    void SetPosition(int x, int y);


    void SetCurrentAnimation(Animation* animation)
    {
        m_CurrentAnimation = animation;
    }

    void SetColliderOffset(const iPoint& offset)
    {
        m_ColliderOffset = offset;
    }

    void SetCurrentTextureID(size_t textureID)
    {
        m_EntityTextureID = textureID;
    }

    void ToDelete()
    {
        m_ToDelete = true;
        m_Active = false;
    }

    bool IsActive()
    {
        return m_Active;
    }

    virtual void OnCollision(Collider* col1, Collider* col2)
    {}

    bool m_ToDelete = false;

protected:
    Animation* m_CurrentAnimation = nullptr;
    iPoint m_Position{ 0, 0 };
    iPoint m_ColliderOffset{ 0 , 0 };
    fPoint m_TextureScale{ 0.0f, 0.0f };
    const char* m_TexturePath;
    size_t m_EntityTextureID = 0;
    Scene* m_Scene = nullptr;
    bool m_Active = true;
    Collider* m_Collider = nullptr;
};

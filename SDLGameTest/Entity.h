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

    virtual bool Init();
    virtual bool Update();
    virtual bool CleanUp()
    {
        //Textures are shared, we release them when changing the scene
        RELEASE(m_TexturePath);
        RELEASE(m_CurrentAnimation);
        return true;
    }

    void SetScale(float x, float y)
    {
        m_TextureScale.x = x;
        m_TextureScale.y = y;
    }

    void SetPosition(int x, int y)
    {
        m_Position.x = x;
        m_Position.y = y;
    }

    void SetCurrentAnimation(Animation* animation)
    {
        m_CurrentAnimation = animation;
    }

    virtual void OnCollision(Collider* col1, Collider* col2)
    {}

protected:
    Animation* m_CurrentAnimation = nullptr;
    iPoint m_Position;
    fPoint m_TextureScale;
    const char* m_TexturePath;
    SDL_Texture* m_EntityTexture;
    Scene* m_Scene;
    bool m_Active = false;
};

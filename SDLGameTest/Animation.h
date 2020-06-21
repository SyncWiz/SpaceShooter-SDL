#pragma once

#pragma once
#include <vector>

#include "MathUtils.h"
#include <SDL_rect.h>

using namespace std;

class Animation
{
public:
    bool m_Loop = true;
    float m_Speed = 1.0f;
    vector<SDL_Rect> m_Frames;
    int m_PivotX = 0;
    int m_PivotY = 0;

private:
    float m_CurrentFrame = 0.0f;
    int m_Loops = 0;

public:
    Animation()
    {}

    Animation(const Animation& animation) : m_Loop(animation.m_Loop), m_Speed(animation.m_Speed), m_Frames(animation.m_Frames)
    {}

    SDL_Rect& GetCurrentFrame()
    {
        float lastFrame = (float)m_Frames.size();

        m_CurrentFrame += m_Speed;
        if (m_CurrentFrame >= lastFrame)
        {
            m_CurrentFrame = (m_Loop) ? 0.0f : MAX(lastFrame - 1.0f, 0.0f);
            m_Loops++;
        }

        return m_Frames[(int)m_CurrentFrame];
    }

    bool Finished() const
    {
        return m_Loops > 0;
    }

    void Reset()
    {
        m_CurrentFrame = 0.0f;
        m_Loops = 0;
    }
};

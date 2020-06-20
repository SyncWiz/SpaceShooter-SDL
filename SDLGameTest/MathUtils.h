#pragma once

#include "Config.h"
#include "Engine.h"
#include "ModuleRender.h"
#include <random>
#include <math.h> 

namespace MathUtils
{
    #define PI 3.14159265

    #define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
    #define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

    static bool IsPointInsideCameraView(const iPoint& point)
    {
        iPoint cameraPosition{ Engine::Instance()->m_Renderer->m_Camera.x, Engine::Instance()->m_Renderer->m_Camera.y };
        int positionCameraViewY = (-point.y) + SCREEN_HEIGHT;
        if (point.x < 0 || point.x > SCREEN_WIDTH || positionCameraViewY > cameraPosition.y + SCREEN_HEIGHT || positionCameraViewY < cameraPosition.y)
        {
            return false;
        }

        return true;
    }

    template<class TYPE>
    static int GetRandomInRange(TYPE from, TYPE to)
    {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<TYPE> distr(from, to);

        return distr(generator);
    }

    static int GetRandomPositionInCameraRangeX()
    {
        return GetRandomInRange(0, SCREEN_WIDTH );
    }
}


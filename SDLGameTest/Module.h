#pragma once

#include "Utils.h"

class Module
{
public:

    Module(bool active = true) : m_Active(active)
    {}

    virtual ~Module()
    {}

    bool IsEnabled() const
    {
        return m_Active;
    }

    bool Enable()
    {
        if (m_Active == false)
            return m_Active = Start();

        return true;
    }

    bool Disable()
    {
        if (m_Active == true)
            return m_Active = !CleanUp();

        return true;
    }

    virtual bool Init()
    {
        return true;
    }

    virtual bool Start()
    {
        return true;
    }

    virtual UpdateStatus PreUpdate()
    {
        return UpdateStatus::UPDATE_CONTINUE;
    }

    virtual UpdateStatus Update()
    {
        return UpdateStatus::UPDATE_CONTINUE;
    }

    virtual UpdateStatus PostUpdate()
    {
        return UpdateStatus::UPDATE_CONTINUE;
    }

    virtual bool CleanUp()
    {
        return true;
    }

private:
    bool m_Active = true;
};
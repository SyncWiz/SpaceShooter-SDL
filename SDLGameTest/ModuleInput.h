#pragma once

#include "Module.h"

enum EventWindow
{
    WE_QUIT = 0,
    WE_HIDE = 1,
    WE_SHOW = 2,
    WE_COUNT
};

enum class KeyState
{
    KEY_IDLE = 0,
    KEY_DOWN,
    KEY_REPEAT,
    KEY_UP
};

class ModuleInput : public Module
{
public:

    ModuleInput();
    virtual ~ModuleInput();

    bool Init() override;
    bool Start() override;

    UpdateStatus PreUpdate() override;

    bool CleanUp();

    // Check key states (includes mouse and joy buttons)
    KeyState GetKey(int id) const
    {
        return m_Keyboard[id];
    }

    bool GetWindowEvent(EventWindow code) const;

private:
    bool		m_WindowEvents[WE_COUNT];
    KeyState* m_Keyboard;
};
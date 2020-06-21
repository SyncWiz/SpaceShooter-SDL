#include <stdlib.h>
#include "Engine.h"
#include "Utils.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

enum class MainStates
{
    MAIN_CREATION,
    MAIN_START,
    MAIN_UPDATE,
    MAIN_FINISH,
    MAIN_EXIT
};

Engine* engine = nullptr;

int main(int argc, char** argv)
{
    int mainReturn = EXIT_FAILURE;
    MainStates state = MainStates::MAIN_CREATION;

    while (state != MainStates::MAIN_EXIT)
    {
        switch (state)
        {
            case MainStates::MAIN_CREATION:
            {
                LOG("Application Creation --------------");
                engine = new Engine();
                state = MainStates::MAIN_START;
            }
            break;

            case MainStates::MAIN_START:
            {
                LOG("Application Init --------------");
                if (engine->Init() == false)
                {
                    LOG("Application Init exits with error -----");
                    state = MainStates::MAIN_EXIT;
                }
                else
                {
                    state = MainStates::MAIN_UPDATE;
                    LOG("Application Update --------------");
                }
            }
            break;

            case MainStates::MAIN_UPDATE:
            {
                UpdateStatus update_return = engine->Update();

                if (update_return == UpdateStatus::UPDATE_ERROR)
                {
                    LOG("Application Update exits with error -----");
                    state = MainStates::MAIN_EXIT;
                }

                if (update_return == UpdateStatus::UPDATE_STOP)
                    state = MainStates::MAIN_FINISH;
            }
            break;

            case MainStates::MAIN_FINISH:
            {
                LOG("Application CleanUp --------------");
                if (engine->CleanUp() == false)
                {
                    LOG("Application CleanUp exits with error -----");
                }
                else
                {
                    mainReturn = EXIT_SUCCESS;
                }
                state = MainStates::MAIN_EXIT;
            }
            break;
        }
    }

    RELEASE(engine);
    return mainReturn;
}
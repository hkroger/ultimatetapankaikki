#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <SDL_gamecontroller.h>
#include <vector>

namespace tk_port
{
class Controller
{
public:
    Controller(const unsigned int index);

    ~Controller();

    bool isConnected();

    bool isButtonPressed(const SDL_GameControllerButton controller_button);

    bool isButtonReleased(const SDL_GameControllerButton controller_button);

private:
    SDL_GameController *m_sdl_controller;
    std::vector<SDL_GameControllerButton> m_pressed;
};
}

#endif

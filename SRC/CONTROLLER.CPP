#include "CONTROLLER.H"

#include <algorithm>

namespace tk_port
{
Controller::Controller(const unsigned int index) :
    m_sdl_controller(SDL_GameControllerOpen(index))
{
}

Controller::~Controller()
{
    if (m_sdl_controller) SDL_GameControllerClose(m_sdl_controller);
}

bool Controller::isConnected()
{
    return m_sdl_controller && SDL_GameControllerGetAttached(m_sdl_controller) == SDL_TRUE;
}

bool Controller::isButtonPressed(const SDL_GameControllerButton controller_button)
{
    const auto it = std::find(m_pressed.begin(), m_pressed.end(), controller_button);

    if (it != m_pressed.end())
    {
        return false;
    }
    else
    {
        const auto is_down = SDL_GameControllerGetButton(m_sdl_controller, controller_button) == 1;

        if (is_down)
        {
            m_pressed.push_back(controller_button);
        }

        return is_down;
    }
}

bool Controller::isButtonReleased(const SDL_GameControllerButton controller_button)
{
    const auto it = std::find(m_pressed.begin(), m_pressed.end(), controller_button);

    if (it != m_pressed.end())
    {
        const auto is_down = SDL_GameControllerGetButton(m_sdl_controller, controller_button);

        if (!is_down)
        {
            m_pressed.erase(it);
            return true;
        }
    }

    return false;
}
}

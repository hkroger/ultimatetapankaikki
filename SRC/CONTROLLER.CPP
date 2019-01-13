#include "CONTROLLER.H"

#include <algorithm>

namespace tk_port
{
Controller::Controller(const unsigned int index, const input::KeyMapVector* key_map) :
    m_sdl_controller(SDL_GameControllerOpen(index)), m_id(-1), m_key_map(key_map)
{
    if (m_sdl_controller)
    {
        m_id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_sdl_controller));
    }
}

Controller::~Controller()
{
    if (m_sdl_controller)
    {
        SDL_GameControllerClose(m_sdl_controller);
    }
}

bool Controller::getControllerInput(const SDL_GameControllerButton sdl_button, input::ControllerInput& r_input) const
{
    const auto inputFinder = [&sdl_button](const input::key_map_s& map) {
        return sdl_button == map.controller;
    };

    const auto it = std::find_if(m_key_map->begin(), m_key_map->end(), inputFinder);

    if (it != m_key_map->end())
    {
        r_input = it->input;
        return true;
    }
    return false;
}

SDL_JoystickID Controller::getId() const
{
    return m_id;
}

const input::KeyMapVector* Controller::getKeyMap() const
{
    return m_key_map;
}
}

#include "CONTROLLERS.H"
#include "KEYB.H"
#include "INPUT.H"

#include "SDL_events.h"
#include <algorithm>

namespace tk_port
{
void Controllers::handle_event(const SDL_Event *e)
{
    switch (e->type)
    {
    case SDL_CONTROLLERBUTTONUP:
    case SDL_CONTROLLERBUTTONDOWN:
    {
        const SDL_JoystickID id = e->cbutton.which;

        for (const auto& controller : m_controllers)
        {
            if (id == controller->getId())
            {
                const SDL_GameControllerButton sdl_button = static_cast<SDL_GameControllerButton>(e->cbutton.button);
                input::ControllerInput input;

                // Check that button is supported
                if (controller->getControllerInput(sdl_button, input))
                {
                    if (e->type == SDL_CONTROLLERBUTTONDOWN)
                    {
                        input::pressed(input, SDL_GetTicks());
                    }
                    else
                    {
                        input::released(input);
                    }
                }
                break;
            }
        }
        break;
    }
    case SDL_CONTROLLERDEVICEADDED:
    {
        const unsigned int index = e->cdevice.which;

        if (SDL_IsGameController(index))
        {
            std::vector<const input::KeyMapVector*> available_maps =
            {
                &input::s_key_map_p1, // Player 1
                &input::s_key_map_p2  // Player 2
            };

            // Make sure there aren't already two connected controllers
            if (m_controllers.size() < available_maps.size())
            {
                // Remove key maps which are already in use
                for (const auto& controller : m_controllers)
                {
                    available_maps.erase(
                        std::remove(available_maps.begin(), available_maps.end(), controller->getKeyMap()),
                        available_maps.end()
                    );
                }

                // There should be always at least one map available if getting this far
                if (!available_maps.empty())
                {
                    // Select first available map
                    m_controllers.emplace_back(new Controller(index, *(available_maps.begin())));
                }
            }
        }
        break;
    }
    case SDL_CONTROLLERDEVICEREMOVED:
    {
        const SDL_JoystickID id = e->cbutton.which;
        const auto controllerFinder = [&id](const std::unique_ptr<Controller>& controller) {
            return id == controller->getId();
        };

        const auto it = std::find_if(m_controllers.begin(), m_controllers.end(), controllerFinder);

        if (it != m_controllers.end())
        {
            m_controllers.erase(it);
        }
        break;
    }
    default:
        break;
    }
}
}

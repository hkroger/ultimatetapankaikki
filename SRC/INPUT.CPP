#include "INPUT.H"
#include <algorithm>
#include <SDL_keyboard.h>

namespace tk_port
{
Input::Input()
    : value(0)
{
}

Input::Input(const SDL_Scancode &scancode)
    : scancode(scancode)
{
}

Input::Input(const ControllerInput &scancode)
    : controller_input(scancode)
{
}

bool Input::operator < (const Input &rhs) const
{
    return value < rhs.value;
}

bool Input::operator == (const Input &rhs) const
{
    return value == rhs.value;
}

bool Input::operator != (const Input &rhs) const
{
    return value != rhs.value;
}

const char* Input::get_name() const
{
    return get_type() == Controller ?
        get_controller_input_name() :
        SDL_GetScancodeName(scancode);
}

Input::InputType Input::get_type() const
{
    return s_controller_input_bit & value ? Controller : Keyboard;
}

const char* Input::get_controller_input_name() const
{
    const auto find_button = [this](const key_name_map_s& map)
    {
        return map.input == controller_input;
    };

    const auto it = std::find_if(s_key_name_map.begin(), s_key_name_map.end(), find_button);

    return it != s_key_name_map.end() ? it->name : "Unknown";
}
}

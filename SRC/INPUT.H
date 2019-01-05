#ifndef _INPUT_H
#define _INPUT_H

#include <vector>
#include "SDL_gamecontroller.h"
#include "SDL_scancode.h"

#define CONTROLLER_NAME "Cntrl"
#define CONTROLLER_ONE_NAME CONTROLLER_NAME" 1"
#define CONTROLLER_TWO_NAME CONTROLLER_NAME" 2"

#define CONTROLLER_UP "Up"
#define CONTROLLER_DOWN "Down"
#define CONTROLLER_LEFT "Left"
#define CONTROLLER_RIGHT "Right"
#define CONTROLLER_A "A"
#define CONTROLLER_B "B"
#define CONTROLLER_Y "Y"
#define CONTROLLER_X "X"
#define CONTROLLER_LEFT_STICK "LS"
#define CONTROLLER_RIGHT_STICK "RS"
#define CONTROLLER_LEFT_SHOULDER "LB"
#define CONTROLLER_RIGHT_SHOULDER "RB"
#define CONTROLLER_BACK "Back"
#define CONTROLLER_START "Start"

namespace tk_port
{
const unsigned int s_controller_input_bit = 0x10000000;

enum ControllerInput : uint32_t
{
    // ControllerInput enum starts beyond SDL_NUM_SCANCODES so that
    // we can identify the type with simple bitwise and operation
    DPAD_UP_1 = s_controller_input_bit,
    DPAD_DOWN_1,
    DPAD_LEFT_1,
    DPAD_RIGHT_1,
    BUTTON_A_1,
    BUTTON_B_1,
    BUTTON_Y_1,
    BUTTON_X_1,
    BUTTON_LEFT_STICK_1,
    BUTTON_RIGHT_STICK_1,
    BUTTON_LEFT_SHOULDER_1,
    BUTTON_RIGHT_SHOULDER_1,
    BUTTON_BACK_1,
    BUTTON_START_1,

    // Leave space for extension
    DPAD_UP_2 = s_controller_input_bit | 0x80,
    DPAD_DOWN_2,
    DPAD_LEFT_2,
    DPAD_RIGHT_2,
    BUTTON_A_2,
    BUTTON_B_2,
    BUTTON_Y_2,
    BUTTON_X_2,
    BUTTON_LEFT_STICK_2,
    BUTTON_RIGHT_STICK_2,
    BUTTON_LEFT_SHOULDER_2,
    BUTTON_RIGHT_SHOULDER_2,
    BUTTON_BACK_2,
    BUTTON_START_2
};

struct key_map_s
{
    ControllerInput input;
    SDL_GameControllerButton controller;
};

const std::vector<key_map_s> s_key_map_p1
{
    { DPAD_UP_1, SDL_CONTROLLER_BUTTON_DPAD_UP, },
    { DPAD_DOWN_1, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
    { DPAD_LEFT_1, SDL_CONTROLLER_BUTTON_DPAD_LEFT },
    { DPAD_RIGHT_1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
    { BUTTON_A_1, SDL_CONTROLLER_BUTTON_A },
    { BUTTON_B_1, SDL_CONTROLLER_BUTTON_B },
    { BUTTON_Y_1, SDL_CONTROLLER_BUTTON_Y },
    { BUTTON_X_1, SDL_CONTROLLER_BUTTON_X },
    { BUTTON_LEFT_STICK_1, SDL_CONTROLLER_BUTTON_LEFTSTICK },
    { BUTTON_RIGHT_STICK_1, SDL_CONTROLLER_BUTTON_RIGHTSTICK },
    { BUTTON_LEFT_SHOULDER_1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
    { BUTTON_RIGHT_SHOULDER_1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },
    { BUTTON_BACK_1, SDL_CONTROLLER_BUTTON_BACK },
    { BUTTON_START_1, SDL_CONTROLLER_BUTTON_START }
};

const std::vector<key_map_s> s_key_map_p2
{
    { DPAD_UP_2, SDL_CONTROLLER_BUTTON_DPAD_UP, },
    { DPAD_DOWN_2, SDL_CONTROLLER_BUTTON_DPAD_DOWN },
    { DPAD_LEFT_2, SDL_CONTROLLER_BUTTON_DPAD_LEFT },
    { DPAD_RIGHT_2, SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
    { BUTTON_A_2, SDL_CONTROLLER_BUTTON_A },
    { BUTTON_B_2, SDL_CONTROLLER_BUTTON_B },
    { BUTTON_Y_2, SDL_CONTROLLER_BUTTON_Y },
    { BUTTON_X_2, SDL_CONTROLLER_BUTTON_X },
    { BUTTON_LEFT_STICK_2, SDL_CONTROLLER_BUTTON_LEFTSTICK },
    { BUTTON_RIGHT_STICK_2, SDL_CONTROLLER_BUTTON_RIGHTSTICK },
    { BUTTON_LEFT_SHOULDER_2, SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
    { BUTTON_RIGHT_SHOULDER_2, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },
    { BUTTON_BACK_2, SDL_CONTROLLER_BUTTON_BACK },
    { BUTTON_START_2, SDL_CONTROLLER_BUTTON_START }
};

struct key_name_map_s
{
    ControllerInput input;
    const char* name;
};

const std::vector<key_name_map_s> s_key_name_map
{
    { DPAD_UP_1, CONTROLLER_ONE_NAME " " CONTROLLER_UP },
    { DPAD_DOWN_1, CONTROLLER_ONE_NAME " " CONTROLLER_DOWN },
    { DPAD_LEFT_1, CONTROLLER_ONE_NAME " " CONTROLLER_LEFT },
    { DPAD_RIGHT_1, CONTROLLER_ONE_NAME " " CONTROLLER_RIGHT },
    { BUTTON_A_1, CONTROLLER_ONE_NAME " " CONTROLLER_A },
    { BUTTON_B_1, CONTROLLER_ONE_NAME " " CONTROLLER_B },
    { BUTTON_Y_1, CONTROLLER_ONE_NAME " " CONTROLLER_Y },
    { BUTTON_X_1, CONTROLLER_ONE_NAME " " CONTROLLER_X },
    { BUTTON_LEFT_STICK_1, CONTROLLER_ONE_NAME " " CONTROLLER_LEFT_STICK },
    { BUTTON_RIGHT_STICK_1, CONTROLLER_ONE_NAME " " CONTROLLER_RIGHT_STICK },
    { BUTTON_LEFT_SHOULDER_1, CONTROLLER_ONE_NAME " " CONTROLLER_LEFT_SHOULDER },
    { BUTTON_RIGHT_SHOULDER_1, CONTROLLER_ONE_NAME " " CONTROLLER_RIGHT_SHOULDER },
    { BUTTON_BACK_1, CONTROLLER_ONE_NAME " " CONTROLLER_BACK },
    { BUTTON_START_1, CONTROLLER_ONE_NAME " " CONTROLLER_START },

    { DPAD_UP_2, CONTROLLER_TWO_NAME " " CONTROLLER_UP },
    { DPAD_DOWN_2, CONTROLLER_TWO_NAME " " CONTROLLER_DOWN },
    { DPAD_LEFT_2, CONTROLLER_TWO_NAME " " CONTROLLER_LEFT },
    { DPAD_RIGHT_2, CONTROLLER_TWO_NAME " " CONTROLLER_RIGHT },
    { BUTTON_A_2, CONTROLLER_TWO_NAME " " CONTROLLER_A },
    { BUTTON_B_2, CONTROLLER_TWO_NAME " " CONTROLLER_B },
    { BUTTON_Y_2, CONTROLLER_TWO_NAME " " CONTROLLER_Y },
    { BUTTON_X_2, CONTROLLER_TWO_NAME " " CONTROLLER_X },
    { BUTTON_LEFT_STICK_2, CONTROLLER_TWO_NAME " " CONTROLLER_LEFT_STICK },
    { BUTTON_RIGHT_STICK_2, CONTROLLER_TWO_NAME " " CONTROLLER_RIGHT_STICK },
    { BUTTON_LEFT_SHOULDER_2, CONTROLLER_TWO_NAME " " CONTROLLER_LEFT_SHOULDER },
    { BUTTON_RIGHT_SHOULDER_2, CONTROLLER_TWO_NAME " " CONTROLLER_RIGHT_SHOULDER },
    { BUTTON_BACK_2, CONTROLLER_TWO_NAME " " CONTROLLER_BACK },
    { BUTTON_START_2, CONTROLLER_TWO_NAME " " CONTROLLER_START },
};

struct Input
{
    Input();

    Input(const SDL_Scancode &scancode);

    Input(const ControllerInput &scancode);

    bool operator < (const Input &rhs) const;

    bool operator == (const Input &rhs) const;

    bool operator != (const Input &rhs) const;

    const char* get_name() const;

    enum InputType : uint8_t
    {
        Keyboard,
        Controller
    };

    InputType get_type() const;

    union
    {
        SDL_Scancode scancode : 32;
        ControllerInput controller_input;
        uint32_t value; // for comparison use
    };

private:
    const char* get_controller_input_name() const;
};

static_assert(sizeof(Input) == 4, "Input size doesn't match to serialized options");
}

#endif

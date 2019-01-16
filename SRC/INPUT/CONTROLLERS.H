#ifndef _CONTROLLERS_H
#define _CONTROLLERS_H

#include "CONTROLLER.H"
#include <memory>

namespace tk_port
{
class Controllers
{
public:
    void handle_event(const SDL_Event *e);

private:
    std::vector< std::unique_ptr<Controller> > m_controllers;
};
}

#endif

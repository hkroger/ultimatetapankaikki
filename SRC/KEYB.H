#ifndef __TK__KEYB__
#define __TK__KEYB__

#include "PORT.H"

struct KeyboardHandler
{
    char state[256];

    void init( int )
    {
        clear_stack();
    }

    void deinit()
    {}

    void clear_stack()
    {
        for ( int i = 0; i < 256; i++ )
        {
            state[i] = 0;
        }
        _kbhit_flag = 0;
        _last_getch = 0;
    }

    int _kbhit_flag;
    int _last_getch;

    int kbhit()
    {
        if ( _kbhit_flag )
        {
            _kbhit_flag = 0;
            return 1;
        }
        return 0;
    }

    int getch()
    {
        while (!kbhit())
        {
            tk_port_event_tick();
        }
        return _last_getch;
    }

};

#endif

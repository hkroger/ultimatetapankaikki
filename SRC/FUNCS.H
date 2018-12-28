const char *quitexts[] =
{
    "no: let's kill !",
    "yes: i'm a loser"
};

int wanna_quit()
{
    int a, cnt = 0, selected = 0, quit = 0, oclock;
    int done = 0;
    int starty = 100 - ( ( 3*15 )  / 2 );
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    draw_box1( 65, starty - 5 - 10, 320 - 65, starty + 5 + ( 3*15 ), 10 );
    while( !done )
    {
        memcpy( virbuff, picture, 64000 );
        FONT_NUM = 0;
        writefonts2( 160 - ( str_length( "quit to menu" )  / 2 ), starty - 10, "quit to menu", 1 );
        for ( a = 0; a < 2; a ++  )
        {
            if ( a == selected )
            {
                FONT_NUM = 0;
                writefonts2( 160 - ( str_length( quitexts[a] )  / 2 ), starty + 15 + ( a*15 ), quitexts[a], 1 );
            }
            else
            {
                FONT_NUM = 2;
                writefonts2( 160 - ( str_length( quitexts[a] )  / 2 ), starty + 15 + ( a*15 ), quitexts[a], 1 );
            }
        }
        memcpy( screen, virbuff, 64000 );
        oclock = clock();
        while ( !k::state[94]&&!k::state[99]&&!k::state[28] )
        {
            if ( clock() != oclock )
            {
                oclock = clock();
                draw_ball( 65, starty + 15 + ( selected*15 )  - 2, cnt );
                draw_ball( 320 - 85, starty + 15 + ( selected*15 )  - 2, cnt );
                cnt ++;
                if ( cnt > 23 ) cnt = 0;
            }
            tk_port::event_tick();
        }
        MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
        if ( k::state[94] )
        {
            selected --;if ( selected < 0 ) selected = 1; k::state[94] = 0;
        }
        if ( k::state[99] )
        {
            selected ++;if ( selected > 1 ) selected = 0; k::state[99] = 0;
        }
        if ( k::state[28] )
        {
            if ( selected == 0 ) quit = 0;
            if ( selected == 1 ) quit = 1;
            done = 1;
        }
    }
    return( quit );
}

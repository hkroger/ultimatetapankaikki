void level_info()
{
    char text[20];
    int a, b;
    load_efp( "EFPS/COOL.EFP", picture, 0 );
    load_efp_pal( "EFPS/WARE.EFP", pal );
    draw_box1( 10, 10, 10 + 100, 10 + 20, 10 );
    draw_box1( 10, 50, 310, 50 + 20, 10 );
    draw_box1( 10, 90, 160, 190, 10 );
    memcpy( virbuff, picture, 64000 );
    FONT_NUM = 0;
    writefonts2( 16, 13, "level", 1 );
    sprintf( text, "%d", LEVEL + 1 );
    writefonts2( 71, 13, text, 1 );
    writefonts2( 160 - ( str_length( general_level_info.comment )  / 2 ), 13 + 40, general_level_info.comment, 1 );
    FONT_NUM = 1;
    writefonts( 16, 96, "The killing list:", M_RED );
    for ( a = 0, b = 111; a < DIFF_ENEMIES; a ++  )
    if ( general_level_info.enemies[a] > 0 )
    {
        sprintf( text, "%d", general_level_info.enemies[a] );
        strcat( text, " " );
        strcat( text, enemy_info[a].name );
        if ( general_level_info.enemies[a] > 1 )
        strcat( text, "s" );
        writefonts( 16, b, text, M_YELLOW );
        b += 8;
    }
    writefonts( 16, 173, "Your action time is:", M_RED );
    writefonts( 16 + 95, 173, c_2_c( general_level_info.time_limit*TIME_ADD ), M_YELLOW );
    fadein( virbuff, pal );
    k.clear_stack();
    if ( k.getch()  == 27 ) if ( wanna_quit()  ) BACK_TO_MENU = 1;
    if ( !BACK_TO_MENU ) fadeout( virbuff, pal );
}

void statics()
{
    char text[32], text2[20];
    int a, b, timebonus;
    auto get_text_buffer_left = [&text]()
    {
        return sizeof( text ) - strlen( text ) - 1;
    };

    load_efp( "EFPS/COOL.EFP", picture, 0 );
    load_efp_pal( "EFPS/WARE.EFP", pal );
    draw_box1( 10, 10, 310, 32, 10 );
    draw_box1( 10, 40, 310, 190, 10 );
    memcpy( virbuff, picture, 64000 );
    timebonus = ( ( general_level_info.time_limit*TIME_ADD )  - complete_time ) *5;
    if ( timebonus <  - 500 ) timebonus =  - 500;
    FONT_NUM = 0;
    if ( next_level &&!quit )
    writefonts2( 160 - ( str_length( "completed !" )  / 2 ), 13, "completed !", 1 );
    if ( quit ||next_level == 0 )
    writefonts2( 160 - ( str_length( "game over" )  / 2 ), 13, "game over", 1 );
    FONT_NUM = 1;
    for ( a = 0, b = 0; a < DIFF_ENEMIES; a ++  )
    if ( aplayer[0]->kills[a] > 0 ) b = 1;
    if ( b == 1 )
    {
        strncpy( text, "Kills for ", sizeof( text ) );
    }
    if ( b == 0 )
    {
        strncpy( text, "No kills for ", sizeof( text ) );
    }
    strncat( text, aplayer[0]->name, get_text_buffer_left() );
    writefonts( 16, 50, text, M_YELLOW );
    for ( a = 0, b = 65; a < DIFF_ENEMIES; a ++  )
    if ( aplayer[0]->kills[a] > 0 )
    {
        aplayer[0]->total_kills += aplayer[0]->kills[a];
        sprintf( text, "%d", aplayer[0]->kills[a] );
        strncat( text, " ", get_text_buffer_left());
        strncat( text, enemy_info[a].name, get_text_buffer_left() );
        if ( aplayer[0]->kills[a] > 1 )
        strncat( text, "s", get_text_buffer_left() );
        writefonts( 16, b, text, M_YELLOW );
        strncpy( text, "+", sizeof( text ) );
        snprintf( text2, sizeof( text ), "%d", enemy_info[a].reward*aplayer[0]->kills[a] );
        aplayer[0]->cash += enemy_info[a].reward*aplayer[0]->kills[a];
        strncat( text, text2, get_text_buffer_left() );
        writefonts( 103, b, text, M_RED );
        b += 8;
    }
    aplayer[0]->cash += timebonus;
    if ( aplayer[0]->cash < 0 ) aplayer[0]->cash = 0;
    writefonts( 16, 130, "Total kills:", M_RED );
    snprintf( text, sizeof( text ), "%d", aplayer[0]->total_kills );
    writefonts( 76, 130, text, M_YELLOW );
    writefonts( 16, 140, "Accuracy:", M_RED );
    if ( aplayer[0]->shooted > 0 )
    {
        snprintf( text, sizeof( text ), "%d", (int) (((float) aplayer[0]->hitten / aplayer[0]->shooted) * 100));
    }
    else strncpy( text, "0", sizeof( text ) );
    strncat( text, "%", get_text_buffer_left() );
    writefonts( 76, 140, text, M_YELLOW );
    writefonts( 16, 150, "Cash:", M_RED );
    snprintf( text, sizeof( text ), "%d", aplayer[0]->cash );
    writefonts( 76, 150, text, M_YELLOW );
    if ( GAME_MODE == SPLIT_SCREEN )
    {
        for ( a = 0, b = 0; a < DIFF_ENEMIES; a ++  )
        if ( aplayer[1]->kills[a] > 0 ) b = 1;
        if ( b == 1 )
        {
            strncpy( text, "Kills for ", sizeof( text ) );
        }
        if ( b == 0 )
        {
            strncpy( text, "No kills for ", sizeof( text ) );
        }
        strncat( text, aplayer[1]->name, get_text_buffer_left() );
        writefonts( 16 + 160, 50, text, M_YELLOW );
        for ( a = 0, b = 65; a < DIFF_ENEMIES; a ++  )
        if ( aplayer[1]->kills[a] > 0 )
        {
            aplayer[1]->total_kills += aplayer[1]->kills[a];
            snprintf( text, sizeof( text ), "%d", aplayer[1]->kills[a] );
            strncat( text, " ", get_text_buffer_left() );
            strncat( text, enemy_info[a].name, get_text_buffer_left() ) ;
            if ( aplayer[1]->kills[a] > 1 )
            strncat( text, "s", get_text_buffer_left() );
            writefonts( 16 + 160, b, text, M_YELLOW );
            strncpy( text, "+", sizeof( text ) );
            snprintf( text2, sizeof( text ), "%d", enemy_info[a].reward*aplayer[1]->kills[a] );
            aplayer[1]->cash += enemy_info[a].reward*aplayer[1]->kills[a];
            strncat( text, text2, get_text_buffer_left() );
            writefonts( 103 + 160, b, text, M_RED );
            b += 8;
        }
        aplayer[1]->cash += timebonus;
        if ( aplayer[1]->cash < 0 ) aplayer[1]->cash = 0;
        writefonts( 16 + 160, 130, "Total kills:", M_RED );
        snprintf( text, sizeof( text ), "%d", aplayer[1]->total_kills );
        writefonts( 76 + 160, 130, text, M_YELLOW );
        writefonts( 16 + 160, 140, "Accuracy:", M_RED );
        if ( aplayer[1]->shooted > 0 )
        {
            snprintf( text, sizeof( text ), "%d", (int) (((float) aplayer[1]->hitten / aplayer[1]->shooted) * 100));
        }
        else strncpy( text, "0", sizeof( text ) );
        strncat( text, "%", get_text_buffer_left() );
        writefonts( 76 + 160, 140, text, M_YELLOW );
        writefonts( 16 + 160, 150, "Cash:", M_RED );
        snprintf( text, sizeof( text ), "%d", aplayer[1]->cash );
        writefonts( 76 + 160, 150, text, M_YELLOW );
    }
    writefonts( 16, 170, "Time limit:", M_RED );
    writefonts( 16 + 55, 170, c_2_c( general_level_info.time_limit*TIME_ADD ), M_BLUE );
    writefonts( 16 + 80, 170, "Your time:", M_RED );
    writefonts( 16 + 80 + 55, 170, c_2_c( complete_time ), M_BLUE );
    writefonts( 16 + 80 + 80, 170, "Time bonus:", M_RED );
    snprintf( text, sizeof( text ), "%d", timebonus );
    writefonts( 16 + 80 + 55 + 80, 170, text, M_YELLOW );
    fadein( virbuff, pal );
    k.clear_stack();
    if ( k.getch()  == 27 ) if ( !quit )
    {
        if ( wanna_quit()  ) BACK_TO_MENU = 1;
    }
    if ( !BACK_TO_MENU ) fadeout( virbuff, pal );
}

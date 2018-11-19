#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ERROR/ERROR.H"
#include "DEFINES.H"
#include "GLOBVAR.H"
#include "CLASSES.H"
#include "ROUT.H"
#include "NEWFUNCS.H"
#include "MISCFUNC.H"
#include "WRITE.H"
#include "DRAW.H"
#include "EFP/EFP.H"
#include "FADE.H"

int ktexts=8;
char killtexts[][MAX_MESSAGE_LENGTH-20]=
{ "%s slaughtered %s.",
  "%s killed %s.",
  "%s punished %s.",
  "%s didn't like %s.",
  "%s made %s leave..",
  "%s cleaned %s.",
  "%s massacred %s.",
  "%s shot %s."
};

int stexts=5;
char suicidetexts[][MAX_MESSAGE_LENGTH-20]=
{ "%s had to go...",
  "%s got bored.",
  "%s had problems.",
  "%s shot himself.",
  "%s ain't EF..."
};



// TODO NETWORK
#define sendipx(...) ((void)0)

void Message_board::add_message(char msg[])
{
 int a;
 msg[MAX_MESSAGE_LENGTH-1]=NULL;

 if (messages<MAX_MESSAGES) strcpy(message[messages++], msg);
	else 
	 {
	 for (a=0;a<MAX_MESSAGES-1;a++)
	  strcpy(message[a],message[a+1]);
	 strcpy(message[MAX_MESSAGES-1],msg);
	 }

 time_count=MESSAGE_TIME_ON_SCREEN;
}

void Message_board::draw(){
 int a,yoffs=5;

 if (KILLING_MODE==DEATHMATCH)	yoffs=30;

 if (time_count>0) time_count--; else clear();
 for (a=0;a<messages;a++)
  writefonts(5,yoffs+a*9,message[a],32);
}

void Message_board::clear()
{	int a;

 time_count=0;
 messages=0;
 for (a=0;a<MAX_MESSAGES;a++)
  message[a][0]=0;
}

void Light::make( int size ) 
{
	int a, b, pow, offs, dist, hsize;
	hsize = size / 2;
	data = ( char * ) malloc( size*size );
	memset( data, 0, size*size );
	for ( a = 0; a < size; a ++  ) 
	for ( b = 0; b < size; b ++  ) 
	{
		dist = sqrt( ( a - hsize ) *( a - hsize ) + ( b - hsize ) *( b - hsize ) );
		offs = ( a*size )  + b;
		pow = 16 - ( dist*( float ) ( 16 / ( float ) ( size / 2 )  ) );
		if ( pow > 15 ) pow = 15;
		if ( pow < 0 ) pow = 0;
		data[offs] = ( char ) pow;
	}
}

void Crate::move_text() 
{
	if ( text_count > 0 ) 
	{
		text_x += cosi[text_angle]*4;
		text_y += sini[text_angle]*4;
		text_count -= 4;
		if ( text_count <= 0 ) 
		{
			IN_USE = 0; if ( KILLING_MODE == DEATHMATCH ) wake_up_count = 1;
		}
	}
}

void Crate::chk_wake_up(int qwe) 
{	int a;

	if ( wake_up_count > 0 ) 
	{
		wake_up_count ++;
		if ( wake_up_count >= CRATE_WAKE_UP_COUNT ) 
		{
			new_effect( &normal_l, TELEPORT, x + 8, y + 8, 0, 0 );
			text_count = 0; IN_USE = 1; wake_up_count = 0;
			if (GAME_MODE==NETWORK)
			{
			 for(a=1;a<MAX_PLAYERS;a++)
	    	  if (player[a].enabled)
			   sendipx(a,IPX_UPDCRATE,0,qwe);
			}
		}
	}
}

void Player::random_place() 
{
	int a, size = 20, ret = 1;
	while ( ret ) 
	{
		ret = 0;
		x = rand()  % level_x_size*20;
		y = rand()  % level_y_size*20;
		if ( level[( int ) ( ( y / 20 ) *level_x_size + ( x / 20 )  ) ].type!= FLOOR ) ret = 1;
		if ( level[( int ) ( ( ( y + size )  / 20 ) *level_x_size + ( x / 20 )  ) ].type!= FLOOR ) ret = 1;
		if ( level[( int ) ( ( y / 20 ) *level_x_size + ( ( x + size )  / 20 )  ) ].type!= FLOOR ) ret = 1;
		if ( level[( int ) ( ( ( y + size )  / 20 ) *level_x_size + ( ( x + size )  / 20 )  ) ].type!= FLOOR ) ret = 1;
		if ( ENEMIES_ON_GAME ) 
		for ( a = 0; a < ENEMIES &&ret == 0; a ++  ) 
		if ( !enemy[a].DEAD ) 
		if ( get_dist( x, y, enemy[a].x, enemy[a].y )  < 30 ) ret = 1;
		for ( a = 0; a < MAX_PLAYERS &&ret == 0; a ++  ) 
		if ( player[a].enabled)
		if ( &player[a]!= this ) 
		if ( get_dist( x, y, player[a].x, player[a].y )  < 30 ) ret = 1;
	}
}

void Player::death_match_start_up() 
{
	int a;
	
	if (DEAD)
	 new_effect( &normal_l, TELEPORT, x + 14, y + 14, 0, 0 );
	random_place();
	new_effect( &normal_l, TELEPORT, x + 14, y + 14, 0, 0 );
	energy = ( max_energy + shield*10 )  / 3;
	for ( a = 0; a < DIFF_WEAPONS + 1; a ++  ) weapons[a] = 0;
	for ( a = 0; a < DIFF_BULLETS; a ++  ) bullets[a] = 0;
	weapons[0] = 1;
	curr_weapon = 0;
	load_cnt = 0;
	DEAD = 0;
	PUSH_POWER = 0;
	PUSH_ANGLE = 0;
	move( 0, 0, 0 );
	burning = 0;
	if (GAME_MODE==NETWORK)
	{ 
		if (NETWORK_MODE==CLIENT) 
			sendipx(0,IPX_BURNING,tindex,0);
		else 
		 for(a=1;a<MAX_PLAYERS;a++)
	      if (player[a].enabled)
		   sendipx(a,IPX_BURNING,tindex,0);
	}
}

void Player::kick_body_parts() 
{
	int a, ang;
	for ( a = 0; a < MAX_BODY_PARTS; a ++  ) 
	if ( body_part[a].IN_USE ) 
	if ( x + 14 > body_part[a].x ) 
	if ( x + 14 < body_part[a].x + 20 ) 
	if ( y + 14 > body_part[a].y ) 
	if ( y + 14 < body_part[a].y + 20 ) 
	{
		KICK = 1;
		ang = ( rangle2 - 5 )  + ( rand()  % 10 );
		if ( ang < 0 ) ang += 360;
		if ( ang >= 360 ) ang -= 360;
		body_part[a].angle = ang;
		body_part[a].speed = 6;
		MIDASplaySample( samplep[HITWAV], MIDAS_CHANNEL_AUTO, 0, 10000, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
	}
}

void Player::chk_keys( int index ) 
{
	int walk_chk = 0;
	int speedi = speed;
	if ( k.state[K_LEFT] ) if ( !k.state[K_STRAFE] ) rotate( 9 );
	if ( ( k.state[K_STRAFE]&&k.state[K_LEFT] ) ||k.state[K_LSTRAFE] ) 
	{
		move( int( rangle2 + 90 )  % 360, speed*0.9, index );
		walk_chk = 1;
		speedi = speed*0.8;
	}
	if ( k.state[K_RIGHT] ) if ( !k.state[K_STRAFE] ) rotate(  - 9 );
	if ( ( k.state[K_STRAFE]&&k.state[K_RIGHT] ) ||k.state[K_RSTRAFE] ) 
	{
		move( int( rangle2 + 270 )  % 360, speed*0.9, index );
		walk_chk = 1;
		speedi = speed*0.8;
	}
	if ( k.state[K_UP] ) 
	{
		move( rangle2, speedi, index );walk_chk = 1; if ( rand()  % 20 == 0 ) kick_body_parts();
	}
	if ( k.state[K_DOWN] ) 
	{
		move( ( int ) ( rangle2 + 180 )  % 360, 0.75*speedi, index );walk_chk = 1;
	}
	if ( walk_chk ) if ( WALK == 0 ) WALK = 1;
	if ( !walk_chk ) WALK = 0;
	if ( k.state[K_SHOOT] ) if ( load_cnt == weapon[curr_weapon].loading_time ){ 
	 if (curr_weapon!=0){
	 if ( bullets[weapon[curr_weapon].btype - 1] > 0 ) 
		shoot(); else {
		curr_weapon = 0; load_cnt = 0;
		}
	 }
	 if ( curr_weapon == 0 ) { shoot();kick_body_parts();}
	 }
	if ( !k.state[K_SHOOT] ) shoot_cnt = 0;else shoot_cnt ++;
	if ( k.state[K_SHIFT] ) 
	{
		change_weapon();k.state[K_SHIFT] = 0;
	}
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		if ( k.state[41] ) {
			curr_weapon = 0; load_cnt=0;
		}
		if ( k.state[2] ) if ( weapons[1] ){
			curr_weapon = 1; load_cnt = 0;
		}
		if ( k.state[3] ) if ( weapons[2] ){
			curr_weapon = 2; load_cnt = 0;
		}
		if ( k.state[4] ) if ( weapons[3] ){
			curr_weapon = 3; load_cnt = 0;
		}
		if ( k.state[5] ) if ( weapons[4] ){
			curr_weapon = 4; load_cnt = 0;
		}
		if ( k.state[6] ) if ( weapons[5] ){
			curr_weapon = 5; load_cnt = 0;
		}
		if ( k.state[7] ) if ( weapons[6] ){
			curr_weapon = 6; load_cnt = 0;
		}
		if ( k.state[8] ) if ( weapons[7] ){
			curr_weapon = 7; load_cnt = 0;
		}
		if ( k.state[9] ) if ( weapons[8] ){
			curr_weapon = 8; load_cnt = 0;
		}
		if ( k.state[10] ) if ( weapons[9] ){
			curr_weapon = 9; load_cnt = 0;
		}
		if ( k.state[11] ) if ( weapons[10] ){
			curr_weapon = 10; load_cnt = 0;
		}
		if ( k.state[12] ) if ( weapons[11] ){
			curr_weapon = 11; load_cnt = 0;
		}
	}
}

void Body_part::move() 
{
	int offs[2*4]; //**
	int reuna = 4; //**chk pointit on näin
	int reuna2 = 4; //**
	int rnx, rny; //**
	float newx, newy;
	newx = x + ( ( int ) speed*sini[angle] );
	newy = y + ( ( int ) speed*cosi[angle] );
	rnx = newx;
	rny = newy;
	if ( num < 12 ) 
	if ( rand()  % 3!= 0 ) new_effect( 0, BLOOD2, int( x )  + 10, int( y )  + 10, 0, 0 );
	offs[0] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 10 - reuna2 )  / 20 );
	offs[1] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 10 + reuna2 )  / 20 );
	offs[2] = ( ( rny + 20 - reuna )  / 20 ) *level_x_size + ( ( rnx + 10 - reuna2 )  / 20 );
	offs[3] = ( ( rny + 20 - reuna )  / 20 ) *level_x_size + ( ( rnx + 10 + reuna2 )  / 20 );
	offs[4] = ( ( rny + 10 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[5] = ( ( rny + 10 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[6] = ( ( rny + 10 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + 20 - reuna )  / 20 );
	offs[7] = ( ( rny + 10 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + 20 - reuna )  / 20 );
	if ( newy < y ) 
	if ( level[offs[0]].type == FLOOR ) 
	if ( level[offs[1]].type == FLOOR ) 
	y = newy;
	if ( newy > y ) 
	if ( level[offs[2]].type == FLOOR ) 
	if ( level[offs[3]].type == FLOOR ) 
	y = newy;
	if ( newx < x ) 
	if ( level[offs[4]].type == FLOOR ) 
	if ( level[offs[5]].type == FLOOR ) 
	x = newx;
	if ( newx > x ) 
	if ( level[offs[6]].type == FLOOR ) 
	if ( level[offs[7]].type == FLOOR ) 
	x = newx;
}

void Player::animate() 
{	int a;

	if ( weapon[curr_weapon].gun == 0 ) ANIM = 1;else ANIM = 4;
	if ( WALK ) 
	{
		if ( WALK - 1 < WALK_DELAY ) WALK ++;else WALK = 1;
		ANIM = ( WALK - 1 )  / ( WALK_DELAY / 4 );
		if ( ANIM >= 3 ) ANIM = 1 ;
		if ( weapon[curr_weapon].gun == 1 ) ANIM += 3;
	}
	if ( FIRE ) 
	{
		FIRE ++;
		if ( FIRE > 3 ) FIRE = 0;
		if ( weapon[curr_weapon].gun == 0 ) ANIM = 7; else ANIM = 6;
	}
	if ( DEAD ) 
	{
		if ( DEAD - 1 < DEAD_DELAY ) DEAD ++;
		ANIM = 8 + ( ( DEAD - 1 )  / ( DEAD_DELAY / 2 )  );
		if ( KILLING_MODE == DEATHMATCH ) 
		if ( DEAD - 1 == DEAD_DELAY ) 
		death_match_start_up();
	}
	if ( !DEAD ) 
	if ( energy < 20 ) ANIM += 11;
	if ( KICK ) 
	{
		KICK ++;if ( KICK > 5 ) KICK = 0;
		if ( curr_weapon == 0 ) ANIM = 19;else ANIM = 20;
		if ( energy < 20 ) ANIM += 2;
	}

	if (GAME_MODE==NETWORK)
	{
	if (NETWORK_MODE==CLIENT) sendipx(0,IPX_PLYANIM,(char)aplayer[0]->tindex);
	else 
		 for(a=1;a<MAX_PLAYERS;a++)
	      if (player[a].enabled)
		   sendipx(a,IPX_PLYANIM,0);
	}
}

void Enemy::animate() 
{
	ANIM = 1;
	if ( WALK ) 
	{
		if ( WALK - 1 < WALK_DELAY ) WALK ++;else WALK = 1;
		ANIM = ( WALK - 1 )  / ( WALK_DELAY / 4 );
		if ( ANIM >= 3 ) ANIM = 1 ;
	}
	if ( FIRE ) 
	{
		FIRE ++;
		if ( FIRE > 3 ) FIRE = 0;
		ANIM = 3;
	}
	if ( !DEAD ) 
	if ( energy < 10 ) ANIM += 4;
}

void Effect::roll() 
{
	if ( IN_USE ) 
	{
		if ( count < type-> tcount*type-> delay ) 
		if ( type-> random == 0 ) count ++;
		else 
		count += rand()  % 3;
		if ( count >= type-> tcount*type-> delay ) IN_USE = 0;
		if ( speed > 0 ) 
		{
			x += speed*sini[angle];
			y += speed*cosi[angle];
			if ( level[( int ) ( y / 20 ) *level_x_size + ( int ) ( x / 20 ) ].type!= FLOOR ) IN_USE = 0;
		}
		if ( IN_USE == 0 ) if ( type-> index == BLOOD ) new_effect( 0, BLOOD2, x, y, 0, 0 );
	}
}

void Player::change_weapon() 
{
	curr_weapon ++;
	while ( weapons[curr_weapon%= DIFF_WEAPONS + 1] == 0 ) 
	{
		curr_weapon ++;curr_weapon%= DIFF_WEAPONS + 1;
	}
	load_cnt = 0;
}

void Player::footprint( int side ) 
{
	int a, do_it = 0, footx, footy, footoffs;
	footx = x + 15 + ( sini[( rangle2 + 90 + ( side*180 )  )  % 360]*4 );
	footy = y + 15 + ( cosi[( rangle2 + 90 + ( side*180 )  )  % 360]*4 );
	footoffs = ( footy / 20 ) *level_x_size + ( footx / 20 );
	if ( level[footoffs].type == FLOOR ) 
	for ( a = 0; a < DUST_BLOCKS &&do_it == 0; a ++  ) 
	if ( level[footoffs].num == dust_blocks[a] ) do_it = 1;
	if ( do_it ) 
	new_effect( 0, FOOTPRINT, footx, footy, 0, 0 );
}

void Player::move( int angle, float spd, int index ) 
{
	int offs[2*4];   //  **
	int reuna = 6;   // *  * chk pointit on näin
	int reuna2 = 4;  // *  *
	int rnx, rny;    //  **
	int crate_taken = 0;
	float newx, newy;
	float speedi;
	int chk, a, b;

	if ( WALK == ( int ) ( WALK_DELAY*0.25 ) ||WALK == ( int ) ( WALK_DELAY*0.75 )  ) footprint( 0 );
	if ( WALK == ( int ) ( WALK_DELAY*0.5 ) ||WALK == WALK_DELAY - 1 ) footprint( 1 );
	if ( energy < 20 ) // veren tiputus
	if ( WALK % 4 == 0 ) 
	if ( rand()  % 3 == 0 ) 
	new_effect( 0, BLOOD2, int( x )  + 15, int( y )  + 15, 0, 0 );
	speedi = spd;
	newx = x + ( speedi*sini[angle] );
	newy = y + ( speedi*cosi[angle] );
	rnx = newx;
	rny = newy;
	offs[0] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 14 - reuna2 )  / 20 );
	offs[1] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 14 + reuna2 )  / 20 );
	offs[2] = ( ( rny + 28 - reuna )  / 20 ) *level_x_size + ( ( rnx + 14 - reuna2 )  / 20 );
	offs[3] = ( ( rny + 28 - reuna )  / 20 ) *level_x_size + ( ( rnx + 14 + reuna2 )  / 20 );
	offs[4] = ( ( rny + 14 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[5] = ( ( rny + 14 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[6] = ( ( rny + 14 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + 28 - reuna )  / 20 );
	offs[7] = ( ( rny + 14 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + 28 - reuna )  / 20 );
	if ( newy < y ) 
	if ( level[offs[0]].type == FLOOR ) 
	if ( level[offs[1]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 15 > enemy[a].x ) 
		if ( rnx + 15 < enemy[a].x + 30 ) 
		if ( rny + reuna > enemy[a].y + reuna ) 
		if ( rny + reuna < enemy[a].y + 28 - reuna ) chk = 1;
		for ( b = 0; b < MAX_PLAYERS &&chk == 0; b ++  ) 
		if ( player[b].enabled)
		if ( this != &player[b] ) 
		if ( !player[b].DEAD ) 
		if ( rnx + 15 > player[b].x ) 
		if ( rnx + 15 < player[b].x + 30 ) 
		if ( rny + reuna > player[b].y + reuna ) 
		if ( rny + reuna < player[b].y + 28 - reuna ) chk = 1;
		if ( !chk ) y = newy;
	}
	if ( newy > y ) 
	if ( level[offs[2]].type == FLOOR ) 
	if ( level[offs[3]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 15 > enemy[a].x ) 
		if ( rnx + 15 < enemy[a].x + 30 ) 
		if ( rny + 28 - reuna > enemy[a].y + reuna ) 
		if ( rny + 28 - reuna < enemy[a].y + 28 - reuna ) chk = 1;
		for ( b = 0; b < MAX_PLAYERS &&chk == 0; b ++  ) 
		if ( player[b].enabled)
		if ( this != &player[b] ) 
		if ( !player[b].DEAD ) 
		if ( rnx + 15 > player[b].x ) 
		if ( rnx + 15 < player[b].x + 30 ) 
		if ( rny + 28 - reuna > player[b].y + reuna ) 
		if ( rny + 28 - reuna < player[b].y + 28 - reuna ) chk = 1;
		if ( !chk ) y = newy;
	}
	if ( newx < x ) 
	if ( level[offs[4]].type == FLOOR ) 
	if ( level[offs[5]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + reuna > enemy[a].x + reuna ) 
		if ( rnx + reuna < enemy[a].x + 28 - reuna ) 
		if ( rny + 15 > enemy[a].y ) 
		if ( rny + 15 < enemy[a].y + 30 ) chk = 1;
		for ( b = 0; b < MAX_PLAYERS &&chk == 0; b ++  ) 
		if ( player[b].enabled)
		if ( this != &player[b] ) 
		if ( !player[b].DEAD ) 
		if ( rnx + reuna > player[b].x + reuna ) 
		if ( rnx + reuna < player[b].x + 28 - reuna ) 
		if ( rny + 15 > player[b].y ) 
		if ( rny + 15 < player[b].y + 30 ) chk = 1;
		if ( !chk ) x = newx;
	}
	if ( newx > x ) 
	if ( level[offs[6]].type == FLOOR ) 
	if ( level[offs[7]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 28 - reuna > enemy[a].x + reuna ) 
		if ( rnx + 28 - reuna < enemy[a].x + 28 - reuna ) 
		if ( rny + 15 > enemy[a].y ) 
		if ( rny + 15 < enemy[a].y + 30 ) chk = 1;
		for ( b = 0; b < MAX_PLAYERS &&chk == 0; b ++  ) 
		if ( this != &player[b] ) 
		if ( player[b].enabled)
		if ( !player[b].DEAD ) 
		if ( rnx + 28 - reuna > player[b].x + reuna ) 
		if ( rnx + 28 - reuna < player[b].x + 28 - reuna ) 
		if ( rny + 15 > player[b].y ) 
		if ( rny + 15 < player[b].y + 30 ) chk = 1;
		if ( !chk ) x = newx;
	}
	for ( a = 0; a < MAX_CRATES; a ++  ) // laatikoiden keräys
	if ( crate[a].IN_USE ) 
	if ( crate[a].text_count == 0 ) 
	if ( crate[a].x + 7 > x ) 
	if ( crate[a].x + 7 < x + 30 ) 
	if ( crate[a].y + 7 > y ) 
	if ( crate[a].y + 7 < y + 30 ) 
	{
		crate_taken = 0;
		if ( crate[a].type == 0 ) 
		if ( weapons[crate[a].weapon + 1] == 0 )  
		{
			weapons[crate[a].weapon + 1] = 1; crate_taken = 1;
		}
		if ( crate[a].type == 1 ) 
		if ( bullets[crate[a].bullet] < bullet_type[crate[a].bullet + 1].max*bullet_type[crate[a].bullet + 1].mul ) 
		{
			bullets[crate[a].bullet] += bullet_type[crate[a].bullet + 1].amount_in_crate;
			if ( bullets[crate[a].bullet] > bullet_type[crate[a].bullet + 1].max*bullet_type[crate[a].bullet + 1].mul ) bullets[crate[a].bullet] = bullet_type[crate[a].bullet + 1].max*bullet_type[crate[a].bullet + 1].mul;
			crate_taken = 1;
		}
		if ( crate[a].type == 2 ) if ( energy < max_energy + shield*10 ) 
		{
			energy += 40; if ( energy > max_energy + shield*10 ) energy = max_energy + shield*10; crate_taken = 1;
		}
		if ( crate_taken ) 
		{
			MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 11250, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
			if ( crate[a].type == 0 ) 
			strcpy( crate[a].text, weapon[crate[a].weapon + 1].name );
			if ( crate[a].type == 1 ) 
			strcpy( crate[a].text, bullet_type[crate[a].bullet + 1].name );
			if ( crate[a].type == 2 ) 
			strcpy( crate[a].text, "Energy" );
			if ( index == 0 ) 
			{
				crate[a].text_x = crate[a].x + 7 - str_length( crate[a].text )  - scr_x;
				crate[a].text_y = crate[a].y + 7 - scr_y;
				crate[a].text_count = get_dist( crate[a].text_x, crate[a].text_y, 10, scr_y_size - 20 )  + 1;
				crate[a].text_angle = get_angle( crate[a].text_x, crate[a].text_y, 10, scr_y_size - 20 );
			}
			if ( GAME_MODE == SPLIT_SCREEN ) 
			if ( index == 1 ) 
			{
				crate[a].text_x = crate[a].x + 7 - str_length( crate[a].text )  - scr_x + 160;
				crate[a].text_y = crate[a].y + 7 - scr_y;
				crate[a].text_count = get_dist( crate[a].text_x, crate[a].text_y, 170, scr_y_size - 20 )  + 1;
				crate[a].text_angle = get_angle( crate[a].text_x, crate[a].text_y, 170, scr_y_size - 20 );
			}
			if (enabled)
			if (GAME_MODE==NETWORK)
			{
			if (NETWORK_MODE==CLIENT) sendipx(0,IPX_UPDCRATE,(char)aplayer[0]->tindex,a);
			else 
				 for(b=1;b<MAX_PLAYERS;b++)
			      if (player[b].enabled)
				   sendipx(b,IPX_UPDCRATE,0,a);
			}
		}
	}


	if (GAME_MODE==NETWORK)
	{
	if (NETWORK_MODE==CLIENT) sendipx(0,IPX_PLYXY,(char)aplayer[0]->tindex);
	 else 			
		 for(a=1;a<MAX_PLAYERS;a++)
	      if (player[a].enabled)
		   sendipx(a,IPX_PLYXY,(char)0);
	}
  
}

void Player::rotate( int change ) 
{	int a;

	angle += change;
	if ( angle >= 360 ) angle -= 360;
	if ( angle < 0 ) angle += 360;
	rangle = angle / 9;
	rangle2 = rangle*9;

	if (GAME_MODE==NETWORK)
	{
	if (NETWORK_MODE==CLIENT) sendipx(0,IPX_PLYRANGLE,(char)aplayer[0]->tindex);
	else 
		 for(a=1;a<MAX_PLAYERS;a++)
	      if (player[a].enabled)
		   sendipx(a,IPX_PLYRANGLE,0);
	}
}

void Player::move_scr() 
{
	int spd;
	if ( GAME_MODE == SPLIT_SCREEN ) 
	{
		if ( abs( ( scr_x + 80 )  - ( x + 14 )  )  > 100 ) scr_x = x - 80;
		spd = abs( ( scr_x + 80 )  - ( ( 35*sini[angle] )  + x + 14 )  )  / 4;
		if ( scr_x + 80 < ( 35*sini[angle] )  + x + 14 ) scr_x += spd;
		if ( scr_x + 80 > ( 35*sini[angle] )  + x + 14 ) scr_x -= spd;
		if ( scr_x < 0 ) scr_x = 0;
		if ( scr_x > ( level_x_size*20 )  - 160 ) scr_x = ( level_x_size*20 )  - 160;
	}
	else 
	{
		if ( abs( ( scr_x + 160 )  - ( x + 14 )  )  > 160 ) scr_x = x - 160;
		spd = abs( ( scr_x + 160 )  - ( ( 25*sini[angle] )  + x + 14 )  )  / 4;
		if ( scr_x + 160 < ( 25*sini[angle] )  + x + 14 ) scr_x += spd;
		if ( scr_x + 160 > ( 25*sini[angle] )  + x + 14 ) scr_x -= spd;
		if ( scr_x < 0 ) scr_x = 0;
		if ( scr_x > ( level_x_size*20 )  - 320 ) scr_x = ( level_x_size*20 )  - 320;
	}
	if ( abs( ( scr_y + ( scr_y_size / 2 )  )  - ( y + 14 )  )  > 120 ) scr_y = y - ( scr_y_size / 2 );
	spd = abs( ( scr_y + ( scr_y_size / 2 )  )  - ( ( 25*cosi[angle] )  + y + 14 )  )  / 4;
	if ( scr_y + ( scr_y_size / 2 )  < ( 25*cosi[angle] )  + y + 14 ) scr_y += spd;
	if ( scr_y + ( scr_y_size / 2 )  > ( 25*cosi[angle] )  + y + 14 ) scr_y -= spd;
	if ( scr_y > ( level_y_size*20 )  - scr_y_size ) scr_y = ( level_y_size*20 )  - scr_y_size;
	if ( scr_y < 0 ) scr_y = 0;
}

void Player::load() 
{
	if ( load_cnt < weapon[curr_weapon].loading_time ) load_cnt ++;
}

void Enemy::load() 
{
	if ( load_cnt < weapon[enemy_info[type].curr_weapon].loading_time ) load_cnt ++;
}

void Enemy::get_angles() 
{
	int a, b;
	for ( b = 0; b < MAX_PLAYERS; b ++  ) 
    if ( player[b].enabled)
	{
		a = 270 - get_angle( player[b].x, player[b].y, x, y );
		if ( a < 0 ) a += 360;
		angle_pl[b] = a;
	}
}

void Enemy::try_see_players() 
{
	int dist, a, b, min, max, rx, ry, hit = 0, go_on = 0;
	int see[MAX_PLAYERS], saw[MAX_PLAYERS];
	int weakest = 0;
	see_somebody = 0;
	min = angle - 90;
	if ( min < 0 ) min += 360;
	max = angle + 90;
	if ( max > 360 ) max -= 360;
	for ( b = 0; b < MAX_PLAYERS; b ++  ) 
	if ( player[b].enabled)
	see[b] = 0;
	for ( b = 0; b < MAX_PLAYERS; b ++  ) 
	if ( player[b].enabled){
	if ( SEE_PL[b] ) saw[b] = 1;else saw[b] = 0;
	}
	for ( b = 0; b < MAX_PLAYERS; b ++  ) 
	if ( player[b].enabled)
	SEE_PL[b] = 0;
	for ( b = 0; b < MAX_PLAYERS; b ++  ) 
	if ( player[b].enabled)
	{
		go_on = 0;
		hit = 0;
		if ( !player[b].DEAD ) 
		{
			if ( angle < 270 &&angle > 90 ) 
			{
				// näkökenttä180 astetta
				if ( angle_pl[b] > min &&angle_pl[b] < max ) go_on = 1;
			}
			else if ( angle_pl[b] > min ||angle_pl[b] < max ) go_on = 1;
		}
		if ( go_on ) 
		for ( a = 0; a < 8*4 &&hit == 0; a ++  ) 
		{
			rx = 14 + x + ( a*5*sini[angle_pl[b]] );
			ry = 14 + y + ( a*5*cosi[angle_pl[b]] );
			if ( level[( ry / 20 ) *level_x_size + ( rx / 20 ) ].type!= FLOOR ) hit = 1;
			if ( rx > player[b].x + 5 ) 
			if ( rx < player[b].x + 25 ) 
			if ( ry > player[b].y + 5 ) 
			if ( ry < player[b].y + 25 ) 
			{
				SEE_PL[b] = 1; see[b] = 1; see_somebody = 1;
			};
		}
	}
	if ( see_somebody ) 
	{
		for ( b = 0; b < MAX_PLAYERS; b ++  ) 
		if (player[b].enabled)
		if ( !player[b].DEAD ) 
		if ( see[b] ) weakest = b;
		for ( b = 0; b < MAX_PLAYERS; b ++  ) 
		if ( player[b].enabled)
		if ( !player[b].DEAD ) 
		if ( see[b] ) 
		{
			if ( player[b].energy < player[weakest].energy ) weakest = b;
		}
		tangle = angle_pl[weakest];
	}
	else 
	{
		for ( b = 0; b < MAX_PLAYERS; b ++  ) 
		if ( player[b].enabled)
		if ( saw[b] ) 
		{
			dist = get_dist( x, y, player[b].x, player[b].y ); // chk the dist and be clever
			walk_cnt = dist / enemy_info[type].speed;
			if ( walk_cnt < 0 ) walk_cnt = 0;
		}
	}
}

void Enemy::footprint( int side ) 
{
	int a, do_it = 0, footx, footy, footoffs;
	footx = x + 15 + ( sini[( angle + 90 + ( side*180 )  )  % 360]*4 );
	footy = y + 15 + ( cosi[( angle + 90 + ( side*180 )  )  % 360]*4 );
	footoffs = ( footy / 20 ) *level_x_size + ( footx / 20 );
	if ( level[footoffs].type == FLOOR ) 
	for ( a = 0; a < DUST_BLOCKS &&do_it == 0; a ++  ) 
	if ( level[footoffs].num == dust_blocks[a] ) do_it = 1;
	if ( do_it ) 
	new_effect( 0, FOOTPRINT, footx, footy, 0, 0 );
}

void Enemy::move( int angle, int spd ) 
{
	int offs[2*4];
	int reuna = 6; // päächk pointien etäisyys reunasta
	int reuna2 = 4; // sivu chk pointien etäisyys päächk pointeista
	int rnx, rny, speedi, chk, a;
	float newx, newy, ox = x, oy = y;
	if ( WALK == ( int ) ( WALK_DELAY*0.25 ) ||WALK == ( int ) ( WALK_DELAY*0.75 )  ) footprint( 0 );
	if ( WALK == ( int ) ( WALK_DELAY*0.5 ) ||WALK == WALK_DELAY - 1 ) footprint( 1 );
	if ( energy < 10 ) 
	if ( WALK % 4 == 0 ) 
	if ( rand()  % 3 == 0 ) 
	new_effect( 0, BLOOD2, int( x )  + 15, int( y )  + 15, 0, 0 );
	if ( spd == 0 ) speedi = enemy_info[type].speed;
	else speedi = spd;
	newx = x + ( speedi*sini[angle] );
	newy = y + ( speedi*cosi[angle] );
	rnx = newx;
	rny = newy;
	offs[0] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 14 - reuna2 )  / 20 );
	offs[1] = ( ( rny + reuna )  / 20 ) *level_x_size + ( ( rnx + 14 + reuna2 )  / 20 );
	offs[2] = ( ( rny + 28 - reuna )  / 20 ) *level_x_size + ( ( rnx + 14 - reuna2 )  / 20 );
	offs[3] = ( ( rny + 28 - reuna )  / 20 ) *level_x_size + ( ( rnx + 14 + reuna2 )  / 20 );
	offs[4] = ( ( rny + 14 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[5] = ( ( rny + 14 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + reuna )  / 20 );
	offs[6] = ( ( rny + 14 - reuna2 )  / 20 ) *level_x_size + ( ( rnx + 28 - reuna )  / 20 );
	offs[7] = ( ( rny + 14 + reuna2 )  / 20 ) *level_x_size + ( ( rnx + 28 - reuna )  / 20 );
	chk = 0;
	if ( newy < y ) 
	if ( level[offs[0]].type == FLOOR ) 
	if ( level[offs[1]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( &enemy[a]!= this ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 15 > enemy[a].x ) 
		if ( rnx + 15 < enemy[a].x + 30 ) 
		if ( rny + reuna > enemy[a].y + reuna ) 
		if ( rny + reuna < enemy[a].y + 28 - reuna ) chk = 1;
		for ( a = 0; a < MAX_PLAYERS &&chk == 0; a ++  ) 
		if ( player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( rnx + 15 > player[a].x ) 
		if ( rnx + 15 < player[a].x + 30 ) 
		if ( rny + reuna > player[a].y + reuna ) 
		if ( rny + reuna < player[a].y + 28 - reuna ) chk = 1;
		if ( !chk ) y = newy;
	}
	chk = 0;
	if ( newy > y ) 
	if ( level[offs[2]].type == FLOOR ) 
	if ( level[offs[3]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( &enemy[a]!= this ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 15 > enemy[a].x ) 
		if ( rnx + 15 < enemy[a].x + 30 ) 
		if ( rny + 28 - reuna > enemy[a].y + reuna ) 
		if ( rny + 28 - reuna < enemy[a].y + 28 - reuna ) chk = 1;
		for ( a = 0; a < MAX_PLAYERS &&chk == 0; a ++  ) 
		if ( player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( rnx + 15 > player[a].x ) 
		if ( rnx + 15 < player[a].x + 30 ) 
		if ( rny + 28 - reuna > player[a].y + reuna ) 
		if ( rny + 28 - reuna < player[a].y + 28 - reuna ) chk = 1;
		if ( !chk ) y = newy;
	}
	chk = 0;
	if ( newx < x ) 
	if ( level[offs[4]].type == FLOOR ) 
	if ( level[offs[5]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( &enemy[a]!= this ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + reuna > enemy[a].x + reuna ) 
		if ( rnx + reuna < enemy[a].x + 28 - reuna ) 
		if ( rny + 15 > enemy[a].y ) 
		if ( rny + 15 < enemy[a].y + 30 ) chk = 1;
		for ( a = 0; a < MAX_PLAYERS &&chk == 0; a ++  ) 
		if ( player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( rnx + reuna > player[a].x + reuna ) 
		if ( rnx + reuna < player[a].x + 28 - reuna ) 
		if ( rny + 15 > player[a].y ) 
		if ( rny + 15 < player[a].y + 30 ) chk = 1;
		if ( !chk ) x = newx;
	}
	chk = 0;
	if ( newx > x ) 
	if ( level[offs[6]].type == FLOOR ) 
	if ( level[offs[7]].type == FLOOR ) 
	{
		for ( chk = 0, a = 0; a < ENEMIES; a ++  ) 
		if ( &enemy[a]!= this ) 
		if ( !enemy[a].DEAD ) 
		if ( rnx + 28 - reuna > enemy[a].x + reuna ) 
		if ( rnx + 28 - reuna < enemy[a].x + 28 - reuna ) 
		if ( rny + 15 > enemy[a].y ) 
		if ( rny + 15 < enemy[a].y + 30 ) chk = 1;
		for ( a = 0; a < MAX_PLAYERS &&chk == 0; a ++  ) 
		if ( player[a].enabled)
		if ( !player[a].DEAD ) 
		if ( rnx + 28 - reuna > player[a].x + reuna ) 
		if ( rnx + 28 - reuna < player[a].x + 28 - reuna ) 
		if ( rny + 15 > player[a].y ) 
		if ( rny + 15 < player[a].y + 30 ) chk = 1;
		if ( !chk ) x = newx;
	}
}

void Enemy::get_damage( float damage, Player *P ) 
{
	int pan = 0, a, alku = rand()  % 7, spd = 1 + damage / 3;
	spd += ( PUSH_POWER / 3 );
	if ( spd > 5 ) spd = 5;
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	 pan = get_pan( player[0].x , x );
	energy -= damage;
	if ( energy <= 0 ) 
	{
		if ( burning && ! enemy_info[type].explo_death) 
		{
			MIDASplaySample( samplep[WOOSHWAV], MIDAS_CHANNEL_AUTO, 0, 20000, EFFECT_VOLUME, pan );
			new_effect( &explo_l, EXPLO, x + 12, y + 12, 0, 0 );
			new_effect( &explo_l, EXPLO, x + 18, y + 12, 0, 0 );
			new_effect( &explo_l, EXPLO, x + 15, y + 18, 0, 0 );
			for ( a = 0; a < 3; a ++  ) 
			new_body_part( x + 4, y + 4, rand()  % 360, 5, a + 12 );
		}
		if ( ! burning && ! enemy_info[type].explo_death) 
		{
			for ( a = 0; a < 16; a ++  ) 
			if ( rand()  % 2 ) 
			new_effect( 0, BLOOD, x + 15, y + 15, a*22.5, 4 );
			for ( a = 0; a < 3; a ++  ) 
			new_body_part( x + 4, y + 4, rand()  % 360, spd + ( rand()  % 2 ), ( a + alku )  % 7 );
			MIDASplaySample( samplep[DEATHWAV], MIDAS_CHANNEL_AUTO, 0, 20000 - ( rand()  % 3000 ), EFFECT_VOLUME, pan );
		}
		if (enemy_info[type].explo_death)
		{
        explo( x+4, y+4, 2, -1);
		for ( a = 0; a < 3; a ++  ) 
		new_body_part( x + 4, y + 4, rand()  % 360, 5, a + 12 );
		}

		DEAD = 1;
		DEAD_ENEMIES ++;
		if ( KILLING_MODE!= DEATHMATCH ) if ( DEAD_ENEMIES >= ENEMIES ) next_level = 1;
		if ( P!= NULL ) 
		P-> kills[type] ++;
	}
}

void Player::get_damage( float damage, Player *P ) 
{   char buf[MAX_MESSAGE_LENGTH];
	int a,b;
	HIT = 15;
	energy -= damage;
	if ( energy <= 0 ) 
	{

	  if (KILLING_MODE==DEATHMATCH)
	  {
	    if (P->tindex!=tindex)		
		 sprintf(buf,killtexts[rand()%ktexts],P->name,name);
		else 
		 sprintf(buf,suicidetexts[rand()%stexts],P->name,name);
		message_board.add_message(buf);
	  }

		for (a=0;a<MAX_PLAYERS;a++)
		 if (a!=tindex)
		  if (player[a].enabled)
		   sendipx(a,IPX_MESSAGE,buf);	 
		
		if ( burning ) 
		{
			MIDASplaySample( samplep[WOOSHWAV], MIDAS_CHANNEL_AUTO, 0, 20000, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
			new_effect( &explo_l, EXPLO, x + 12, y + 12, 0, 0 );
			new_effect( &explo_l, EXPLO, x + 18, y + 12, 0, 0 );
			new_effect( &explo_l, EXPLO, x + 15, y + 18, 0, 0 );

		}

		else 
		{
			for ( a = 0; a < 16; a ++  ) 
			if ( rand()  % 2 ) 
			new_effect( 0, BLOOD, x + 15, y + 15, a*22.5, 4 );
			MIDASplaySample( samplep[DEATHWAV], MIDAS_CHANNEL_AUTO, 0, 20000 - ( rand()  % 3000 ), EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
		}
		DEAD = 1;
		if ( P!= NULL ) 
		{
			if ( P!= this ) 
				P->player_kills++;
			else player_kills --;

			if (GAME_MODE==NETWORK)
			{
			if (NETWORK_MODE==CLIENT) sendipx(0,IPX_UPDKILLS,(char)aplayer[0]->tindex,P->tindex);
			else 
				 for(b=1;b<MAX_PLAYERS;b++)
			      if (player[b].enabled)
				   sendipx(b,IPX_UPDKILLS,0,P->tindex);
			}

		}
	}
}

void Player::see() 
{
	int offs, a, b, ang, stop;
	int tx, ty, mx, my, ox, oy, nx, ny;
	int range = 12, korona = 2*pi*range + 5;
	tx = ( x + 15 )  / 20;
	ty = ( y + 15 )  / 20;
	for ( a = 0; a < korona; a ++  ) 
	{
		ang = ( int ) ( a*( 360 / ( float ) korona )  )  % 360;
		stop = 0;
		nx = 0;
		ny = 0;
		for ( b = 0; b < range &&stop == 0; b ++  ) 
		{
			ox = nx;
			oy = ny;
			nx = ( int ) ( b*sini[ang] );
			ny = ( int ) ( b*cosi[ang] );
			mx = nx - ox;
			my = ny - oy;
			offs = ( ( ty + oy ) *level_x_size )  + ox + tx;
			if ( abs( mx )  == 1 &&abs( my )  == 1 ) 
			if ( level[offs + mx].type!= FLOOR &&level[offs + ( my*level_x_size ) ].type!= FLOOR ) 
			stop = 1;
			offs = ( ( ty + ny ) *level_x_size )  + nx + tx;
			if ( level[offs].type!= FLOOR ) stop = 1;
			if ( !stop ) see_level[offs] = 1;
		}
	}
}

void Bullet::move() 
{
	int bp, speed, vx, vy, a, b, rx = x, ry = y, hit = 0, kosh = 0;
	int looptimes;
	
	speed = type-> speed;
	looptimes = speed >> 2;
	if ( speed == 0 ) looptimes = 1;
	time --;
	count ++;
	for( a = 0; a < looptimes &&hit == 0 &&kosh == 0 ; a ++  ) 
	{
		if ( speed > 0 ) 
		{
			x += ( float ) speed / ( speed >> 2 ) *sini[angle];
			y += ( float ) speed / ( speed >> 2 ) *cosi[angle];
		}
		rx = x;
		ry = y;
		if ( type-> speed > 0 ) 
		if ( level[( int ) ( ry / 20 ) *level_x_size + ( int ) ( rx / 20 ) ].type!= FLOOR ) 
		{
			kosh = 1;
		}
		if ( count > type-> sleep_time ) 
		if ( type-> stop ) 
		{
			if (!(GAME_MODE == NETWORK && NETWORK_MODE == CLIENT))
			for ( b = 0; b < ENEMIES; b ++  ) // chk for enemy
			if ( !enemy[b].DEAD ) 
			if ( rx > enemy[b].x + 5 ) 
			if ( rx < enemy[b].x + 22 ) 
			if ( ry > enemy[b].y + 5 ) 
			if ( ry < enemy[b].y + 22 ) 
			{
				hit = 1;
				if ( P!= NULL ) 
				enemy[b].tangle = enemy[b].angle_pl[index];// äijäkääntyy kohti, kun sitäampuu
				vx = enemy[b].x + 14 + ( 4*sini[angle] );
				vy = enemy[b].y + 14 + ( 4*cosi[angle] );
				enemy[b].PUSH_ANGLE = angle;// äijälentää
				if ( type-> push_power > enemy[b].PUSH_POWER ) 
				enemy[b].PUSH_POWER = ( float ) type-> push_power;// äijälentää
				enemy[b].get_damage( type-> power, P );
				if ( type-> type == INCENDIARY ) 
				{
					enemy[b].burning += 10;
					enemy[b].Frying_player = P;
				}
			}

			for ( b = 0; b < MAX_PLAYERS; b ++  ) 
			if ( player[b].enabled ) 
			if ( !player[b].DEAD ) 
			if ( rx > player[b].x + 5 ) 
			if ( rx < player[b].x + 22 ) 
			if ( ry > player[b].y + 5 ) 
			if ( ry < player[b].y + 22 ) 
			{
			    hit = 1;
				vx = player[b].x + 14 + ( 4*sini[angle] );
				vy = player[b].y + 14 + ( 4*cosi[angle] );
				if ( (GAME_MODE!=NETWORK) || (aplayer[0]==&player[b]))
			   {
				player[b].PUSH_ANGLE = angle;// äijälentää
				if ( type-> push_power > player[b].PUSH_POWER ) 
				player[b].PUSH_POWER = ( float ) type-> push_power;// äijälentää
				player[b].get_damage( type-> power, P );
				if ( type-> type == INCENDIARY ) 
				{
					player[b].burning += 10;
					player[b].Frying_player = P;
				}
			   }
			}
		}
	}
	if ( type-> type == NORMAL ) 
	if ( kosh ) if ( rand()  % 8 == 1 ) new_effect( &normal_l, METAL, rx, ry, rand()  % 360, ( rand()  % 3 )  + 5 );

	if ( hit ) 
	{
	if ( type-> type == NORMAL ) 
		{

			bp = 6 + ( type-> power / 3 )  + rand()  % 3;
			if ( bp > 8 ) bp = 8;
			for ( a = 0; a < ( type-> power / 5 )  + 1; a ++  ) 
			   new_effect( 0, BLOOD, vx + ( rand()  % 6 )  - 3, vy + ( rand()  % 6 )  - 3, angle + ( ( rand()  % 30 )  - 15 ), bp );
		}
		if ( P!=NULL ) P->hitten++;
		if ( type-> speed < 10 &&type-> time == 1 ) // if fist
		 {
		  if (GAME_MODE==SPLIT_SCREEN)		  
		  MIDASplaySample( samplep[weapon[0].sound], MIDAS_CHANNEL_AUTO, 0, sample_rates[0], EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
		  else
		  MIDASplaySample( samplep[weapon[0].sound], MIDAS_CHANNEL_AUTO, 0, sample_rates[0], get_volume(aplayer[0]->x,aplayer[0]->y,rx,ry),get_pan(aplayer[0]->x,rx) );
		 }
	}

	
	if ( type-> type == NORMAL ) 
	if ( !hit ) 
	if ( time == 0 ||kosh ) 
	{
		if ( type-> speed < 10 &&type-> time == 1 ) 
		{
			}
		else new_effect( &normal_l, SMOKE, rx, ry, 0, 0 );
	}
	if ( hit ||kosh ||time == 0 ) 
	{
		if ( type-> type == EXPLOSIVE ) explo( rx - ( 5*sini[angle] ), ry - ( 5*cosi[angle] ), type-> explos, index );
		IN_USE = 0;
	}
	if ( type-> effect!= 0 ) 
	if ( type-> effect == EXPLO ) new_effect( &explo_l, type-> effect, rx, ry, angle + 180, 1 );
	else new_effect( 0, type-> effect, rx, ry, 0, 0 );
}

void Player::shoot() 
{
	int r, a, rx = x, ry = y;
	FIRE = 1;
	load_cnt = 0;
	for ( a = 0; a < weapon[curr_weapon].bullet_amount; a ++  ) 
	{
		shooted ++;
		r = ( weapon[curr_weapon].angle_add >> 1 )  - rand()  % weapon[curr_weapon].angle_add;
		new_bullet( weapon[curr_weapon].btype, rx + 14 + ( 10*sini[rangle2] ), ry + 14 + ( 10*cosi[rangle2] ), rangle2 + r, 0, tindex);
	}
	if ( curr_weapon > 0 ) bullets[weapon[curr_weapon].btype - 1] --;
	if ( curr_weapon > 0 ) 
	if ( shoot_cnt % bullet_type[weapon[curr_weapon].btype].mul == 0 ) 
	{
	if ( weapon[curr_weapon].sound != NULL )
	{ 
		MIDASplaySample( samplep[weapon[curr_weapon].sound], MIDAS_CHANNEL_AUTO, 0, sample_rates[curr_weapon], EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
	  if (GAME_MODE==NETWORK)
	  {
	   if (NETWORK_MODE==CLIENT)
	    sendipx(0,IPX_SOUND,tindex,(char)weapon[curr_weapon].sound,sample_rates[curr_weapon],rx,ry);
	   else
	    for (a=1;a<MAX_PLAYERS;a++)
		 if (player[a].enabled)
	       sendipx(a,IPX_SOUND,0,(char)weapon[curr_weapon].sound,sample_rates[curr_weapon],rx,ry);
	  }
	}
	if ( weapon[curr_weapon].smoke ) 
	new_effect( 0, SMOKE3, rx + 14 + ( 8*sini[rangle2] + 8*sini[( rangle2 + 270 )  % 360] ), ry + 14 + ( 8*cosi[rangle2] + 8*cosi[( rangle2 + 270 )  % 360] ), rangle2, 1 );
	}
}

void Enemy::shoot() 
{
	int r, a, rx = x, ry = y, weaponi = enemy_info[type].curr_weapon;
	int pan = 0;
	FIRE = 1;
	load_cnt = 0;
	for ( a = 0; a < weapon[weaponi].bullet_amount; a ++  ) 
	{
		r = ( weapon[weaponi].angle_add / 2 )  - rand()  % weapon[weaponi].angle_add;
		new_bullet( weapon[weaponi].btype, rx + 14 + ( 10*sini[angle] ), ry + 14 + ( 10*cosi[angle] ), angle + r, 0, -1);
	}
	if ( GAME_MODE!= SPLIT_SCREEN ) 
	{
		// stereo effect
		pan = ( float ) (  - ( player[0].x - x )  )  / 2;
		if ( pan <  - 63 ) pan =  - 63;
		if ( pan > 63 ) pan = 63;
	}
	if ( weaponi > 0 ) 
	if ( shoot_cnt % bullet_type[weapon[weaponi].btype].mul == 0 ) 
	MIDASplaySample( samplep[weapon[weaponi].sound], MIDAS_CHANNEL_AUTO, 0, sample_rates[weaponi], EFFECT_VOLUME, pan );
	if ( shoot_cnt % bullet_type[weapon[weaponi].btype].mul == 0 ) 
	if ( weapon[weaponi].smoke ) 
	new_effect( 0, SMOKE3, rx + 14 + ( 8*sini[( int ) ( rangle*9 ) ] + 5*sini[( ( int ) ( rangle*9 )  + 270 )  % 360] ), ry + 14 + ( 8*cosi[( int ) ( rangle*9 ) ] + 5*cosi[( ( int ) ( rangle*9 )  + 270 )  % 360] ), angle, 1 );
}

void Enemy::chk_for_shoot() 
{
	int dist, a, wannashoot = 0;
	if ( enemy_info[type].curr_weapon > 0 ) 
	for ( a = 0; a < MAX_PLAYERS; a ++  ) 
	if ( player[a].enabled)
	if ( SEE_PL[a] ) 
	if ( tangle == angle ) 
	{
		shoot_cnt ++;
		wannashoot = 1;
		if ( load_cnt == weapon[enemy_info[type].curr_weapon].loading_time ) 
		{
			shoot();
			dist = get_dist( x, y, player[a].x, player[a].y ); // chk if close enough
			if ( dist > bullet_type[weapon[enemy_info[type].curr_weapon].btype].time*bullet_type[weapon[enemy_info[type].curr_weapon].btype].speed ) 
			walk_cnt = 20 / enemy_info[type].speed;
		}
	}
	if ( !wannashoot ) shoot_cnt = 0;
 }

void Enemy::rotate() 
{
	 // Nerokas kääntymis funktio !!!
	int kaanty_jo = 0;
	if ( angle!= tangle ) 
	{
		if ( angle < 180 ) 
		{
			if ( tangle < angle + 180 &&tangle > angle ) angle += ROT_SPEED; else angle -= ROT_SPEED;
			kaanty_jo = 1;
		}
		if ( angle >= 180 &&kaanty_jo == 0 ) 
		if ( tangle > angle - 180 &&tangle < angle ) angle -= ROT_SPEED; else angle += ROT_SPEED;
		angle%= 360;
		if ( angle < 0 ) angle += 360;
 // 		vähän pyöristystä, niin angle saavuttaa joskus tanglen ...
		if ( abs( angle - tangle )  < ROT_SPEED ||abs( ( angle + 360 )  - tangle )  < ROT_SPEED ) angle = tangle;
	}
}

void Level::load( char name[13] ) 
{
	int a, bullets, weapons;
	FILE *dat;
	dat = fopen( name, "rb" );
	if ( dat != NULL ) 
	{
		fread( &version, 4, 1, dat );
		if (version>5) {
                    // TODO VIDEO vesa_set_mode(0x3);
                    std::cout<<"Invalid level version: Get newer version of TK !";
                    exit(1);
                }
		fread( &level_x_size, 4, 1, dat );
		fread( &level_y_size, 4, 1, dat );
		level = ( struct BLOCK* ) malloc( level_y_size*level_x_size*sizeof( struct BLOCK )  );
		fread( level, sizeof( struct BLOCK ) *level_x_size*level_y_size, 1, dat );
		fread( &pl_start_x[0], 4, 1, dat );
		fread( &pl_start_y[0], 4, 1, dat );
		fread( &pl_start_x[1], 4, 1, dat );
		fread( &pl_start_y[1], 4, 1, dat );
		fread( &Spot_amount, 4, 1, dat );
		for ( a = 0; a < Spot_amount; a ++  ) 
		{
			fread( &spot_light[a].x, 4, 1, dat );
			fread( &spot_light[a].y, 4, 1, dat );
			fread( &spot_light[a].size, 4, 1, dat );
		}
		fread( &Steam_amount, 4, 1, dat );
		for ( a = 0; a < Steam_amount; a ++  ) 
		{
			fread( &steam[a].x, 4, 1, dat );
			fread( &steam[a].y, 4, 1, dat );
			fread( &steam[a].angle, 4, 1, dat );
			fread( &steam[a].speed, 4, 1, dat );
		}

		fread(general_level_info.comment,20, 1, dat);
		fread(&general_level_info.time_limit,4, 1, dat);
		memset(general_level_info.enemies,0,4*DIFF_ENEMIES);
		if (version>=4)
		 fread(general_level_info.enemies,4*DIFF_ENEMIES, 1, dat);
		if (version<4)
		 fread(general_level_info.enemies,4*(DIFF_ENEMIES-1), 1, dat);

		if ( version == 1 ){
			weapons = DIFF_WEAPONS - 2; bullets = DIFF_BULLETS - 2;
		}
		if ( version == 2 ){
			weapons = DIFF_WEAPONS - 1; bullets = DIFF_BULLETS - 1;
		}
		if ( version == 3 || version >= 4){
			weapons = DIFF_WEAPONS; bullets = DIFF_BULLETS;
		}
		fread( normal_game_level_info.weapon_crates, 4 * weapons, 1, dat );
		fread( normal_game_level_info.bullet_crates, 4 * bullets, 1, dat );
		fread( &normal_game_level_info.energy_crates, 4, 1, dat );
		fread( death_match_level_info.weapon_crates, 4 * weapons, 1, dat );
		fread( death_match_level_info.bullet_crates, 4 * bullets, 1, dat );
		fread( &death_match_level_info.energy_crates, 4, 1, dat );

		if (version>=5){
		 fread( &normal_crate_amount, 4, 1, dat );
		 fread( normal_crate_info, sizeof(Crate_info) * normal_crate_amount, 1, dat );
		 fread( &deathmatch_crate_amount, 4, 1, dat );
		 fread( deathmatch_crate_info, sizeof(Crate_info) * deathmatch_crate_amount, 1, dat );
		}
		fclose( dat );
	}
	else std::cout << "Level file missing ! \n";
}

void Keys::change() 
{
	int a = 0, quit = 0, cnt = 0, oclock = 0, selected = 0, b = 0;
	first = 1;
	fadeout( virbuff, pal );
	load_efp( "efps\\cool.efp", picture, 0 );
	draw_box1( 10, 10, 310, 32, 10 );
	draw_box1( 10, 40, 310, 190, 10 );
	while( !quit ) 
	{
		memcpy( virbuff, picture, 64000 );
		FONT_NUM = 0;
		writefonts2( 160 - ( str_length( "define keys" )  / 2 ), 13, "define keys", 1 );
		for ( a = 0; a < DIFF_K; a ++  ) 
		{
			if ( a == selected ) 
			{
				FONT_NUM = 1;
				writefonts( 40, 50 + ( a*8 ), k_text[a], M_YELLOW );
			}
			else
			{
				FONT_NUM = 1;
				writefonts( 40, 50 + ( a*8 ), k_text[a], M_RED );
			}
		}
		for ( a = 0; a < DIFF_K - 1; a ++  ) 
		{
			switch ( a ) 
			{
				case 0:b = K_UP;break;
				case 1:b = K_DOWN;break;
				case 2:b = K_LEFT;break;
				case 3:b = K_RIGHT;break;
				case 4:b = K_SHOOT;break;
				case 5:b = K_SHIFT;break;
				case 6:b = K_STRAFE;break;
				case 7:b = K_LSTRAFE;break;
				case 8:b = K_RSTRAFE;break;
			}
			if ( selected == a ) 
			{
				FONT_NUM = 1;
				writefonts( 190, 50 + ( a*8 ), k_2_c( b ), M_YELLOW );
			}
			else 
			{
				FONT_NUM = 1;
				writefonts( 190, 50 + ( a*8 ), k_2_c( b ), M_RED );
			}
		}
		if ( first == 1 ) 
		{
			fadein( virbuff, pal ); first = 0;
		}
		memcpy( screen, virbuff, 64000 );
		while ( !k.state[1]&&!k.state[94]&&!k.state[99]&&!k.state[28] ) 
		if ( clock() != oclock ) 
		{
			oclock = clock();
			draw_ball( 20, 45 + ( selected*8 ), cnt );
			cnt ++;
			if ( cnt > 23 ) cnt = 0;
		}
		MIDASplaySample( samplep[KLIKWAV], MIDAS_CHANNEL_AUTO, 0, 22500, EFFECT_VOLUME, MIDAS_PAN_MIDDLE );
		if ( k.state[94] ) 
		{
			selected --;if ( selected < 0 ) selected = DIFF_K - 1; k.state[94] = 0;
		}
		if ( k.state[99] ) 
		{
			selected ++;if ( selected >= DIFF_K ) selected = 0; k.state[99] = 0;
		}
		if ( k.state[28] ) 
		{
			k.state[28] = 0;
			k.clear_stack();
			if ( selected == 0 ) 
			{
				clear_shit( 47 ); K_UP = get_key( 160, 45 );
			}
			if ( selected == 1 ) 
			{
				clear_shit( 47 + 8 ); K_DOWN = get_key( 160, 45 + 8 );
			}
			if ( selected == 2 ) 
			{
				clear_shit( 47 + ( 2*8 )  ); K_LEFT = get_key( 160, 45 + ( 2*8 )  );
			}
			if ( selected == 3 ) 
			{
				clear_shit( 47 + ( 3*8 )  ); K_RIGHT = get_key( 160, 45 + ( 3*8 )  );
			}
			if ( selected == 4 ) 
			{
				clear_shit( 47 + ( 4*8 )  ); K_SHOOT = get_key( 160, 45 + ( 4*8 )  );
			}
			if ( selected == 5 ) 
			{
				clear_shit( 47 + ( 5*8 )  ); K_SHIFT = get_key( 160, 45 + ( 5*8 )  );
			}
			if ( selected == 6 ) 
			{
				clear_shit( 47 + ( 6*8 )  ); K_STRAFE = get_key( 160, 45 + ( 6*8 )  );
			}
			if ( selected == 7 ) 
			{
				clear_shit( 47 + ( 7*8 )  ); K_LSTRAFE = get_key( 160, 45 + ( 7*8 )  );
			}
			if ( selected == 8 ) 
			{
				clear_shit( 47 + ( 8*8 )  ); K_RSTRAFE = get_key( 160, 45 + ( 8*8 )  );
			}
			if ( selected == 9 ) 
			{
				quit = 1;
			}
		}
		if ( k.state[1] ) 
		{
			quit = 1; k.state[1] = 0;
		}
	}
	fadeout( virbuff, pal );
}


#include "defines.h"
#include "globvar.h"
#include "types.h"
#include "classes.h"

extern void sendipx(int, int, ... );

void new_body_part( int x, int y, int angle, int speed, int num ) 
{
	int a = 0;
	if ( angle >= 360 ) angle -= 360;
	if ( angle < 0 ) angle += 360;
	while ( body_part[a].IN_USE &&a < MAX_BODY_PARTS ) if ( body_part[a].IN_USE ) a ++;
	if ( a < MAX_BODY_PARTS ) 
	{
		body_part[a].num = num;
		body_part[a].x = x;
		body_part[a].y = y;
		body_part[a].angle = angle;
		body_part[a].speed = speed;
		body_part[a].IN_USE = 1;
	}
}

void new_effect( Light_table *lt, int type, int x, int y, int angle, int speed ) 
{
	int min, a = 0, num = 0;

	if ( angle >= 360 ) angle -= 360;
	if ( angle < 0 ) angle += 360;

	effect_age_count ++; // can be large after 10 hour session : ) 


	while ( effect[a].IN_USE &&a < MAX_EFFECTS ) if ( effect[a].IN_USE ) a++;
	if ( a < MAX_EFFECTS ) num = a;
	else 
	{
		min = 0;
		for ( a = 1; a < MAX_EFFECTS; a ++  ) if ( effect[a].age < effect[min].age ) 
		min = a;
		num = min;
	}
	effect[num].age = effect_age_count;
	effect[num].lt = lt;
	effect[num].type = &effect_type[type];
	effect[num].x = x;
	effect[num].y = y;
	effect[num].count = 0;
	effect[num].angle = angle;
	effect[num].speed = speed;
	effect[num].IN_USE = 1;


}

void new_bullet( int type, int x, int y, int angle, int time_add, int index) 
{
	int a = 0,b;

	if ( angle >= 360 ) angle -= 360;
	if ( angle < 0 ) angle += 360;
	while ( bullet[a].IN_USE &&a < MAX_BULLETS ) a ++;
	if ( a < MAX_BULLETS ) 
	{
		bullet[a].type = &bullet_type[type];
		bullet[a].x = x;
		bullet[a].y = y;
		if (index>-1)
		 bullet[a].P = &player[index];
	  	  else
		   bullet[a].P =0;
		bullet[a].index = index;
		bullet[a].angle = angle;
		bullet[a].time = bullet_type[type].time + time_add;
		bullet[a].count = 0;
		bullet[a].IN_USE = 1;
	
	if (GAME_MODE==NETWORK)
	if (type != EXPLO_RAY)
	{
	if (NETWORK_MODE==CLIENT) sendipx(0,IPX_NEWBULLET,(char)index,(int)type,(int)a);
		else 
		 for(b=1;b<MAX_PLAYERS;b++)
	      if (player[b].enabled)
		   sendipx(b,IPX_NEWBULLET,(char)0,(int)type,(int)a);
	}
  }

}


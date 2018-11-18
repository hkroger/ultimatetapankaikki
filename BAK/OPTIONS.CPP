#include <stdlib.h>
#include <stdio.h>
#include <gfunc.h>
#include <string.h>
#include <dos.h>

#include "efp/efp.h"
#include "defines.h"
#include "types.h"
#include "globvar.h"
#include "miscfunc.h"
#include "fade.h"
#include "draw.h"
#include "write.h"

extern void setup_ipx();
extern void desetup_ipx();
extern void game();

void save_options(){
FILE *cfg;

cfg=fopen("options.cfg","wb");

fwrite(&keys1,sizeof(class Keys),1,cfg);

fwrite(&keys2,sizeof(class Keys),1,cfg);

fwrite(name1,sizeof(name1),1,cfg);
fwrite(name2,sizeof(name2),1,cfg);

fwrite(&DARK_MODE,sizeof(DARK_MODE),1,cfg);
fwrite(&LIGHT_EFFECTS,sizeof(LIGHT_EFFECTS),1,cfg);
fwrite(&SHADOWS,sizeof(SHADOWS),1,cfg);

fwrite(&MUSIC_VOLUME,sizeof(MUSIC_VOLUME),1,cfg);
fwrite(&EFFECT_VOLUME,sizeof(EFFECT_VOLUME),1,cfg);

fwrite(&ENEMIES_ON_GAME,sizeof(ENEMIES_ON_GAME),1,cfg);
fwrite(DEATH_MATCH_LEVEL,sizeof(DEATH_MATCH_LEVEL),1,cfg);
fwrite(&DEATH_MATCH_EPISODE,sizeof(DEATH_MATCH_EPISODE),1,cfg);
fwrite(&DEATH_MATCH_SPEED,sizeof(DEATH_MATCH_SPEED),1,cfg);

fwrite(&KILLING_MODE,sizeof(KILLING_MODE),1,cfg);

fclose(cfg);
}

void load_options(){
FILE *cfg;

cfg=fopen("options.cfg","rb");

if (cfg){

fread(&keys1,sizeof(class Keys),1,cfg);

fread(&keys2,sizeof(class Keys),1,cfg);

fread(name1,sizeof(name1),1,cfg);
fread(name2,sizeof(name2),1,cfg);

fread(&DARK_MODE,sizeof(DARK_MODE),1,cfg);
fread(&LIGHT_EFFECTS,sizeof(LIGHT_EFFECTS),1,cfg);
fread(&SHADOWS,sizeof(SHADOWS),1,cfg);

fread(&MUSIC_VOLUME,sizeof(MUSIC_VOLUME),1,cfg);
fread(&EFFECT_VOLUME,sizeof(EFFECT_VOLUME),1,cfg);
 
fread(&ENEMIES_ON_GAME,sizeof(ENEMIES_ON_GAME),1,cfg);
fread(DEATH_MATCH_LEVEL,sizeof(DEATH_MATCH_LEVEL),1,cfg);
fread(&DEATH_MATCH_EPISODE,sizeof(DEATH_MATCH_EPISODE),1,cfg);
fread(&DEATH_MATCH_SPEED,sizeof(DEATH_MATCH_SPEED),1,cfg);

fread(&KILLING_MODE,sizeof(KILLING_MODE),1,cfg);

fclose(cfg);

}

}

int choose_load_file(char *name,int episode){

int a,b,sel=0,pos=0,cnt=0,oclock=0;
int files=files_in_episode[episode];

load_efp("efps\\cool.efp",picture,0);

draw_box1(60,10,320-60,190,15);

first=1;

while (! k.state[1] && ! k.state[28]){

memcpy(virbuff,picture,64000);

FONT_NUM=0;

writefonts2(160-(str_length("select level:")>>1),20,"select level:",1);

b=files;
if (b>15) b=15;

FONT_NUM=1;

for (a=0;a<b;a++)
 if (a+pos==sel)
 writefonts(160-(str_length(strlwr(filelist[episode][a+pos].name))>>1),50+a*8,strlwr(filelist[episode][a+pos].name),M_YELLOW);
  else 
 writefonts(160-(str_length(strlwr(filelist[episode][a+pos].name))>>1),50+a*8,strlwr(filelist[episode][a+pos].name),M_BLUE);

 if (pos>0) writefonts(156,42,"",M_BLUE);
 if (files>15) if (pos<files-15) writefonts(156,50+15*8,"",M_BLUE);

if (first==1) {fadein(virbuff,pal);first=0;}
memcpy(screen,virbuff,64000);

while(! k.state[94] && ! k.state[99] && ! k.state[28])

	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(65,46+((sel-pos)*8),cnt);
	   draw_ball(320-85,46+((sel-pos)*8),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }

 if (k.state[94]) {if (sel>0) sel--; k.state[94]=0; }
 if (k.state[99]) {if (sel<files-1) sel++; k.state[99]=0; }

 pos=sel-8; 
 if (pos<0)	pos=0;
 if (files>15) if (pos>files-15) pos=files-15;
 if (files<=15) if (pos>0)	pos=0;

}

a=0;

strcpy(name,filelist[episode][sel].name);a=1;k.state[28]=0;

return(a);
}

int choose_episode(){

int a,b,sel=0,pos=0,cnt=0,oclock=0;
char text[20];

load_efp("efps\\cool.efp",picture,0);
draw_box1(60,10,320-60,190,15);
first=1;

k.state[1]=0;
k.state[28]=0;

fadeout(virbuff,pal);

while (! k.state[1] && ! k.state[28]){

memcpy(virbuff,picture,64000);

FONT_NUM=0;

writefonts2(160-(str_length("select episode")>>1),20,"select episode",1);

b=dirs;
if (b>7) b=7;

for (a=0;a<b;a++){
 if (a+pos==sel){
 FONT_NUM=0;
 writefonts2(160-(str_length(strlwr(dirlist[a+pos].name))>>1),50+a*16,strlwr(dirlist[a+pos].name),1);
} else {
 FONT_NUM=2;
 writefonts2(160-(str_length(strlwr(dirlist[a+pos].name))>>1),50+a*16,strlwr(dirlist[a+pos].name),1);
}
}

FONT_NUM=1;
 itoa(files_in_episode[sel+1],text,10);
 strcat(text," levels");
 writefonts(70,175,text,M_BLUE);

if (first==1) {fadein(virbuff,pal);first=0;}
memcpy(screen,virbuff,64000);

while(! k.state[94] && ! k.state[99] && ! k.state[28])

	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(65,48+((sel-pos)*16),cnt);
	   draw_ball(320-85,48+((sel-pos)*16),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }

 if (k.state[94]) {if (sel>0) sel--; k.state[94]=0; }
 if (k.state[99]) {if (sel<dirs-1) sel++; k.state[99]=0; }

 pos=sel-3; 
 if (pos<0)	pos=0;
 if (dirs>7) if (pos>dirs-7) pos=dirs-7;
 if (dirs<=7) if (pos>0)	pos=0;

}

k.state[28]=0;

return(sel);
}


void options_scr(int opt,char *filename){

load_efp(filename,picture,0);
load_efp_pal("efps\\ware.efp",pal);
draw_box1(45,90-((opt*15)/2),320-45,110+((opt*15)/2),13);

first=1;
}

void multip_options_scr(int opt,char *filename){

load_efp(filename,picture,0);
load_efp_pal("efps\\ware.efp",pal);
draw_box1(10,90-((opt*15)/2),320-10,110+((opt*15)/2),13);

first=1;
}

void player_options(){
int yoffs=(15*num_player_options)/2,a,cnt=0,selected=0,quit=0,oclock;

fadeout(virbuff,pal);

options_scr(num_player_options,"efps\\cool.efp");

k.state[28]=0;

while(! quit){
memcpy(virbuff,picture,64000);

for (a=0;a<num_player_options;a++){ 
 if (a==selected) {
 FONT_NUM=0;  
 writefonts2(160-(str_length(player_opt_text[a])/2),(100-yoffs)+(a*15),player_opt_text[a],1);
 } else
 {
 FONT_NUM=2;
 writefonts2(160-(str_length(player_opt_text[a])/2),(100-yoffs)+(a*15),player_opt_text[a],M_RED);
 }

}

if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,64000);

while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[28])  { 
	   
	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(65,(98-yoffs)+(selected*15),cnt);
	   draw_ball(320-85,(98-yoffs)+(selected*15),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }
       
       };
 
 MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);

 if (k.state[94]) {selected--;if (selected<0) selected=num_player_options-1;k.state[94]=0;}
 if (k.state[99]) {selected++;if (selected>=num_player_options) selected=0;k.state[99]=0;}
 if (k.state[28]) {
             if (selected==0) {keys1.change();options_scr(num_player_options,"efps\\cool.efp");}
             if (selected==1) {keys2.change();options_scr(num_player_options,"efps\\cool.efp");}
             if (selected==2) {change_name(1);options_scr(num_player_options,"efps\\cool.efp");}
             if (selected==3) {change_name(2);options_scr(num_player_options,"efps\\cool.efp");}
             if (selected==4) {quit=1;}
            }
 if (k.state[1]) {quit=1; k.state[1]=0;}
}

fadeout(virbuff,pal);

}

void game_options(){

int yoffs=(15*num_game_options)/2,a,cnt=0,selected=0,quit=0,oclock;

fadeout(virbuff,pal);
options_scr(num_game_options,"efps\\cool.efp");
k.state[28]=0;

while(! quit){
memcpy(virbuff,picture,64000);

if (DARK_MODE) game_opt_text[0]=strdup("darkness on");
if (! DARK_MODE) game_opt_text[0]=strdup("darkness off");
if (LIGHT_EFFECTS) game_opt_text[1]=strdup("lights on");
if (! LIGHT_EFFECTS) game_opt_text[1]=strdup("lights off");
if (SHADOWS) game_opt_text[2]=strdup("shadows on");
if (! SHADOWS) game_opt_text[2]=strdup("shadows off");

for (a=0;a<num_game_options;a++){ 
 if (a==selected) {
 FONT_NUM=0;  
 writefonts2(160-(str_length(game_opt_text[a])/2),(100-yoffs)+(a*15),game_opt_text[a],1);
 } else
 {
 FONT_NUM=2;
 writefonts2(160-(str_length(game_opt_text[a])/2),(100-yoffs)+(a*15),game_opt_text[a],M_RED);
 }

}

if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,64000);

while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[28])  { 
	   
	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(65,(98-yoffs)+(selected*15),cnt);
	   draw_ball(320-85,(98-yoffs)+(selected*15),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }
       
       };
 
 MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);

 if (k.state[94]) {selected--;if (selected<0) selected=num_game_options-1;k.state[94]=0;}
 if (k.state[99]) {selected++;if (selected>=num_game_options) selected=0;k.state[99]=0;}
 if (k.state[28]) {
             if (selected==0) {if (! DARK_MODE) DARK_MODE=1; else DARK_MODE=0; k.state[28]=0;}
             if (selected==1) {if (! LIGHT_EFFECTS) LIGHT_EFFECTS=1; else LIGHT_EFFECTS=0; k.state[28]=0;}
             if (selected==2) {if (! SHADOWS) SHADOWS=1; else SHADOWS=0; k.state[28]=0;}
             if (selected==3) {quit=1;}
            }
 if (k.state[1]) {quit=1; k.state[1]=0;}
}

fadeout(virbuff,pal);
}

void sound_options(){

int yoffs=(15*num_sound_options)/2,a,cnt=0,selected=0,quit=0,oclock;
char text[30],text2[30];

fadeout(virbuff,pal);

options_scr(num_sound_options,"efps\\subbari.efp");

k.state[28]=0;

while(! quit){
memcpy(virbuff,picture,64000);

strcpy(text,"music volume ");
itoa((float)MUSIC_VOLUME/63*100,text2,10);
strcat(text2,"%");
strcat(text,text2);
sound_opt_text[0]=strdup(text);

strcpy(text,"effect volume ");
itoa((float)EFFECT_VOLUME/63*100,text2,10);
strcat(text2,"%");
strcat(text,text2);
sound_opt_text[1]=strdup(text);

for (a=0;a<num_sound_options;a++){ 
 if (a==selected) {
 FONT_NUM=0;  
 writefonts2(160-(str_length(sound_opt_text[a])/2),(100-yoffs)+(a*15),sound_opt_text[a],1);
 } else
 {
 FONT_NUM=2;
 writefonts2(160-(str_length(sound_opt_text[a])/2),(100-yoffs)+(a*15),sound_opt_text[a],M_RED);
 }

}

if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,64000);

while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[96] && ! k.state[97] && ! k.state[28])  { 
	   
	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(55,(98-yoffs)+(selected*15),cnt);
	   draw_ball(320-75,(98-yoffs)+(selected*15),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }
       
       }
 
 MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);

 if (k.state[94]) {selected--;if (selected<0) selected=num_sound_options-1;k.state[94]=0;}
 if (k.state[99]) {selected++;if (selected>=num_sound_options) selected=0;k.state[99]=0;}
 if (k.state[96]) {
              if (selected==0) MUSIC_VOLUME-=5;
 			  if (selected==1) EFFECT_VOLUME-=5;
			  if (MUSIC_VOLUME<0) MUSIC_VOLUME=0;
			  if (EFFECT_VOLUME<0) EFFECT_VOLUME=0;
 				if (selected==0) MIDASsetMusicVolume(ph,MUSIC_VOLUME);
 				k.state[96]=0;
 				}
 if (k.state[97]) {
              if (selected==0) MUSIC_VOLUME+=5;
 			  if (selected==1) EFFECT_VOLUME+=5;
			  if (MUSIC_VOLUME>63) MUSIC_VOLUME=63;
			  if (EFFECT_VOLUME>63) EFFECT_VOLUME=63;
 				if (selected==0) MIDASsetMusicVolume(ph,MUSIC_VOLUME);
 				k.state[97]=0;
 				}
 if (k.state[28]) if (selected==2) {quit=1;}
 if (k.state[1]) {quit=1; k.state[1]=0;}
}

fadeout(virbuff,pal);
}


void deathmatch_options(){

int yoffs=(15*num_deathmatch_options)/2,a,cnt=0,selected=0,quit=0,oclock;

fadeout(virbuff,pal);

options_scr(num_deathmatch_options,"efps\\cool.efp");

k.state[28]=0;

while(! quit){
memcpy(virbuff,picture,64000);

if (DEATH_MATCH_SPEED==2)
deathmatch_opt_text[0]=strdup("normal moving");
else 
deathmatch_opt_text[0]=strdup("fast moving");

if (ENEMIES_ON_GAME)
deathmatch_opt_text[1]=strdup("enemies on");
else 
deathmatch_opt_text[1]=strdup("enemies off");

for (a=0;a<num_deathmatch_options;a++){ 
 if (a==selected) {
 FONT_NUM=0;  
 writefonts2(160-(str_length(deathmatch_opt_text[a])/2),(100-yoffs)+(a*15),deathmatch_opt_text[a],1);
 } else
 {
 FONT_NUM=2;
 writefonts2(160-(str_length(deathmatch_opt_text[a])/2),(100-yoffs)+(a*15),deathmatch_opt_text[a],1);
 }

}

if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,64000);

while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[28])  { 
	   
	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(50,(98-yoffs)+(selected*15),cnt);
	   draw_ball(320-70,(98-yoffs)+(selected*15),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }
       
       };
 
 MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);

 if (k.state[94]) {selected--;if (selected<0) selected=num_deathmatch_options-1;k.state[94]=0;}
 if (k.state[99]) {selected++;if (selected>=num_deathmatch_options) selected=0;k.state[99]=0;}
 if (k.state[28]) {
             k.state[28]=0;
             if (selected==0) if (DEATH_MATCH_SPEED==2)	DEATH_MATCH_SPEED=3; else DEATH_MATCH_SPEED=2;
             if (selected==1) if (ENEMIES_ON_GAME) ENEMIES_ON_GAME=0; else ENEMIES_ON_GAME=1;
             if (selected==2) {quit=1;}
            }
 if (k.state[1]) {quit=1; k.state[1]=0;}
}

fadeout(virbuff,pal);
}

void multiplayer_options()
{
	int yoffs=(15*num_multip_options)/2,a,cnt=0,selected=0,quit=0,oclock;
	int dontfade=0;
	char text[50];

	fadeout(virbuff,pal);
	multip_options_scr(num_multip_options,"efps\\cool.efp");

	k.state[28]=0;
	GAME_MODE=1;

	while(! quit)
	{
		memcpy(virbuff,picture,64000);
		if (KILLING_MODE==COOPERATIVE && DEATH_MATCH_EPISODE==0) 
		{
			DEATH_MATCH_EPISODE=1;
			strcpy(DEATH_MATCH_LEVEL,strlwr(filelist[DEATH_MATCH_EPISODE][0].name));
		}
		strcpy(text,"episode: ");
		if (DEATH_MATCH_EPISODE>0) strcat(text,strlwr(dirlist[DEATH_MATCH_EPISODE-1].name));
							  else strcat(text,"dm levels");
		multip_opt_text[2]=strdup(text);
		strcpy(text,"level: ");
		if (KILLING_MODE==DEATHMATCH) 
			strcat(text,DEATH_MATCH_LEVEL);
		multip_opt_text[3]=strdup(text);
		strcpy(text,"game type: ");
 		if (GAME_MODE==SPLIT_SCREEN)
			strcat(text,"split screen");
		if (GAME_MODE==NETWORK)
			strcat(text,"network");
		multip_opt_text[4]=strdup(text);
		strcpy(text,"game mode: ");
		if (KILLING_MODE==COOPERATIVE)
			strcat(text,"cooperative");
		if (KILLING_MODE==DEATHMATCH)
			strcat(text,"deathmatch");
		multip_opt_text[5]=strdup(text);

		for (a=0;a<num_multip_options;a++)
		{ 
			if (a==selected)
			{
				FONT_NUM=0;  
				writefonts2(45,(100-yoffs)+(a*15),multip_opt_text[a],1);
			} else
			{
				FONT_NUM=2;
				writefonts2(45,(100-yoffs)+(a*15),multip_opt_text[a],1);
			}
		}
		if (first==1) {fadein(virbuff,pal);first=0;}
		memcpy(screen,virbuff,64000);
		while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[28])
		{ 
			if (clock()!=oclock)
			{	   
				oclock=clock();
				draw_ball(20,(98-yoffs)+(selected*15),cnt);
				cnt++;
				if (cnt>23) cnt=0;
			}
		};
		MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);
		if (k.state[94]) {selected--;if (selected<0) selected=num_multip_options-1;k.state[94]=0;}
		if (k.state[99]) {selected++;if (selected>=num_multip_options) selected=0;k.state[99]=0;}
		if (k.state[28])
		{
             k.state[28]=0;
             if (selected==0)
             { 
             	NETWORK_MODE=SERVER;
				set_keys();
				if (GAME_MODE==NETWORK) setup_ipx();						
             	game();
				if (GAME_MODE==NETWORK) desetup_ipx();
             	quit=1;
             	dontfade=1;
             }
             if (selected==1)
             {
             	GAME_MODE=NETWORK;
             	NETWORK_MODE=CLIENT;
             	set_keys();
				setup_ipx();
				//  selectgame();
             	game();
				desetup_ipx();
             	quit=1;
             	dontfade=1; 
             }
             if (selected==2) {DEATH_MATCH_EPISODE++;if (DEATH_MATCH_EPISODE>dirs) DEATH_MATCH_EPISODE=0;strcpy(DEATH_MATCH_LEVEL,strlwr(filelist[DEATH_MATCH_EPISODE][0].name));}
             if (selected==3) if (KILLING_MODE==DEATHMATCH) {choose_load_file(DEATH_MATCH_LEVEL,DEATH_MATCH_EPISODE);multip_options_scr(num_multip_options,"efps\\cool.efp"); }
             if (selected==4) {GAME_MODE++;if (GAME_MODE>2) GAME_MODE=1;}
             if (selected==5) {KILLING_MODE++;if (KILLING_MODE>1) KILLING_MODE=0;}
             if (selected==6) {quit=1;}
		}
		if (k.state[1]) {quit=1; k.state[1]=0;}
	}
	if (! dontfade) fadeout(virbuff,pal);
	save_options();
}

void options(){

int yoffs=(15*num_options)/2,a,cnt=0,selected=0,quit=0,oclock;

fadeout(virbuff,pal);

options_scr(num_options,"efps\\cool.efp");

k.state[28]=0;

while(! quit){
memcpy(virbuff,picture,64000);

for (a=0;a<num_options;a++){ 
 if (a==selected) {
 FONT_NUM=0;  
 writefonts2(160-(str_length(opt_text[a])/2),(100-yoffs)+(a*15),opt_text[a],1);
 } else
 {
 FONT_NUM=2;
 writefonts2(160-(str_length(opt_text[a])/2),(100-yoffs)+(a*15),opt_text[a],M_RED);
 }

}

if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,64000);

while (! k.state[1] && ! k.state[94] && ! k.state[99] && ! k.state[28])  { 
	   
	   if (clock()!=oclock){	   
	   oclock=clock();
	   draw_ball(50,(98-yoffs)+(selected*15),cnt);
	   draw_ball(320-70,(98-yoffs)+(selected*15),cnt);
	   cnt++;
	   if (cnt>23) cnt=0;
       }
       
       };
 
 MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);

 if (k.state[94]) {selected--;if (selected<0) selected=num_options-1;k.state[94]=0;}
 if (k.state[99]) {selected++;if (selected>=num_options) selected=0;k.state[99]=0;}
 if (k.state[28]) {
             if (selected==0) {player_options();options_scr(num_options,"efps\\cool.efp");}
             if (selected==1) {game_options();options_scr(num_options,"efps\\cool.efp");}
             if (selected==2) {sound_options();options_scr(num_options,"efps\\cool.efp");}
             if (selected==3) {deathmatch_options();options_scr(num_options,"efps\\cool.efp");}
             if (selected==4) {quit=1;}
            }
 if (k.state[1]) {quit=1; k.state[1]=0;}
}

save_options();

fadeout(virbuff,pal);

}

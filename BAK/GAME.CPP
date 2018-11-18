#include <stdarg.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdiobuf.h>
#include <stdlib.h>
#include <midasdll.h>
#include <vesa.h>
#include <gfunc.h>
#include <math.h>
#include <time.h>
#include <iostream.h>
#include <image.hpp>
#include <keyb.hpp>
#include <vesa2.h>

void game();
#include "defines.h"
#include "error/error.h"
#include "efp/efp.h"
#include "globvar.h"

/* ************************ NET STUFF **************************************/

#include "int/int.h"
#include "ipx/ipx.h"

word SOCKET=0x869C;  // DOOM Socket

#define IPX_END 0
#define IPX_SERVERSEARCH 1
#define IPX_TEXTMESSAGE 2
#define IPX_SERVERANSWER 3

int IPXMSGLEN[]={1,  // END
				 1,  // SERVERSEARCH
				 0, // TEXTMESSAGE
				 17}; // SERVERANSWER

int ipxofs[MAX_PLAYERS]={0,0,0,0 
						,0,0,0,0};
char ipxstream[MAX_PLAYERS][MAXDATASIZE];

struct nodeaddr serveraddr;
char servername[10];
struct packet far *send,far *rec[RECEIVERS];
unsigned short sendl,sendseg,recl[RECEIVERS],recseg[RECEIVERS];


/* ************************ NET STUFF **************************************/


VBEINFO *vbeInfo;
VBESURFACE *vbe;

#include "miscfunc.h"
#include "write.h"  // tosi paskoja rutiineja !!!
#include "classes.h"
#include "rout.h"
#include "fade.h"

#include "draw.h"
#include "funcs.h"
#include "shop.h"
#include "newfuncs.h"
#include "options.h"

void menu_biisi()
{
if (ph) MIDASstopModule(ph);
if (biisi!=NULL) MIDASfreeModule(biisi);
biisi=MIDASloadModule("music\\menu.s3m");
ph=MIDASplayModule(biisi,TRUE);
MIDASsetMusicVolume(ph,MUSIC_VOLUME);
}

void peli_biisi()
{
	static int biisi_count;

	if (ph) MIDASstopModule(ph);

	if (biisi!=NULL) MIDASfreeModule(biisi);

	biisi_count++;
	biisi_count%=3;

	if (biisi_count==0)
	 biisi=MIDASloadModule("music\\unic_1x.s3m");
	if (biisi_count==1) 
	 biisi=MIDASloadModule("music\\unic_2x.s3m");
	if (biisi_count==2)
	 biisi=MIDASloadModule("music\\unic_3x.s3m");

	ph=MIDASplayModule(biisi,TRUE);
	MIDASsetMusicVolume(ph,MUSIC_VOLUME);
}	

void MIDAS_CALL prevr(void){
framecount++;
}

void make_floors(){
int a,lum;
char col;

for (a=0;a<64000;a++){
lum=(int)((float)(pal[floorspr[a]*3]*0.2)+(float)(pal[floorspr[a]*3+1]*0.5)+(float)(pal[floorspr[a]*3+2]*0.3))>>2;
col=(char)((208+15)-lum);
floorspr2[a]=col;
}

}

void load_tables(){
FILE *tab;
tab=fopen("palette.tab","rb");
fread(trans_table,sizeof(trans_table),1,tab);
fread(shadow_table,sizeof(shadow_table),1,tab);
fread(&normal_l,sizeof(normal_l),1,tab);
fread(&red_l,sizeof(red_l),1,tab);
fread(&yellow_l,sizeof(yellow_l),1,tab);
fread(&explo_l,sizeof(explo_l),1,tab);
fclose(tab);
}

void allocipxmem()
{ int a;
 
  send=(struct packet far *)MK_FP(getlowmem(sizeof(struct packet)/16+1),0);
  if (send==NULL)
  	{
  	 cout <<"Can't alloc lowmem!\n"<< endl;
	 ipxavail=0;
	 return;
	}
  sendl=lsel;
  sendseg=lseg;

  for (a=0;a<RECEIVERS;a++)
  {		rec[a]=(struct packet far *)MK_FP(getlowmem(sizeof(struct packet)/16+1),0);
		if (send==NULL)
	  	{
		  	cout <<"Can't alloc lowmem!\n" << endl;
			ipxavail=0;
			return;
		}
		recl[a]=lsel;
		recseg[a]=lseg;
  }

}

void freeipxmem()
{ int a;
 
  freelowmem(sendl);

  for (a=0;a<RECEIVERS;a++)
  {		
  		freelowmem(recl[a]);
		recseg[a]=lseg;
  }

}


void do_all(){
int a;
char text[30];

vesa_set_mode(0x3);

cout << "\n                ================== Starting TK =================\n\n";

if( (vbeInfo = vbeDetect()) == NULL )
    printf("VESA not supported\n"); else {

  printf("VESA found...");
  printf("\nVBE Version      : %d.%d\n"
	 "Oem              : %s\n"
	 "Total memory     : %gK\n",
	  vbeInfo->VbeVersion>>8,
	  vbeInfo->VbeVersion&0xFF,
	  vbeInfo->OemStringPtr,
	  ((float)vbeInfo->TotalMemory*64000.0f)/1024.0f);
  if (vbeInfo->VbeVersion>>8>=2) VESA2=1;
 }

if (initipx())
	{	
		char str[256];
		cout <<"IPX protocol detected!\n";
		getlocaladdress();
		printf("IPX address: %s\n",decode_localaddress(str,localadd));
		ipxavail=1;
		allocipxmem();
	} else cout <<"IPX _NOT_ detected!\n";

if (MIDASstartup()){
if (MIDASloadConfig("sound.cfg")){
 cout<<"Sound configuration loaded\n"; 
if (MIDASinit()){
 cout<<"MIDAS initialized\n"; 
 if (MIDASsetTimerCallbacks(target_frames*1000,FALSE,&prevr,NULL,NULL)) cout<<"Timer synchronized\n"; 
 }
 else error("Midas initialization failed\n");
 } else error("Sound configuration not found! run SETUP.exe\n");

if (! MIDASopenChannels(16)) error("Error opening sound channels\n");
if (! MIDASallocAutoEffectChannels(4)) error("Error allocating effect channels\n");

magnumwav = MIDASloadWaveSample("wavs\\magnum.wav",FALSE);
pistolwav = MIDASloadWaveSample("wavs\\pistol.wav",FALSE);
shotgunwav= MIDASloadWaveSample("wavs\\rifle.wav",FALSE);
explowav  = MIDASloadWaveSample("wavs\\explo.wav",FALSE);
deathwav  = MIDASloadWaveSample("wavs\\death.wav",FALSE);
klikwav   = MIDASloadWaveSample("wavs\\klik.wav",FALSE);
hitwav    = MIDASloadWaveSample("wavs\\hit.wav",FALSE);
sinkowav  = MIDASloadWaveSample("wavs\\sinko.wav",FALSE);
wooshwav  = MIDASloadWaveSample("wavs\\woosh.wav",FALSE);
flamewav  = MIDASloadWaveSample("wavs\\flame.wav",FALSE);

cout<<"Sound effects loaded\n";

} else error("Error in MIDAS startup !!!\n");

set_keyb_rate();
k.init(0);
cout << "Keyboard handler initialized\n"	;

FONT_NUM=0;loadfonts("fnts\\bloody.fnt");
FONT_NUM=1;loadfonts("fnts\\8x8b.fnt");
FONT_NUM=2;loadfonts("fnts\\bloody2.fnt");
FONT_NUM=3;loadfonts("fnts\\snumbers.fnt");
cout << "Fonts loaded\n"	;
load_efp("efps\\floor1.efp",floorspr,0);
load_efp("efps\\walls1.efp",wallspr,0);
load_efp("efps\\enemy0.efp",enemyspr0,0);
load_efp("efps\\enemy1.efp",enemyspr1,0);
load_efp("efps\\enemy2.efp",enemyspr2,0);
load_efp("efps\\enemy3.efp",enemyspr3,0);
load_efp("efps\\enemy4.efp",enemyspr4,0);
load_efp("efps\\enemy5.efp",enemyspr5,0);
load_efp("efps\\enemy6.efp",enemyspr6,0);
load_efp("efps\\rambo2.efp",rambospr,0);
load_efp("efps\\bodyp.efp",bodypartspr,0);
load_efp("efps\\effects.efp",effectspr,0);
load_efp("efps\\ware.efp",warespr,0);
load_efp("efps\\target.efp",targetspr,0);
load_efp("efps\\c4.efp",c4spr,0);
load_efp("efps\\mine.efp",minespr,0);
load_efp("efps\\bomb.efp",grenadespr,0);
load_efp("efps\\shadows.efp",shadowspr,0);
load_efp("efps\\crates.efp",cratespr,0);
cout << "Bitmaps loaded\n";

pre_count();
CalCuLaTE_Sine_Table_AND_Be_Happy(); // for fade
cout<<"Sine and Cosine tables ready\n"; 

for (a=0;a<LIGHTS;a++)
 light[a].make(Light_sizes[a]);
cout << "Lights mapped\n"	;

load_tables();
cout << "Palette tables loaded \n";

load_efp_pal("efps\\walls1.efp",pal);

cout << "Calculating dark floor... ";
 make_floors();
cout << "OK \n"	;

load_options();
cout << "Options loaded\n" ;

list_files(&dirs,dirlist,"levs\\*.*",_A_SUBDIR);
cout << dirs << " episodes found\n";

list_files(&files_in_episode[0],filelist[0],"levs\\*.lev",_A_NORMAL);
for (a=0;a<dirs && a<256;a++){
 strcpy(text,"levs\\");
 strcat(text,dirlist[a].name);
 strcat(text,"\\*.lev");
 list_files(&files_in_episode[a+1],filelist[a+1],text,_A_NORMAL);
}

define_bullet_types();
define_effects();
}

void free_all()
{
	MIDASclose();

	free(filelist);
	free(dirlist);

	freeipxmem();

	k.deinit();

}

int check_place(int x,int y,int size){
int a,ret=0;

if (level[(y/20)*level_x_size+(x/20)].type!=FLOOR)	ret=1;
if (level[((y+size)/20)*level_x_size+(x/20)].type!=FLOOR) ret=1;
if (level[(y/20)*level_x_size+((x+size)/20)].type!=FLOOR)  ret=1;
if (level[((y+size)/20)*level_x_size+((x+size)/20)].type!=FLOOR) ret=1;

for (a=0;a<PLAYERS;a++)
 if (get_dist(x,y,player[a].x,player[a].y)<50) ret=1;

return(ret);
}

void place_enemies(){
int a,done,nx,ny,b,c;

for (a=0;a<ENEMIES;a++){
done=0;
while (done==0){
nx=(rand()%(level_x_size-2))+1;
ny=(rand()%(level_y_size-2))+1;
 if (level[ny*level_x_size+nx].type==FLOOR){
  done=1;
  if (a>0)
  for (b=0;b<a;b++)
  if (nx==enemy[b].x && ny==enemy[b].y) done=0;
 
 for (c=0;c<PLAYERS && done==1;c++)
  if (get_dist(nx,ny,pl_start_x[c],pl_start_y[c])<8) done=0;

  }
 }
enemy[a].x=nx;
enemy[a].y=ny;
}

for (a=0;a<ENEMIES;a++){
enemy[a].x=((enemy[a].x*20)-5);
enemy[a].y=((enemy[a].y*20)-5);
enemy[a].angle=rand()%360;
}

}

void place_normal_game_crates(){
int a,b,x=0,y=0,crat=0,bullets,weapons;

if (leveldata.version==1) {weapons=DIFF_WEAPONS-2;bullets=DIFF_BULLETS-2;}
if (leveldata.version==2)	{weapons=DIFF_WEAPONS-1;bullets=DIFF_BULLETS-1;}
if (leveldata.version==3)	{weapons=DIFF_WEAPONS;bullets=DIFF_BULLETS;}

for (a=0;a<weapons;a++)
for (b=0;b<normal_game_level_info.weapon_crates[a];b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=0;
crate[crat].weapon=a;
while (check_place(x,y,14)) if (check_place(x,y,14)){ x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}
for (a=0;a<bullets;a++)
for (b=0;b<normal_game_level_info.bullet_crates[a];b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=1;
crate[crat].bullet=a;
while (check_place(x,y,14)) if (check_place(x,y,14)) { x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}

for (b=0;b<normal_game_level_info.energy_crates;b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=2;
crate[crat].energy=1;
while (check_place(x,y,14)) if (check_place(x,y,14)) { x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}
}

void place_death_match_crates(){
int a,b,x=0,y=0,crat=0,weapons,bullets;

if (leveldata.version==1) {weapons=DIFF_WEAPONS-2;bullets=DIFF_BULLETS-2;}
if (leveldata.version==2)	{weapons=DIFF_WEAPONS-1;bullets=DIFF_BULLETS-1;}
if (leveldata.version==3)	{weapons=DIFF_WEAPONS;bullets=DIFF_BULLETS;}

for (a=0;a<weapons;a++)
for (b=0;b<death_match_level_info.weapon_crates[a];b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=0;
crate[crat].weapon=a;
while (check_place(x,y,14)) if (check_place(x,y,14)){ x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}
for (a=0;a<bullets;a++)
for (b=0;b<death_match_level_info.bullet_crates[a];b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=1;
crate[crat].bullet=a;
while (check_place(x,y,14)) if (check_place(x,y,14)) { x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}

for (b=0;b<death_match_level_info.energy_crates;b++,crat++)
if (crat<MAX_CRATES)
{
x=0;y=0;
crate[crat].type=2;
crate[crat].energy=1;
while (check_place(x,y,14)) if (check_place(x,y,14)) { x=rand()%(level_x_size*20);y=rand()%(level_y_size*20);} 
crate[crat].x=x;
crate[crat].y=y;
crate[crat].IN_USE=1;
}
}

void alusta_enemies(){
int a,b,c,d;
	   
ENEMIES=0;
c=0;
for (b=0;b<DIFF_ENEMIES;b++){
ENEMIES+=general_level_info.enemies[b];

for (a=0;a<general_level_info.enemies[b] && c<MAX_ENEMIES;a++){
 enemy[c].PUSH_POWER=0;
 enemy[c].burning=0;
 enemy[c].rangle=0;
 enemy[c].angle=0;
 enemy[c].load_cnt=0;
 enemy[c].WALK=0;
 enemy[c].FIRE=0;
 for (d=0;d<MAX_PLAYERS;d++)
  enemy[c].SEE_PL[d]=0;
 enemy[c].DEAD=0;
 enemy[c].walk_cnt=0;
 enemy[c].type=b;
 enemy[c].energy=enemy_info[enemy[c].type].energy;
 c++;
 }

}

if (ENEMIES>MAX_ENEMIES) ENEMIES=MAX_ENEMIES;

}

void null_effects(){
int a;

for (a=0;a<MAX_EFFECTS;a++)
 effect[a].IN_USE=0;
}

void null_crates(){
int a;
for (a=0;a<MAX_CRATES;a++){
 crate[a].text_count=0;
 crate[a].weapon=0;
 crate[a].bullet=0;
 crate[a].wake_up_count=0;
 crate[a].IN_USE=0;
}
}

void null_body_parts(){
int a;

	for (a=0;a<MAX_BODY_PARTS;a++)
	 body_part[a].IN_USE=0;
}

void null_bullets()
{	
	int a;
	for (a=0;a<MAX_BULLETS;a++)
	 bullet[a].IN_USE=0;
}

void animate_players()
{
	int a;
	for (a=0;a<ACTIVE_PLAYERS;a++)
	 player[a].animate();
}

void animate_enemies()
{
	int a;
	for (a=0;a<ENEMIES;a++)
	 enemy[a].animate();
}

void quiti(){
 int a,x=160-(str_length("REALLY WANNA QUIT ? (y)")/2);
 clock_t time;

 for (a=0;a<12;a++)
  memset(virbuff+(a+(scr_y_size/2)-6)*320+x-2,0,320-(x*2)+4);
 writefonts(x,(scr_y_size/2)-4,"REALLY WANNA QUIT ? (y)",1);

 memcpy(screen,virbuff,64000);

 k.clear_stack();

 time=clock();

 if (k.getch()=='y') quit=1;

 st+=clock()-time;

}

void screen_capture(){
FILE *sci;
int free=0,num=0;
char text1[13],text2[13];
Image pic;

pic.alloc(320,scr_y_size,8,1);

while (free==0 && num<99){
 strcpy(text1,"screen");
 itoa(num,text2,10);
 strcat(text1,text2);
 strcat(text1,".pcx");
 if ((sci=fopen(text1,"rb"))==NULL) free=1; else fclose(sci);
  num++;
}

memcpy(pic.image,screen,320*scr_y_size);
memcpy(pic.palette,pal,3*256);
pic.save_pcx(text1);
}

void chkeys(){
int a;

if (k.state[1])  quiti();	 

for (a=0;a<ACTIVE_PLAYERS;a++)
if (! player[a].DEAD)
  player[a].chk_keys(a);
 
 if (k.state[57]) { if (! MAP_ON) MAP_ON=1; else MAP_ON=0; k.state[57]=0;}
 if (k.state[33]) { if (FRAMES_ON==0) FRAMES_ON=1; else FRAMES_ON=0; k.state[33]=0; }
 if (k.state[88]) {screen_capture();k.state[88]=0;}

}

void chk_wake_ups(){
int a;
 for (a=0;a<MAX_CRATES;a++)
  if (crate[a].IN_USE==0)
   crate[a].chk_wake_up();
}

void move_crate_texts(){
int a;
 for (a=0;a<MAX_CRATES;a++)
  if (crate[a].IN_USE)
   crate[a].move_text();
}

void move_body_parts(){
int a;

for (a=0;a<MAX_BODY_PARTS;a++)
if (body_part[a].IN_USE)
if (body_part[a].speed>0){
body_part[a].move();
body_part[a].speed-=0.2;
}

}

void roll_enemies(){
int a;

for (a=0;a<ENEMIES;a++)
if (! enemy[a].DEAD)
{
 if (enemy[a].tangle==enemy[a].angle)
 if (enemy[a].walk_cnt==0)
 { 
   if (enemy[a].see_somebody==0) 
    if (rand()%50==1) 
         enemy[a].tangle=rand()%360;
 }
 enemy[a].rotate();
}

}

void move_enemies(){
int a,b,see;

for (a=0;a<ENEMIES;a++){
if (! enemy[a].DEAD){
see=0;
 for (b=0;b<PLAYERS && see==0;b++)
  if (enemy[a].SEE_PL[b]) see=1;
if (see==0)
if (enemy[a].walk_cnt==0)
if (rand()%100==1) enemy[a].walk_cnt=(rand()%40)+20;
if (enemy[a].walk_cnt>0) {enemy[a].move(enemy[a].rangle*9,0);enemy[a].walk_cnt--;if (enemy[a].WALK==0) enemy[a].WALK=1;}	else enemy[a].WALK=0;
}

if (enemy[a].PUSH_POWER>0){
 if ((int)enemy[a].PUSH_POWER>0) 
  enemy[a].move(enemy[a].PUSH_ANGLE,(int)enemy[a].PUSH_POWER);
 enemy[a].PUSH_POWER-=0.2;
}

}

}

void push_players(){

int a;

for (a=0;a<ACTIVE_PLAYERS;a++)
if (player[a].PUSH_POWER>0){
 if ((int)player[a].PUSH_POWER>0) 
  player[a].move(player[a].PUSH_ANGLE,(int)player[a].PUSH_POWER,a);
 player[a].PUSH_POWER-=0.2;
}

}

void roll_effects(){
int a;
for (a=0;a<MAX_EFFECTS;a++)
 effect[a].roll();

}

void move_bullets(){
int a;
for (a=0;a<MAX_BULLETS;a++)
if (bullet[a].IN_USE==1)
 bullet[a].move();

}

void get_angles(){
int a;
for (a=0;a<ENEMIES;a++)
 if (! enemy[a].DEAD)
 enemy[a].get_angles();
}

void try_see_players(){
int a;
for (a=0;a<ENEMIES;a++)
 if (! enemy[a].DEAD)
 enemy[a].try_see_players();
}

void enemies_load(){
int a;
for (a=0;a<ENEMIES;a++)
 if (! enemy[a].DEAD)
 enemy[a].load();
}

void chk_for_shoot(){
int a;
for (a=0;a<ENEMIES;a++)
 if (! enemy[a].DEAD)
 enemy[a].chk_for_shoot();
}

void complete(){
int a;

next_level++ ;	 
a=(next_level*5)%360;
writefonts(160+(( 160-((float)next_level/((float)C_DELAY/160))) *sini[a])-45,(scr_y_size/2)+(100-((float)next_level/((float)C_DELAY/100)))*cosi[a],"MISSION COMPLETED",3*16);

}

void kello(){
 char col=1;

 if (! next_level)
 complete_time=(clock()-st)/CLOCKS_PER_SEC;

 if (complete_time>general_level_info.time_limit*TIME_ADD) col=3*16;

 writefonts(151,10,c_2_c(complete_time),col);

}

void frames_per_sec(){

char text[5];

itoa((int)F_PER_SEC,text,10);
writefonts(151,20,text,9*16);
itoa((1000-OVER_POWER*F_PER_SEC)/10,text,10);
writefonts(151,30,text,7*16);

}

void dec_hits(){
int a;

for (a=0;a<PLAYERS;a++)
if (player[a].HIT>0) player[a].HIT--;

}

void are_all_dead(){

int a;

if (KILLING_MODE!=DEATHMATCH){
all_dead=1;
for (a=0;a<PLAYERS;a++)
 if (player[a].DEAD<=14) all_dead=0;
}

if (all_dead) 
  complete_time=(general_level_info.time_limit*TIME_ADD)+100;

}


void seeing(){
int a;

for (a=0;a<PLAYERS;a++){
 memset(player[a].see_level,0,level_x_size*level_y_size);
 player[a].see();
}
}

void fire_lights(){
int	a,xoffs,yoffs;

if (GAME_MODE!=SPLIT_SCREEN)
 for (a=0;a<PLAYERS;a++){
  if (weapon[player[a].curr_weapon].gun==1)
  if (player[a].FIRE){ 
   xoffs=(sini[player[a].rangle2]*10)+(sini[(int)((player[a].rangle2)+270)%360]*6);
   yoffs=(cosi[player[a].rangle2]*10)+(cosi[(int)((player[a].rangle2)+270)%360]*6);
   draw_light(&normal_l,player[a].x+14+xoffs-player[0].scr_x,player[a].y+13+yoffs-player[0].scr_y,3,2-player[a].FIRE,0); 
  }
 }

if (GAME_MODE==SPLIT_SCREEN){
 for (a=0;a<PLAYERS;a++){
  if (weapon[player[a].curr_weapon].gun==1)
  if (player[a].FIRE){ 
   xoffs=(sini[player[a].rangle2]*10)+(sini[(int)((player[a].rangle2)+270)%360]*6);
   yoffs=(cosi[player[a].rangle2]*10)+(cosi[(int)((player[a].rangle2)+270)%360]*6);
   draw_light(&normal_l,player[a].x+14+xoffs-player[0].scr_x,player[a].y+13+yoffs-player[0].scr_y,3,2-player[a].FIRE,1); 
   draw_light(&normal_l,(player[a].x+14+xoffs-player[1].scr_x)+160,player[a].y+13+yoffs-player[1].scr_y,3,2-player[a].FIRE,2); 
  }
 }

}

if (GAME_MODE!=SPLIT_SCREEN)
for (a=0;a<ENEMIES;a++)
  if (! enemy[a].DEAD) 
  if (enemy[a].FIRE){ 
   xoffs=(sini[(int)(enemy[a].rangle*9)]*10)+(sini[(int)((enemy[a].rangle*9)+270)%360]*3);
   yoffs=(cosi[(int)(enemy[a].rangle*9)]*10)+(cosi[(int)((enemy[a].rangle*9)+270)%360]*3);
    draw_light(&normal_l,enemy[a].x+14+xoffs-player[0].scr_x,enemy[a].y+13+yoffs-player[0].scr_y,3,2-enemy[a].FIRE,0); 
   }
 
if (GAME_MODE==SPLIT_SCREEN)
for (a=0;a<ENEMIES;a++)
  if (! enemy[a].DEAD) 
  if (enemy[a].FIRE){ 
   xoffs=(sini[(int)(enemy[a].rangle*9)]*10)+(sini[(int)((enemy[a].rangle*9)+270)%360]*3);
   yoffs=(cosi[(int)(enemy[a].rangle*9)]*10)+(cosi[(int)((enemy[a].rangle*9)+270)%360]*3);
   draw_light(&normal_l,enemy[a].x+14+xoffs-player[0].scr_x,enemy[a].y+13+yoffs-player[0].scr_y,3,2-enemy[a].FIRE,1); 
   draw_light(&normal_l,(enemy[a].x+14+xoffs-player[1].scr_x)+160,enemy[a].y+13+yoffs-player[1].scr_y,3,2-enemy[a].FIRE,2); 
   }
   
}

void chk_burn(){
int a;
for (a=0;a<ACTIVE_PLAYERS;a++)
if (! player[a].DEAD)
if (player[a].burning){
 player[a].burning--; 
 player[a].get_damage(0.4,player[a].Frying_player);
 if (player[a].burning%6==0)
  new_effect(0,STEAM,player[a].x+14,player[a].y+14,135,1);
}

for (a=0;a<ENEMIES;a++)
if (! enemy[a].DEAD)
if (enemy[a].burning){
 enemy[a].burning--; 
 enemy[a].get_damage(0.4,enemy[a].Frying_player);
 if (enemy[a].burning%6==0)
  new_effect(0,STEAM,enemy[a].x+14,enemy[a].y+14,135,1);
}

}

#define s_r_a 8
 
int steam_rates[s_r_a]={15,10,5,5,4,4,3,3};

void do_steam(){
int a,b,du=0,yl1=scr_y_size/2,yl2=scr_y_size*1.5;

for (a=0;a<Steam_amount;a++,du=0){
if (steam[a].x>player[0].scr_x-160)
if (steam[a].x<player[0].scr_x+480)
if (steam[a].y>player[0].scr_y-yl1)
if (steam[a].y<player[0].scr_y+yl2)
 du=1;

if (GAME_MODE==SPLIT_SCREEN)
if (steam[a].x>player[1].scr_x-160)
if (steam[a].x<player[1].scr_x+480)
if (steam[a].y>player[1].scr_y-yl1)
if (steam[a].y<player[1].scr_y+yl2)
 du=1;

if (du)
 {
 if (steam[a].speed<s_r_a) b=steam_rates[steam[a].speed];
  else b=1;
 if (Steam_count%b==0)
 new_effect(0,STEAM,steam[a].x,steam[a].y,steam[a].angle,steam[a].speed);
 }
}
}

void game_shit(){
int a;

chkeys();
push_players();

chk_burn();

move_bullets();
move_body_parts();
move_crate_texts();
if (KILLING_MODE==DEATHMATCH) chk_wake_ups();

for (a=0;a<ACTIVE_PLAYERS;a++)
 player[a].move_scr();

if (DARK_MODE) seeing();

roll_enemies();
get_angles();
try_see_players();
if (GAME_MODE!=NETWORK||
   (GAME_MODE==NETWORK&&NETWORK_MODE==SERVER)) move_enemies();
chk_for_shoot();
draw_floor();
enemies_load();

for (a=0;a<ACTIVE_PLAYERS;a++) player[a].load();

animate_enemies();
animate_players();

draw_players(0); // DEad players
draw_effects(0);
draw_body_parts();
if (SHADOWS) draw_gae_shadows();

draw_crates();
draw_bullets();
if (LIGHT_EFFECTS) fire_lights();

draw_live_enemies();
draw_players(1);  // Live players

if (SMOKE_EFFECTS) do_steam();
draw_walls();
if (SHADOWS) draw_shadows();
draw_effects(1);

if (LIGHT_EFFECTS) draw_spots();
draw_targets();
if (DARK_MODE)	draw_black();
dec_hits();
draw_hits();

if (MAP_ON)	map();
draw_crate_texts();
if (FRAMES_ON) frames_per_sec();
if (KILLING_MODE==DEATHMATCH) draw_kills();
draw_infos();
draw_energies();
if (next_level)	complete();
if (KILLING_MODE!=DEATHMATCH) kello();
if (first==1) {fadein(virbuff,pal);first=0;}

memcpy(screen,virbuff,320*scr_y_size);
roll_effects();
are_all_dead();

}

#include "i_funcs.h"	// paska funkkareita

void do_prices(){
int a;

for (a=0;a<DIFF_WEAPONS;a++)
sell_price[a]=(0.8*weapon[a+1].cost)-(rand()%(weapon[a+1].cost/2));

sell_price[DIFF_WEAPONS]=(0.8*shield_data.cost)-(rand()%(shield_data.cost/2));
sell_price[DIFF_WEAPONS+1]=(0.8*target_cost)-(rand()%(target_cost/2));

}

void victory()
{
	int a=0,oclock;

	load_efp("efps\\sunset.efp",picture,0);
	load_efp_pal("efps\\sunset.efp",pal);
	setpal(pal,0);
	memcpy(screen,picture,64000);

	oclock=clock();

	while (a<255)
	 if (oclock!=clock()) 
	 {
		oclock=clock();
		a+=10;
		setpal(pal,a);
	 }

	k.state[1]=0;
	while (! k.state[1]);
 
	while (a>0)
	 if (oclock!=clock()) 
	  {
		oclock=clock();
		a-=10;
		setpal(pal,a);
	  }
}

void flushipx()
{
	int a;
	for (a=0;a<PLAYERS;a++)
	 {
	 	ipxstream[a][ipxofs[a]++]=IPX_END;
		_fmemcpy(send->data,ipxstream[a],ipxofs[a]);
		sendpacket(sendl,player[a].ipxnode,send,(word)ipxofs[a]);
		// RESET
		ipxofs[a]=0;
	 }
}

void sendipxnow(struct nodeaddr *node, int type, ...)
{	int a=0;

// TEMP VARIABLES 
	int b;
	struct nodeaddr n;
	char *str;
// TEMP VARIABLES END

	auto va_list ap;
	va_start(ap,type);

	switch(type)
	{
	 case IPX_SERVERSEARCH:
		send->data[a++]=(char)type;
	 break;
	 case IPX_SERVERANSWER:
		send->data[a++]=(char)type;
	    n=va_arg(ap,struct nodeaddr);
		for (b=0;b<6;b++)
			send->data[a++]=n.node[b];
		str=va_arg(ap,char *);
		for (b=0;b<10;b++)
			send->data[a++]=str[b];
	 break;
	 default: error("IPX message %d not yet implemented\n",type);
	}
	send->data[a++]=IPX_END;
	sendpacket(sendl,*node,send,(word)a);

	va_end( ap );

}

void sendipx(int dest, int type, ...)
{
	va_list ap;
	va_start(ap,type);

	switch(type)
	{
	 case IPX_SERVERSEARCH:
	 	error("sendipx: Use sendipxnow for serversearch\n");
	 break;
	 case IPX_SERVERANSWER:
	   	error("sendipx: Use sendipxnow for serveranswer\n");
	 break;
	 default: error("sendipx: Not implemented yet!\n");
	/*ipxstream[dest][ipxofs[dest]++]=(char)type;
    n=va_arg(ap,struct nodeaddr node);
	for (a=0;a<6;a++)
		ipxstream[dest][ipxofs[dest]++]=n.node[a];
	 break;*/
	}
	va_end(ap);
}


void setup_ipx()
{
	int a;
	if (opensocket(0,SOCKET))
	 error("Couldn't open socket (%Xh)\n",SOCKET);

	strcpy(servername,name1);
	for (a=0;a<MAX_PLAYERS;a++)
	 ipxofs[a]=0;
	if (NETWORK_MODE==SERVER) PLAYERS=1;
}

void desetup_ipx()
{
	flushipx();
	closesocket((word)socketid);
}

void do_server_shit()
{
	int a,data,ofs;

	for (a=0;a<RECEIVERS;a++)
	if (rec[a]->ecb.inuse==0)
	{
		data=-1;
		ofs=0;
		while(data!=IPX_END)  
		{ 
			data=rec[a]->data[ofs++];
			switch (data)
			{
			case IPX_SERVERSEARCH:
				 sendipxnow(&rec[a]->ipx.src.node,
				 			IPX_SERVERANSWER,
					 		serveraddr,
				 		 	servername);
				 break;
			case IPX_TEXTMESSAGE:
				 while ((data=rec[a]->data[ofs++])!=0)
				  putch(data);
				 printf("\n");
				 break;
			case IPX_END:
			break;

			default:error("IPX: receiver %d: unknown command %d\n",a,data);
			};

		}
		listenforpacket(recseg[a]);
	}
	flushipx();
}

void do_client_shit()
{
}

void game(){

int c,a;
int frame_count;
int oclock;
int op[5],opcnt=0,opcnt2=0;
char match_level[50];
char filename[50];
char text[10];

if (GAME_MODE==SPLIT_SCREEN && KILLING_MODE!=DEATHMATCH) episode=DEATH_MATCH_EPISODE-1;

if (GAME_MODE==ONE_PLAYER) PLAYERS=1;
if (GAME_MODE==SPLIT_SCREEN){ PLAYERS=2;
							  ACTIVE_PLAYERS=2;
							}
						else ACTIVE_PLAYERS=1;

strcpy(match_level,"levs\\");
if (DEATH_MATCH_EPISODE>0){
 strcat(match_level,dirlist[DEATH_MATCH_EPISODE-1].name);
 strcat(match_level,"\\");
}
strcat(match_level,DEATH_MATCH_LEVEL);

fadeout(virbuff,pal);

alusta_players1();

LEVELS=files_in_episode[episode+1];

LEVEL=START_LEVEL;
if (LEVEL>LEVELS-1) LEVEL=LEVELS-1;

quit=0;

all_dead=0;

BACK_TO_MENU=0;

while (! quit && ! BACK_TO_MENU){	  ////

strcpy(filename,"levs\\");
strcat(filename,dirlist[episode].name);
strcat(filename,"\\level");
itoa(LEVEL+1,text,10);
strcat(filename,text);
strcat(filename,".lev");

if (KILLING_MODE!=DEATHMATCH) leveldata.load(filename);
 else (leveldata.load(match_level));

if (KILLING_MODE!=DEATHMATCH) level_info();

if (! BACK_TO_MENU){
				 
if (DARK_MODE) {
for (a=0;a<PLAYERS;a++)
player[a].see_level=(char *)malloc(level_x_size*level_y_size);
}

if (KILLING_MODE!=DEATHMATCH){
 do_prices();
 if (LEVEL>0){
 for (a=0;a<PLAYERS;a++) player[a].shop();
 }
}

if (! FORCE_0x13)
 if (VESA2) { scr_y_size=240;
           	 vbe=vbeOpen(320,240,8);
			 screen=vbe->lfb;
            }

peli_biisi();

load_efp_pal("efps\\walls1.efp",pal);

alusta_players2();

DEAD_ENEMIES=0;

if (ENEMIES_ON_GAME==0 && KILLING_MODE==DEATHMATCH) ENEMIES=0; else {
 alusta_enemies();
 place_enemies();
}

null_body_parts();
null_effects();
null_bullets();
null_crates();

if (KILLING_MODE==COOPERATIVE) place_normal_game_crates();
 else place_death_match_crates();

FONT_NUM=1;
frame_count=0;
oclock=clock();
st=clock();

first=1;

OVER_POWER=0;

Spot_count=0;
Steam_count=0;

next_level=0;

if (KILLING_MODE==DEATHMATCH)
 for (a=0;a<ACTIVE_PLAYERS;a++)	player[a].random_place();

effect_age_count=0;

while(! all_dead && ! quit && next_level<C_DELAY){ // GAME loop
 
 c=clock();

 if ( ((float)c/CLOCKS_PER_SEC) - ((float)oclock/CLOCKS_PER_SEC) >2){
 F_PER_SEC=((float)frame_count/(c-oclock))*CLOCKS_PER_SEC;
 oclock=c;
 
 frame_count=0;
 }

opcnt++;	// OVER_POWER count
delay(1);
if (framecount){

 op[opcnt2]=opcnt;
 opcnt2++;
 if (opcnt2>5) opcnt2=0;

 opcnt=0;
 framecount=0;	  // incremented by MIDAS timer

 game_shit(); // do the game !!!!

 Steam_count++;
 Steam_count%=360;

 Spot_count+=2;
 Spot_count%=360;

 if (opcnt2==0)
 OVER_POWER=(op[0]+op[1]+op[2]+op[3]+op[4])/5;

 frame_count++;	   
}

if (GAME_MODE==NETWORK&&NETWORK_MODE==CLIENT)
	do_client_shit();
if (GAME_MODE==NETWORK&&NETWORK_MODE==SERVER)
	do_server_shit();

}

if (DARK_MODE)
for (a=0;a<PLAYERS;a++)
 free(player[a].see_level);

}

fadeout(virbuff,pal);

menu_biisi();

 if (! FORCE_0x13)
 if (VESA2) {scr_y_size=200;vbeClose(vbe);vesa_set_mode(0x13);screen=(char *)0xA0000;}

if (KILLING_MODE!=DEATHMATCH){

 all_dead=0;

 are_all_dead();
  if (all_dead) quit=1; else next_level=1;

 if (LEVEL==0 && BACK_TO_MENU) {} else
  statics();

 if (! all_dead && GAME_MODE==SPLIT_SCREEN) two_to_one();
 if (LEVEL==LEVELS-1) if (! BACK_TO_MENU) if (next_level==1) if (quit==0) {quit=1;next_level=0;victory();}
 if (next_level) LEVEL++;
 next_level=0;
}

}  /////

load_efp_pal("efps\\ware.efp",pal);

}

void menu_scr()
{
	load_efp("efps\\tk.efp",picture,0);
	load_efp_pal("efps\\ware.efp",pal);
	draw_box1(55,90-((Menu_texts*15)/2),320-55,110+((Menu_texts*15)/2),10);
	k.state[28]=0;
	first=1;
}

void menu()
{
	int a,cnt=0,selected=0,quit=0,oclock;
	int starty=(Menu_texts*15)/2;

	ph=0;

	menu_scr();
	menu_biisi();

	while(! quit)
	{
		memcpy(virbuff,picture,64000);
		FONT_NUM=1;
		writefonts(320-str_length(Version),190,Version,M_BLUE);
		for (a=0;a<Menu_texts;a++)
		{ 
			if (a==selected)
			{
				FONT_NUM=0;  
				writefonts2(160-(str_length(menu_text[a])/2),100-starty+(a*15),menu_text[a],1);
			} else 
			{
				FONT_NUM=2;
				writefonts2(160-(str_length(menu_text[a])/2),100-starty+(a*15),menu_text[a],M_RED);
 			}
		}
		if (first==1) {fadein(virbuff,pal);first=0;}
		memcpy(screen,virbuff,64000);
		while (! k.state[94] && ! k.state[99] && ! k.state[28])
		{ 
			if (clock()!=oclock)
			{	   
				oclock=clock();
				draw_ball(55,100-starty+(selected*15)-2,cnt);
				draw_ball(320-75,100-starty+(selected*15)-2,cnt);
				cnt++;
				if (cnt>23) cnt=0;
			}
		};
		MIDASplaySample(klikwav,MIDAS_CHANNEL_AUTO,0,22500,EFFECT_VOLUME,MIDAS_PAN_MIDDLE);
		if (k.state[94]) {selected--;if (selected<0) selected=Menu_texts-1;k.state[94]=0;}
		if (k.state[99]) {selected++;if (selected>Menu_texts-1) selected=0;k.state[99]=0;}
		if (k.state[28])
		{
             if (selected==0) {GAME_MODE=ONE_PLAYER;KILLING_MODE=COOPERATIVE;set_keys();episode=choose_episode();game();menu_scr();}
             if (selected==1) {multiplayer_options();menu_scr();}
             if (selected==2) {options();menu_scr();}
             if (selected==3) quit=1;
		}
	}
	fadeout(virbuff,pal);
}
 
void credits()
{
	cout<<"\n";
	cout<<"              Thanks for playing --The Ultimate TK-- (Tapan Kaikki) !  \n";
	cout<<"\n";
	cout<<"	Credits: \n";
	cout<<"\n";
	cout<<"	Coding: Allu and Unski \n";
	cout<<"	Graphics: Allu \n";
	cout<<"	Music: Allu and Unski \n";
	cout<<"	Sound effects: Not from DOOM \n";
	cout<<"	Thanks to MIDAS the best Sound System !!! \n";
	cout<<"	The game name idea by Janne Laitinen (PM rules !) \n";
	cout<<"	New episode and some ideas by Blanket \n";  
	cout<<"	Also thanks to Jaakko Pimperi for new blocks \n";
	cout<<"	EF would also like to thank Niko Kotilainen, etc.\n";
	cout<<"\n";
	cout<<"	Contact the Error Free Productions by e-mail: \n";
	cout<<"\n";
	cout<<"	 ef@tuusniemi.fi \n";
	cout<<"	 allu@tuusniemi.fi \n";
	cout<<"	 unski@tuusniemi.fi \n";
	cout<<"\n";
	cout<<"	And visit the Error Free Productions homepage: \n";
	cout<<"\n";
	cout<<"	 http://www.tuusniemi.fi/~ef \n";
}

void logo()
{
	int a=0,oclock;

	load_efp("efps\\eflogo.efp",picture,0);
	load_efp_pal("efps\\eflogo.efp",pal);
	setpal(pal,0);
	memcpy(screen,picture,64000);

	oclock=clock();

	while (a<255)
	if (oclock!=clock()) 
	{
		oclock=clock();
		a+=10;
		setpal(pal,a);
	}

	while (! k.kbhit() && abs(oclock-clock())<500) ;
 
	while (a>0)
	if (oclock!=clock()) 
	{
		oclock=clock();
		a-=10;
		setpal(pal,a);
	}
}

main(int argc, char *argv[])
{
 int a=0,b;
 int pause=0;

 for (a=1;a<argc;a++)
 { strupr(argv[a]);
   if ((argv[a][0]=='-')||
        (argv[a][0]=='/'))
	switch(argv[a][1])
	{ case 'F':
	      b=atoi(argv[++a]);
	      if (b>=30 && b<=500) target_frames=b;
	  break;

	  case 'L':
	  	  b=atoi(argv[++a]);
		  if (b>0) START_LEVEL=b-1;
	  break;

	  case 'P':
		  pause=1;
	  break;

	  case 'M':
	      SHOW_ENEMIES=1;
	  break;

	  case 'V':
	      FORCE_0x13=1;
	  break;

	  case 'H':
	  case '?':
	      cout << endl;
	  	  cout <<"Ultimate Tapan Kaikki "<< Version << endl;
		  cout <<"Compiled at " << __DATE__ << ' ' << __TIME__ << endl;
	  	  cout <<"parameters:"<< endl;
		  cout <<"-?,-h this help" << endl;
	  	  cout <<"-v    force mcga mode"<< endl;
	  	  cout <<"-p    pause at startup"<< endl;
		  cout <<"-m    show enemies on map"<< endl;
	  	  cout <<"-l ## start at level ##"<< endl;
	  	  cout <<"-f ## set gamespeed to ## fps (def. 40)"<< endl;
		  cout << endl;
	  	  exit(1);
	  break;
    }

  }

 randomize();
 do_all();
 k.clear_stack();

 if (pause) k.getch();

 vesa_set_mode(0x13);

// logo();
 menu();
 
 if (ph) MIDASstopModule(ph);

 vesa_set_mode(0x3);
 free_all();
 credits();
}


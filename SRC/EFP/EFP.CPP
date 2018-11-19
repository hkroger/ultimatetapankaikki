#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EFP.H"
#include "../ERROR/ERROR.H"

void load_efp(char *name,char *kuva,int offs){

char header[7];
unsigned int p;
unsigned short w,h;

short tavu,tavu2;
FILE *efp=fopen(name,"rb");

if (!efp) error("File %s is missing\n",name);

fseek(efp,offs,SEEK_SET);

fread(header,6,1,efp);
if (strncmp(header,"EF pic",6) ) error("File is not an EF pic!\n");

fseek(efp,6+offs,SEEK_SET);

fread(&w,2,1,efp);
fread(&h,2,1,efp);

for(p=0;p<w*h;){
tavu=(short)fgetc(efp);
if (tavu>192) { 
 tavu2=(short)fgetc(efp);
 memset(kuva+p,tavu2,tavu-192);
 p+=tavu-192;
  } else kuva[p++]=(char)tavu;
  } 
fclose(efp);

}


void load_efp_pal(char *name,char *paletti){
short c;
FILE *efp=fopen(name,"rb");
fseek(efp,-768,SEEK_END);
 for(c=0;c<768;c++) paletti[c]=(char)fgetc(efp);
fclose(efp);
}


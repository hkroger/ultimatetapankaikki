#ifndef __TK___WRITE__
#define __TK___WRITE__

void pixeli(int x,int y,char col);
void loadfonts(char *name);
void writefonts(int x,int y,char *str,int cq);
void writefonts2(int x,int y,char *str,int cq);
int str_length(char *str);
void readline(int x,int y,int len,char *str,char *screen);
void freefonts();

#endif

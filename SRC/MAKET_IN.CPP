#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "EFP/EFP.H"
#include "RGBHSL.H"
#include "GLOBVAR.H"

static char get_closest(int re,int gr,int bl){
	int a,v,col,minv=1000;
	char closest=0;

	for (a=0,col=0;a<256;a++,col+=3){
		v=abs(pal[col]-re)+abs(pal[col+1]-gr)+abs(pal[col+2]-bl);
		if (v<minv) {minv=v;closest=a;}
	}

	return(closest);
}

static void do_trans_table(){
	int a,b,r1,g1,b1,r2,g2,b2,col1,col2;
	for (a=0,col1=0;a<256;a++,col1+=3){
		if (a%16==0) std::cout<<".";
		r1=pal[col1];
		g1=pal[col1+1];
		b1=pal[col1+2];
		for (b=0,col2=0;b<256;b++,col2+=3){
			r2=pal[col2];
			g2=pal[col2+1];
			b2=pal[col2+2];
			trans_table[a][b]=get_closest((r1+r2)>>1,(g1+g2)>>1,(b1+b2)>>1);
		}

	}
}

static void do_shadow_table(){
	int a,z;
	float r,g,b,h,s,l,e;

	for (a=0;a<256;a++){
		r=pal[a*3];
		g=pal[a*3+1];
		b=pal[a*3+2];
		rgb2hsl(r,g,b,&h,&s,&l);
		e=l/16;
		for (z=0;z<16;z++){
			l-=e/2.5;
			if (l<0) l=0;
			hsl2rgb(h,s,l,&r,&g,&b);
			shadow_table[a][z]=get_closest(r,g,b);
		}
	}

}

static void make_light_table(Light_table *tab, int R,int G,int B){


	int a,b;
	float re,gr,bl,h,s,l;

	for (a=0;a<256;a++){

		if (a%(16*3)==0) std::cout<<".";

		re=pal[a*3]+R;
		gr=pal[(a*3)+1]+G;
		bl=pal[(a*3)+2]+B;

		if (re>63) re=63;
		if (gr>63) gr=63;
		if (bl>63) bl=63;

		if (re<0) re=0;
		if (gr<0) gr=0;
		if (bl<0) bl=0;

		rgb2hsl(re,gr,bl,&h,&s,&l);
		for (b=0;b<16;b++){

			if (R==0 && G==0 && B==0)
				if (b>0) {
					l+=((float)(b)/255)/2;
					if (l>1) l=1;
					hsl2rgb(h,s,l,&re,&gr,&bl);
				}

			re+=R/2;
			gr+=G/2;
			bl+=B/2;

			if (re>63) re=63;
			if (gr>63) gr=63;
			if (bl>63) bl=63;

			if (re<0) re=0;
			if (gr<0) gr=0;
			if (bl<0) bl=0;

			tab->data[a][b]=get_closest(re,gr,bl);
		}

	}

}

static void make_explosive_light_table(Light_table *tab){ // explosive palette

	int a,b,re,gr,bl,ra,ga,ba;

	for (a=0;a<256;a++){

		ra=20;
		ga=0;
		ba=0;

		re=pal[a*3];
		gr=pal[(a*3)+1];
		bl=pal[(a*3)+2];

		for (b=0;b<16;b++){

			if (b>1) {
				if (b<9){
					ra+=1;
					ga+=5;
					ba+=1;
				}
				if (b>8)
					ba+=5;

// re=(pal[a*3]+ra)/2;
// gr=(pal[(a*3)+1]+ga)/2;
// bl=(pal[(a*3)+2]+ba)/2;

				re=(pal[a*3]+ra);
				gr=(pal[(a*3)+1]+ga);
				bl=(pal[(a*3)+2]+ba);

			}

			if (re>63) re=63;
			if (gr>63) gr=63;
			if (bl>63) bl=63;

			tab->data[a][b]=get_closest(re,gr,bl);
		}

	}
}

void make_light_tables(){
	load_efp_pal("EFPS/WALLS1.EFP",pal);
	do_trans_table();
	do_shadow_table();
	make_light_table(&normal_l, 0, 0, 0);
	make_light_table(&red_l, 5, 0, 0);
	make_light_table(&yellow_l, 5, 5, 0);
	make_explosive_light_table(&explo_l);
}


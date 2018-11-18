#ifndef __RGBHSL__H_
#define __RGBHSL__H_

extern "C"
{
void rgb2hsl(float  r,float  g,float  b, float  *H, float  *S,float  *L );
static float  HuetoRGB(float  m1, float  m2, float  h );
void hsl2rgb(float  H, float  S, float  L,float  *R,float  *G,float  *B);
}

#endif

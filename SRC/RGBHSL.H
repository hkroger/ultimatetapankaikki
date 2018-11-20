#ifndef __RGBHSL__H_
#define __RGBHSL__H_

void rgb2hsl( float r, float g, float b, float *H, float *S, float *L );

void hsl2rgb( float H, float S, float L, float *R, float *G, float *B );

#endif

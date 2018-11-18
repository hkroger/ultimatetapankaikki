#include < math.h > 
#define max( a, b ) ( a > b?a:b ) 
#define min( a, b ) ( a < b?a:b ) 
void rgb2hsl( float r, float g, float b, float *H, float *S, float *L ) 
{
	float delta;
	float cmax;
	float cmin;
	r /= 63;
	g /= 63;
	b /= 63;
	cmax = max( r, max( g, b )  );
	cmin = min( r, min( g, b )  );
*	L = ( cmax + cmin )  / 2.0;
	if( cmax == cmin ) 
	{
		*S = 0;
*		H = 0;// it's really undefined

	}
	else 
	{
		if( *L < 0.5 ) 
*		S = ( cmax - cmin )  / ( cmax + cmin );
		else
*		S = ( cmax - cmin )  / ( 2.0 - cmax - cmin );
		delta = cmax - cmin;
		if( r == cmax ) 
*		H = ( g - b )  / delta;
		else if( g == cmax ) 
*		H = 2.0 + ( b - r )  / delta;
		else
*		H = 4.0 + ( r - g )  / delta;
*		H /= 6.0;
		if( *H < 0.0 ) 
*		H += 1;
	}
}

static float HuetoRGB( float m1, float m2, float h ) 
{
	if( h < 0 ) h += 1.0;
	if( h > 1 ) h -= 1.0;
	if( 6.0*h < 1 ) 
	return ( m1 + ( m2 - m1 ) *h*6.0 );
	if( 2.0*h < 1 ) 
	return m2;
	if( 3.0*h < 2.0 ) 
	return ( m1 + ( m2 - m1 ) *( ( 2.0 / 3.0 )  - h ) *6.0 );
	return m1;
}

void hsl2rgb( float H, float S, float L, float *R, float *G, float *B ) 
{
	float m1, m2;
	if( S == 0 ) 
	{
		*R = *G = *B = L;
	}
	else 
	{
		if( L <= 0.5 ) 
		m2 = L*( 1.0 + S );
		else
		m2 = L + S - L*S;
		m1 = 2.0*L - m2;
*		R = HuetoRGB( m1, m2, H + 1.0 / 3.0 );
*		G = HuetoRGB( m1, m2, H );
*		B = HuetoRGB( m1, m2, H - 1.0 / 3.0 );
	}
*	R*=63;
*	G*=63;
*	B*=63;
}


/***************************************************************************
                          nil_math.h  -  description
                             -------------------
    begin                : Sun Jan 9 2000
    copyright            : (C) 2000 by Flemming Frandsen
    email                : dion@swamp.dk
 ***************************************************************************/

#ifndef GMATH_H
#define GMATH_H

#include <math.h>
#include "SDL.h"

//some general purpose macros:
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define SIGN(x) (((x) > 0)?1:(((x) < 0)?-1:0))
#define ABS(x) ((x) < 0?-(x):(x))
#define SQR(x) ((x)*(x))
#define ROUND(x) ((int)((x)+0.5))
#define CLAMP(x,min,max) ((x)<(min)?(min):((x)>(max)?(max):(x)))

#define PI (float)3.14159265


#define STEP (PI/8)
#define STEP_HALF (STEP/2)
#define MINELEVATION (STEP*2)
#define MAXELEVATION PI

typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} rect_t;

class DECLSPEC Vector {
	public:
	
	float x;
	float y;

	void zero() 				{x=0;y=0;}
	void add(Vector &v) 		{x += v.x; y += v.y;}
	float length() 		        {return (float)sqrt(SQR(x)+SQR(y));}
	void normalize() 			{float len=length(); if (len==0) return; x/=len; y/=len;};
	void scale(float factor)	{x*=factor;y*=factor;}
	void rotate(float angle);
};


int minimum(int x, int y);
float dist(float x0, float y0, float x1, float y1);

bool point_in_rect(SDL_Rect * r, Uint16 x, Uint16 y);
bool point_in_rect(Uint16 x, Uint16 y, Uint16 w, Uint16 h, Uint16 px, Uint16 py);

bool RectOverlap(rect_t * r1, rect_t * r2);

Vector rad2vector(float rad);
Vector rot90ccw(Vector v);
int elev2dir(float elev, int dir);
float elev2rad(float elev, int dir);
Vector normalize(Vector v);

//=============================================================================
int triangle_direction (
	float ax, float ay,
	float bx, float by,
	float cx, float cy
);

//=============================================================================
bool point_in_triangle(
	float px, float py,
	float ax, float ay,
	float bx, float by,
	float cx, float cy
);

//=============================================================================
bool lines_intersect(
	long x1, long y1,   /* First line segment */
	long x2, long y2,

	long x3, long y3,   /* Second line segment */
	long x4, long y4,

	long *x, long *y    /* Output value:* point of intersection */
);


#endif

/***************************************************************************
                          nil_math.cpp  -  description
                             -------------------
    begin                : Sun Jan 9 2000
    copyright            : (C) 2000 by Flemming Frandsen
    email                : dion@swamp.dk
 ***************************************************************************/

//=============================================================================
/* lines_intersect, stolen from Graphic Gems 2
 *
 *   This function computes whether two line segments,
 *   respectively joining the input points (x1,y1) -- (x2,y2)
 *   and the input points (x3,y3) -- (x4,y4) intersect.
 *   If the lines intersect, the output variables x, y are
 *   set to coordinates of the point of intersection.
 *
 *   All values are in integers.  The returned value is rounded
 *   to the nearest integer point.
 *
 *   If non-integral grid points are relevant, the function
 *   can easily be transformed by substituting floating point
 *   calculations instead of integer calculations.
 *
 *   Entry
 *        x1, y1,  x2, y2   Coordinates of endpoints of one segment.
 *        x3, y3,  x4, y4   Coordinates of endpoints of other segment.
 *
 *   Exit
 *        x, y              Coordinates of intersection point.
 *
 *   The value returned by the function is one of:
 *
 *        DONT_INTERSECT    0
 *        DO_INTERSECT      1
 *        COLLINEAR         2
 *
 * Error conditions:
 *
 *     Depending upon the possible ranges, and particularly on 16-bit
 *     computers, care should be taken to protect from overflow.
 *
 *     In the following code, 'long' values have been used for this
 *     purpose, instead of 'int'.
 *
 */

#include "gmath.h"

/**************************************************************
 *                                                            *
 *    NOTE:  The following macro to determine if two numbers  *
 *    have the same sign, is for 2's complement number        *
 *    representation.  It will need to be modified for other  *
 *    number systems.                                         *
 *                                                            *
 **************************************************************/

#define SAME_SIGNS( a, b )	\
		(((long) ((unsigned long) a ^ (unsigned long) b)) >= 0 )


bool lines_intersect(
	long x1, long y1,   /* First line segment */
	long x2, long y2,

	long x3, long y3,   /* Second line segment */
	long x4, long y4,

	long *x,        /* Output value:* point of intersection */
	long *y
)
{
    /* Compute a1, b1, c1, where line joining points 1 and 2
     * is "a1 x  +  b1 y  +  c1  =  0".
     */

    long a1 = y2 - y1;
    long b1 = x1 - x2;
    long c1 = x2 * y1 - x1 * y2;

    /* Compute r3 and r4.
     */
    long r3 = a1 * x3 + b1 * y3 + c1;
    long r4 = a1 * x4 + b1 * y4 + c1;

    /* Check signs of r3 and r4.  If both point 3 and point 4 lie on
     * same side of line 1, the line segments do not intersect.
     */

    if ( r3 != 0 &&
         r4 != 0 &&
         SAME_SIGNS( r3, r4 ))
        return false;

    /* Compute a2, b2, c2 */

    long a2 = y4 - y3;
    long b2 = x3 - x4;
    long c2 = x4 * y3 - x3 * y4;

    /* Compute r1 and r2 */

    long r1 = a2 * x1 + b2 * y1 + c2;
    long r2 = a2 * x2 + b2 * y2 + c2;

    /* Check signs of r1 and r2.  If both point 1 and point 2 lie
     * on same side of second line segment, the line segments do
     * not intersect.
     */

    if ( r1 != 0 &&
         r2 != 0 &&
         SAME_SIGNS( r1, r2 ))
        return false;

    /* Line segments intersect: compute intersection point.
     */

    long denom = a1 * b2 - a2 * b1;
    if ( denom == 0 )
        return false;
    long offset = denom < 0 ? - denom / 2 : denom / 2;

    /* The denom/2 is to get rounding instead of truncating.  It
     * is added or subtracted to the numerator, depending upon the
     * sign of the numerator.
     */

    long num = b1 * c2 - b2 * c1;
    *x = ( num < 0 ? num - offset : num + offset ) / denom;

    num = a2 * c1 - a1 * c2;
    *y = ( num < 0 ? num - offset : num + offset ) / denom;

    return true;
} /* lines_intersect */

//=============================================================================
int triangle_direct_tion (
	float ax, float ay,
	float bx, float by,
	float cx, float cy
)
{
	float abx = ax-bx;
	float aby = ay-by;
	float cbx = cx-bx;
	float cby = cy-by;

	float d  = abx*cby-aby*cbx;

	if (d > 0)
		return 1;
	else
		return -1;
}

//=============================================================================
bool point_in_triangle(
	float px, float py,
	float ax, float ay,
	float bx, float by,
	float cx, float cy)
{
	int d1 = triangle_direct_tion(px,py,ax,ay,bx,by);
	int d2 = triangle_direct_tion(px,py,bx,by,cx,cy);
	int d3 = triangle_direct_tion(px,py,cx,cy,ax,ay);

	return (d1 == d2) && (d2 == d3);
}

//=============================================================================
Vector rot90ccw(Vector v)
{
	Vector vr;
	vr.x = -v.y;
	vr.y = v.x;
	return vr;
}

//=============================================================================
Vector rad2vector(float rad)
{
	float mat=-rad-PI/2;
	Vector v;
	v.x = (float)sin(mat);
	v.y = (float)-cos(mat);
	return v;
}

//=============================================================================
Vector normalize(Vector v)
{
	float length = (float)sqrt(SQR(v.x)+SQR(v.y));
	if (length != 0) {
		v.x /= length;
		v.y /= length;
	}
	return v;
}





//=============================================================================
/*

0: n
1: nne
2: ne
3: ene
4: e

5:

rad: 0=north, +=cw.
*/

//dir: -1: west, +1: east
float elev2rad(float elev, int dir)
{
	if (dir < 0)
		return PI/2-elev;
	else
		return PI/2+elev;
}

//=============================================================================
float dist(float x0, float y0, float x1, float y1) {return (float)sqrt(SQR(x1-x0)+SQR(y1-y0));}

//=============================================================================
int elev2dir(float elev, int dir)
{
	if (elev > PI) elev = PI;
	if (elev < 0) elev = 0;

	if (dir > 0)
		return ROUND(8-8*elev/PI);
	else
		return ROUND(8+8*elev/PI);
}

int minimum(int x,int y)
{
	if (x < y)
		return x;
	else
		return y;
}

void Vector::rotate(float angle)
{
	float newx = (float)(x*sin(angle) - y*cos(angle));
	float newy = (float)(y*sin(angle) + x*cos(angle));
	x = newx;
	y = newy;
}


bool point_in_rect(SDL_Rect * r, Uint16 x, Uint16 y)
{
  if (x>=r->x && x<=r->x+r->w &&
      y>=r->y && y<=r->y+r->h)
    return true;
  else
    return false;
}

bool point_in_rect(Uint16 x, Uint16 y, Uint16 w, Uint16 h, Uint16 px, Uint16 py)
{
  if (px>=x && px<=x+w &&
      py>=y && py<=y+h)
    return true;
  else
    return false;
}

bool RectOverlap(rect_t * r1, rect_t * r2)
{
    if (r1->left >= r2->right || r2->left >= r1->right ||
        r1->top >= r2->bottom || r2->top >= r1->bottom)
        return false;
    else 
        return true;
}


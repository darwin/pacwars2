#include "float.h"

#include "aibase.h"

double eNumber = .001;
double eDistance = .01;
double NaN = -9999.212121;

bool FEqual(const double a, const double b)
{
  return ABS(a - b) < eNumber;
}

bool FLessEqual(const double a, const double b)
{
  return (a - b) < eNumber;
}

bool FLess(const double a, const  double b)
{
  return (a - b) < -eNumber;
}

bool LineIntersection(const CXPoint& A1, const CXPoint& A2, const CXPoint& B1, const CXPoint& B2, CXPoint* X)
{
  double x21 = A2.x - A1.x;
  double y21 = A2.y - A1.y;
  double x31 = B1.x - A1.x;
  double y13 = A1.y - B1.y;
  double x43 = B2.x - B1.x;
  double y43 = B2.y - B1.y;
  double del = x21 * y43 - x43 * y21;
  if (FEqual(del, 0))
  {
    X->x = NaN;		
    X->y = NaN;		
    return false;
  }
  double a = (x43 * y13 + x31 * y43) / del;
  double b = (x21 * y13 + x31 * y21) / del;
  
  X->x = A1.x + a * x21;
  X->y = A1.y + a * y21;
  return (a>=0 && a<=1 && b>=0 && b<=1);
}

bool LineIntersection(const CXPoint& A1, const CXPoint& A2, const CXPoint& B1, const CXPoint& B2, CXPoint* X, double *ta, double *tb)
{
  double x21 = A2.x - A1.x;
  double y21 = A2.y - A1.y;
  double x31 = B1.x - A1.x;
  double y13 = A1.y - B1.y;
  double x43 = B2.x - B1.x;
  double y43 = B2.y - B1.y;
  double del = x21 * y43 - x43 * y21;
  if (FEqual(del, 0))
  {
    X->x = NaN;		
    X->y = NaN;		
    return false;
  }
  *ta = (x43 * y13 + x31 * y43) / del;
  *tb = (x21 * y13 + x31 * y21) / del;
  
  X->x = A1.x + *ta * x21;
  X->y = A1.y + *ta * y21;
  return (*ta>=0 && *ta<=1 && *tb>=0 && *tb<=1);
}

bool LineIntersection(const CIPoint& A1, const CIPoint& A2, const CIPoint& B1, const CIPoint& B2, CXPoint* X)
{
  double x21 = A2.x - A1.x;
  double y21 = A2.y - A1.y;
  double x31 = B1.x - A1.x;
  double y13 = A1.y - B1.y;
  double x43 = B2.x - B1.x;
  double y43 = B2.y - B1.y;
  double del = x21 * y43 - x43 * y21;
  if (FEqual(del, 0))
  {
    X->x = NaN;		
    X->y = NaN;		
    return false;
  }
  double a = (x43 * y13 + x31 * y43) / del;
  double b = (x21 * y13 + x31 * y21) / del;
  
  X->x = A1.x + a * x21;
  X->y = A1.y + a * y21;
  return (a>=0 && a<=1 && b>=0 && b<=1);
}

bool LineIntersection(const CIPoint& A1, const CIPoint& A2, const CIPoint& B1, const CIPoint& B2, CXPoint* X, double *ta, double *tb)
{
  double x21 = A2.x - A1.x;
  double y21 = A2.y - A1.y;
  double x31 = B1.x - A1.x;
  double y13 = A1.y - B1.y;
  double x43 = B2.x - B1.x;
  double y43 = B2.y - B1.y;
  double del = x21 * y43 - x43 * y21;
  if (FEqual(del, 0))
  {
    X->x = NaN;		
    X->y = NaN;		
    return false;
  }
  *ta = (x43 * y13 + x31 * y43) / del;
  *tb = (x21 * y13 + x31 * y21) / del;
  
  X->x = A1.x + *ta * x21;
  X->y = A1.y + *ta * y21;
  return (*ta>=0 && *ta<=1 && *tb>=0 && *tb<=1);
}


bool PointAtLine(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, double* t)
{
  if (A1.x==A2.x && A1.y==A2.y) return false;
  if (A1.y - A2.y)
  {
    *t = (P.y - A1.y) / (A1.y - A2.y);
    if (P.x - A1.x == *t*(A1.x - A2.x))
      return true;
    else
      return false;
  }
  else
  {
    *t = (P.x - A1.x) / (A1.x - A2.x);
    if (P.y - A1.y == *t*(A1.y - A2.y))
      return true;
    else
      return false;
  }
}

bool IsPointOnLine(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, double* t)
{
  CXPoint Base;
  PerpendicularBase(A1, A2, P, &Base, t);
  return IsSamePoint(Base, P);
}

bool PerpendicularBase(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, CXPoint* Base)
{
  double  ix, iy, jx, jy, t;
  
  ix = A2.x - A1.x;
  iy = A2.y - A1.y;
  jx = A1.x -  P.x;
  jy = A1.y -  P.y;
  
  t = - (ix * jx + iy * jy) / (SQR(ix) + SQR(iy));
  
  Base->x = A1.x + t * ix;
  Base->y = A1.y + t * iy;
  
  return (t>=0 && t<=1);
}

bool PerpendicularBase(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, CXPoint* Base, double *t)
{
  double  ix, iy, jx, jy;
  
  ix = A2.x - A1.x;
  iy = A2.y - A1.y;
  jx = A1.x -  P.x;
  jy = A1.y -  P.y;
  
  (*t) = - (ix * jx + iy * jy) / (SQR(ix) + SQR(iy));
  
  Base->x = A1.x + (*t) * ix;
  Base->y = A1.y + (*t) * iy;
  
  return ((*t)>=0 && (*t)<=1);
}

double EuclideanDistance(const CXPoint& A, const CXPoint& B)
{
  double dx = A.x - B.x;
  double dy = A.y - B.y;
  
  return (sqrt(SQR(dx) + SQR(dy)));
}

bool IsSamePoint(const CXPoint& A, const CXPoint& B)
{
  return EuclideanDistance(A, B) < eDistance;
}

CXVector Rad2Vector(const double rad)
{
  return CXVector(sin(rad), cos(rad));
}

double LinearDependent(const CXVector& A, const CXVector& B)
{
  return A.LinearDependent(B);
}

double HalfPlane(const CXPoint& A, const CXPoint& B, const CXPoint& P)
{
  return (B.y - A.y)*(P.x - A.x) - (B.x - A.x)*(P.y - A.y); // dot product of normal and P-A vector
}

//-----------------------------------------------------------------------------
// CXPoint

CXPoint::CXPoint(const CXVector& v)
{
  x = v.x;
  y = v.y;  
}

CXPoint CXPoint::operator+(const CXVector& rs) const 
{ 
  return CXPoint(x+rs.x, y+rs.y); 
}

CXPoint CXPoint::operator-(const CXVector& rs) const   
{ 
  return CXPoint(x-rs.x, y-rs.y); 
}

CXVector CXPoint::operator-(const CXPoint& rs) const  
{ 
  return CXVector(x-rs.x, y-rs.y); 
}

CXPoint CXPoint::operator+=(const CXVector& rs)  
{ 
  return CXPoint(x+=rs.x, y+=rs.y); 
}

CXPoint CXPoint::operator-=(const CXVector& rs)  
{ 
  return CXPoint(x-=rs.x, y-=rs.y); 
}

void CXPoint::Add(const CXVector& v)
{ 
  x += v.x; 
  y += v.y; 
}

//-----------------------------------------------------------------------------
// CIPoint

CIPoint::CIPoint(const CIVector& v)
{
  x = v.x;
  y = v.y;  
}

CIPoint CIPoint::operator+(const CIVector& rs) const 
{ 
  return CIPoint(x+rs.x, y+rs.y); 
}

CIPoint CIPoint::operator-(const CIVector& rs) const   
{ 
  return CIPoint(x-rs.x, y-rs.y); 
}

CIVector CIPoint::operator-(const CIPoint& rs) const  
{ 
  return CIVector(x-rs.x, y-rs.y); 
}

CIPoint CIPoint::operator+=(const CIVector& rs)  
{ 
  return CIPoint(x+=rs.x, y+=rs.y); 
}

CIPoint CIPoint::operator-=(const CIVector& rs)  
{ 
  return CIPoint(x-=rs.x, y-=rs.y); 
}

void CIPoint::Add(const CIVector& v)
{ 
  x += v.x; 
  y += v.y; 
}



//-----------------------------------------------------------------------------
// CXVector

void CXVector::Rotate(const double rad)
{
  double newx = x*cos(rad) + y*sin(rad);
  double newy = -x*sin(rad) + y*cos(rad);
  //	  double newx = x*sin(rad) - y*cos(rad);
  //	  double newy = y*sin(rad) + x*cos(rad);
  x = newx;
  y = newy;
}

double CXVector::Angle()
{
  Normalize();
  
  double ac = acos(x);
  double as = asin(y);
  
  return as>=0?ac:2*PI-ac;
}

double CXVector::LinearDependent(const CXVector& rs) const 
{
  if ((FEqual(rs.x,0) && !FEqual(x,0))) return 0;
  if ((FEqual(rs.y,0) && !FEqual(y,0))) return 0;
  return FEqual(x / rs.x, y / rs.y) ? x / rs.x : 0;
}

//-----------------------------------------------------------------------------
// CIVector

double CIVector::LinearDependent(const CIVector& rs) const 
{
  if (rs.x==0 && x!=0) return 0;
  if (rs.y==0 && y!=0) return 0;
  return x / (double)rs.x==y / (double)rs.y ? x / rs.x : 0;
}


//-----------------------------------------------------------------------------
// CXRect

void CXRect::Normalize()  // make [A,B] points [upper left, lower right]
{
  if (A.x<B.x) Swap(A.x, B.x);
  if (A.y<B.y) Swap(A.y, B.y);
}

// following functions need normalized rectangle

// intersect
bool CXRect::Intersect(double x, double y) // point intersection
{
  if (A.x<=x && x<=B.x && A.y<=y && y<=B.y) return true; else return false;
}

bool CXRect::Intersect(const CXPoint& C) // point intersection
{
  return Intersect(C.x, C.y);
}

bool CXRect::Intersect(const CXPoint& C, const CXPoint& D)  // line intersection
{
  if (Sector(A.x, A.y)&Sector(B.x, B.y)) return false;
  CXPoint X;
  if (LineIntersection(C,D, CXPoint(A.x, DBL_MIN), CXPoint(A.x, DBL_MAX), &X)) return true;
  if (LineIntersection(C,D, CXPoint(B.x, DBL_MIN), CXPoint(B.x, DBL_MAX), &X)) return true;
  if (LineIntersection(C,D, CXPoint(DBL_MIN, A.y), CXPoint(DBL_MAX, A.y), &X)) return true;
  if (LineIntersection(C,D, CXPoint(DBL_MIN, B.y), CXPoint(DBL_MAX, B.y), &X)) return true;
  return false;
}

bool CXRect::Intersect(const CXLine& L) // line intersection
{
  return Intersect(L.A, L.B);
}

bool CXRect::Intersect(const CXRect& R) // rect intersection
{
  if (Sector(R.A.x, R.A.y)&Sector(R.B.x, R.B.y)) return false;
  return true;
}

// union
void CXRect::Union(double x, double y) // union with point 
{
  A.x = MIN(A.x, x);
  A.y = MIN(A.y, y);
  B.x = MAX(B.x, x);
  B.y = MAX(B.y, y);
}

void CXRect::Union(const CXPoint& A) // union with point 
{
  Union(A.x, A.y);
}

void CXRect::Union(const CXPoint& A, const CXPoint& B)  // union with line
{
  Union(A);
  Union(B);
}

void CXRect::Union(const CXLine& L) // union with line
{
  Union(L.A);
  Union(L.B);
}

void CXRect::Union(const CXRect& R) // union with rect
{
  Union(R.A);
  Union(R.B);
}

// special
char inline CXRect::Sector(double x, double y)
{
  char s = 0;
  if (x<A.x) s|=8;      // 1000
  else
    if (x>B.x) s|=4;    // 0100
    if (y<A.y) s|=1;      // 0001
    else
      if (y>B.y) s|=2;    // 0010
      return s;
}

//-----------------------------------------------------------------------------
// CIRect

void CIRect::Normalize()  // make [A,B] points [upper left, lower right]
{
  if (A.x<B.x) Swap(A.x, B.x);
  if (A.y<B.y) Swap(A.y, B.y);
}

// following functions need normalized rectangle

// intersect
bool CIRect::Intersect(double x, double y) // point intersection
{
  if (A.x<=x && x<=B.x && A.y<=y && y<=B.y) return true; else return false;
}

bool CIRect::Intersect(const CIPoint& C) // point intersection
{
  return Intersect(C.x, C.y);
}

bool CIRect::Intersect(const CIPoint& C, const CIPoint& D)  // line intersection
{
  if (Sector((int)A.x, (int)A.y)&Sector((int)B.x, (int)B.y)) return false;
  CXPoint X;
  if (LineIntersection(C,D, CIPoint(A.x, DBL_MIN), CIPoint(A.x, DBL_MAX), &X)) return true;
  if (LineIntersection(C,D, CIPoint(B.x, DBL_MIN), CIPoint(B.x, DBL_MAX), &X)) return true;
  if (LineIntersection(C,D, CIPoint(DBL_MIN, A.y), CIPoint(DBL_MAX, A.y), &X)) return true;
  if (LineIntersection(C,D, CIPoint(DBL_MIN, B.y), CIPoint(DBL_MAX, B.y), &X)) return true;
  return false;
}

bool CIRect::Intersect(const CILine& L) // line intersection
{
  return Intersect(L.A, L.B);
}

bool CIRect::Intersect(const CIRect& R) // rect intersection
{
  if (Sector((int)R.A.x, (int)R.A.y)&Sector((int)R.B.x, (int)R.B.y)) return false;
  return true;
}

// union
void CIRect::Union(int x, int y) // union with point 
{
  A.x = MIN(A.x, x);
  A.y = MIN(A.y, y);
  B.x = MAX(B.x, x);
  B.y = MAX(B.y, y);
}

void CIRect::Union(const CIPoint& A) // union with point 
{
  Union((int)A.x, (int)A.y);
}

void CIRect::Union(const CIPoint& A, const CIPoint& B)  // union with line
{
  Union(A);
  Union(B);
}

void CIRect::Union(const CILine& L) // union with line
{
  Union(L.A);
  Union(L.B);
}

void CIRect::Union(const CIRect& R) // union with rect
{
  Union(R.A);
  Union(R.B);
}

// special
char inline CIRect::Sector(int x, int y)
{
  char s = 0;
  if (x<A.x) s|=8;        // 1000
  else
    if (x>B.x) s|=4;      // 0100
    if (y<A.y) s|=1;      // 0001
    else
      if (y>B.y) s|=2;    // 0010
      return s;
}


//-----------------------------------------------------------------------------
// CIWrapRect

void CIWrapRect::Normalize()  // make [A,B] points [upper left, lower right]
{
  for (iterator it=begin(); it!=end(); it++) it->Normalize();
}

void CIWrapRect::Decompose(CIPoint A, CIPoint B)
{
  clear();

  while (A.x<0) A.x+=bx;
  while (A.y<0) A.y+=by;
  while (B.x<A.x) B.x+=bx;
  while (B.y<A.y) B.y+=by;
  while (B.x-bx>A.x) B.x-=bx;
  while (B.y-by>A.y) B.y-=by;

  if (B.x<bx)
  {
    if (B.y<by)
    { // no decomposition takes effect
      push_back(CIRect(A,B));
    }
    else
    {  // decompose in y
      push_back(CIRect(A, CIPoint(B.x,by-1)));
      push_back(CIRect(CIPoint(A.x,0), B));
    }
  }
  else
  {
    if (B.y<by)
    { // decompose in x
      push_back(CIRect(A, CIPoint(bx-1,B.y)));
      push_back(CIRect(CIPoint(0, A.y), B));
    }
    else
    {  // decompose both in x and in y
      push_back(CIRect(A, CIPoint(bx-1,by-1)));
      push_back(CIRect(CIPoint(0,0), B));
      push_back(CIRect(CIPoint(0,A.y), CIPoint(B.x,by-1)));
      push_back(CIRect(CIPoint(A.x,0), CIPoint(bx-1,B.y)));
    }
  }

}


// following functions need normalized rectangle

// intersect
bool CIWrapRect::Intersect(int x, int y) // point intersection
{
  for (iterator it=begin(); it!=end(); it++) if (it->Intersect(x, y)) return true;
  return false;
}

bool CIWrapRect::Intersect(const CIPoint& C) // point intersection
{
  return Intersect((int)C.x, (int)C.y);
}

//-----------------------------------------------------------------------------
// CXBoundingBox

bool CXBoundingBox::Intersect(const CXBoundingBox& R) // BBox intersection
{
  if (valid && R.valid) return CXRect::Intersect(R);
  else return true;
}

void CXBoundingBox::Union(const CXBoundingBox& R)
{
  if (valid && R.valid) CXRect::Union(R);
  else valid = false;
}

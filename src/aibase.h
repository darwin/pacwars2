/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   structures for AI implementation
//## author:  Antonin Hildebrand
//##         
//## started: 16.6.2001
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _AIBASE_H_
#define _AIBASE_H_

#include <math.h>
#include <float.h>
#include <vector>

using namespace std;
// TODO: optimize, optimize and optimize

#ifndef SIGN
#define SIGN(x) (((x) > 0)?1:(((x) < 0)?-1:0))
#endif

#ifndef ABS
  #define ABS(x) ((x) < 0?-(x):(x))
#endif

#ifndef SQR
  #define SQR(x) ((x)*(x))
#endif

#ifndef ROUND
  #define ROUND(x) ((int)((x)+0.5))
#endif

#ifndef CLAMP
  #define CLAMP(x,min,max) ((x)<(min)?(min):((x)>(max)?(max):(x)))
#endif

#ifndef PI
  #define PI (double)3.14159265
#endif

#ifndef MIN
  #define MIN(a,b) ((a)<(b)?a:b)
#endif

#ifndef MAX
  #define MAX(a,b) ((a)>(b)?a:b)
#endif

extern double eNumber;		// max rounding error
extern double eDistance;	// max rounding error in point distance
extern double NaN;			  // Not a Number !!!

bool FEqual(const double a, const double b);
bool FLessEqual(const double a, const double b);
bool FLess(const double a, const double b);

// needed forward declarations
class CXVector;
class CXPoint;
class CXLine;
class CXRect;
class CXBoundingBox;

class CIVector;
class CIPoint;
class CILine;
class CIRect;

// math functions
bool        LineIntersection(const CXPoint& A1, const CXPoint& A2, const CXPoint& B1, const CXPoint& B2, CXPoint* X);
bool        LineIntersection(const CXPoint& A1, const CXPoint& A2, const CXPoint& B1, const CXPoint& B2, CXPoint* X, double *ta, double *tb);
bool        LineIntersection(const CIPoint& A1, const CIPoint& A2, const CIPoint& B1, const CIPoint& B2, CXPoint* X);
bool        LineIntersection(const CIPoint& A1, const CIPoint& A2, const CIPoint& B1, const CIPoint& B2, CXPoint* X, double *ta, double *tb);
double      EuclideanDistance(const CXPoint& A, const CXPoint& B);
bool        IsSamePoint(const CXPoint& A, const CXPoint& B);
bool        PerpendicularBase(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, CXPoint* Base);
bool        PerpendicularBase(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, CXPoint* Base, double *t);
bool        IsPointOnLine(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, double* t);
bool        PointAtLine(const CXPoint& A1, const CXPoint& A2, const CXPoint& P, double* t);
double      LinearDependent(const CXVector& A, const CXVector& B);
CXVector    Rad2Vector(const double rad);
double      HalfPlane(const CXPoint& A, const CXPoint& B, const CXPoint& P);

// special funcitons
template <class T>
void Swap(T& a, T& b) { T s; s=a; a=b; b=s; }

// class declaration
class CXPoint
{
public:
  CXPoint() {}
  CXPoint(const double x0, const double y0) { x = x0; y = y0; }
  CXPoint(const CXVector& v);
  CXPoint(const CXPoint& A, const CXPoint& B, const double ratio) { Interpolate(A, B, ratio); } 
  
  CXPoint     operator+(const CXVector& rs) const;
  CXPoint     operator-(const CXVector& rs) const;
  CXVector    operator-(const CXPoint& rs) const;
  CXPoint     operator+=(const CXVector& rs);
  CXPoint     operator-=(const CXVector& rs);
  
  bool        operator==(const CXPoint& p) const { return IsSamePoint(*this, p); } 
  bool        operator!=(const CXPoint& p) const { return !IsSamePoint(*this, p); }
  
  CXPoint&    operator=(const CXPoint& rs) { x = rs.x; y = rs.y; return *this; }
  
  void        Zero() { x=0; y=0; }
  void        Add(const CXVector& v);	  
  void        SetXY(const double nx, const double ny) { x=nx; y=ny; }
  void        SetX(const double nx) { x = nx; }
  void        SetY(const double ny) { y = ny; }
  double      GetX() const { return x; }
  double      GetY() const { return y; }
  
  void        Interpolate(const CXPoint& A, const CXPoint& B, const double ratio) { x = A.x + (B.x-A.x) * ratio; y = A.y + (B.y-A.y) * ratio; }
  bool        ObjectInsideArea(const double x1, const double y1, const double x2, const double y2) { return (x1 <= x) && (x2 >= x) && (y1 <= y) && (y2 >= y); }
  
  double      x;    // point x-coordinate
  double      y;    // point y-coordinate
};

class CIPoint
{
public:  
  CIPoint() {}
  CIPoint(const double x0, const double y0) { x = x0; y = y0; }
  CIPoint(const CIVector& v);
  CIPoint(const CIPoint& A, const CIPoint& B, const int ratio) { Interpolate(A, B, ratio); } 
  
  CIPoint     operator+(const CIVector& rs) const;
  CIPoint     operator-(const CIVector& rs) const;
  CIVector    operator-(const CIPoint& rs) const;
  CIPoint     operator+=(const CIVector& rs);
  CIPoint     operator-=(const CIVector& rs);
  CIPoint&    operator=(const CIPoint& rs) { x = rs.x; y = rs.y; return *this; }
              operator CXPoint() { return CXPoint(x,y); }
              //operator const CXPoint&() { return CXPoint(x,y); }
  
  void        Zero() { x=0; y=0; }
  void        Add(const CIVector& v);	  
  void        SetXY(const int nx, const int ny) { x=nx; y=ny; }
  void        SetX(const int nx) { x = nx; }
  void        SetY(const int ny) { y = ny; }
  double         GetX() const { return x; }
  double         GetY() const { return y; }
  
  void        Interpolate(const CIPoint& A, const CIPoint& B, const int ratio) { x = A.x + (B.x-A.x) * ratio; y = A.y + (B.y-A.y) * ratio; }
  bool        ObjectInsideArea(const int x1, const int y1, const int x2, const int y2) { return (x1 <= x) && (x2 >= x) && (y1 <= y) && (y2 >= y); }
  
  double         x;    // point x-coordinate
  double         y;    // point y-coordinate
};

class CXVector : public CXPoint
{
public:
  CXVector() : CXPoint(0, 0) { }
  CXVector(const CXPoint& p) : CXPoint(p.x, p.y) { }
  CXVector(const CXVector& v) : CXPoint(v.x, v.y) { }
  CXVector(const double rad) : CXPoint(cos(rad), sin(rad)) {}
  CXVector(const double x, const double y) : CXPoint(x,y) { }
  
  
  CXVector    operator+(const CXVector& rs) const { return CXVector(x+rs.x, y+rs.y); }
  CXVector    operator-(const CXVector& rs) const { return CXVector(x-rs.x, y-rs.y); }
  CXVector    operator+=(const CXVector& rs) { return CXVector(x+=rs.x, y+=rs.y); }
  CXVector    operator-=(const CXVector& rs) { return CXVector(x-=rs.x, y-=rs.y); }
  CXVector    operator*(const double factor) const { return CXVector(x*factor, y*factor); }
  CXVector    operator/(const double factor) const { if (factor) return CXVector(x/factor, y/factor); else return CXVector(0,0); }
  CXVector    operator*=(const double factor) { return CXVector(x*=factor, y*=factor); }
  CXVector    operator/=(const double factor) { if (factor) return CXVector(x/=factor, y/=factor); else return CXVector(0,0); }
  CXPoint     operator+(const CXPoint& rs)  const { return CXPoint(rs.x+x, rs.y+y); }
  CIPoint     operator+(const CIPoint& rs)  const { return CIPoint(ROUND(rs.x+x), ROUND(rs.y+y)); }
  double      operator*(const CXVector& rs)     { return DotProduct(rs); }
  
  double      Length() const            { return (double)sqrt(SQR(x)+SQR(y)); }
  void        Add(const CXVector& v)  { x += v.x; y += v.y; }  // redefinition for being inline
  void        Normalize() 			        { double len=Length(); if (len==0) return; x/=len; y/=len; };
  double      Angle();  // also normalizes vector
  void        Scale(const double factor){ x*=factor; y*=factor; }
  void        Rotate(const double angle);
  double      DotProduct(const CXPoint& v2) const { return x*v2.x + y*v2.y; }
  double      LinearDependent(const CXVector& rs) const; 
  
  void        Rotate90cw()              { double sx = -x; x = y; y = sx; }
  void        Rotate90ccw()             { double sy = -y; y = x; x = sy; }
  void        Rotate180()               { x = -x; y = -y; }
  
  void        FromAngle(double rad) { y = sin(rad); x = cos(rad); }
};

class CIVector : public CIPoint
{
public:
  CIVector() : CIPoint(0, 0) { }
  CIVector(const CIPoint& p) : CIPoint(p.x, p.y) { }
  CIVector(const CIVector& v) : CIPoint(v.x, v.y) { }
  CIVector(const double rad) : CIPoint(cos(rad), sin(rad)) {}
  CIVector(const double x, const double y) : CIPoint(x,y) { }
  
  CIVector    operator+(const CIVector& rs) const { return CIVector(x+rs.x, y+rs.y); }
  CIVector    operator-(const CIVector& rs) const { return CIVector(x-rs.x, y-rs.y); }
  CIVector    operator+=(const CIVector& rs) { return CIVector(x+=rs.x, y+=rs.y); }
  CIVector    operator-=(const CIVector& rs) { return CIVector(x-=rs.x, y-=rs.y); }
  CIVector    operator*(const int factor) const { return CIVector(x*factor, y*factor); }
  CIVector    operator/(const int factor) const { if (factor) return CIVector(ROUND(x/factor), ROUND(y/factor)); else return CIVector(0,0); }
  CIVector    operator*=(const int factor) { return CIVector(x*=factor, y*=factor); }
  CIVector    operator/=(const int factor) { if (factor) return CIVector(x/=factor, y/=factor); else return CIVector(0,0); }
  CIPoint     operator+(const CIPoint& rs)  const { return CIPoint(rs.x+x, rs.y+y); }
  CIPoint     operator+(const CXPoint& rs)  const { return CIPoint(ROUND(rs.x+x), ROUND(rs.y+y)); }
  double      operator*(const CIVector& rs)     { return DotProduct(rs); }
              operator CXVector() { return CXVector(x,y); }
              //operator CXVector&() { return CXVector(x,y); }
  
  double      Length() const            { return (double)sqrt(SQR(x)+SQR(y)); }
  void        Add(const CIVector& v)  { x += v.x; y += v.y; }  // redefinition for being inline
  double      Angle();  // also normalizes vector
  void        Scale(const int factor){ x*=factor; y*=factor; }
  double      DotProduct(const CIPoint& v2) const { return x*v2.x + y*v2.y; }
  double      LinearDependent(const CIVector& rs) const; 
  
  void        Rotate90cw()              { double sx = -x; x = y; y = sx; }
  void        Rotate90ccw()             { double sy = -y; y = x; x = sy; }
  void        Rotate180()               { x = -x; y = -y; }
};

class CXParametrization
{
public:
  void      SetT( const double nt)	  { t = nt; }
  double    GetT() const   	          { return t; }
  
  double t;	  // parameter
};

class CXLine 
{
public:
  CXLine() {}
  CXLine(double AX, double AY, double BX, double BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); }
  CXLine(const CXPoint& iA, const CXPoint& iB) { A=iA; B=iB; }
  
  void Set(double AX, double AY, double BX, double BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); }
  void Set(const CXPoint& iA, const CXPoint& iB) { A=iA; B=iB; }
  void Set(const CXLine& L) { A=L.A; B=L.B; }
  
  CXPoint A;
  CXPoint B;
};

class CILine 
{
public:
  CILine() {}
  CILine(int AX, int AY, int BX, int BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); }
  CILine(const CIPoint& iA, const CIPoint& iB) { A=iA; B=iB; }
  
  void Set(int AX, int AY, int BX, int BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); }
  void Set(const CIPoint& iA, const CIPoint& iB) { A=iA; B=iB; }
  void Set(const CILine& L) { A=L.A; B=L.B; }
  
  CIPoint A;
  CIPoint B;
};

class CXRect : public CXLine
{
public:
  // A is upper left corner
  // B is lower right corner (higher values)
  void Set(const CXRect& R) { A=R.A; B=R.B; }
  
  void Normalize(); // make [A,B] points [upper left, lower right]
  
  // following functions need normalized rectangle
  
  // intersection
  bool Intersect(double x, double y); // point intersection
  bool Intersect(const CXPoint& A); // point intersection
  bool Intersect(const CXPoint& A, const CXPoint& B);  // line intersection
  bool Intersect(const CXLine& L); // line intersection
  bool Intersect(const CXRect& R); // rect intersection
  
  // union
  void Union(double x, double y); // union with point 
  void Union(const CXPoint& A); // union with point 
  void Union(const CXPoint& A, const CXPoint& B);  // union with line
  void Union(const CXLine& L); // union with line
  void Union(const CXRect& R); // union with rect
  
  // special
  char inline Sector(double x, double y);
};

class CIRect : public CILine
{
public:
  CIRect(const CIPoint& iA, const CIPoint& iB) { A=iA; B=iB; }

  // A is upper left corner
  // B is lower right corner (higher values)
  void Set(const CIRect& R) { A=R.A; B=R.B; }
  
  void Normalize(); // make [A,B] points [upper left, lower right]
  
  // following functions need normalized rectangle
  
  // intersection
  bool Intersect(double x, double y); // point intersection
  bool Intersect(const CIPoint& A); // point intersection
  bool Intersect(const CIPoint& A, const CIPoint& B);  // line intersection
  bool Intersect(const CILine& L); // line intersection
  bool Intersect(const CIRect& R); // rect intersection
  
  // union
  void Union(int x, int y); // union with point 
  void Union(const CIPoint& A); // union with point 
  void Union(const CIPoint& A, const CIPoint& B);  // union with line
  void Union(const CILine& L); // union with line
  void Union(const CIRect& R); // union with rect
  
  // special
  char inline Sector(int x, int y);
};


class CIWrapRect : public vector<CIRect>
{
public:
  CIWrapRect() {}

  void Bounds(int _bx, int _by) { bx = _bx; by = _by; }

  // following functions need normalized (simple) rectangle
  
  // intersection
  bool Intersect(int x, int y); // point intersection
  bool Intersect(const CIPoint& A); // point intersection

  // following functions need correct boundaries !

  // A is upper left corner
  // B is lower right corner (higher values)
  void Set(const CIRect& R) { Decompose(R.A, R.B); }
  void Decompose(CIPoint A, CIPoint B);
  void Normalize(); // make [A,B] points [upper left, lower right]

  int  bx;
  int  by;
};

class CXBoundingBox : public CXRect
{
public:
  CXBoundingBox() { valid = false; }
  CXBoundingBox(double AX, double AY, double BX, double BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); valid = true; }
  CXBoundingBox(const CXPoint& iA, const CXPoint& iB) { A=iA; B=iB; valid = true; }
  CXBoundingBox(const CXLine& L) { A=L.A; B=L.B; valid = true; }
  CXBoundingBox(const CXRect& R) { A=R.A; B=R.B; valid = true; }
  
  inline void Set(double AX, double AY, double BX, double BY) { A.SetXY(AX,AY); B.SetXY(BX,BY); valid = true; }
  inline void Set(const CXPoint& iA, const CXPoint& iB) { A=iA; B=iB; valid = true; }
  inline void Set(const CXLine& L) { A=L.A; B=L.B; valid = true; }
  inline void Set(const CXRect& R) { A=R.A; B=R.B; valid = true; }
  inline void Set(const CXBoundingBox& X) { A=X.A; B=X.B; valid = X.valid; }
  
  inline bool Intersect(double x, double y) { return CXRect::Intersect(x, y); } // point intersection
  inline bool Intersect(const CXPoint& A) { return CXRect::Intersect(A); }  // point intersection
  inline bool Intersect(const CXPoint& A, const CXPoint& B) { return CXRect::Intersect(A, B); }   // line intersection
  inline bool Intersect(const CXLine& L) { return CXRect::Intersect(L); }  // line intersection
  inline bool Intersect(const CXRect& R) { return CXRect::Intersect(R); }  // rect intersection
  bool Intersect(const CXBoundingBox& R); // BBox intersection
  
  inline void Union(double x, double y) { CXRect::Union(x, y); } // union with point 
  inline void Union(const CXPoint& A) { CXRect::Union(A); } // union with point 
  inline void Union(const CXPoint& A, const CXPoint& B) { CXRect::Union(A, B); }  // union with line
  inline void Union(const CXLine& L) { CXRect::Union(L); } // union with line
  inline void Union(const CXRect& R) { CXRect::Union(R); } // union with rect
  void Union(const CXBoundingBox& R);
  
  bool valid;
};

#endif

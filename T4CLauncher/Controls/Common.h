#ifndef __COMMON_H_
#define __COMMON_H_

#define RADIUS		70
#define PI			3.14159265358

#define RECT_WIDTH	5

#define RAD2DEG(x)  ((180.0 * (x))/PI)
#define DEG2RAD(x)	(((x) * PI)/180.0)
#define TOSCALE(x)	(((x)*RADIUS)/255.0)
#define SCALETOMAX(x) (((x)*255.0)/RADIUS)


#define RED	0
#define GREEN 1
#define BLUE 2


#define BAD_SLOPE	1000000.0


struct HSVType;

struct RGBType
{
	COLORREF color() { return RGB(r,g,b); };
	HSVType toHSV();
	int r,g,b;
};

struct HSVType
{
	RGBType toRGB();
	int h,s,v;
};

struct LineDesc
{
	double x,y;
	double slope;
	double c;
};


int Distance(CPoint pt1,CPoint pt2);
CPoint PointOnLine(CPoint pt1,CPoint p2,int len,int maxlen);
double Slope(CPoint pt1,CPoint pt2);
double FindC(LineDesc& l); 
CPoint Intersection(LineDesc desc1,LineDesc desc2);
double AngleFromPoint(CPoint pt,CPoint center);
CPoint PtFromAngle(double angle,double sat,CPoint center);



#endif
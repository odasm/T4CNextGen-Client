#include "..\StdAfx.h"
#include "math.h"
#include "Common.h"

double Slope(CPoint pt1,CPoint pt2)
{
double x,y;
	y = pt2.y - pt1.y;
	x = pt2.x - pt1.x;
	if(x)
	{
		return y/x;
	}
	else
	{
		return BAD_SLOPE;
	}
}



CPoint Intersection(LineDesc l1,LineDesc l2)
{
CPoint pt;
double x,y;
	if(l1.slope == l2.slope)
	{
		// Parallel lines, no intersection
		return CPoint(0,0);
	}
	else
	if(l1.slope == BAD_SLOPE )
	{
		// First Line is vertical, eqn is x=0
		// Put x = 0 in second line eqn to get y;
		x = l1.x;
		y = l2.slope * x + l2.c;
	}
	else
	if(l2.slope == BAD_SLOPE)
	{
		// second line is vertical Equation of line is x=0;
		// Put x = 0 in first line eqn to get y;
		x = l2.x;
		y = l1.slope * l2.x + l1.c;
	}
	else
	{
		y = ((l1.c * l2.slope) - (l2.c * l1.slope))/(l2.slope - l1.slope);
		x = (y - l1.c)/l1.slope;
	}

	return CPoint((int)x,(int)y);
}

double FindC(LineDesc& l)
{
double c;
	if(l.slope == BAD_SLOPE)
	{
		c = l.y;
	}
	else
	{
		c = l.y - l.slope * l.x;
	}
	return c;
}

CPoint PointOnLine(CPoint pt1,CPoint pt2,int len,int maxlen )
{
double x,y,m,a,c,C,A;
double a2,c2,m2,B;	
CPoint opt = pt1;
CPoint pt;
	pt1.y *= -1;
	pt2.y *= -1;

	a = (double)len;

	if(pt2.x != pt1.x)
	{
		m = (double)(pt2.y - pt1.y)/(pt2.x - pt1.x);
		m2 = m*m;
		a2 = a*a;
		c = (double)pt1.y - m * (double)pt1.x;
		c2 = c*c;


		A = 1.0;
		
		x = pt1.x;

		B = 2.0 * pt1.x;

		x *= x;
		C = x - a2/(m2 + 1);
		
		x = (B + sqrt(B*B - (4.0*A*C)))/(2.0*A);
		y = m*x + c;
		pt = CPoint((int)x,(int)y);
		if(Distance(pt,pt1) > maxlen || Distance(pt,pt2) > maxlen)
		{
			x = (B - sqrt(B*B - (4.0*A*C)))/(2.0 * A);
			y = m*x + c;
			pt = CPoint((int)x,(int)y);
		}
	}
	else
	{
		a2 = a*a;
		y = sqrt(a2);
		x = 0;
		pt = CPoint((int)x,(int)y);
		pt += pt1;
		if(Distance(pt,pt1) > maxlen || Distance(pt,pt2) > maxlen)
		{
			y = -1.0 *y;
			pt = CPoint((int)x,(int)y);
			pt+=pt1;
		}
	}
	pt.y *= -1;
	return pt;
}


int Distance(CPoint pt1,CPoint pt2)
{
double a;
int x,y;
	y = (pt1.y - pt2.y);
	y *= y;

	x = (pt1.x - pt2.x);
	x *= x;
	
	a = (double)x + (double)y ;
	a = sqrt(a);
	return (int)a;
}

double AngleFromPoint(CPoint pt,CPoint center)
{
double x,y;
	y = -1 * (pt.y - center.y);
	x = pt.x - center.x;
	if(x == 0 && y == 0)
	{
		return 0.0;
	}
	else
	{
		return atan2(y,x);
	}
}

CPoint PtFromAngle(double angle,double sat,CPoint center)
{
	angle = DEG2RAD(angle);
	sat = TOSCALE(sat);
double x,y;
	x = sat * cos(angle);
	y = sat * sin(angle);

CPoint pt ;
	pt = CPoint((int)x,(int)y);
	pt.y *= -1;
	pt += center;
	return pt;
}

RGBType HSVType::toRGB()
{
RGBType rgb;
	if(!h  && !s)
	{
		rgb.r = rgb.g = rgb.b = v;
	}
double min,max,delta,hue;
	
	max = v;
	delta = (max * s)/255.0;
	min = max - delta;

	hue = h;
	if(h > 300 || h <= 60)
	{
		rgb.r = (int)max;
		if(h > 300)
		{
			rgb.g = (int)min;
			hue = (hue - 360.0)/60.0;
			rgb.b = (int)((hue * delta - min) * -1);
		}
		else
		{
			rgb.b = (int)min;
			hue = hue / 60.0;
			rgb.g = (int)(hue * delta + min);
		}
	}
	else
	if(h > 60 && h < 180)
	{
		rgb.g = (int)max;
		if(h < 120)
		{
			rgb.b = (int)min;
			hue = (hue/60.0 - 2.0 ) * delta;
			rgb.r = (int)(min - hue);
		}
		else
		{
			rgb.r = (int)min;
			hue = (hue/60 - 2.0) * delta;
			rgb.b = (int)(min + hue);
		}
	}
	else
	{
		rgb.b = (int)max;
		if(h < 240)
		{
			rgb.r = (int)min;
			hue = (hue/60.0 - 4.0 ) * delta;
			rgb.g = (int)(min - hue);
		}
		else
		{
			rgb.g = (int)min;
			hue = (hue/60 - 4.0) * delta;
			rgb.r = (int)(min + hue);
		}
	}
	return rgb;
}


HSVType RGBType::toHSV()
{
HSVType hsv;

double min,max,delta,temp;
	min = __min(r,__min(g,b));	
	max = __max(r,__max(g,b));
	delta = max - min;
	
	hsv.v = (int)max;
	if(!delta)
	{
		hsv.h = hsv.s = 0;
	}
	else
	{
		temp = delta/max;
		hsv.s = (int)(temp*255);

		if(r == (int)max)
		{
			temp = (double)(g-b)/delta;
		}
		else
		if(g == (int)max)
		{
			temp = 2.0 + ((double)(b-r)/delta);
		}
		else
		{
			temp = 4.0 + ((double)(r-g)/delta);
		}
		temp *= 60;
		if(temp < 0)
		{
			temp+=360;
		}
		if(temp == 360)
		{
			temp = 0;
		}
		hsv.h = (int)temp;
	}
	return hsv;

}
#include <u.h>
#include <libc.h>
#include <geometry.h>

double min(double, double);
double dotvec2_sse4(Point2, Point2);
double dotvec2_avx(Point2, Point2);
double dotvec3_sse4(Point3, Point3);
double dotvec3_avx(Point3, Point3);
Point2 Pt2b(double, double, double);
Point3 crossvec3_sse(Point3, Point3);
double hsubpd(double, double);
double fma(double, double, double);
Point2 addpt2_sse(Point2, Point2);
Point2 addpt2_avx(Point2, Point2);
Point3 addpt3_avx(Point3, Point3);

double
fmin(double a, double b)
{
	return a<b? a: b;
}

double
madd(double a, double b, double c)
{
	return a + b*c;
}

void
main(int argc, char *argv[])
{
	double a, b, r;
	Point2 p0, p1, pr;
	Point3 p0t, p1t, prt;

	GEOMfmtinstall();
	ARGBEGIN{default:sysfatal("shit");}ARGEND
	if(argc != 2)
		sysfatal("shit");
	a = strtod(argv[0], nil);
	b = strtod(argv[1], nil);

	r = 0;
	r = fmin(a, b);
	print("fmin(%g, %g) = %g\n", a, b, r);
	r = 0;
	r = min(a, b);
	print("min(%g, %g) = %g\n", a, b, r);

	print("\n");

	p0 = Pt2b(a, 1, 1);
	p1 = Pt2b(b, 3, 1);
	r = 0;
	r = dotvec2(p0, p1);
	print("dotvec2(%v, %v) = %g\n", p0, p1, r);
	r = 0;
	r = dotvec2_sse4(p0, p1);
	print("dotvec2_sse4(%v, %v) = %g\n", p0, p1, r);
	r = 0;
	r = dotvec2_avx(p0, p1);
	print("dotvec2_avx(%v, %v) = %g\n", p0, p1, r);

	print("\n");

	p0t = Pt3(a, 1, 9, 1);
	p1t = Pt3(b, 3, 4, 1);
	r = 0;
	r = dotvec3(p0t, p1t);
	print("dotvec3(%V, %V) = %g\n", p0t, p1t, r);
	r = 0;
	r = dotvec3_sse4(p0t, p1t);
	print("dotvec3_sse4(%V, %V) = %g\n", p0t, p1t, r);
	r = 0;
	r = dotvec3_avx(p0t, p1t);
	print("dotvec3_avx(%V, %V) = %g\n", p0t, p1t, r);

	print("\n");

	r = 0;
	r = hsubpd(a, b);
	print("hsubpd(%g, %g) = %g\n", a, b, r);

	print("\n");

	p0t = Pt3(a, 1, 9, 1);
	p1t = Pt3(b, 3, 4, 1);
	prt = Vec3(0,0,0);
	prt = crossvec3(p0t, p1t);
	print("crossvec3(%V, %V) = %V\n", p0t, p1t, prt);
	prt = Vec3(0,0,0);
	prt = crossvec3_sse(p0t, p1t);
	print("crossvec3_sse(%V, %V) = %V\n", p0t, p1t, prt);

	print("\n");

	r = 0;
	r = madd(a, b, 21);
	print("madd(%g, %g, 21) = %g\n", a, b, r);
	r = 0;
	r = fma(a, b, 21);
	print("fma(%g, %g, 21) = %g\n", a, b, r);

	print("\n");

	p0 = Pt2b(a, 1, 1);
	p1 = Pt2b(b, 3, 1);
	pr = Vec2(0,0);
	pr = addpt2(p0, p1);
	print("addpt2(%v, %v) = %v\n", p0, p1, pr);
	pr = Vec2(0,0);
	pr = addpt2_sse(p0, p1);
	print("addpt2_sse(%v, %v) = %v\n", p0, p1, pr);
	pr = Vec2(0,0);
	pr = addpt2_avx(p0, p1);
	print("addpt2_avx(%v, %v) = %v\n", p0, p1, pr);

	print("\n");

	p0t = Pt3(a, 1, 1, b);
	p1t = Pt3(b, 3, 1, a);
	prt = Vec3(0,0,0);
	prt = addpt3(p0t, p1t);
	print("addpt3(%V, %V) = %V\n", p0t, p1t, prt);
	prt = Vec3(0,0,0);
	prt = addpt3_avx(p0t, p1t);
	print("addpt3_avx(%V, %V) = %V\n", p0t, p1t, prt);

	exits(nil);
}

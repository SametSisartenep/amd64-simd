#include <u.h>
#include <libc.h>
#include <geometry.h>

uvlong nanosec(void);
double min(double, double);
double dppd(Point2, Point2);
double dppd3(Point3, Point3);
Point2 Pt2b(double, double, double);
Point3 xvec3(Point3, Point3);
double hsubpd(double, double);
double fma(double, double, double);

double
fmin(double a, double b)
{
	return a<b? a: b;
}

void
main(int argc, char *argv[])
{
	uvlong t0, t1;
	double a, b, r;
	Point2 p0, p1;
	Point3 p0t, p1t, pr;

	GEOMfmtinstall();
	ARGBEGIN{default:sysfatal("shit");}ARGEND
	if(argc != 2)
		sysfatal("shit");
	a = strtod(argv[0], nil);
	b = strtod(argv[1], nil);

	t0 = nanosec();
	r = fmin(a, b);
	t1 = nanosec();
	print("fmin(%g, %g) = %g\ttook %lludns\n", a, b, r, t1-t0);
	t0 = nanosec();
	r = min(a, b);
	t1 = nanosec();
	print("min(%g, %g) = %g\ttook %lludns\n", a, b, r, t1-t0);

	p0 = Pt2b(a, 1, 1);
	p1 = Pt2b(b, 3, 1);
	t0 = nanosec();
	r = dppd(p0, p1);
	t1 = nanosec();
	print("dppd(%v, %v) = %g\ttook %lludns\n", p0, p1, r, t1-t0);
	t0 = nanosec();
	r = dotvec2(p0, p1);
	t1 = nanosec();
	print("dotvec2(%v, %v) = %g\ttook %lludns\n", p0, p1, r, t1-t0);

	p0t = Pt3(a, 1, 9, 1);
	p1t = Pt3(b, 3, 4, 1);
	t0 = nanosec();
	r = dppd3(p0t, p1t);
	t1 = nanosec();
	print("dppd3(%V, %V) = %g\ttook %lludns\n", p0t, p1t, r, t1-t0);
	t0 = nanosec();
	r = dotvec3(p0t, p1t);
	t1 = nanosec();
	print("dotvec3(%V, %V) = %g\ttook %lludns\n", p0t, p1t, r, t1-t0);

	t0 = nanosec();
	r = hsubpd(a, b);
	t1 = nanosec();
	print("hsubpd(%g, %g) = %g\ttook %lludns\n", a, b, r, t1-t0);

	p0t = Pt3(a, 1, 9, 1);
	p1t = Pt3(b, 3, 4, 1);
	t0 = nanosec();
	pr = xvec3(p0t, p1t);
	t1 = nanosec();
	print("xvec3(%V, %V) = %V\ttook %lludns\n", p0t, p1t, pr, t1-t0);
	t0 = nanosec();
	pr = crossvec3(p0t, p1t);
	t1 = nanosec();
	print("crossvec3(%V, %V) = %V\ttook %lludns\n", p0t, p1t, pr, t1-t0);

	t0 = nanosec();
	r = fma(a, b, 21);
	t1 = nanosec();
	print("fma(%g, %g, 21) = %g\ttook %lludns\n", a, b, r, t1-t0);

	exits(nil);
}

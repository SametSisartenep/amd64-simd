#include <u.h>
#include <libc.h>
#include <geometry.h>

double min(double, double);
double dotvec2_sse(Point2, Point2);
double dotvec2_sse4(Point2, Point2);
double dotvec2_avx(Point2, Point2);
double dotvec2_sse_a(Point2*, Point2*);
double dotvec2_sse4_a(Point2*, Point2*);
double dotvec2_avx_a(Point2*, Point2*);
double dotvec3_sse4(Point3, Point3);
double dotvec3_avx(Point3, Point3);
double dotvec3_sse4_a(Point3*, Point3*);
Point2 Pt2b(double, double, double);
Point3 crossvec3_sse(Point3, Point3);
double hsubpd(double, double);
double fma(double, double, double);
Point2 addpt2_sse(Point2, Point2);
Point2 addpt2_avx(Point2, Point2);
Point3 addpt3_avx(Point3, Point3);
void addsub_sse(double*,double*);
double round(double);

void *
amalloc(ulong n, ulong a)
{
	void *p;

	assert(a > 1 && (a&1) == 0);

	a--;
	p = malloc(n+a);
	if(p == nil)
		sysfatal("malloc: %r");
	p = (void*)(((uintptr)p + a)&~a);
	return p;
}

void
addsub(double *a, double *b)
{
	b[0] = b[0]-a[0];
	b[1] = b[1]+a[1];
}

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
	double va[2], vb[2];
	Point2 p0, p1, pr;
	Point3 p0t, p1t, prt;
	Point2 *ap0, *ap1, *apr;
	Point3 *ap0t, *ap1t, *aprt;

	GEOMfmtinstall();
	ARGBEGIN{default:sysfatal("shit");}ARGEND
	if(argc != 2)
		sysfatal("shit");
	a = strtod(argv[0], nil);
	b = strtod(argv[1], nil);

	ap0 = amalloc(sizeof(Point2), 16);
	ap1 = amalloc(sizeof(Point2), 16);
	apr = amalloc(sizeof(Point2), 16);

	ap0t = amalloc(sizeof(Point3), 16);
	ap1t = amalloc(sizeof(Point3), 16);
	aprt = amalloc(sizeof(Point3), 16);

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
	r = dotvec2_sse(p0, p1);
	print("dotvec2_sse(%v, %v) = %g\n", p0, p1, r);
	r = 0;
	r = dotvec2_sse4(p0, p1);
	print("dotvec2_sse4(%v, %v) = %g\n", p0, p1, r);
	r = 0;
	r = dotvec2_avx(p0, p1);
	print("dotvec2_avx(%v, %v) = %g\n", p0, p1, r);

	print("\n");

	*ap0 = Pt2b(a, 1, 1);
	*ap1 = Pt2b(b, 3, 1);
	r = 0;
	r = dotvec2_sse_a(ap0, ap1);
	print("dotvec2_sse_a(%v, %v) = %g\n", *ap0, *ap1, r);
	r = 0;
	r = dotvec2_sse4_a(ap0, ap1);
	print("dotvec2_sse4_a(%v, %v) = %g\n", *ap0, *ap1, r);
	r = 0;
	r = dotvec2_avx_a(ap0, ap1);
	print("dotvec2_avx_a(%v, %v) = %g\n", *ap0, *ap1, r);

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

	*ap0t = Pt3(a, 1, 9, 1);
	*ap1t = Pt3(b, 3, 4, 1);
	r = 0;
	r = dotvec3_sse4_a(ap0t, ap1t);
	print("dotvec3_sse4_a(%V, %V) = %g\n", *ap0t, *ap1t, r);

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

	print("\n");

	va[0] = va[1] = a;
	vb[0] = vb[1] = b;
	print("addsub([%g %g], [%g %g]) = ", va[0], va[1], vb[0], vb[1]);
	addsub(va, vb);
	print("[%g %g]\n", vb[0], vb[1]);

	va[0] = va[1] = a;
	vb[0] = vb[1] = b;
	print("addsub_sse([%g %g], [%g %g]) = ", va[0], va[1], vb[0], vb[1]);
	addsub_sse(va, vb);
	print("[%g %g]\n", vb[0], vb[1]);

	print("\n");

	r = 0;
	r = round(a);
	print("round(%g) = %g\n", a, r);

	exits(nil);
}

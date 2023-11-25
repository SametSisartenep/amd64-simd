#include <u.h>
#include <libc.h>
#include <thread.h>
#include <geometry.h>
#include "../bench9/b.h"

double min(double, double);
double dotvec2_sse4(Point2, Point2);
double dotvec2_avx(Point2, Point2);
double dotvec3_sse4(Point3, Point3);
double dotvec3_avx(Point3, Point3);
Point2 Pt2b(double, double, double);
Point3 crossvec3_sse(Point3, Point3);
double hsubpd(double, double);
double fma(double, double, double);
Point2 addpt2_avx(Point2, Point2);

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

static void
bmin(int fd)
{
	Bgr g;
	B *b0, *b1;
	double a, b;
	int i;

	benchinitgr(&g, "min");
	b0 = benchadd(&g, "fmin");
	b1 = benchadd(&g, "fmin_sse");

	while(b0->n > 0 || b1->n > 0){
		a = truerand()*frand();
		b = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			fmin(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			min(a, b);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bdotvec2(int fd)
{
	Bgr g;
	B *b0, *b1, *b2;
	Point2 a, b;
	int i;

	benchinitgr(&g, "2d dot product");
	b0 = benchadd(&g, "dotvec2");
	b1 = benchadd(&g, "dotvec2_sse4");
	b2 = benchadd(&g, "dotvec2_avx");

	while(b0->n > 0 || b1->n > 0){
		a = Vec2(truerand()*frand(), truerand()*frand());
		b = Vec2(truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			dotvec2(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			dotvec2_sse4(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			dotvec2_avx(a, b);
		benchout(b2);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bdotvec3(int fd)
{
	Bgr g;
	B *b0, *b1, *b2;
	Point3 a, b;
	int i;

	benchinitgr(&g, "3d dot product");
	b0 = benchadd(&g, "dotvec3");
	b1 = benchadd(&g, "dotvec3_sse4");
	b2 = benchadd(&g, "dotvec3_avx");

	while(b0->n > 0 || b1->n > 0){
		a = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			dotvec3(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			dotvec3_sse4(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			dotvec3_avx(a, b);
		benchout(b2);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bcrossvec3(int fd)
{
	Bgr g;
	B *b0, *b1;
	Point3 a, b;
	int i;

	benchinitgr(&g, "3d cross product");
	b0 = benchadd(&g, "crossvec3");
	b1 = benchadd(&g, "crossvec3_sse");

	while(b0->n > 0 || b1->n > 0){
		a = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			crossvec3(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			crossvec3_sse(a, b);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bPt2(int fd)
{
	Bgr g;
	B *b0, *b1;
	double x, y, w;
	int i;

	benchinitgr(&g, "Pt2");
	b0 = benchadd(&g, "Pt2");
	b1 = benchadd(&g, "Pt2b");

	while(b0->n > 0 || b1->n > 0){
		x = truerand()*frand();
		y = truerand()*frand();
		w = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			Pt2(x, y, w);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			Pt2b(x, y, w);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bfma(int fd)
{
	Bgr g;
	B *b0, *b1;
	double a, b, c;
	int i;

	benchinitgr(&g, "multiply + add");
	b0 = benchadd(&g, "madd");
	b1 = benchadd(&g, "fma_avx");

	while(b0->n > 0 || b1->n > 0){
		a = truerand()*frand();
		b = truerand()*frand();
		c = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			madd(a, b, c);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			fma(a, b, c);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
baddpt2(int fd)
{
	Bgr g;
	B *b0, *b1;
	Point2 a, b;
	int i;

	benchinitgr(&g, "2d point sum");
	b0 = benchadd(&g, "addpt2");
	b1 = benchadd(&g, "addpt2_avx");

	while(b0->n > 0 || b1->n > 0){
		a = Pt2(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Pt2(truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			addpt2(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			addpt2_avx(a, b);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

void
threadmain(int argc, char **argv)
{
	ARGBEGIN{
	}ARGEND

	if(benchwire(0) != 0)
		fprint(2, "failed to wire: %r\n");

	bmin(1);
	bseparator(1);
	bdotvec2(1);
	bseparator(1);
	bdotvec3(1);
	bseparator(1);
	bcrossvec3(1);
	bseparator(1);
	bPt2(1);
	bseparator(1);
	bfma(1);
	bseparator(1);
	baddpt2(1);

	threadexitsall(nil);
}

#include <u.h>
#include <libc.h>
#include <thread.h>
#include <geometry.h>
#include "b.h"

double dppd(Point2, Point2);
double dppda(Point2, Point2);
double dppd3(Point3, Point3);
double dppd3a(Point3, Point3);
Point3 xvec3(Point3, Point3);

static void
bdotvec2(int fd)
{
	Bgr g;
	B *b0, *b1, *b2;
	Point2 a, b;
	int i;

	benchinitgr(&g, "2d dot product");
	b0 = benchadd(&g, "dotvec2");
	b1 = benchadd(&g, "dotvec2_simd");
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
			dppd(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			dppda(a, b);
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
	b1 = benchadd(&g, "dotvec3_simd");
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
			dppd3(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			dppd3a(a, b);
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
	b1 = benchadd(&g, "crossvec3_simd");

	while(b0->n > 0 || b1->n > 0){
		a = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			crossvec3(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			xvec3(a, b);
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

	bdotvec2(1);
	bseparator(1);
	bdotvec3(1);
	bseparator(1);
	bcrossvec3(1);

	threadexitsall(nil);
}

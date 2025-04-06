#include <u.h>
#include <libc.h>
#include <thread.h>
#include <geometry.h>
#include "../bench9/b.h"

double min(double, double);

double dotvec2_sse(Point2, Point2);
double dotvec2_sse4(Point2, Point2);
double dotvec2_avx(Point2, Point2);
double dotvec2_sse_a(Point2*, Point2*);
double dotvec2_sse4_a(Point2*, Point2*);
double dotvec2_avx_a(Point2*, Point2*);
#define dotvec2_m(a, b) (a.x*b.x + a.y*b.y)

double dotvec3_sse4(Point3, Point3);
double dotvec3_avx(Point3, Point3);
double dotvec3_sse4_a(Point3*, Point3*);
double dotvec3_avx_a(Point3*, Point3*);
#define dotvec3_m(a, b) (a.x*b.x + a.y*b.y + a.z*b.z)

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

double
dotvec2_p(Point2 *a, Point2 *b)
{
	return a->x*b->x + a->y*b->y;
}

double
dotvec3_p(Point3 *a, Point3 *b)
{
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

void
mulm_T(Matrix a, Matrix b)
{
	int i, j, k;
	Matrix tmp, bT;

	memmove(bT, b, 3*3*sizeof(double));
	transposem(bT);

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++){
			tmp[i][j] = 0;
			for(k = 0; k < 3; k++)
				tmp[i][j] += a[i][k]*bT[j][k];
		}
	memmove(a, tmp, 3*3*sizeof(double));
}

#define SM 3
void
mulm_subm(Matrix a, Matrix b)
{
	int i, j, k, i2, j2, k2;
	Matrix tmp;
	double *tmpp, *ap, *bp;

	memset(tmp, 0, 3*3*sizeof(double));
	for (i = 0; i < 3; i += SM)
	for (j = 0; j < 3; j += SM)
	for (k = 0; k < 3; k += SM)
	for (i2 = 0, tmpp = &tmp[i][j], ap = &a[i][k]; i2 < SM; ++i2, tmpp += 3, ap += 3)
	for (k2 = 0, bp = &b[k][j]; k2 < SM; ++k2, bp += 3)
	for (j2 = 0; j2 < SM; ++j2)
		tmpp[j2] += ap[k2] * bp[j2];
	memmove(a, tmp, 3*3*sizeof(double));
}

void
mulm_unrl(Matrix a, Matrix b)
{
	double t0, t1, t2;

	t0 = a[0][0]; t1 = a[0][1]; t2 = a[0][2];
	a[0][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0];
	a[0][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1];
	a[0][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2];
	t0 = a[1][0]; t1 = a[1][1]; t2 = a[1][2];
	a[1][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0];
	a[1][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1];
	a[1][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2];
	t0 = a[2][0]; t1 = a[2][1]; t2 = a[2][2];
	a[2][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0];
	a[2][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1];
	a[2][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2];
}

void
smulm_unrl(Matrix m, double s)
{
	m[0][0] *= s; m[0][1] *= s; m[0][2] *= s;
	m[1][0] *= s; m[1][1] *= s; m[1][2] *= s;
	m[2][0] *= s; m[2][1] *= s; m[2][2] *= s;
}

void
mulm3_T(Matrix3 a, Matrix3 b)
{
	int i, j, k;
	Matrix3 tmp, bT;

	memmove(bT, b, 4*4*sizeof(double));
	transposem3(bT);

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++){
			tmp[i][j] = 0;
			for(k = 0; k < 4; k++)
				tmp[i][j] += a[i][k]*bT[j][k];
		}
	memmove(a, tmp, 4*4*sizeof(double));
}

#undef SM
#define SM 4
void
mulm3_subm(Matrix3 a, Matrix3 b)
{
	int i, j, k, i2, j2, k2;
	Matrix3 tmp;
	double *tmpp, *ap, *bp;

	memset(tmp, 0, 4*4*sizeof(double));
	for (i = 0; i < 4; i += SM)
	for (j = 0; j < 4; j += SM)
	for (k = 0; k < 4; k += SM)
	for (i2 = 0, tmpp = &tmp[i][j], ap = &a[i][k]; i2 < SM; ++i2, tmpp += 4, ap += 4)
	for (k2 = 0, bp = &b[k][j]; k2 < SM; ++k2, bp += 4)
	for (j2 = 0; j2 < SM; ++j2)
		tmpp[j2] += ap[k2] * bp[j2];
	memmove(a, tmp, 4*4*sizeof(double));
}

void
mulm3_unrl(Matrix3 a, Matrix3 b)
{
	double t0, t1, t2, t3;

	t0 = a[0][0]; t1 = a[0][1]; t2 = a[0][2]; t3 = a[0][3];
	a[0][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0] + t3*b[3][0];
	a[0][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1] + t3*b[3][1];
	a[0][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2] + t3*b[3][2];
	a[0][3] = t0*b[0][3] + t1*b[1][3] + t2*b[2][3] + t3*b[3][3];
	t0 = a[1][0]; t1 = a[1][1]; t2 = a[1][2]; t3 = a[1][3];
	a[1][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0] + t3*b[3][0];
	a[1][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1] + t3*b[3][1];
	a[1][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2] + t3*b[3][2];
	a[1][3] = t0*b[0][3] + t1*b[1][3] + t2*b[2][3] + t3*b[3][3];
	t0 = a[2][0]; t1 = a[2][1]; t2 = a[2][2]; t3 = a[2][3];
	a[2][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0] + t3*b[3][0];
	a[2][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1] + t3*b[3][1];
	a[2][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2] + t3*b[3][2];
	a[2][3] = t0*b[0][3] + t1*b[1][3] + t2*b[2][3] + t3*b[3][3];
	t0 = a[3][0]; t1 = a[3][1]; t2 = a[3][2]; t3 = a[3][3];
	a[3][0] = t0*b[0][0] + t1*b[1][0] + t2*b[2][0] + t3*b[3][0];
	a[3][1] = t0*b[0][1] + t1*b[1][1] + t2*b[2][1] + t3*b[3][1];
	a[3][2] = t0*b[0][2] + t1*b[1][2] + t2*b[2][2] + t3*b[3][2];
	a[3][3] = t0*b[0][3] + t1*b[1][3] + t2*b[2][3] + t3*b[3][3];
}

void
smulm3_unrl(Matrix3 m, double s)
{
	m[0][0] *= s; m[0][1] *= s; m[0][2] *= s; m[0][3] *= s;
	m[1][0] *= s; m[1][1] *= s; m[1][2] *= s; m[1][3] *= s;
	m[2][0] *= s; m[2][1] *= s; m[2][2] *= s; m[2][3] *= s;
	m[3][0] *= s; m[3][1] *= s; m[3][2] *= s; m[3][3] *= s;
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
	B *b0, *b1, *b2, *b3, *b4, *b5, *b6, *b7, *b8;
	Point2 a, b;
	Point2 *aa, *bb;
	int i;

	benchinitgr(&g, "2d dot product");
	b0 = benchadd(&g, "dotvec2");
	b1 = benchadd(&g, "dotvec2_sse");
	b2 = benchadd(&g, "dotvec2_sse4");
	b3 = benchadd(&g, "dotvec2_avx");
	b4 = benchadd(&g, "dotvec2_sse_a");
	b5 = benchadd(&g, "dotvec2_sse4_a");
	b6 = benchadd(&g, "dotvec2_avx_a");
	b7 = benchadd(&g, "dotvec2_p");
	b8 = benchadd(&g, "dotvec2_m");

	while(b0->n > 0 || b1->n > 0){
		a = Vec2(truerand()*frand(), truerand()*frand());
		b = Vec2(truerand()*frand(), truerand()*frand());
		aa = mallocalign(sizeof(Point2), 16, 0, 0);
		bb = mallocalign(sizeof(Point2), 16, 0, 0);
		*aa = a;
		*bb = b;

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			dotvec2(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			dotvec2_sse(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			dotvec2_sse4(a, b);
		benchout(b2);

		benchin(b3);
		for(i = 0; i < 1e6; i++)
			dotvec2_avx(a, b);
		benchout(b3);

		benchin(b4);
		for(i = 0; i < 1e6; i++)
			dotvec2_sse_a(aa, bb);
		benchout(b4);

		benchin(b5);
		for(i = 0; i < 1e6; i++)
			dotvec2_sse4_a(aa, bb);
		benchout(b5);

		benchin(b6);
		for(i = 0; i < 1e6; i++)
			dotvec2_avx_a(aa, bb);
		benchout(b6);

		benchin(b7);
		for(i = 0; i < 1e6; i++)
			dotvec2_p(aa, bb);
		benchout(b7);

		benchin(b8);
		for(i = 0; i < 1e6; i++)
			USED(dotvec2_m(a, b));
		benchout(b8);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bdotvec3(int fd)
{
	Bgr g;
	B *b0, *b1, *b2, *b3, *b4, *b5, *b6;
	Point3 a, b;
	Point3 *aa, *bb;
	int i;

	benchinitgr(&g, "3d dot product");
	b0 = benchadd(&g, "dotvec3");
	b1 = benchadd(&g, "dotvec3_sse4");
	b2 = benchadd(&g, "dotvec3_avx");
	b3 = benchadd(&g, "dotvec3_sse4_a");
	b4 = benchadd(&g, "dotvec3_avx_a");
	b5 = benchadd(&g, "dotvec3_p");
	b6 = benchadd(&g, "dotvec3_m");

	while(b0->n > 0 || b1->n > 0){
		a = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Vec3(truerand()*frand(), truerand()*frand(), truerand()*frand());
		aa = mallocalign(sizeof(Point3), 16, 0, 0);
		bb = mallocalign(sizeof(Point3), 16, 0, 0);
		*aa = a;
		*bb = b;

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

		benchin(b3);
		for(i = 0; i < 1e6; i++)
			dotvec3_sse4_a(aa, bb);
		benchout(b3);

		benchin(b4);
		for(i = 0; i < 1e6; i++)
			dotvec3_avx_a(aa, bb);
		benchout(b4);

		benchin(b5);
		for(i = 0; i < 1e6; i++)
			dotvec3_p(aa, bb);
		benchout(b5);

		benchin(b6);
		for(i = 0; i < 1e6; i++)
			USED(dotvec3_m(a, b));
		benchout(b6);
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
	B *b0, *b1, *b2;
	Point2 a, b;
	int i;

	benchinitgr(&g, "2d point sum");
	b0 = benchadd(&g, "addpt2");
	b1 = benchadd(&g, "addpt2_sse");
	b2 = benchadd(&g, "addpt2_avx");

	while(b0->n > 0 || b1->n > 0){
		a = Pt2(truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Pt2(truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			addpt2(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			addpt2_sse(a, b);
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++)
			addpt2_avx(a, b);
		benchout(b2);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
baddpt3(int fd)
{
	Bgr g;
	B *b0, *b1;
	Point3 a, b;
	int i;

	benchinitgr(&g, "3d point sum");
	b0 = benchadd(&g, "addpt3");
	b1 = benchadd(&g, "addpt3_avx");

	while(b0->n > 0 || b1->n > 0){
		a = Pt3(truerand()*frand(), truerand()*frand(), truerand()*frand(), truerand()*frand());
		b = Pt3(truerand()*frand(), truerand()*frand(), truerand()*frand(), truerand()*frand());

		benchin(b0);
		for(i = 0; i < 1e6; i++)
			addpt3(a, b);
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++)
			addpt3_avx(a, b);
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static int
eqmat2(Matrix a, Matrix b)
{
	int i, j;

	for(i = 0; i < 3; i++)
	for(j = 0; j < 3; j++)
		if(a[i][j] != b[i][j])
			return 0;
	return 1;
}

static int
eqmat3(Matrix3 a, Matrix3 b)
{
	int i, j;

	for(i = 0; i < 4; i++)
	for(j = 0; j < 4; j++)
		if(a[i][j] != b[i][j])
			return 0;
	return 1;
}

static void
bmulm(int fd)
{
	Bgr g;
	B *b0, *b1, *b2, *b3;
	Matrix a0, a, b;
	int i, j;

	benchinitgr(&g, "3x3 matrix mul");
	b0 = benchadd(&g, "mulm");
	b1 = benchadd(&g, "mulm_T");
	b2 = benchadd(&g, "mulm_subm");
	b3 = benchadd(&g, "mulm_unrl");

	while(b0->n > 0 || b1->n > 0){
		for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			a0[i][j] = a[i][j] = truerand()*frand();
		for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			b[i][j] = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++){
			mulm(a, b);
//			if(i == 0){
//				Matrix t;
//				memmove(t, a, 3*3*sizeof(double));
//				memmove(a, a0, 3*3*sizeof(double));
//				mulm_unrl(a, b);
//				print("match %d\n", eqmat2(t, a));
//				return;
//			}
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++){
			mulm_T(a, b);
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++){
			mulm_subm(a, b);
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b2);

		benchin(b3);
		for(i = 0; i < 1e6; i++){
			mulm_unrl(a, b);
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b3);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bmulm3(int fd)
{
	Bgr g;
	B *b0, *b1, *b2, *b3;
	Matrix3 a0, a, b;
	int i, j;

	benchinitgr(&g, "4x4 matrix mul");
	b0 = benchadd(&g, "mulm3");
	b1 = benchadd(&g, "mulm3_T");
	b2 = benchadd(&g, "mulm3_subm");
	b3 = benchadd(&g, "mulm3_unrl");

	while(b0->n > 0 || b1->n > 0){
		for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			a0[i][j] = a[i][j] = truerand()*frand();
		for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			b[i][j] = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++){
			mulm3(a, b);
//			if(i == 0){
//				Matrix3 t;
//				memmove(t, a, 4*4*sizeof(double));
//				memmove(a, a0, 4*4*sizeof(double));
//				mulm3_unrl(a, b);
//				print("match %d\n", eqmat3(t, a));
//				return;
//			}
			memmove(a, a0, 4*4*sizeof(double));
		}
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++){
			mulm3_T(a, b);
			memmove(a, a0, 4*4*sizeof(double));
		}
		benchout(b1);

		benchin(b2);
		for(i = 0; i < 1e6; i++){
			mulm3_subm(a, b);
			memmove(a, a0, 4*4*sizeof(double));
		}
		benchout(b2);

		benchin(b3);
		for(i = 0; i < 1e6; i++){
			mulm3_unrl(a, b);
			memmove(a, a0, 4*4*sizeof(double));
		}
		benchout(b3);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bsmulm(int fd)
{
	Bgr g;
	B *b0, *b1;
	Matrix a0, a;
	double s;
	int i, j;

	benchinitgr(&g, "3x3 matrix smul");
	b0 = benchadd(&g, "smulm");
	b1 = benchadd(&g, "smulm_unrl");

	while(b0->n > 0 || b1->n > 0){
		for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			a0[i][j] = a[i][j] = truerand()*frand();
		s = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++){
			smulm(a, s);
//			if(i == 0){
//				Matrix t;
//				memmove(t, a, 3*3*sizeof(double));
//				memmove(a, a0, 3*3*sizeof(double));
//				smulm_unrl(a, s);
//				print("match %d\n", eqmat2(t, a));
//				return;
//			}
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++){
			smulm_unrl(a, s);
			memmove(a, a0, 3*3*sizeof(double));
		}
		benchout(b1);
	}

	benchprintgr(&g, fd);
	benchfreegr(&g);
}

static void
bsmulm3(int fd)
{
	Bgr g;
	B *b0, *b1;
	Matrix3 a0, a;
	double s;
	int i, j;

	benchinitgr(&g, "4x4 matrix smul");
	b0 = benchadd(&g, "smulm3");
	b1 = benchadd(&g, "smulm3_unrl");

	while(b0->n > 0 || b1->n > 0){
		for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			a0[i][j] = a[i][j] = truerand()*frand();
		s = truerand()*frand();

		benchin(b0);
		for(i = 0; i < 1e6; i++){
			smulm3(a, s);
//			if(i == 0){
//				Matrix3 t;
//				memmove(t, a, 4*4*sizeof(double));
//				memmove(a, a0, 4*4*sizeof(double));
//				smulm3_unrl(a, s);
//				print("match %d\n", eqmat3(t, a));
//				return;
//			}
			memmove(a, a0, 4*4*sizeof(double));
		}
		benchout(b0);

		benchin(b1);
		for(i = 0; i < 1e6; i++){
			smulm3_unrl(a, s);
			memmove(a, a0, 4*4*sizeof(double));
		}
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
	bseparator(1);
	baddpt3(1);
	bseparator(1);
	bmulm(1);
	bseparator(1);
	bmulm3(1);
	bseparator(1);
	bsmulm(1);
	bseparator(1);
	bsmulm3(1);

	threadexitsall(nil);
}

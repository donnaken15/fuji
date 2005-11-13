#include "Fuji.h"
#include "MFVector.h"
#include "MFMatrix.h"
#include "MFCollision.h"


bool MFCollision_RaySphereTest(const MFVector& rayPos, const MFVector& rayDir, const MFVector& spherePos, float radius, float *pTime)
{
	MFVector diff = rayPos - spherePos;

	float b = diff.Dot3(rayDir);
	float c = diff.MagSquared3() - radius;
	float d = b*b - c;

	if(d <= 0.0f)
		return false;

	float t = -b - MFSqrt(d);

	if(t<0.0f) return false;

	if(pTime)
		*pTime = t;
	return true;
}

bool MFCollision_RayPlaneTest(const MFVector& rayPos, const MFVector& rayDir, const MFVector& plane, float *pTime)
{
	float a = plane.Dot3(rayDir);

	// if ray is parallel to plane
	if(a > -MFALMOST_ZERO && a < MFALMOST_ZERO)
		return false;

	float t = -rayPos.DotH(plane) / a;

	if(t<0.0f) return false;

	if(pTime)
		*pTime = t;
	return true;
}

bool MFCollision_SpherePlaneTest(const MFVector& spherePos, float radius, const MFVector& plane, MFCollisionResult *pResult)
{
	if(!pResult)
	{
		return spherePos.DotH(plane) < radius;
	}
	else
	{
		float d = spherePos.DotH(plane);

		pResult->bCollide = d < radius;

		if(pResult->bCollide)
		{
			pResult->depth = radius - d;
			pResult->normal = plane;
			pResult->intersectionPoint.Mad3(pResult->normal, -(d + pResult->depth*0.5f), spherePos);
		}

		return pResult->bCollide;
	}
}

bool MFCollision_SphereSphereTest(const MFVector &pos1, float radius1, const MFVector &pos2, float radius2, MFCollisionResult *pResult)
{
	MFVector diff = pos2 - pos1;

	if(!pResult)
	{
		return diff.MagSquared3() < radius1*radius1 + radius2*radius2;
	}
	else
	{
		float length = diff.Magnitude3();
		float totalRadius = radius1 + radius2;

		pResult->bCollide = length < totalRadius;

		if(pResult->bCollide)
		{
			pResult->depth = totalRadius - length;
			pResult->normal = -diff.Normalise3();
			pResult->intersectionPoint = diff * ((length / totalRadius) * (radius1 / radius2));
		}

		return pResult->bCollide;
	}
}

bool MFCollision_RayTriTest(const MFVector& rayPos, const MFVector& rayDir, const MFVector& p0,  const MFVector& p1, const MFVector& p2, float *pT, float *pU, float *pV)
{
	MFVector edge1, edge2, tvec, pvec, qvec;
	float det, inv_det;
	float u, v;

	/* find vectors for two edges sharing vert0 */
	edge1 = p1 - p0;
	edge2 = p2 - p0;

	/* begin calculating determinant - also used to calculate U parameter */
	pvec.Cross3(rayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = edge1.Dot3(pvec);

	if(det > -MFALMOST_ZERO && det < MFALMOST_ZERO)
		return false;
	inv_det = 1.0f / det;

	/* calculate distance from vert0 to ray origin */
	tvec = rayPos - p0;

	/* calculate U parameter and test bounds */
	u = tvec.Dot3(pvec) * inv_det;
	if(u < 0.0f || u > 1.0f)
		return false;

	/* prepare to test V parameter */
	qvec.Cross3(tvec, edge1);

	/* calculate V parameter and test bounds */
	v = rayDir.Dot3(qvec) * inv_det;
	if(v < 0.0f || u + v > 1.0f)
		return false;

	/* calculate t, ray intersects triangle */
	if(pT) *pT = edge2.Dot3(qvec) * inv_det;

	if(pU)
	{
		*pU = u;
		*pV = v;
	}

	return true;
}

// culls backfaces
bool MFCollision_RayTriCullTest(const MFVector& rayPos, const MFVector& rayDir, const MFVector& p0,  const MFVector& p1, const MFVector& p2, float *pT, float *pU, float *pV, MFVector *pIntersectionPoint)
{
	MFVector	edge1, edge2, tvec, pvec, qvec;
	float	det, inv_det;
	float	u, v;

	/* find vectors for two edges sharing vert0 */
	edge1 = p1 - p0;
	edge2 = p2 - p0;

	/* begin calculating determinant - also used to calculate U parameter */
	pvec.Cross3(rayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = edge1.Dot3(pvec);

	if (det < MFALMOST_ZERO)
		return false;

	/* calculate distance from vert0 to ray origin */
	tvec = rayPos - p0;

	/* calculate U parameter and test bounds */
	u = tvec.Dot3(pvec);
	if (u < 0.0f || u > det)
		return false;

	/* prepare to test V parameter */
	qvec.Cross3(tvec, edge1);

	/* calculate V parameter and test bounds */
	v = rayDir.Dot3(qvec);
	if (v < 0.0f || u + v > det)
		return false;

	/* calculate t, scale parameters, ray intersects triangle */
	if(pIntersectionPoint || pT || pU)
	{
		inv_det = 1.0f / det;
		u *= inv_det;
		v *= inv_det;

		if(pT) *pT = edge2.Dot3(qvec) * inv_det;

		if(pU)
		{
			*pU = u;
			*pV = v;
		}

		if(pIntersectionPoint)
		{
			*pIntersectionPoint = p0*(1.0f-u-v) + p1*u + p2*v;
		}
	}

	return true;
}

bool MFCollision_SphereTriTest(const MFVector& sphere, const MFVector& p0,  const MFVector& p1, const MFVector& p2, MFVector *pIntersectionPoint)
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}

bool MFCollision_PlaneTriTest(const MFVector& plane, const MFVector& p0,  const MFVector& p1, const MFVector& p2, MFVector *pIntersectionPoint)
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}


/* sort so that a<=b */
#define SORT(a,b)       \
             if(a>b)    \
             {          \
               float c; \
               c=a;     \
               a=b;     \
               b=c;     \
             }

/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */ 
#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return true;                   \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return true;                   \
    }                                                 \
  }

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  float a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return true;              \
  }                                         \
}

bool coplanar_tri_tri(const MFVector& N, const MFVector& V0, const MFVector& V1, const MFVector& V2, const MFVector& U0, const MFVector& U1, const MFVector& U2)
{
	MFVector A;
	short i0,i1;

	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A.x=fabsf(N.x);
	A.y=fabsf(N.y);
	A.z=fabsf(N.z);

	if(A.x>A.y)
	{
		if(A.x>A.z)  
		{
			i0=1;      /* A[0] is greatest */
			i1=2;
		}
		else
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if(A.z>A.y)
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
		else
		{
			i0=0;      /* A[1] is greatest */
			i1=2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0,U0,U1,U2);
	POINT_IN_TRI(U0,V0,V1,V2);

	return false;
}

#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
{ \
	if(D0D1>0.0f) \
	{ \
		/* here we know that D0D2<=0.0 */ \
		/* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
	} \
	else if(D0D2>0.0f)\
	{ \
		/* here we know that d0d1<=0.0 */ \
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
	} \
	else if(D1*D2>0.0f || D0!=0.0f) \
	{ \
		/* here we know that d0d1<=0.0 or that D0!=0.0 */ \
		A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
	} \
	else if(D1!=0.0f) \
	{ \
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
	} \
	else if(D2!=0.0f) \
	{ \
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
	} \
	else \
	{ \
		/* triangles are coplanar */ \
		return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2); \
	} \
}

bool MFCollision_TriTriTest(const MFVector& V0,  const MFVector& V1, const MFVector& V2, const MFVector& U0,  const MFVector& U1, const MFVector& U2)
{
	MFVector E1, E2;
	MFVector N1, N2;
	MFVector D;

	float d1,d2;
	float du0,du1,du2,dv0,dv1,dv2;
	float isect1[2], isect2[2];
	float du0du1,du0du2,dv0dv1,dv0dv2;
	short index;
	float vp0,vp1,vp2;
	float up0,up1,up2;
	float bb,cc,max;
	float a,b,c,x0,x1;
	float d,e,f,y0,y1;
	float xx,yy,xxyy,tmp;

	/* compute plane equation of triangle(V0,V1,V2) */
	E1 = V1-V0;
	E2 = V2-V0;
	N1.Cross3(E1, E2);
	d1 = -N1.Dot3(V0);
	/* plane equation 1: N1.X+d1=0 */

	/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
	du0 = N1.Dot3(U0) + d1;
	du1 = N1.Dot3(U1) + d1;
	du2 = N1.Dot3(U2) + d1;

	/* coplanarity robustness check */
	if(fabsf(du0)<MFALMOST_ZERO) du0=0.0f;
	if(fabsf(du1)<MFALMOST_ZERO) du1=0.0f;
	if(fabsf(du2)<MFALMOST_ZERO) du2=0.0f;

	du0du1 = du0*du1;
	du0du2 = du0*du2;

	if(du0du1>0.0f && du0du2>0.0f)	/* same sign on all of them + not equal 0 ? */
		return false;				/* no intersection occurs */

	/* compute plane of triangle (U0,U1,U2) */
	E1 = U1 - U0;
	E2 = U2 - U0;
	N2.Cross3(E1, E2);
	d2 = -N2.Dot3(U0);
	/* plane equation 2: N2.X+d2=0 */

	/* put V0,V1,V2 into plane equation 2 */
	dv0 = N2.Dot3(V0) + d2;
	dv1 = N2.Dot3(V1) + d2;
	dv2 = N2.Dot3(V2) + d2;

	if(fabsf(dv0)<MFALMOST_ZERO) dv0=0.0;
	if(fabsf(dv1)<MFALMOST_ZERO) dv1=0.0;
	if(fabsf(dv2)<MFALMOST_ZERO) dv2=0.0;

	dv0dv1 = dv0*dv1;
	dv0dv2 = dv0*dv2;

	if(dv0dv1>0.0f && dv0dv2>0.0f)	/* same sign on all of them + not equal 0 ? */
		return false;				/* no intersection occurs */

	/* compute direction of intersection line */
	D.Cross3(N1, N2);

	/* compute and index to the largest component of D */
	max = fabsf(D.x);
	index = 0;
	bb = fabsf(D.y);
	cc = fabsf(D.z);
	if(bb>max) max = bb, index = 1;
	if(cc>max) max = cc, index = 2;

	/* this is the simplified projection onto L*/
	vp0 = V0[index];
	vp1 = V1[index];
	vp2 = V2[index];

	up0 = U0[index];
	up1 = U1[index];
	up2 = U2[index];

	/* compute interval for triangle 1 */
	NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

	/* compute interval for triangle 2 */
	NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

	xx=x0*x1;
	yy=y0*y1;
	xxyy=xx*yy;

	tmp=a*xxyy;
	isect1[0]=tmp+b*x1*yy;
	isect1[1]=tmp+c*x0*yy;

	tmp=d*xxyy;
	isect2[0]=tmp+e*xx*y1;
	isect2[1]=tmp+f*xx*y0;

	SORT(isect1[0],isect1[1]);
	SORT(isect2[0],isect2[1]);

	if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;

	return true;
}

bool MFCollision_RayHull()
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}

bool MFCollision_SphereHull()
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}

bool MFCollision_HullHull()
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}

bool MFCollision_TriHull()
{
	MFDebug_Assert(false, "Not Written!");
	return false;
}
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "TriangleMesh.h"
#include "Ray.h"

class Intersection {

public:
	Intersection();

	~Intersection();

	int intersectRayObjectsEarliest(const Ray<float> &ray, float &t, float &u, float &v, unsigned int &hitTri, int &prevHitTri,
		float &t_min, float &u_min, float &v_min, vector<TriangleMesh> &meshes, unsigned int &intersectionTests);

	bool rayAABBIntersect(const Ray<float> &r, const Vec3f& vmin, const Vec3f& vmax, float t0, float t1);

private:

};

#endif 
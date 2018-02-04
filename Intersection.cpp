#include "Intersection.h"

Intersection::Intersection() {

}
Intersection::~Intersection() {

}

int Intersection::intersectRayObjectsEarliest(const Ray<float> &ray, float &t, float &u, float &v, unsigned int &hitTri, int &prevHitTri, 
	float &t_min, float &u_min, float &v_min, vector<TriangleMesh> &meshes, unsigned int &intersectionTests) {
	//======== added ========//
	t_min = t;
	int currentNearestMesh = -1;
	//=======================//

	// iterate over all meshes
	for (unsigned int i = 0; i < meshes.size(); i++) {
		// optional: check ray versus bounding box first (t must have been initialized!)
		if (rayAABBIntersect(ray, meshes[i].boundingBoxMin, meshes[i].boundingBoxMax, 0.0f, t) == false) continue;
		// get triangle information
		vector<Vec3f>& vertices = meshes[i].getVertices();
		vector<Vec3ui>& triangles = meshes[i].getTriangles();
		// brute force: iterate over all triangles of the mesh
		for (unsigned int j = 0; j < triangles.size(); j++) {
			Vec3f& p0 = vertices[triangles[j][0]];
			Vec3f& p1 = vertices[triangles[j][1]];
			Vec3f& p2 = vertices[triangles[j][2]];
			int hit = ray.triangleIntersect(&p0.x, &p1.x, &p2.x, u, v, t);
			intersectionTests++;
			if (hit == 1 && t > 0.0f && j != prevHitTri) {
				//return i;

				//======== added ========//
				if (t < t_min) {
					t_min = t;
					hitTri = j;
					currentNearestMesh = i;
					u_min = u;
					v_min = v;
				}
				//=======================//
			}
		}
	}
	// changed
	return currentNearestMesh;
}

// Smits’ method: Brian Smits. Efficient bounding box intersection. Ray tracing news, 15(1), 2002.
bool Intersection::rayAABBIntersect(const Ray<float> &r, const Vec3f& vmin, const Vec3f& vmax, float t0, float t1) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (r.d.x >= 0) {
		tmin = (vmin.x - r.o.x) / r.d.x;
		tmax = (vmax.x - r.o.x) / r.d.x;
	}
	else {
		tmin = (vmax.x - r.o.x) / r.d.x;
		tmax = (vmin.x - r.o.x) / r.d.x;
	}
	if (r.d.y >= 0) {
		tymin = (vmin.y - r.o.y) / r.d.y;
		tymax = (vmax.y - r.o.y) / r.d.y;
	}
	else {
		tymin = (vmax.y - r.o.y) / r.d.y;
		tymax = (vmin.y - r.o.y) / r.d.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (r.d.z >= 0) {
		tzmin = (vmin.z - r.o.z) / r.d.z;
		tzmax = (vmax.z - r.o.z) / r.d.z;
	}
	else {
		tzmin = (vmax.z - r.o.z) / r.d.z;
		tzmax = (vmin.z - r.o.z) / r.d.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < t1) && (tmax > t0));
}
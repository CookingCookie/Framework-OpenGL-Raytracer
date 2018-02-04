#ifndef HELPERS_H
#define HELPERS_H

#include "TriangleMesh.h"
#include <algorithm>

class helpers {

public:
	helpers();

	~helpers();

	// calculates the standard norm of a 3d vector
	float eukl(Vec3f vec);

	float capInZeroToOne(float &x);

	Vec3f helpers::reflectiveRayDirection(Vec3f direction, Vec3f normal);

	Vec3f getRefrDir(Vec3f rayDir, Vec3f normal, int hitMesh);

private:

	float refr_transm();

	float getNR(int hitMesh);

};

#endif
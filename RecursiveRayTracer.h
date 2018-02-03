#ifndef RECURSIVERAYTRACER_H
#define RECURSIVERAYTRACER_H

#include "TriangleMesh.h"
#include "Ray.h"
#include "helpers.h"

class RecursiveRayTracer {

public:
	RecursiveRayTracer();

	~RecursiveRayTracer();

	bool inShadow(Vec3f &hitPoint, float t_min, Vec3f &lightDir, Vec3f &lightPos);

private:
	helpers helper;

};

#endif
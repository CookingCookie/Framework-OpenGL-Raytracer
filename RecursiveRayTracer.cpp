#include "RecursiveRayTracer.h"

RecursiveRayTracer::RecursiveRayTracer() {

}

RecursiveRayTracer::~RecursiveRayTracer() {

}

bool RecursiveRayTracer::inShadow(Vec3f &hitPoint, float t_min, Vec3f &lightDir, Vec3f &lightPos) {
	Vec3f V = lightPos - hitPoint;
	float ditanceToLight = helper.eukl(V);
	Ray <float> shadowRay(&hitPoint[0], &lightDir[0]);
	return true;
}
#ifndef RECURSIVERAYTRACER_H
#define RECURSIVERAYTRACER_H

#include "TriangleMesh.h"
#include "helpers.h"
#include "Intersection.h"
#include "Light.h"
#include "SceneObject.h"
#include "PhongIllumination.h"

class RecursiveRayTracer {

public:
	RecursiveRayTracer();

	~RecursiveRayTracer();

	vector <unsigned int> shadowRay(Vec3f hitPoint, unsigned int hitTri, vector <light> lights, vector<TriangleMesh> &meshes, unsigned int &intersectionTests);

	Vec3f calculateColor(Vec3f color, Ray <float> ray, float u_min, float v_min, float t_min, unsigned int hitTri, int hitMesh, unsigned int d_max,
		unsigned int &d, float reflectiveRayIntensity, float refractiveRayIntensity, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, vector <light> lights, Vec3f cameraDir, vector<SceneObject> objects);

private:
	helpers helper;

	Intersection intersection;

	PhongIllumination phong;

};

#endif
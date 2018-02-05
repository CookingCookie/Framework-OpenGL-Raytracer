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

	Vec3f calculateInterpolatedNormal(vector<TriangleMesh> &meshes, int hitMesh, unsigned int hitTri, float u_min, float v_min);

	Vec3f calculateReflection(vector<SceneObject> objects, int hitMesh, Ray <float> ray, Vec3f interpolatedNormal, Vec3f hitPoint,
		unsigned int hitTri, unsigned int &d, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, float reflectiveRayIntensity, unsigned int d_max, float refractiveRayIntensity, Vec3f color, vector <light> lights, Vec3f cameraDir);

	Vec3f calculateTransparency(vector<SceneObject> objects, int hitMesh, Ray <float> ray, Vec3f interpolatedNormal, Vec3f hitPoint,
		unsigned int hitTri, unsigned int &d, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, float reflectiveRayIntensity, unsigned int d_max, float refractiveRayIntensity, Vec3f color, vector <light> lights, Vec3f cameraDir);
	
	Vec3f calculateColor(Vec3f color, Ray <float> ray, float u_min, float v_min, float t_min, unsigned int hitTri, int hitMesh, unsigned int d_max,
		unsigned int &d, float reflectiveRayIntensity, float refractiveRayIntensity, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, vector <light> lights, Vec3f cameraDir, vector<SceneObject> objects);

private:
	helpers helper;

	Intersection intersection;

	PhongIllumination phong;

};

#endif
#include "RecursiveRayTracer.h"

RecursiveRayTracer::RecursiveRayTracer() {

}

RecursiveRayTracer::~RecursiveRayTracer() {

}

vector <unsigned int> RecursiveRayTracer::shadowRay(Vec3f hitPoint, unsigned int hitTri, vector <light> lights, vector<TriangleMesh> &meshes, unsigned int &intersectionTests) {
	vector <unsigned int> S;

	// we iterate over all light sources to see which of them are shining on our hit point
	for (unsigned int i = 0; i < lights.size(); i++) {
		// large t, that is further than anything we could hit
		float t = 1000.0f;

		// calculate the direction and distance to the light source
		Vec3f V = lights[i].lightPos - hitPoint;
		float distanceToLight = helper.eukl(V);
		V.normalize();

		// small epsilon > 0 which shall help avoiding numerical problems
		float epsilon = (float)pow(-8, 10);

		// we move our hit point a little so we won't hit it again
		Vec3f hitPointE = hitPoint + epsilon * V;

		// make a shadow ray pointing in direction of the light source
		Ray <float> shadowRay(&hitPointE[0], &V[0]);

		// to the hit test 
		float u, v, t_min, u_min, v_min;
		int prevHitTri = hitTri;
		int hitMesh = intersection.intersectRayObjectsEarliest(shadowRay, t, u, v, hitTri, prevHitTri, t_min, u_min, v_min, meshes, intersectionTests);

		// check whether the closest thing we hit is the light source or something else
		if (t_min >= distanceToLight - epsilon) S.push_back(1);
		else S.push_back(0);

		// check whether there were no numerical problems resulting in hitting the same triangle
		if (hitTri == prevHitTri && hitMesh != -1) {
			cout << "something went wrong, we hit the same triangle! Namely: " << hitTri << endl;
		}
	}
	return S;
}

Vec3f RecursiveRayTracer::calculateColor(Vec3f color, Ray <float> ray, float u_min, float v_min, float t_min, unsigned int hitTri, int hitMesh, unsigned int d_max,
	unsigned int &d, float reflectiveRayIntensity, float refractiveRayIntensity, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, vector <light> lights, Vec3f cameraDir, vector<SceneObject> objects) {
	// recursion anchor. Ray has been traced for the maximum amount of times already
	if (d > d_max) return Vec3f();

	// calculate where the triangle has been hit by the ray
	Vec3f hitPoint = ray.o + ray.d * t_min;

	// send out shadow feeler ray
	vector <unsigned int> S = shadowRay(hitPoint, hitTri, lights, meshes, intersectionTests);

	// get the triangle's vertex normals
	Vec3f vec1, vec2, vertexNormal0, vertexNormal1, vertexNormal2, interpolatedNormal;
	unsigned int id0, id1, id2;
	TriangleMesh m = meshes[hitMesh];
	Vec3ui tri = m.triangles[hitTri];
	id0 = tri[0];
	id1 = tri[1];
	id2 = tri[2];
	vertexNormal0 = m.normals[id0];
	vertexNormal1 = m.normals[id1];
	vertexNormal2 = m.normals[id2];

	// use characteristic of barycentric coordinates (u + v + w = 1) to get w
	float w_min = 1 - u_min - v_min;
	// interpolate the normal 
	interpolatedNormal = w_min * vertexNormal0 + u_min * vertexNormal1 + v_min * vertexNormal2;

	// calculate vector pointing from the hit point to the camera (eye)
	Vec3f V = cameraDir - hitPoint;
	V.normalize();

	// do the phong illumination algorithm
	color = phong.IlluminationCalculation(objects[hitMesh], lights, hitPoint, interpolatedNormal, V, S);

	// reflection ray
	if (objects[hitMesh].reflectivity > 0.0f) {
		// calculate reflective ray
		Vec3f reflectiveRayDirection = helper.reflectiveRayDirection(ray.d, interpolatedNormal);
		Ray <float> reflectiveRay(&hitPoint[0], &reflectiveRayDirection[0]);
		float t2 = 1000.0f;              // ray parameter hit point, initialized with max view length
		float u2, v2;                     // barycentric coordinates (w = 1-u-v)
		int prev2 = hitTri;
		int hitMesh2;
		unsigned int hitTri2;
		float t_min2, u_min2, v_min2;

		if ((hitMesh2 = intersection.intersectRayObjectsEarliest(reflectiveRay, t2, u2, v2, hitTri2, prev2, t_min2, u_min2, v_min2, meshes, intersectionTests)) != -1) {
			d++;
			color += reflectiveRayIntensity * calculateColor(color, reflectiveRay, u_min2, v_min2, t_min2, hitTri2, hitMesh2, d_max, d, reflectiveRayIntensity, refractiveRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
		}
	}

	// transparency ray
	if (objects[hitMesh].opacity > 0.0f) {
		// calculate reflective ray
		Vec3f refractiveRayDirection = helper.getRefrDir(ray.d, interpolatedNormal, hitMesh);
		Ray <float> refractiveRay(&hitPoint[0], &refractiveRayDirection[0]);
		float t2 = 1000.0f;              // ray parameter hit point, initialized with max view length
		float u2, v2;                     // barycentric coordinates (w = 1-u-v)
		int prev2 = hitTri;
		int hitMesh2;
		unsigned int hitTri2;
		float t_min2, u_min2, v_min2;

		if ((hitMesh2 = intersection.intersectRayObjectsEarliest(refractiveRay, t2, u2, v2, hitTri2, prev2, t_min2, u_min2, v_min2, meshes, intersectionTests)) != -1) {
			d++;
			color += refractiveRayIntensity * calculateColor(color, refractiveRay, u_min2, v_min2, t_min2, hitTri2, hitMesh2, d_max, d, reflectiveRayIntensity, refractiveRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
		}
	}

	return color;
}
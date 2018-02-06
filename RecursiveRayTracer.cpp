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

Vec3f RecursiveRayTracer::calculateInterpolatedNormal(vector<TriangleMesh> &meshes, int hitMesh, unsigned int hitTri, float u_min, float v_min) {
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
	return w_min * vertexNormal0 + u_min * vertexNormal1 + v_min * vertexNormal2;
}

Vec3f RecursiveRayTracer::calculateReflection(vector<SceneObject> objects, int hitMesh, Ray <float> ray, Vec3f interpolatedNormal, Vec3f hitPoint, 
	unsigned int hitTri, unsigned int &d, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, float reflectiveRayIntensity, 
	unsigned int d_max, float transparentRayIntensity, Vec3f color, vector <light> lights, Vec3f cameraDir) {
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
			// calculate reflective ray
			return lights.size() * objects[hitMesh].reflectivity * calculateColor(color, reflectiveRay, u_min2, v_min2, t_min2, hitTri2, hitMesh2, d_max, d, reflectiveRayIntensity, transparentRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
		}
	}

	return Vec3f(0,0,0);
}

float clamp(const float &lo, const float &hi, const float &v)
{
	return std::max(lo, std::min(hi, v));
}

Vec3f RecursiveRayTracer::refract(Ray <float> ray, Vec3f interpolatedNormal, const float &ior)
{
	float cosi = clamp((float)-1, (float)1, (float)(ray.d * interpolatedNormal));
	float etai = 1, etat = ior;
	Vec3f n = interpolatedNormal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = interpolatedNormal * -1; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	Vec3f tmp = ray.d * eta + (eta * cosi - sqrtf(k)) * n;
	return k < 0 ? Vec3f(0, 0, 0) : tmp;
}

void RecursiveRayTracer::fresnel(Ray <float> ray, Vec3f interpolatedNormal, const float &ior, float &kr)
{
	float cosi = clamp((float) -1, (float) 1, (float)(ray.d * interpolatedNormal));
	float etai = 1, etat = ior;
	if (cosi > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}

Vec3f RecursiveRayTracer::calculateRefraction(vector<SceneObject> objects, int hitMesh, Ray <float> ray, Vec3f interpolatedNormal, Vec3f hitPoint,
	unsigned int hitTri, unsigned int &d, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, float reflectiveRayIntensity, 
	unsigned int d_max, float transparentRayIntensity, Vec3f color, vector <light> lights, Vec3f cameraDir)
{
	if (objects[hitMesh].refraction > 0.0f) {
		Vec3f refractionColor = Vec3f(0, 0, 0);
		// compute fresnel
		float kr;
		float refractionRayIntensity = objects[hitMesh].refraction;
		fresnel(ray, interpolatedNormal, refractionRayIntensity, kr);
		bool outside = (ray.d * interpolatedNormal) < 0;
		Vec3f bias = interpolatedNormal * 0.0001;
		// compute refraction if it is not a case of total internal reflection
		if (kr < 1) {
			Vec3f refractionDirection = refract(ray, interpolatedNormal, refractionRayIntensity).normalized();
			//Vec3f refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;
			Ray <float> refractionRay(&hitPoint[0], &refractionDirection[0]);
			float t2 = 1000.0f;              // ray parameter hit point, initialized with max view length
			float u2, v2;                     // barycentric coordinates (w = 1-u-v)
			int prev2 = hitTri;
			int hitMesh2;
			unsigned int hitTri2;
			float t_min2, u_min2, v_min2;

			if ((hitMesh2 = intersection.intersectRayObjectsEarliest(refractionRay, t2, u2, v2, hitTri2, prev2, t_min2, u_min2, v_min2, meshes, intersectionTests)) != -1) {
				d++;
				refractionColor = calculateColor(color, refractionRay, u_min2, v_min2, t_min2, hitTri2, hitMesh2, d_max, d, reflectiveRayIntensity, transparentRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
			}
		}

		/*Vec3f reflectionDirection = helper.reflectiveRayDirection(ray.d, interpolatedNormal);
		Vec3f reflectionRayOrig = outside ? hitPoint + bias : hitPoint - bias;
		Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);*/

		// mix the two
		return /*reflectionColor * kr +*/ refractionColor * (1 - kr);
	}

	return Vec3f(0, 0, 0);
}

Vec3f RecursiveRayTracer::calculateTransparency(vector<SceneObject> objects, int hitMesh, Ray <float> ray, Vec3f interpolatedNormal, Vec3f hitPoint,
	unsigned int hitTri, unsigned int &d, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, float reflectiveRayIntensity, unsigned int d_max, float transparentRayIntensity, Vec3f color, vector <light> lights, Vec3f cameraDir) {
	// transparency ray
	if (objects[hitMesh].opacity > 0.0f) {
		// calculate reflective ray
		//Vec3f transparentRayDirection = helper.getRefrDir(ray.d, interpolatedNormal, hitMesh);
		//Ray <float> transparentRay(&hitPoint[0], &transparentRayDirection[0]);
		Vec3f ray_d = ray.d;
		Ray <float> transparentRay(&hitPoint[0], &ray_d[0]);
		float t2 = 1000.0f;              // ray parameter hit point, initialized with max view length
		float u2, v2;                     // barycentric coordinates (w = 1-u-v)
		int prev2 = hitTri;
		int hitMesh2;
		unsigned int hitTri2;
		float t_min2, u_min2, v_min2;

		if ((hitMesh2 = intersection.intersectRayObjectsEarliest(transparentRay, t2, u2, v2, hitTri2, prev2, t_min2, u_min2, v_min2, meshes, intersectionTests)) != -1) {
			d++;
			return objects[hitMesh].opacity * calculateColor(color, transparentRay, u_min2, v_min2, t_min2, hitTri2, hitMesh2, d_max, d, reflectiveRayIntensity, transparentRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
		}
	}

	return Vec3f(0, 0, 0);
}

Vec3f RecursiveRayTracer::calculateColor(Vec3f color, Ray <float> ray, float u_min, float v_min, float t_min, unsigned int hitTri, int hitMesh, unsigned int d_max,
	unsigned int &d, float reflectiveRayIntensity, float transparentRayIntensity, vector<TriangleMesh> &meshes, unsigned int &intersectionTests, vector <light> lights, Vec3f cameraDir, vector<SceneObject> objects) {
	// recursion anchor. Ray has been traced for the maximum amount of times already
	if (d > d_max) return Vec3f();

	// calculate where the triangle has been hit by the ray
	Vec3f hitPoint = ray.o + ray.d * t_min;

	// send out shadow feeler ray
	vector <unsigned int> S = shadowRay(hitPoint, hitTri, lights, meshes, intersectionTests);
	
	Vec3f interpolatedNormal = calculateInterpolatedNormal(meshes, hitMesh, hitTri, u_min, v_min);

	// calculate vector pointing from the hit point to the camera (eye)
	Vec3f V = cameraDir - hitPoint;
	V.normalize();

	// calculate reflexion
	Vec3f  recursiveRayIntensity  = calculateReflection(objects, hitMesh, ray, interpolatedNormal, hitPoint, hitTri, d, meshes, intersectionTests,
		reflectiveRayIntensity, d_max, transparentRayIntensity, color, lights, cameraDir);

	// do the phong illumination algorithm
	color = phong.IlluminationCalculation(objects[hitMesh], lights, hitPoint, interpolatedNormal, V, S, recursiveRayIntensity, objects[hitMesh].reflectivity);

	// calculate transparency
	//color += calculateTransparency(objects, hitMesh, ray, interpolatedNormal, hitPoint, hitTri, d, meshes, intersectionTests,
	//	reflectiveRayIntensity, d_max, transparentRayIntensity, color, lights, cameraDir);

	// calculate refraction
	//color += calculateRefraction(objects, hitMesh, ray, interpolatedNormal, hitPoint, hitTri, d, meshes, intersectionTests,
	//	reflectiveRayIntensity, d_max, transparentRayIntensity, color, lights, cameraDir);

	return color;
}
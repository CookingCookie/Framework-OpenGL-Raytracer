#include "helpers.h"

helpers::helpers() {

}

helpers::~helpers() {

}

float helpers::eukl(Vec3f vec) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

float helpers::capInZeroToOne(float &x) {
	return max(min(x, 1.0f), 0.0f);
}

// calculate ideal reflection of a ray
Vec3f helpers::reflectiveRayDirection(Vec3f direction, Vec3f normal) {
	direction.normalize();
	normal.normalize();
	float s = 2.0f * (direction*normal);
	Vec3f r = Vec3f(s*normal[0], s*normal[1], s*normal[2]) - direction;
	r.normalize();
	return r;
}

float helpers::refr_transm() {
	/* The index of refraction in a vacuum is defined to be 1, with all
	the other transmitting media having an index of refraction greater than 1.
	Air has an index of refraction just slightly greater than 1,
	so it is typical to assume n = 1 for the environment of a typical raytraced scene.
	However, water has an index of refraction n = 1.33,
	glass has an index n = 1.52,
	and diamond n = 2.42
	*/
	return 1.0f;
}

float helpers::getNR(int hitMesh) {
	if (hitMesh == 0) return 1.0f;
	else if (hitMesh == 1) return 1.4f;
	else if (hitMesh == 2) return 1.0f;
	else if (hitMesh == 3) return 1.2f;
	else return 1.0f;
}

Vec3f helpers::getRefrDir(Vec3f rayDir, Vec3f normal, int hitMesh) {
	float n1 = refr_transm(); // air
	float n2 = getNR(hitMesh);
	float cosI = (rayDir * normal) * (-1);
	float n = n1 / n2;
	float sin2t = n * n * (1.0f - cosI * cosI);
	if (sin2t > 1.0f) cout << "invalid refractive vector" << endl;
	float cosT = sqrt(1.0f - sin2t);
	return n * rayDir + (n * cosI - cosT) * normal;
}
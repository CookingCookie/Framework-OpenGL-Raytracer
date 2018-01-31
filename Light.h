#ifndef LIGHT_H
#define LIGHT_H

#include "TriangleMesh.h"

static Vec3f environmentalLightIntensity = { 0.1f, 0.1f, 0.1f };

struct light {
	Vec3f lightPos;
	float lightMotionSpeed;
	bool  moveLight;
	float lightIntensity;
};

#endif
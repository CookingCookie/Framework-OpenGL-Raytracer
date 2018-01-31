#ifndef PHONGILLUMINATION_H
#define PHONGILLUMINATION_H

#include "Vec3.h"
#include "TriangleMesh.h"
#include "Light.h"
#include "SceneObject.h"

class PhongIllumination {

public:
	// constructor
	PhongIllumination();
	// destructor
	~PhongIllumination();

	// performes the phong illumination calculation
	Vec3f IlluminationCalculation(SceneObject& sceneObject, vector <light>& lightSources, Vec3f hitPoint, Vec3f N, Vec3f V);

private:
	// mirror coefficient 
	unsigned int Ke = 64;

	// calculates the standard norm of a 3d vector
	float eukl(Vec3f vec);

};

#endif
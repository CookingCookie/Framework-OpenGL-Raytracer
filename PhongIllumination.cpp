#include "PhongIllumination.h"

// constructor
PhongIllumination::PhongIllumination() {

}

// destructor
PhongIllumination::~PhongIllumination() {

}

float PhongIllumination::eukl(Vec3f vec) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

Vec3f PhongIllumination::IlluminationCalculation(SceneObject& sceneObject, vector <light>& lightSources, Vec3f hitPoint, Vec3f N, Vec3f V) {
	unsigned int lss = lightSources.size();
	Vec3f I_ambient, DS_sumOverLights, L, LV, H;
	// iterate over r,g,b
	for (unsigned int i = 0; i < 3; i++) {
		I_ambient[i] = environmentalLightIntensity[i] * sceneObject.matAmbient[i];
		// iterate over all light sources
		for (unsigned int l = 0; l < lss; l++) {
			L = lightSources[l].lightPos - hitPoint;
			L.normalize();
			LV = L + V;
			H = (LV) / (eukl(LV));
			float KdFd = sceneObject.matDiffuse[i] * (L * N);
			float KsFs = sceneObject.matSpecular[i] * pow(Ke, (H * N));
			DS_sumOverLights[i] += lightSources[l].lightIntensity * (KdFd + KsFs);
		}
	}
	return I_ambient + DS_sumOverLights;
}
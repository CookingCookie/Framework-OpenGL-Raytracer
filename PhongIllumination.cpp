#include "PhongIllumination.h"

// constructor
PhongIllumination::PhongIllumination() {

}

// destructor
PhongIllumination::~PhongIllumination() {

}

Vec3f PhongIllumination::IlluminationCalculation(SceneObject& sceneObject, vector <light>& lightSources,
	Vec3f hitPoint, Vec3f N, Vec3f V, vector <unsigned int> S, Vec3f RecursiveRayIntensity, float Reflection) {
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
			H = (LV) / (helper.eukl(LV));
			float KdFd = sceneObject.matDiffuse[i] * (L * N);
			float KsFs = sceneObject.matSpecular[i] * (float)pow(Ke, (H * N));
			// S[l] is 0 when the hitpoint is not shown on by the light source
			// else S[l] is 1
			DS_sumOverLights[i] += S[l] * lightSources[l].lightIntensity * (KdFd + KsFs);
		}

		DS_sumOverLights[i] += Reflection * RecursiveRayIntensity[i];
	}
	return 	I_ambient + DS_sumOverLights;
}
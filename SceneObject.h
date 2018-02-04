#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

struct SceneObject {
	float matAmbient[4];
	float matDiffuse[4];
	float matSpecular[4];
	float matShininess;
	float reflectivity;
	float opacity;
	GLuint textureID;
};

#endif
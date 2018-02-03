#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdlib.h>  
#include <string>
#include <iostream>       // cout
#include <vector>
#include "Vec3.h"
#include "imageloader.h"

using namespace std;

class Skybox {

public:
	// constructor
	Skybox();

	// destructor
	~Skybox();

	// skybox texture IDs
	vector<GLuint> skyboxTextureIDs;

	void loadSkyBox(int selection, string filename, Image *image);

	void loadSpaceBox(string filename, Image *image);

	void loadSkyBox1(string filename, Image *image);

	void loadSkyBox2(string filename, Image *image);

	void loadSkyBox3(string filename, Image *image);

	void drawSkybox(Vec3f cameraDir);

	const unsigned int skyboxSelection = 0;

	bool inPosZDir;

private:

	GLuint textureID0, textureID1, textureID2, textureID3, textureID4;

	void renderQuads(Vec3f cameraDir);
};

#endif
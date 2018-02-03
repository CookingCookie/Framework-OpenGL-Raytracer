#include "Skybox.h"

// constructor
Skybox::Skybox() {

};

// destructor
Skybox::~Skybox() {

};

void Skybox::loadSkyBox(int selection, string filename, Image *image) {
	if (selection == 0) loadSpaceBox(filename, image);
	else if (selection == 1) loadSkyBox1(filename, image);
	else if (selection == 2) loadSkyBox2(filename, image);
	else if (selection == 3) loadSkyBox3(filename, image);
	else cout << "error: there is no skybox represented by this number!" << endl;
}

void Skybox::loadSpaceBox(string filename, Image *image) {
	// load space sky box
	filename = "Textures/skybox_space_blue/bkg1_back.bmp"; // neg-z
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[0] = loadTexture(image);
	filename = "Textures/skybox_space_blue/bkg1_right.bmp"; // pos-x
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[1] = loadTexture(image);
	filename = "Textures/skybox_space_blue/bkg1_front.bmp"; // pos-z
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[2] = loadTexture(image);
	filename = "Textures/skybox_space_blue/bkg1_left.bmp"; // neg-x
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[3] = loadTexture(image);
	filename = "Textures/skybox_space_blue/bkg1_top.bmp"; // pos-y
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[4] = loadTexture(image);
	filename = "Textures/skybox_space_blue/bkg1_bot.bmp"; // neg-y
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[5] = loadTexture(image);
	// we're setting up the quads looking in negative z-direction
	inPosZDir = false;
}

void Skybox::loadSkyBox1(string filename, Image *image) {
	filename = "Textures/skybox1/neg_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[0] = loadTexture(image);
	filename = "Textures/skybox1/pos_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[1] = loadTexture(image);
	filename = "Textures/skybox1/pos_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[2] = loadTexture(image);
	filename = "Textures/skybox1/neg_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[3] = loadTexture(image);
	filename = "Textures/skybox1/pos_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[4] = loadTexture(image);
	filename = "Textures/skybox1/neg_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[5] = loadTexture(image);
	// we're setting up the quads looking in positive z-direction
	inPosZDir = true;
}

void Skybox::loadSkyBox2(string filename, Image *image) {
	filename = "Textures/skybox2/neg_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[0] = loadTexture(image);
	filename = "Textures/skybox2/pos_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[1] = loadTexture(image);
	filename = "Textures/skybox2/pos_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[2] = loadTexture(image);
	filename = "Textures/skybox2/neg_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[3] = loadTexture(image);
	filename = "Textures/skybox2/pos_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[4] = loadTexture(image);
	filename = "Textures/skybox2/neg_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[5] = loadTexture(image);
	// we're setting up the quads looking in positive z-direction
	inPosZDir = true;
}

void Skybox::loadSkyBox3(string filename, Image *image) {
	filename = "Textures/skybox3/neg_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[0] = loadTexture(image);
	filename = "Textures/skybox3/pos_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[1] = loadTexture(image);
	filename = "Textures/skybox3/pos_z.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[2] = loadTexture(image);
	filename = "Textures/skybox3/neg_x.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[3] = loadTexture(image);
	filename = "Textures/skybox3/pos_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[4] = loadTexture(image);
	filename = "Textures/skybox3/neg_y.bmp";
	image = loadBMP(filename.c_str());
	skyboxTextureIDs[5] = loadTexture(image);
	// we're setting up the quads looking in positive z-direction
	inPosZDir = true;
}

void Skybox::renderQuads(Vec3f cameraDir) {
	// Store the current matrix
	glPushMatrix();

	// Reset and transform the matrix.
	glLoadIdentity();
	gluLookAt(
		0, 0, 0,
		cameraDir.x, cameraDir.y, cameraDir.z,
		0, 1, 0);

	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_BLEND);

	float size = 2.0f;

	// render neg-z quad (back of doppeldecker)
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(size, size, -size);
	glEnd();

	// render pos-x quad (left of doppeldecker)
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(size, size, size);
	glEnd();

	// render pos-z quad (front of doppeldecker)
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(size, size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	glEnd();


	// render neg-x quad (right of doppeldecker)
	glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
	glEnd();

	if (inPosZDir) {
		// render pos-y quad (top)
		glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
		glTexCoord2f(1, 0); glVertex3f(-size, size, size);
		glTexCoord2f(1, 1); glVertex3f(size, size, size);
		glTexCoord2f(0, 1); glVertex3f(size, size, -size);
		glEnd();

		// render neg-y quad (bottom)
		glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[5]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(size, -size, size);
		glEnd();
	}
	else {
		// render pos-y quad (top)
		glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, size, size);
		glTexCoord2f(1, 0); glVertex3f(size, size, size);
		glTexCoord2f(1, 1); glVertex3f(size, size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
		glEnd();

		// render neg-y quad (bottom)
		glBindTexture(GL_TEXTURE_2D, skyboxTextureIDs[5]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f(size, -size, size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, size);
		glEnd();
	}

	// restore matrix and attributes
	glPopMatrix();
}

void Skybox::drawSkybox(Vec3f cameraDir) {
	// prepare (no lighting, no depth test, texture filter)
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	// ===================== added ================================ //

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	renderQuads(cameraDir);

	// ============================================================ //

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// ========================================================================= //
// Authors: Roman Getto, Matthias Bein                                       //
// mailto:roman.getto@gris.informatik.tu-darmstadt.de                        //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Content: Simple and extendable openGL program offering                    //
//   * basic navigation and basic pipeline rendering                         //
// ========================================================================= //

#include <GL/glew.h>      // openGL helper
#include <GL/glut.h>      // openGL helper

#include <stdlib.h>       // namespace std
#include <math.h>         // fmod
#include <time.h>         // clock_t
#include <stdio.h>        // cout
#include <iostream>       // cout
#include <fstream>        // read file
#include <algorithm>	  // min, max	

#include "imageloader.h"  // simple class for loading bmp files

#include "main.h"

// ==============
// === BASICS ===
// ==============

int main(int argc, char** argv) {
	// initialize openGL window
	glutInit(&argc, argv);
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(300, 300);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("OpenGL Raytracer");
	// link functions to certain openGL events
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	// further initializations
	initialize();
	setDefaults();
	// load meshes
	string filename;
	TriangleMesh tm1;
	filename = "Models/ballon.off";
	tm1.loadOFF(filename.c_str(), Vec3f(0.0f, 0.0f, 0.0f), 20.0f);
	meshes.push_back(tm1);
	TriangleMesh tm2;
	filename = "Models/delphin.off";
	tm2.loadOFF(filename.c_str(), Vec3f(0.6f, 0.0f, 0.3f), 7.0f);
	meshes.push_back(tm2);
	TriangleMesh tm3;
	filename = "Models/Sketched-Teddy-org.off";
	tm3.loadOFF(filename.c_str(), Vec3f(6.6f, 0.0f, 0.9f), 7.0f);
	meshes.push_back(tm3);
	for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].coutData();
	// load textures
	Image* image;
	filename = "Textures/TEST_GRID.bmp";
	image = loadBMP(filename.c_str());
	textureIDs.push_back(loadTexture(image));
	// add object attributes (material, texture, ...)
	SceneObject so;
	//============ added ===============//
	so.reflectivity = 0.0f;
	so.opacity = 0.0f;
	//==================================//
	so.matAmbient[0] = 0.2f; so.matAmbient[1] = 0.1f; so.matAmbient[2] = 0.1f; so.matAmbient[3] = 1.0f;
	so.matDiffuse[0] = 0.6f; so.matDiffuse[1] = 0.3f; so.matDiffuse[2] = 0.3f; so.matDiffuse[3] = 1.0f;
	so.matSpecular[0] = 0.4f; so.matSpecular[1] = 0.4f; so.matSpecular[2] = 0.4f; so.matSpecular[3] = 1.0f;
	so.matShininess = 0.8f * 128.0f;
	so.textureID = textureIDs[0];
	objects.push_back(so);

	//============ added ===============//
	so.reflectivity = 0.0f;
	so.opacity = 0.2f;
	//==================================//
	so.matAmbient[0] = 0.1f; so.matAmbient[1] = 0.2f; so.matAmbient[2] = 0.1f; so.matAmbient[3] = 1.0f;
	so.matDiffuse[0] = 0.3f; so.matDiffuse[1] = 0.6f; so.matDiffuse[2] = 0.3f; so.matDiffuse[3] = 1.0f;
	so.matSpecular[0] = 0.4f; so.matSpecular[1] = 0.4f; so.matSpecular[2] = 0.4f; so.matSpecular[3] = 1.0f;
	so.matShininess = 0.8f * 128.0f;
	so.textureID = textureIDs[0];
	objects.push_back(so);


	//============ added ===============//
	so.reflectivity = 1.0f;
	so.opacity = 0.0f;
	so.matAmbient[0] = 0.3f; so.matAmbient[1] = 0.3f; so.matAmbient[2] = 0.3f; so.matAmbient[3] = 1.0f;
	so.matDiffuse[0] = 0.3f; so.matDiffuse[1] = 0.3f; so.matDiffuse[2] = 0.3f; so.matDiffuse[3] = 1.0f;
	so.matSpecular[0] = 0.8f; so.matSpecular[1] = 0.8f; so.matSpecular[2] = 0.8f; so.matSpecular[3] = 1.0f;
	so.matShininess = 0.9f * 128.0f;
	so.textureID = textureIDs[0];
	objects.push_back(so);
	//==================================//

	//======== added ========//
	lights.push_back(light0);
	lights.push_back(light1);
	numOfLights = lights.size();
	//=======================//

	/*
	****** choose skybox to load ******

	0 is skybox_space_blue
	1 is skybox1
	2 is skybox2
	3 is skybox3
	*/

	skybox.loadSkyBox(skybox.skyboxSelection, filename, image);

	delete image;
	// activate main loop
	coutHelp();
	glutTimerFunc(10, processTimedEvent, clock());
	glutMainLoop();
	return 0;
}

void initialize() {
	// black screen
	glClearColor(0, 0, 0, 0);
	// initialize glew
	GLenum err = glewInit();
	if (GLEW_OK != err) cout << "GLEW Error (" << err << "): " << glewGetErrorString(err) << endl;
	cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
	// check if VBOs are supported
	if (glGenBuffers && glBindBuffer && glBufferData && glBufferSubData &&
		glMapBuffer && glUnmapBuffer && glDeleteBuffers && glGetBufferParameteriv)
		cout << "VBOs are supported!" << endl;
	else cout << "VBOs are NOT supported!" << endl;
	// set shading model
	glShadeModel(GL_SMOOTH);
	// activate depth tests
	glEnable(GL_DEPTH_TEST);
	// enable light with defaults
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// sky box texture IDs (6 IDs, initialized with 0)
	skybox.skyboxTextureIDs.resize(6, 0);
}

void setDefaults() {
	// scene Information
	cameraPos.set(0, 0, 10);
	cameraDir.set(0, 0, -1);
	movementSpeed = 0.02f;
	// light0 information
	light0.lightPos.set(20.0f, 2.0f, 0.0f);
	light0.lightMotionSpeed = 0.05f;
	light0.moveLight = false;
	light0.lightIntensity = 0.2f;
	// light1 information
	light1.lightPos.set(14.0f, 6.0f, 0.0f);
	light1.lightMotionSpeed = 0.05f;
	light1.moveLight = false;
	light1.lightIntensity = 0.3f;
	// mouse information
	mouseButton = 0;
	mouseSensitivy = 1.0f;
	// key information, all false
	keyDown.resize(256, false);
	// draw mode (VBO)
	drawMode = 0;
}

void reshape(const GLint width, const GLint height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width / (float)height, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void processTimedEvent(const int x) {
	// Time now, int x is time of last run
	clock_t clock_this_run = clock();
	int msPassed = (clock_this_run - x) * 1000 / CLOCKS_PER_SEC;
	// perform light movement and trigger redisplay
	for (unsigned int i = 0; i < lights.size(); i++) {
		if (lights[i].moveLight == true) {
			lights[i].lightPos.rotY(lights[i].lightMotionSpeed*msPassed);
			glutPostRedisplay();
		}
	}
	// first person movement
	if (keyDown[(int)'w'] || keyDown[(int)'W']) {
		cameraPos += movementSpeed * cameraDir*(float)msPassed;
		glutPostRedisplay();
	}
	if (keyDown[(int)'s'] || keyDown[(int)'S']) {
		cameraPos -= movementSpeed * cameraDir*(float)msPassed;
		glutPostRedisplay();
	}
	if (keyDown[(int)'a'] || keyDown[(int)'A']) {
		Vec3f ortho(-cameraDir.z, 0.0f, cameraDir.x);
		ortho.normalize();
		cameraPos -= movementSpeed * ortho*(float)msPassed;
		glutPostRedisplay();
	}
	if (keyDown[(int)'d'] || keyDown[(int)'D']) {
		Vec3f ortho(-cameraDir.z, 0.0f, cameraDir.x);
		ortho.normalize();
		cameraPos += movementSpeed * ortho*(float)msPassed;
		glutPostRedisplay();
	}
	if (keyDown[(int)'q'] || keyDown[(int)'Q']) {
		cameraPos.y += movementSpeed * (float)msPassed;
		glutPostRedisplay();
	}
	if (keyDown[(int)'e'] || keyDown[(int)'E']) {
		cameraPos.y -= movementSpeed * (float)msPassed;
		glutPostRedisplay();
	}
	// start event again
	glutTimerFunc(std::max(1, 20 - msPassed), processTimedEvent, clock_this_run);
}

// =================
// === RENDERING ===
// =================

void drawCS() {
	glBegin(GL_LINES);
	// red X
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(5, 0, 0);
	// green Y
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);
	// blue Z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);
	glEnd();
}

// modified to support multiple light sources
void drawLight() {
	for (unsigned int i = 0; i < lights.size(); i++) {
		GLfloat lp[] = { lights[i].lightPos.x, lights[i].lightPos.y, lights[i].lightPos.z, 1.0f };
		GLfloat ld[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ls[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0 + i, GL_POSITION, lp);
		// these values differ for all lights other than GL_LIGHT0
		if (i != 0) {
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, ld);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, ls);
		}
		// draw yellow sphere for light source
		glPushMatrix();
		glTranslatef(lp[0], lp[1], lp[2]);
		glColor3f(1, 1, 0);
		glutSolidSphere(0.2f, 16, 16);
		glPopMatrix();
	}
}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	skybox.drawSkybox(cameraDir);
	glEnable(GL_DEPTH_TEST);

	// render ray trace result on a quad in front of the camera
	if (raytracedTextureID) {

		// save matrix and load identities => no transformation, no projection
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		skybox.drawSkybox(cameraDir);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		// render textured quad in the middle of the view frustum (depth 0) filling complete view window
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, raytracedTextureID);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex3d(-1.0, -1.0, 0);
		glTexCoord2d(1.0, 0.0); glVertex3d(1.0, -1.0, 0);
		glTexCoord2d(1.0, 1.0); glVertex3d(1.0, 1.0, 0);
		glTexCoord2d(0.0, 1.0); glVertex3d(-1.0, 1.0, 0);
		glEnd();
		// restore matrix
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	// else, standard openGL rendering
	else {
		// clear and set camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Vec3f cameraLookAt = cameraPos + cameraDir;
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,    // Position
			cameraLookAt.x, cameraLookAt.y, cameraLookAt.z, // Lookat
			0.0, 1.0, 0.0);           // Up-direction  
		// draw coordinate system without lighting
		glDisable(GL_LIGHTING);
		drawCS();
		// draw sphere for light still without lighting
		drawLight();
		// draw object
		glEnable(GL_LIGHTING);
		for (unsigned int i = 0; i < meshes.size(); i++) {
			// apply materials and texture
			glMaterialfv(GL_FRONT, GL_AMBIENT, objects[i].matAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, objects[i].matDiffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, objects[i].matSpecular);
			glMaterialf(GL_FRONT, GL_SHININESS, objects[i].matShininess);
			glBindTexture(GL_TEXTURE_2D, objects[i].textureID);
			// draw
			meshes[i].draw();
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// swap Buffers
	glutSwapBuffers();
}

void raytrace() {
	// initialization
	GLdouble MV[16];
	GLdouble PR[16];
	GLint VP[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, MV);
	glGetDoublev(GL_PROJECTION_MATRIX, PR);
	glGetIntegerv(GL_VIEWPORT, VP);
	vector<Vec3f> pictureRGB(VP[2] * VP[3]);
	intersectionTests = 0;
	unsigned int hits = 0;
	clock_t clockStart = clock();
	std::cout << "   10   20   30   40   50   60   70   80   90  100" << endl;
	std::cout << "====|====|====|====|====|====|====|====|====|====|" << endl;
	// iterate over all pixel
	unsigned int pixelCounter = 0;
#pragma omp parallel for schedule(dynamic)
	for (int y = VP[1]; y < VP[1] + VP[3]; y++) {
		for (int x = VP[0]; x < VP[0] + VP[2]; x++) {
			// get pixel index for addressing pictureRGB array
			int pixel = (y - VP[1])*(VP[2] - VP[0]) + x - VP[0];
			GLdouble end[3];
			GLdouble eye[3];
			// convert pixel coordinate to two points on near and far plane in world coordinates
			gluUnProject(x, y, -1, MV, PR, VP, &eye[0], &eye[1], &eye[2]);
			gluUnProject(x, y, 1, MV, PR, VP, &end[0], &end[1], &end[2]);
			// create primary ray
			float endF[3]; endF[0] = (float)end[0];  endF[1] = (float)end[1];  endF[2] = (float)end[2];
			float eyeF[3]; eyeF[0] = (float)eye[0];  eyeF[1] = (float)eye[1];  eyeF[2] = (float)eye[2];
			Ray<float> ray(&eyeF[0], &endF[0]);
			float t = 1000.0f;              // ray parameter hit point, initialized with max view length
			float u, v;                     // barycentric coordinates (w = 1-u-v)
			// intersection test
			int hitMesh;
			int prev = -1;
			unsigned int hitTri;
			unsigned int d_max = 6;
			unsigned int d = 0;
			float t_min, u_min, v_min;
			float reflectiveRayIntensity = 0.1f;
			float refractiveRayIntensity = 0.2f;
			Vec3f color = Vec3f();
			if ((hitMesh = intersection.intersectRayObjectsEarliest(ray, t, u, v, hitTri, prev, t_min, u_min, v_min, meshes, intersectionTests)) != -1) {
				// TODO: calculate color
				pictureRGB[pixel] = rrt.calculateColor(color, ray, u_min, v_min, t_min, hitTri, hitMesh, d_max, d, reflectiveRayIntensity, refractiveRayIntensity, meshes, intersectionTests, lights, cameraDir, objects);
				hits++;
			}
			// cout "." every 1/50 of all pixels
#pragma omp flush (pixelCounter)
			pixelCounter++;
#pragma omp flush (pixelCounter)
			if (pixelCounter % (VP[2] * VP[3] / 50) == 0) cout << ".";
		}
	}
	clock_t clockEnd = clock();
	int msPassed = (clockEnd - clockStart) * 1000 / CLOCKS_PER_SEC;
	cout << endl << "finished. tests: " << intersectionTests << ", hits: " << hits << ", ms: " << msPassed << endl;
	// generate openGL texture
	float mul = 255.0f; // multiply rgb values within [0,1] by 255 
	cout << "normalizing picture with multiplicator " << mul << endl << endl;
	vector<GLuint> picture(VP[2] * VP[3]);
	for (int y = VP[1]; y < VP[1] + VP[3]; y++) {
		for (int x = VP[0]; x < VP[0] + VP[2]; x++) {
			// get pixel index for addressing pictureRGB array
			int pixel = (y - VP[1])*(VP[2] - VP[0]) + x - VP[0];
			// cap R G B within 0 and 255
			int R = max(0, min((int)(mul*pictureRGB[pixel][0]), 255));
			int G = max(0, min((int)(mul*pictureRGB[pixel][1]), 255));
			int B = max(0, min((int)(mul*pictureRGB[pixel][2]), 255));
			// enter R G B values into GLuint array
			picture[pixel] = R | (G << 8) | (B << 16);
		}
	}
	// generate texture
	glGenTextures(1, &raytracedTextureID);
	glBindTexture(GL_TEXTURE_2D, raytracedTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, VP[2], VP[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, &picture[0]);
}

// =================
// === CALLBACKS ===
// =================

void keyPressed(const unsigned char key, const int x, const int y) {
	// added
	const unsigned char key2 = -1;

	keyDown[key] = true;
	switch (key) {
	case 27:
		exit(0);
		break;
	case '1':
		glShadeModel(GL_FLAT);
		glutPostRedisplay();
		break;
	case '2':
		glShadeModel(GL_SMOOTH);
		glutPostRedisplay();
		break;
	case 'M':
	case 'm':
		drawMode = (drawMode + 1) % 3;
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].setDrawMode(drawMode);
		glutPostRedisplay();
		break;
	case 'h':
	case 'H':
		coutHelp();
		break;
		// reset view
	case 'r':
	case 'R':
		setDefaults();
		glutPostRedisplay();
		break;
		// light movement
	case 'l':
	case 'L':
		l = (l + 1) % numOfLights;
		lights[l].moveLight = !lights[l].moveLight;
		break;
	case 'p':
	case 'P':
		lights[0].lightPos = cameraPos;
		glutPostRedisplay();
		break;
		// raytrace
	case 'x':
	case 'X':
		raytrace();
		glutPostRedisplay();
		break;
	case 'y':
	case 'Y':
		glDeleteTextures(1, &raytracedTextureID);
		raytracedTextureID = 0;
		glutPostRedisplay();
		break;
		// color array
	case 'c':
	case 'C':
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].toggleWithColorArray();
		glutPostRedisplay();
		break;
		// texture
	case 't':
	case 'T':
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].toggleWithTexture();
		glutPostRedisplay();
		break;
		// bounding box
	case 'b':
	case 'B':
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].toggleWithBB();
		glutPostRedisplay();
		break;
		// normal
	case 'n':
	case 'N':
		for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].toggleWithNormals();
		glutPostRedisplay();
		break;
		// movement speed up
	case '+':
		movementSpeed *= 2.0f;
		break;
		// movement speed down
	case '-':
		movementSpeed /= 2.0f;
		break;
	}
}

void keyReleased(const unsigned char key, const int x, const int y) {
	keyDown[key] = false;
}

void mousePressed(const int button, const int state, const int x, const int y) {
	mouseButton = button;
	mouseX = x;
	mouseY = y;
}

void mouseMoved(int x, int y) {
	// update angle with relative movement
	angleX = fmod(angleX + (x - mouseX)*mouseSensitivy, 360.0f);
	angleY -= (y - mouseY)*mouseSensitivy;
	angleY = max(-70.0f, min(angleY, 70.0f));
	// calculate camera direction (vector length 1)
	cameraDir.x = sin(angleX*M_RadToDeg) * cos(angleY*M_RadToDeg);
	cameraDir.z = -cos(angleX*M_RadToDeg) * cos(angleY*M_RadToDeg);
	cameraDir.y = max(0.0f, min(sqrtf(1.0f - cameraDir.x*cameraDir.x - cameraDir.z* cameraDir.z), 1.0f));
	if (angleY < 0) cameraDir.y = -cameraDir.y;
	// update mouse for next relative movement
	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

// ===============
// === VARIOUS ===
// ===============

void coutHelp() {
	cout << endl;
	cout << "====== KEY BINDINGS ======" << endl;
	cout << "ESC: exit" << endl;
	cout << "H: show this (h)elp file" << endl;
	cout << "R: (r)eset to default values" << endl;
	cout << "L: toggle (l)ight movement" << endl;
	cout << "P: move the light to current (p)osition" << endl;
	cout << endl;
	cout << "A,W,S,D,Q,E: first person movement" << endl;
	cout << "+,-: movement speed up and down" << endl;
	cout << endl;
	cout << "1: GL Shader FLAT" << endl;
	cout << "2: GL Shader SMOOTH" << endl;
	cout << endl;
	cout << "M: switch draw (m)ode. 0: immediate, 1: array, 2: VBO" << endl;
	cout << "C: toggle use of (c)olor array" << endl;
	cout << "T: toggle use of (t)exture coordinates" << endl;
	cout << "B: toggle drawing (b)ounding box" << endl;
	cout << "N: toggle drawing (n)ormals" << endl;
	cout << endl;
	cout << "X: raytrace scene" << endl;
	cout << "Y: clear and switch back to openGL rendering" << endl;
	cout << "==========================" << endl;
	cout << endl;
}
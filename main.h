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

#ifndef MAIN_H
#define MAIN_H

#include "Vec3.h"         // basic vector arithmetic class (embedded in std::)
#include "TriangleMesh.h" // simple class for reading and rendering triangle meshes
#include "Ray.h"          // class for ray intersection with triangles
#include "PhongIllumination.h"
#include "Light.h"
#include "SceneObject.h"
#include "Skybox.h"

using namespace std;

// ===================
// === GLOBAL DATA ===
// ===================

//=========================================================//

//------------- changable values -------------------// 

// lights
light light0, light1;
// vector containing all lights
vector <light> lights;
// for switching the light movement
unsigned int l;
// the total number of lights
unsigned int numOfLights;

//=========================================================//


PhongIllumination phong;

helpers helper;

vector <unsigned int> shadowRay(Vec3f hitPoint, unsigned int hitTri);

Vec3f calculateColor(Ray <float> ray, float u_min, float v_min, float t_min, unsigned int hitTri, int hitMesh);

Skybox skybox;

// camera Information
Vec3f cameraPos;
Vec3f cameraDir;
float angleX, angleY, movementSpeed;
int mouseX, mouseY, mouseButton;
float mouseSensitivy;
// key pressed information
vector<bool> keyDown;
// objects
vector<TriangleMesh> meshes;
vector<GLuint> textureIDs;
vector<SceneObject> objects;
// draw mode
unsigned int drawMode;
// raytraced texture
GLuint raytracedTextureID;
unsigned int intersectionTests;

// ==============
// === BASICS ===
// ==============

int main(int argc, char **argv);

void initialize();

void setDefaults();

void reshape(const GLint width, const GLint height);

void processTimedEvent(const int x);

// =================
// === RENDERING ===
// =================

void drawCS();

void drawLight();

void renderScene(void);

void raytrace();

int intersectRayObjectsEarliest(const Ray<float> &ray, float &t, float &u, float &v, unsigned int &hitTri, int &prevHitTri, float &t_min, float &u_min, float &v_min);

bool rayAABBIntersect(const Ray<float> &r, const Vec3f& vmin, const Vec3f& vmax, float t0, float t1);

// =================
// === CALLBACKS ===
// =================

void keyPressed(const unsigned char key, const int x, const int y);

void keyReleased(const unsigned char key, const int x, const int y);

void mousePressed(const int button, const int state, const int x, const int y);

void mouseMoved(const int x, const int y);

// ===============
// === VARIOUS ===
// ===============

void coutHelp();

#endif
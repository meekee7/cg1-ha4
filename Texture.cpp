/* ----------------------------------------------------------------
   name:           Texture.cpp
   purpose:        texturing tutorial
   'introduction to computer graphics'
   winter term 2012/2013, assignment 4
   version:	   SKELETON CODE
   TODO:           see XXX
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#ifdef __APPLE__ 
#include <GL/glew.h>
#include <GL/freeglut.h>
#elif _WIN32
#include "win32/GL/glew.h"
#include "win32/GL/freeglut.h"
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "Context.hpp"
#include "Texture.hpp"
#include "Image.hpp"
#include "Model.hpp"

using namespace glm;
using namespace std;

static const float PI = 3.14159265358979323846264338327950288f;

// current state of mouse action
typedef enum{
	ROTATE, SHIFT_XY, SHIFT_Z, SCALE, NO_DRAG, DRAW, ERASE
} dragstate;
static dragstate drag = NO_DRAG;

static bool showTexture = true;
static bool textureCorrection = true;
static bool lighting = true;
static bool showCoordinates = true;
static bool showOrigin = true;
static bool environmentMapping = false;
static bool moveEnvironment = false;
static bool drawRect = false;

static GLuint modulation = GL_MODULATE;
static GLuint wrap = GL_CLAMP_TO_BORDER;

static mat4 cameraMatrix;
static mat4 rotation = mat4(1); // current rotation of object
static vec3 shift = vec3(0); // offset
static float scaling = 1; // scale

static vec2 screen;  // screen size
static float fov = 60.0; // field of view
// camera setup
// camera position
static GLfloat cameraZ;
// cameraZ in original sphere map setup
static GLfloat cameraZMap = 0;
// near and far plane
static GLfloat nearPlane;
static GLfloat farPlane;

static Image texture;

static vec3 cursor = vec3(1, 0, 0);

/*************************************************************************************/

// calculate cursor position
// XXX: NEEDS TO BE IMPLEMENTED
static void updateCursor(int x, int y){

	// XXX 
	//TODO this is not ideal so far and drifts off , enhance this
	//This is also related to the spherical texture mapping algorithm
	float radius = 2 * PI * (x / screen.x);
	cursor = vec3(-sin(radius), -((y / screen.y) * 2 - 1), sin(radius - PI / 2));
	//IMPORTANT NODE: currently, this vector is later normalized, scaled and rotate by +90/0/1/0
	//maybe this has to be modified, especially the rotation
	// INSERT YOUR CODE HERE


	// END XXX
}

static void reset(){

	rotation = mat4(1); // current rotation of object
	cameraZMap = 0;
	shift = vec3(0); // offset
	fov = 60.0;
}

void Common::keyPressed(unsigned char key, int x, int y){

	switch (key){

	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'r':
		reset();
		break;

	case 's':
		scaling *= 0.9f;
		break;

	case 'S':
		scaling *= 1.1f;
		break;

	case 'c':
		cameraZMap += 0.1f;
		break;

	case 'C':
		cameraZMap -= 0.1f;
		break;

	default:
		break;
	}

	Context::display();
}

// draw a full screen quad
// XXX: NEEDS TO BE IMPLEMENTED
static void fullScreenQuad(){
	// XXX

	// INSERT YOUR CODE HERE
	GLfloat depth = 0.0f;
	glBegin(GL_QUADS); {
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, screen.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(screen.x, screen.y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(screen.x, 0.0f);
	}glEnd();
	// END XXX
}

// -------------------------------------------------------
// TEXTURE WINDOW
// -------------------------------------------------------

int Texture::menuOptions[] = { 0, 17, 18, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 0, 19, 20, 21, 22, 23, 24, 25, 26, 0, 27, 28 };
string Texture::menuText[] = { "TOOLS:", "    Pen", "    Eraser",
"SPHERICAL TEXTURES:", "    Earth", "    Earth (ice)", "    Earth (night)", "    Saturn", "    Marble",
"ENVIRONMENT TEXTURES:", "    St Peters dome", "    Uffizium", "    Supernova", "    Landscape", "    Forest", "    Phong Light",
"MISC TEXTURES", "    Checkerboard", "    Grid1", "    Grid2", "    Spectrum", "    Nemo",
"FILTERING", "    mag: NEAREST", "    mag: LINEAR", "    min: NEAREST", "    min: LINEAR", "    min: NEAREST_MIPMAP_NEAREST  ", "    min: LINEAR_MIPMAP_NEAREST", "    min: NEAREST_MIPMAP_LINEAR", "    min: LINEAR_MIPMAP_LINEAR",
"SILHOUETTES", "    narrow", "    broad" };

int Texture::numOptions = sizeof(Texture::menuOptions) / sizeof(Texture::menuOptions[0]);

string textures[] = { "", "data/earthcyl2.ppm", "data/earth2.ppm", "data/earthlights.ppm", "data/saturncyl1.ppm", "data/marble.ppm", "data/stpeters.ppm", "data/uffizi.ppm", "data/supernova.ppm", "data/test5b.ppm", "data/test7b.ppm", "data/test6b.ppm", "data/checker2.ppm", "data/test3b.ppm", "data/test4b.ppm", "data/test2b.ppm", "data/test8b.ppm", "data/blackcircle.ppm", "data/blackplane.ppm" };

vec2 Texture::previousMouse; // previous mouse position

void Texture::reshape(int width, int height){

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	gluOrtho2D(0, width, 0, height);*/

	screen = vec2(width, height);
}

bool usedefaulttex = true;
// display texture
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::display(void){
	//Texture::reshape(screen.x, screen.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, (GLsizei)screen.x, (GLsizei)screen.y);

	gluOrtho2D(0, screen.x, 0, screen.y);

	// setup model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// display texture
	// XXX
	if (usedefaulttex) {
		texture.load(textures[9]);
		texture.generateTexture();
		usedefaulttex = false;
	}
	// INSERT YOUR CODE HERE
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	texture.bind();
	fullScreenQuad();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	// END XXX
	glutSwapBuffers();
}

void Texture::mousePressed(int button, int state, int x, int y){

	if (button == GLUT_DOWN) previousMouse = vec2(x, y);
}

// mouse dragged callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::mouseDragged(int x, int y){

	// paint on texture
	// XXX
	if (drag == DRAW)
		texture.paint(x / screen.x, (screen.y - y) / screen.y);
	if (drag == ERASE)
		texture.erase(x / screen.x, (screen.y - y) / screen.y);
	// INSERT YOUR CODE HERE

	// END XXX

	updateCursor(x, y);

	previousMouse = vec2(x, y);

	Context::display();
}

void Texture::mouseMoved(int x, int y){

	if (x > 0 && x < screen.x && y > 0 && y < screen.y){
		updateCursor(x, y);
		Context::display();
	}
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::menu(int value){
	switch (value){
	case 27:
	case 28:
		value -= 10; //purposely no break here
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		texture.load(textures[value]);
		texture.generateTexture();
		if (value < 6)
			environmentMapping = false;
		else if (value < 13)
			environmentMapping = true;
		else if (value > 16)
			environmentMapping = true;
		break;
	case 17:
		drag = DRAW;
		break;
	case 18:
		drag = ERASE;
		break;
	case 19:
		texture.setMagFilter(GL_NEAREST);
		break;
	case 20:
		texture.setMagFilter(GL_LINEAR);
		break;
	case 21:
		texture.setMinFilter(GL_NEAREST);
		break;
	case 22:
		texture.setMinFilter(GL_LINEAR);
		break;
	case 23:
		texture.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
		break;
	case 24:
		texture.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
		break;
	case 25:
		texture.setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
		break;
	case 26:
		texture.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		break;
		// add cases for texture filtering
		// XXX

		// INSERT YOUR CODE HERE

		// END XXX
	default:
		break;
	}

	Context::display();
}

// -------------------------------------------------------
// WORLD-SPACE WINDOW
// -------------------------------------------------------

int World::menuOptions[] = { 24, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
0, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
string World::menuText[] = { "    reset", "MODEL", "    Plane", "    Spiky Sphere", "    Car", "    Bunny", "    Cone", "    Cow", "    Cowboy Hat", "    Dragon", "    Chess", "    Temple", "    Cup", "    Space Shuttle", "    Sphere", "    None",
"RENDERING", "    Lighting on/off", "    Texture on/off", "    Coordinate System on/off", "    Origin on/off",
"    Texture Coordinate Correction on/off  ", "    Texture Mode (DECAL/MODULATE) ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment" };

int World::numOptions = sizeof(World::menuOptions) / sizeof(World::menuOptions[0]);

static string models[] = { "", "", "data/4cow.off", "data/auto3.off", "data/bunny2.off", "data/cone.off", "data/cow.off", "data/cowboyhut.off", "data/MEGADRACHE.off", "data/Schachfigur.off", "data/tempel.off", "data/tasse.off", "data/spaceshuttle.off", "data/sphere.off" };
int meshnumber = 13;
Mesh* meshes[14];
GLSLShader* evshader = nullptr;

vec2 World::previousMouse;

void World::reshape(int width, int height){

	// setup projection matrix
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	cameraZ = 1 / tan(fov / 180.0);

	// near and far plane
	nearPlane = cameraZ / 10.0;
	farPlane = cameraZ*10.0;

	gluPerspective(fov, (float)width / (float)height, nearPlane, farPlane);

	//position the camera at (0,0,cameraZ) looking down the
	//negative z-axis at (0,0,0)
	cameraMatrix = lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));*/

	screen = vec2(width, height);
}

// display callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::display(void){
	if (meshes[meshnumber] == nullptr && meshnumber != 0 && meshnumber != 1){
		//Draw a loading screen
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glColor3ub(255, 255, 255);
		glRasterPos2i(30, 30);
		char loading[] = "LOADING MODEL, PLEASE WAIT";
		for (char* s = loading; *s; s++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
		glEnable(GL_LIGHTING);
		glutSwapBuffers();

		//now actually load mesh
		meshes[meshnumber] = new Mesh();
		meshes[meshnumber]->loadOff(models[meshnumber]);
		meshes[meshnumber]->setRenderMode(Mesh::TEXTURE_RENDERER);
	}

	//World::reshape(screen.x, screen.y);
	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, (GLsizei)screen.x, (GLsizei)screen.y);

	cameraZ = 1.0f / tanf(fov / 180.0f);

	// near and far plane
	nearPlane = cameraZ / 10.0f;
	farPlane = cameraZ*10.0f;

	gluPerspective(fov, (float)screen.x / (float)screen.y, nearPlane, farPlane);

	//position the camera at (0,0,cameraZ) looking down the
	//negative z-axis at (0,0,0)
	cameraMatrix = lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&cameraMatrix[0][0]);

	GLfloat lightPosition[] = { 5.0, 5.0, 5.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	// shift object
	glTranslatef(shift.x, shift.y, shift.z);

	// rotate Object
	glMultMatrixf(&rotation[0][0]);

	//show coordinate system
	if (showCoordinates){
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);

		glColor3ub(0, 255, 0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);

		glColor3ub(0, 0, 255);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}

	// show center of spherical mapping
	if (showOrigin){
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 0.0);
		glPushMatrix();

		glutSolidSphere(0.05f, 20, 20);
		glPopMatrix();
	}

	// draw cursor
	// XXX
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES); {
			GLfloat endpoint[3] = { cursor.x, cursor.y, cursor.z };
			Mesh::normalizevector(endpoint);
			for (int i = 0; i < 3; i++)
				endpoint[i] *= 1.1f;
			glColor3ub(255, 255, 0);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(endpoint);
		}glEnd();
		glPopMatrix();
	}
	// INSERT YOUR CODE HERE

	// END XXX

	glScalef(scaling, scaling, scaling);

	if (lighting) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	// draw the geometry
	if (environmentMapping){
		if (evshader == nullptr){
			evshader = new GLSLShader();
			evshader->load("envphong");
		}
		evshader->bindShader();
		const int envrotate = moveEnvironment ? 1 : 0;
		const int light = lighting ? 1 : 0;
		glm::mat4 rotatemat;
		glm::mat4 cameramat;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++){
				rotatemat[i][j] = rotation[i][j];
				cameramat[i][j] = cameraMatrix[i][j];
			}
		evshader->setIntParam("lighting", light);
		evshader->setIntParam("envrotate", envrotate);
		evshader->setMatrix4Param("rotatemat", rotatemat);
		evshader->setMatrix4Param("cameramat", cameramat);
	}

	// if showTexture is true, enable texturing in opengl
	// XXX
	if (meshnumber != 0){
		if (showTexture)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		if (moveEnvironment){
			glLoadIdentity();
			glMultMatrixf(&cameraMatrix[0][0]);
		}
		if (!drawRect)
			meshes[meshnumber]->render();
		else {
			glBegin(GL_QUADS); {
				glNormal3f(0.0f, -1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(-1.0f, -1.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(-1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(1.0f, -1.0f);
			} glEnd();
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	// INSERT YOUR CODE HERE
	if (environmentMapping)
		evshader->unbindShader();
	// END XXX

	glColor3f(1, 1, 1);

	// build matrices and pass them to shader
	// REMEMBER when transforming: Environment appears mirrored in object
	// XXX 

	// INSERT YOUR CODE HERE

	// END XXX

	// else draw model
	// XXX

	// INSERT YOUR CODE HERE

	// END XXX

	glutSwapBuffers();
}

void World::mousePressed(int button, int state, int x, int y){

	int modifier;

	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			previousMouse = vec2(x, y);
			modifier = glutGetModifiers();
			if (modifier & GLUT_ACTIVE_CTRL)
				drag = SHIFT_XY;
			else if (modifier & GLUT_ACTIVE_SHIFT)
				drag = SHIFT_Z;
			else if (modifier & GLUT_ACTIVE_ALT)
				drag = SCALE;
			else
				drag = ROTATE;
		}
		else if (state == GLUT_UP){
			drag = NO_DRAG;
		}
		break;
	default: break;
	}
}

void World::mouseDragged(int x, int y){

	vec2 v = (vec2(x, y) - previousMouse) / screen;

	switch (drag){
	case ROTATE:
		if (length(v) == 0) break;
		rotation = rotate(mat4(1), 180 * length(v), normalize(vec3(v.y, v.x, 0))) * rotation;
		break;
	case SHIFT_XY:
		if (false){
			shift.x += 3.3f*v.x;
			shift.y -= 3.3f*v.y;
		}
		else{
			shift.x += 3.3f*v.x;
			shift.y -= 3.3f*v.y;
		}
		break;
	case SHIFT_Z:
		if (false) shift.z -= 3.3f*sign(dot(v, vec2(1, 1))) * length(v);
		else shift.z += 3.3f*sign(dot(v, vec2(1, 1))) * length(v);
		break;
	case SCALE:
		if (false) scaling -= sign(dot(v, vec2(1, 1))) * length(v);
		else scaling += sign(dot(v, vec2(1, 1))) * length(v);
		break;
	default:
		break;
	}
	previousMouse = vec2(x, y);

	Context::display();
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::menu(int value){

	switch (value){
	case 24:
		reset();
		break;
	case 0:
	case 1:
		drawRect = true;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		// load model from models[value]
		// XXX
		meshnumber = value;
		// INSERT YOUR CODE HERE

		// END XXX
		drawRect = false;
		break;
	case 14:
		// no model should be displayed with this option selected
		// XXX
		meshnumber = 0;
		// INSERT YOUR CODE HERE

		// END XXX
		break;
	case 15:
		lighting = !lighting;
		break;
	case 16:
		showTexture = !showTexture;
		break;
	case 17:
		showCoordinates = !showCoordinates;
		break;
	case 18:
		showOrigin = !showOrigin;
		break;

	case 19:
		textureCorrection = !textureCorrection;
		// enable/disable texture correction in Image (not obligatory, but useful for debugging)
		// XXX

		// INSERT YOUR CODE HERE

		// END XXX
		break;
	case 20:
		if (modulation == GL_MODULATE) modulation = GL_DECAL;
		else  modulation = GL_MODULATE;
		texture.setModulation(modulation);
		break;
	case 21:
		// set texture wrapping in Image (not obligatory, but useful for debugging)
		// XXX

		// INSERT YOUR CODE HERE

		// END XXX
		break;
	case 22:
		environmentMapping = !environmentMapping;
		if (!environmentMapping) moveEnvironment = false;
		break;
	case 23:
		reset();
		moveEnvironment = !moveEnvironment;
		break;
	default:
		break;
	}
	Context::display();
}

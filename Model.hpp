#include "GLSLShader.h"
#pragma once
class Mesh
{
public:
	Mesh();
	~Mesh();
	bool loadOff(const char* file);
	void printmesh();
	enum RenderMode{
		FLAT_RENDERER,
		GOURAUD_RENDERER,
	};
	void setRenderMode(RenderMode mode);
	void render();
	
private:
	typedef struct {
		int size;
		GLfloat normal[3];
		int* nodes;
	} poly;
	typedef struct {
		GLfloat normal[3];
		GLfloat node[3];
	} nodestruct;
	int nodes;
	int polygons;
	int edges;
	nodestruct* node;
	poly* polygon;
	RenderMode rendermode;
	void renderFlat();
	void renderSmooth();
	void normalizevector(GLfloat* vector);
	void crossproduct(GLfloat* a, GLfloat* b, GLfloat* result);
};

typedef struct meshcircle {
	Mesh* mesh;
	const char* name;
	meshcircle* next;
} meshcircle;
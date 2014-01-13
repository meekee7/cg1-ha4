#include "GLSLShader.h"
#pragma once
class Mesh
{
public:
	Mesh();
	~Mesh();
	bool loadOff(std::string file);
	void printmesh();
	enum RenderMode{
		FLAT_RENDERER,
		GOURAUD_RENDERER,
		TEXTURE_RENDERER
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
		GLfloat tex[2];
	} nodestruct;
	int nodes;
	int polygons;
	int edges;
	nodestruct* node;
	poly* polygon;
	RenderMode rendermode;
	void renderFlat();
	void renderSmooth();
	void renderTextured();
	void normalizevector(GLfloat* vector);
	void crossproduct(GLfloat* a, GLfloat* b, GLfloat* result);
};
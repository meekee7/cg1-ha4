/* ----------------------------------------------------------------
   name:           Image.cpp
   purpose:        texturing tutorial
   'introduction to computer graphics'
   winter term 2012/2013, assignment 4
   version:	   SKELETON CODE
   TODO:           texture and mipmap generation, texture filtering, modulation, texel get, painting in texture
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#ifdef _WIN32
#include "win32/GL/glew.h"
#endif

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Image.hpp"
#include "Context.hpp"

using namespace std;
using namespace glm;

Image::Image() : width(0), height(0), wrap(GL_CLAMP_TO_BORDER), min(GL_LINEAR), mag(GL_LINEAR), modulate(GL_MODULATE), textureID(0), drawmode(false){
}

Image::Image(int width, int height)
: width(width)
, height(height)
, data(width*height)
, wrap(GL_CLAMP_TO_BORDER)
, min(GL_LINEAR)
, mag(GL_LINEAR)
, modulate(GL_MODULATE)
, textureID(0)
, drawmode(false)
, drawlayer(width*height)
{}

Image::Image(const std::string& filename) : wrap(GL_CLAMP_TO_BORDER), min(GL_LINEAR), mag(GL_LINEAR), modulate(GL_MODULATE), textureID(0){
	load(filename);
}

Image::~Image(){
}

// generate OpenGL texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::generateTexture(){

	if (textureID == 0){
		// generate texture id
		// XXX
		glGenTextures(1, &textureID);
		cout << "id " << textureID << " generiert\n";
		this->bind();

		// INSERT YOUR CODE HERE

		// END XXX
	}

	// texture filtering and repeat
	// XXX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	this->setMinFilter(GL_LINEAR);
	this->setMagFilter(GL_LINEAR);
	// INSERT YOUR CODE HERE

	// END XXX

	//enable automatic mipmap generation
	// XXX
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->width, this->height, GL_RGBA, GL_FLOAT, &data[0]);
	// INSERT YOUR CODE HERE

	// END XXX

	// upload texture data
	// XXX
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	this->setModulation(GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_FLOAT, &data[0]);
	// INSERT YOUR CODE HERE

	// END XXX
}

void Image::setMinFilter(GLuint min){
	this->min = min;

	// set texture parameter
	// XXX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->min);
	// INSERT YOUR CODE HERE

	// END XXX
}

// set magnifying filter
// XXX: NEEDS TO BE IMPLEMENTED
void Image::setMagFilter(GLuint mag){

	this->mag = mag;

	// set texture parameter
	// XXX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mag);
	// INSERT YOUR CODE HERE

	// END XXX
}

void Image::setModulation(GLuint modulation){
	this->modulate = modulation;
}

// bind texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::bind(){
	// bind texture
	// XXX
	// INSERT YOUR CODE HERE
	glBindTexture(GL_TEXTURE_2D, textureID);
	// END XXX
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->modulate);
	// set modulation
	// XXX

	// INSERT YOUR CODE HERE

	// END XXX
}

// unbind texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::unbind(){
	// XXX
	glBindTexture(GL_TEXTURE_2D, 0);
	// INSERT YOUR CODE HERE

	// END XXX
}

// read a pixel from image
// XXX: NEEDS TO BE IMPLEMENTED
vec4 Image::get(unsigned int x, unsigned int y){

	// XXX
	return data[this->height*y + x];
	// INSERT YOUR CODE HERE 
	//return vec4(0);

	// END XXX
}
// draw in texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::paint(float x, float y){
	// XXX
	x *= this->width;
	y *= this->height;
	const glm::vec4 colorpixel = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::vec4 colorpixel = this->get(x, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, 1, 1, GL_RGBA, GL_FLOAT, &colorpixel);
	// INSERT YOUR CODE HERE

	// END XXX
}

// erase drawing from texture
// XXX: NEEDS TO BE IMPLEMENTED
void Image::erase(float x, float y){
	// XXX 
	x *= this->width;
	y *= this->height;
	// INSERT YOUR CODE HERE
	glm::vec4 texpixel = this->get(x, y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, 1, 1, GL_RGBA, GL_FLOAT, &texpixel);
	// END XXX
}

void Image::load(const std::string& filename){

	data.clear();

	if (filename.substr(filename.size() - 4, 4) == ".ppm") loadPPM(filename);
	else{
		cerr << "file " << filename << " is not a PPM file" << endl;
		return;
	}

	drawlayer.resize(width*height);
}

void Image::loadPPM(const std::string& filename){

	ifstream file(filename.c_str(), ios::binary);

	if (!file.is_open()){
		cerr << "opening file " << filename << " failed" << endl;
		return;
	}

	// grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	string magic;
	getline(file, magic);
	if (magic.substr(0, 2) != "P6"){
		cerr << "File " << filename << " is not a raw PPM file" << endl;
		return;
	}

	// grab the three elements in the header (width, height, maxval).
	string dimensions;
	do{
		getline(file, dimensions);
	} while (dimensions[0] == '#');

	stringstream(dimensions) >> width >> height;

	string max;
	getline(file, max);
	int maxValue;
	stringstream(max) >> maxValue;
	// grab all the image data in one fell swoop.
	vector<char> raw(width*height * 3);
	file.read(&raw[0], raw.capacity());
	file.close();

	data.resize(width*height);
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			data[y*width + x] = vec4((unsigned char)raw[(height - y - 1) * width * 3 + 3 * x], (unsigned char)raw[(height - y - 1) * width * 3 + 3 * x + 1], (unsigned char)raw[(height - y - 1) * width * 3 + 3 * x + 2], maxValue);
			data[y*width + x] /= maxValue;
			//cout << data[i].r << " " + data[i].g << " " + data[i].b << " " + data[i].a << endl;
		}
	}

	raw.clear();

	std::cout << "Image " << filename << " loaded. width=" << width << " height=" << height << endl;
}

/*************************************************************************
　　TGALoader.h

　　TGA Texture Loader

　　Date : October 10, 2007
　　Version : 1.0
　　Author : Pocol
　　Memo :
*************************************************************************/

#ifndef _TGA_LOADER_H_INCLUDED_
#define _TGA_LOADER_H_INCLUDED_

//
//　include
//
#include <iostream>
#include <GL/glut.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//　　TGALoader class
//////////////////////////////////////////////////////////////////////////
class TGAImage
{
protected:
	GLuint imageSize;
	GLubyte *imageData;
	GLenum format;
	GLuint internalFormat;
	GLuint width;
	GLuint height;
	GLuint bpp;

public:
	GLuint ID;
	TGAImage();
	~TGAImage();
	bool ReadTGA(const char *filename);
	GLuint Load(const char *filename);
};

#endif		//　_TGA_LOADER_H_INCLUDED_
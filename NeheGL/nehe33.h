//
//  nehe33.h
//  NeheGL
//
//  Created by Andong Li on 10/16/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe33__
#define __NeheGL__nehe33__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "utils.h"
#include "SOIL.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <ctime>


struct Texture{
    GLubyte* imageData;	// Hold All The Color Values For The Image.
    GLuint  bpp;	// Hold The Number Of Bits Per Pixel.
    GLuint width;	// The Width Of The Entire Image.
    GLuint height;	// The Height Of The Entire Image.
    GLuint texID;	// Texture ID For Use With glBindTexture.
    GLuint type;	// Data Stored In * ImageData (GL_RGB Or GL_RGBA)
};

struct TGAHeader{
    GLubyte Header[12];	// File Header To Determine File Type
};

struct TGA{
    GLubyte header[6];	// Holds The First 6 Useful Bytes Of The File
    GLuint bytesPerPixel;	// Number Of BYTES Per Pixel (3 Or 4)
    GLuint imageSize;	// Amount Of Memory Needed To Hold The Image
    GLuint type;	// The Type Of Image, GL_RGB Or GL_RGBA
    GLuint Height;	// Height Of Image
    GLuint Width;	// Width Of Image
    GLuint Bpp;	// Number Of BITS Per Pixel (24 Or 32)
};


class NEHE33{
public:
	
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	static GLvoid KeyboardFuction(unsigned char key, int x, int y);
	static GLvoid KeyboardUpFuction(unsigned char key, int x, int y);
	static GLvoid KeySpecialFuction(int key, int x, int y);
	static GLvoid KeySpecialUpFuction(int key, int x, int y);
	
	static const char* TITLE;
	const static int EXPECT_FPS = 60;	// expect FPS during rendering
	const static int FPS_UPDATE_CAP = 100; // time period for updating FPS
	
private:
	static GLfloat sleepTime;	//delay time for limiting FPS
	static void computeFPS();
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static TGAHeader tgaheader;	// Used To Store Our File Header
	static TGA tga;	// Used To Store File Information
	
	// Uncompressed TGA Header
	static GLubyte uTGAcompare[12];
	// Compressed TGA Header
	static GLubyte cTGAcompare[12];
	
	static bool LoadGLTextures();
	static bool LoadTGA(Texture * texture, const char * filename);
	static bool LoadUncompressedTGA(Texture * texture, const char * filename, FILE * fTGA);
	static bool LoadCompressedTGA(Texture * texture, const char * filename, FILE * fTGA);
	
	static float spin;
	static Texture texture[2];
	
	static bool keys[256];
	static bool specialKeys[256];
};


#endif /* defined(__NeheGL__nehe33__) */

//
//  nehe29.h
//  NeheGL
//
//  Created by Andong Li on 10/8/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe29__
#define __NeheGL__nehe29__



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

struct TEXTURE_IMAGE{
    int width;	// Width Of Image In Pixels
    int height;	// Height Of Image In Pixels
    int format;	// Number Of Bytes Per Pixel
    unsigned char *data;	// Texture Data
};

typedef TEXTURE_IMAGE *P_TEXTURE_IMAGE; // A Pointer To The Texture Image

class NEHE29{
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
	
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLfloat zrot;	// Z Rotation
	
	static GLuint texture[1];	// Storage For 1 Texture
	
	static P_TEXTURE_IMAGE t1;	// Pointer To The Texture Image Data Type
	static P_TEXTURE_IMAGE t2;	// Pointer To The Texture Image Data Type
	
	static P_TEXTURE_IMAGE AllocateTextureBuffer(GLint w, GLint h, GLint f);
	static void DeallocateTexture(P_TEXTURE_IMAGE t);
	static int ReadTextureData (const char *filename, P_TEXTURE_IMAGE buffer);
	static void BuildTexture (P_TEXTURE_IMAGE tex);
	static void Blit( P_TEXTURE_IMAGE src, P_TEXTURE_IMAGE dst, int src_xstart, int src_ystart, int src_width, int src_heigh, int dst_xstart, int dst_ystart, int blend, int alpha);
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe29__) */

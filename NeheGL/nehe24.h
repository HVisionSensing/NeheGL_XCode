//
//  nehe24.h
//  NeheGL
//
//  Created by Andong Li on 10/5/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe24__
#define __NeheGL__nehe24__

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
#include <stdarg.h>

struct TextureImage{
	GLubyte *imageData;	// Image Data (Up To 32 Bits)
    GLuint  bpp;	// Image Color Depth In Bits Per Pixel
    GLuint  width;	// Image Width
    GLuint  height;	// Image Height
    GLuint  texID;	// Texture ID Used To Select A Texture
};

class NEHE24{
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
	static bool LoadTGA(TextureImage *texture, const char *filename);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static int scroll;	// Used For Scrolling The Screen
	static int maxtokens;	// Keeps Track Of The Number Of Extensions Supported
	static int swidth;	// Scissor Width
	static int sheight;	// Scissor Height
	
	static GLuint base;	// Base Display List For The Font
	static TextureImage textures[1];	// Storage For One Texture
	
	static GLvoid buildFont();
	static GLvoid KillFont();
	static GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);
	
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe24__) */

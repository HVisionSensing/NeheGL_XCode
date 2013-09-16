//
//  nehe17.h
//  NeheGL
//
//  Created by Andong Li on 9/15/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe17__
#define __NeheGL__nehe17__


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

struct RGBIMG{
	GLuint   w;    // image's Width
	GLuint   h;    // image's Height
	GLubyte* data; // image's Data(Pixels)
};

class NEHE17{
public:
	
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);

	static const char* TITLE;
	const static int EXPECT_FPS = 60;	// expect FPS during rendering
	const static int FPS_UPDATE_CAP = 100; // time period for updating FPS
	
private:
	static GLfloat sleepTime;	//delay time for limiting FPS
	static void computeFPS();
	static bool LoadGLTextures(const char* dir, int textureID);
	static GLvoid buildFont();
	static GLvoid glPrint(GLint x, GLint y, const char *string, int set);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static GLuint texture[2];	// Storage For Two Texture
	static GLuint base;	//display list for font
	static GLfloat cnt1;
	static GLfloat cnt2;
	
};


#endif /* defined(__NeheGL__nehe17__) */

//
//  nehe12.h
//  NeheGL
//
//  Created by Andong Li on 9/11/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe12__
#define __NeheGL__nehe12__

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

class NEHE12{
public:
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	//handle keys press and up functions
	static GLvoid KeySpecialFuction(int key, int x, int y);
	static GLvoid KeySpecialUpFuction(int key, int x, int y);
	
	static const char* TITLE;
	const static int EXPECT_FPS = 60;	// expect FPS during rendering
	const static int FPS_UPDATE_CAP = 100; // time period for updating FPS
	
private:
	static GLfloat sleepTime;	//delay time for limiting FPS
	static void computeFPS();
	static bool LoadGLTextures(const char* dir);
	static GLvoid BuildLists();
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLuint  box;	// Storage For The Display List
	static GLuint  top;	// Storage For The Second Display List
	static GLuint  xloop;	// Loop For X Axis
	static GLuint  yloop;	// Loop For Y Axis
	static GLuint texture[1];	// Storage For One Texture
	
	static bool specialKeys[256];
	
	static GLfloat boxcol[5][3];	// Array For Box Colors
	static GLfloat topcol[5][3];	// Array For Top Colors
	
};


#endif /* defined(__NeheGL__nehe12__) */

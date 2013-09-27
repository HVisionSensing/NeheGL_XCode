//
//  nehe20.h
//  NeheGL
//
//  Created by Andong Li on 9/27/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe20__
#define __NeheGL__nehe20__

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

class NEHE20{
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
	static bool LoadGLTextures(const char* dir, int index);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static GLuint texture[5];
	static bool masking;
	static bool scene;	//which screne to draw
	static bool mp;
	static bool sp;
	static GLuint loop;
	static GLfloat roll;	// Rolling Texture

	
	static bool keys[256];
	static bool specialKeys[256];
};
#endif /* defined(__NeheGL__nehe20__) */

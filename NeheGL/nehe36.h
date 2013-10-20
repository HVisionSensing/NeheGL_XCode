//
//  nehe36.h
//  NeheGL
//
//  Created by Andong Li on 10/19/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe36__
#define __NeheGL__nehe36__

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


class NEHE36{
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
	static bool LoadGLTextures(const char* dir);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static float angle;	// Used To Rotate The Helix
	static float vertexes[4][3];	// Holds Float Info For 4 Sets Of Vertices
	static float normal[3];	// An Array To Store The Normal Data
	static GLuint BlurTexture;	// An Unsigned Int To Store The Texture Number
	
	static GLuint EmptyTexture();
	static void ReduceToUnit(float vector[3]);
	static void calcNormal(float v[3][3], float out[3]);
	static void ProcessHelix();
	static void ViewOrtho();
	static void ViewPerspective();
	static void RenderToTexture();
	static void DrawBlur(int times, float inc);
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe36__) */

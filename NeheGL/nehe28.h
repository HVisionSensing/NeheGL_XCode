//
//  nehe28.h
//  NeheGL
//
//  Created by Andong Li on 10/7/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe28__
#define __NeheGL__nehe28__


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

struct POINT_3D{
	double x;
	double y;
	double z;
};

struct BEZIER_PATCH{
	POINT_3D anchors[4][4];	// 4x4 Grid Of Anchor Points
    GLuint dlBPatch;	// Display List For Bezier Patch
    GLuint texture;
};


class NEHE28{
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
	static bool LoadGLTextures(const char* dir, GLuint *texPntr);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static GLfloat rotz;	// Rotation About The Z Axis
	static BEZIER_PATCH mybezier;	// The Bezier Patch We're Going To Use
	static bool showCPoints;	// Toggles Displaying The Control Point Grid
	static int divs;	// Number Of Intrapolations (Controls Poly Resolution)
	
	
	static POINT_3D pointAdd(POINT_3D p, POINT_3D q);
	static POINT_3D pointTimes(double c, POINT_3D p);
	static POINT_3D makePoint(double a, double b, double c);
	static POINT_3D Bernstein(float u, POINT_3D *p);
	static GLuint genBezier(BEZIER_PATCH patch, int divs);
	static void initBezier();
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe28__) */

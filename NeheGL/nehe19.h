//
//  nehe19.h
//  NeheGL
//
//  Created by Andong Li on 9/26/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe19__
#define __NeheGL__nehe19__

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

#define MAX_PARTICLES 1000	// define the max particles number

struct Particle{
	bool active;	// Active (Yes/No)
    float life;	// Particle Life
    float fade;	// Fade Speed
	float r;
	float g;
	float b;
	float x;	// X position
	float y;	// Y position
	float z;	// Z position
	float xi;	// X direction
	float yi;	// Y direction
	float zi;	// Z direction
	float xg;	// X Gravity
	float yg;	// Y Gravity
	float zg;	// Z Gravity
};


class NEHE19{
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
	
	static GLuint col;	// Current Color Selection
	static GLuint delay;	// Rainbow Effect Delay
	static GLuint texture[1];
	
	static bool sp;
	static bool rp;
	static bool rainbow;
	static GLfloat xspeed;
	static GLfloat yspeed;
	static GLfloat zoom;
	static GLfloat slowdown;
	static Particle particles[MAX_PARTICLES];
	static GLfloat colors[12][3];	// Rainbow Of Colors
	
	static bool keys[256];
	static bool specialKeys[256];
	
};


#endif /* defined(__NeheGL__nehe19__) */

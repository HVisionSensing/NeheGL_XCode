//
//  nehe26.h
//  NeheGL
//
//  Created by Andong Li on 10/6/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe26__
#define __NeheGL__nehe26__


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

class NEHE26{
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
	
	static GLfloat LightAmb[];	// Ambient Light
	static GLfloat LightDif[];	// Diffuse Light
	static GLfloat LightPos[];	// Light Position
	
	static GLUquadricObj *q;	// Quadratic For Drawing A Sphere
	
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLfloat xrotspeed;	// X Rotation Speed
	static GLfloat yrotspeed;	// Y Rotation Speed
	static GLfloat zoom;	// Depth Into The Screen
	static GLfloat height;	// Height Of Ball From Floor
	
	static GLuint texture[3];	// 3 Textures
	
	static void DrawObject();
	static void DrawFloor();
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe26__) */

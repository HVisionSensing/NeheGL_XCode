//
//  nehe23.h
//  NeheGL
//
//  Created by Andong Li on 10/4/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe23__
#define __NeheGL__nehe23__

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

class NEHE23{
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
	static bool LoadGLTextures();
	static GLvoid glDrawCube();
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLfloat xspeed;	// X Rotation Speed
	static GLfloat yspeed;	// Y Rotation Speed
	static GLfloat z;	//depth into the screen
	static GLuint texture[6];	// Storage For Six Texture
	static GLuint  filter;	// Which Filter To Use
	
	static int part1;	// Start Of Disc
	static int part2;	// End Of Disc
	static int p1;	// Increase 1
	static int p2;	// Increase 2
	
	static GLUquadricObj* quadratic;	// Storage For Our Quadratic Objects
	static GLuint  object;	// Which Object To Draw
	
	static bool light;
	static bool lp;
	static bool fp;
	static bool sp;
	
	static GLfloat LightAmbient[];	// Ambient Light Values
	static GLfloat LightDiffuse[];	// Diffuse Light Values
	static GLfloat LightPosition[];	// Light Position
	
	static bool keys[256];
	static bool specialKeys[256];
	
};



#endif /* defined(__NeheGL__nehe23__) */

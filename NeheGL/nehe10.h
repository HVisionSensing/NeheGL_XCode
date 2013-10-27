//
//  nehe10.h
//  NeheGL
//
//  Created by Andong Li on 9/7/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe10__
#define __NeheGL__nehe10__

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

struct Vertex{
	GLfloat x,y,z,u,v;
};

struct Triangle{
	Vertex vertex[3];
};

struct Sector{
	int numTriangles;
	Triangle* triangles;
};

class NEHE10{
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
	
	static GLuint  filter;	// Which Filter To Use
	static GLuint texture[3];	// Storage For Three Texture
	
	static bool setupWorld(const char* fileDir);
	static void readstr(FILE *f,char *string);	//helper function read one line

	static bool keys[256];
	static bool specialKeys[256];
	static bool bp;
	static bool blend;
	static bool fp;
	static Sector sector;
	
	static const GLfloat piover180;
	static GLfloat walkbias;
	static GLfloat walkbiasangle;
	static GLfloat heading;
	static GLfloat xpos;
	static GLfloat zpos;
	static GLfloat yrot;
	static GLfloat lookupdown;
	static GLfloat z;
	
};


#endif /* defined(__NeheGL__nehe10__) */

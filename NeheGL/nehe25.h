//
//  nehe25.h
//  NeheGL
//
//  Created by Andong Li on 10/5/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe25__
#define __NeheGL__nehe25__

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

struct VERTEX{
	float x;
	float y;
	float z;
};

struct OBJECT{
	int verts;
	VERTEX* points;
};

class NEHE25{
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
	
	static GLfloat xrot;
	static GLfloat yrot;
	static GLfloat zrot;
	static GLfloat xspeed;
	static GLfloat yspeed;
	static GLfloat zspeed;
	static GLfloat cx;
	static GLfloat cy;
	static GLfloat cz;
	
	static int key;	// Used To Make Sure Same Morph Key Is Not Pressed
	static int step;	// Step Counter
	static int steps;	// Maximum Number Of Steps
	static bool morph;	// Default morph To False (Not Morphing)
	
	static int maxver;	// Will Eventually Hold The Maximum Number Of Vertices
	static OBJECT morph1;
	static OBJECT morph2;
	static OBJECT morph3;
	static OBJECT morph4;
	static OBJECT helper;
	static OBJECT* sour;
	static OBJECT* dest;
	
	static void objallocate(OBJECT *k,int n);
	static void objfree(OBJECT *k);
	static void readstr(FILE *f,char *string);
	static void objload(const char *name,OBJECT *k);
	static VERTEX calculate(int i);
	
	static bool keys[256];
	static bool specialKeys[256];
};


#endif /* defined(__NeheGL__nehe25__) */

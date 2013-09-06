//
//  nehe05.h
//  NeheGL
//
//  Created by Andong Li on 9/4/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe05__
#define __NeheGL__nehe05__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "utils.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
using namespace std;

class NEHE05{
public:
	
	static const char* TITLE;
	
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	
	static GLfloat rtri;	// Rotate angle For The Triangle
	static GLfloat rquad;	// Rotate angle For The Quad
	const static int EXPECT_FPS = 60;	// expect FPS during rendering
	const static int FPS_UPDATE_CAP = 100; // time period for updating FPS
	
private:
	static GLfloat sleepTime;	//delay time for limiting FPS
	static void computeFPS();
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
};


#endif /* defined(__NeheGL__nehe05__) */

//
//  nehe11.h
//  NeheGL
//
//  Created by Andong Li on 9/11/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe11__
#define __NeheGL__nehe11__

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

class NEHE11{
public:
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	
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
	
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLfloat zrot;	// Z Rotation
	static GLuint texture[1];	// Storage For One Texture
	
	static float points[45][45][3];
	static int wiggle_count;	// Counter Used To Control How Fast Flag Waves
	static GLfloat hold;	// Temporarily Holds A Floating Point Value
	
};


#endif /* defined(__NeheGL__nehe11__) */

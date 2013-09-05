//
//  nehe06.h
//  NeheGL
//
//  Created by Andong Li on 9/4/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//	Implement Nehe lession 6
//	Download SOIL library from:
//	http://faculty.ycp.edu/~dbabcock/PastCourses/cs370/resources.html
//

#ifndef __NeheGL__nehe06__
#define __NeheGL__nehe06__

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

class NEHE06{
public:
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	
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
	
	
};


#endif /* defined(__NeheGL__nehe06__) */

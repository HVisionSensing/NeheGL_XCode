//
//  nehe22.h
//  NeheGL
//
//  Created by Andong Li on 9/29/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe22__
#define __NeheGL__nehe22__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "utils.h"
#include "SOIL.h"
#include "glext.h"	// Header File For Multitexturing

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <ctime>

#define MAX_EMBOSS (GLfloat)0.01f	// Maximum Emboss-Translate. Increase To Get Higher Immersion
#define __ARB_ENABLE true	// Used To Disable ARB Extensions Entirely
//#define EXT_INFO	// Uncomment To See Your Extensions At Start-Up?
#define MAX_EXTENSION_SPACE 10240	// Characters For Extension-Strings
#define MAX_EXTENSION_LENGTH 256	// Maximum Characters In One Extension-String

class NEHE22{
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
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static bool multitextureSupported;	// Flag Indicating Whether Multitexturing Is Supported
	static bool useMultitexture;	// Use It If It Is Supported?
	static GLint maxTexelUnits;	// Number Of Texel-Pipelines. This Is At Least 1.
	
#ifdef _WIN32
	static PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	static PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
	static PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
	static PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB;
	static PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
#endif
	
	static GLuint filter;	// Which Filter To Use
	static GLuint texture[3];	// Storage For 3 Textures
	static GLuint bump[3];	// Our Bumpmappings
	static GLuint invbump[3];	// Inverted Bumpmaps
	static GLuint glLogo;	// Handle For OpenGL-Logo
	static GLuint multiLogo;	// Handle For Multitexture-Enabled-Logo
	static GLfloat LightAmbient[];	// Ambient Light Is 20% White
	static GLfloat LightDiffuse[];	// Diffuse Light Is White
	static GLfloat LightPosition[];	// Position Is Somewhat In Front Of Screen
	static GLfloat Gray[];
	static GLfloat data[];
	
	static bool isInString(char *string, const char *search);
	static bool initMultitexture();
	static void initLights();
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe22__) */

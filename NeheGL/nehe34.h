//
//  nehe34.h
//  NeheGL
//
//  Created by Andong Li on 10/17/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe34__
#define __NeheGL__nehe34__

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

#define MAP_SIZE 1024	// Size Of Our .RAW Height Map
#define STEP_SIZE 16	// Width And Height Of Each Quad
#define HEIGHT_RATIO 1.5f	// Ratio That The Y Is Scaled According To The X And Z

typedef unsigned char BYTE;

class NEHE34{
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
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static bool bRender;
	static BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];	// Holds The Height Map Data
	static float scaleValue;	// Scale Value For The Terrain
	
	static void LoadRawFile(const char* strName, int nSize, BYTE *pHeightMap);
	static int Height(BYTE *pHeightMap, int X, int Y);
	static void SetVertexColor(BYTE *pHeightMap, int x, int y);
	static void RenderHeightMap(BYTE pHeightMap[]);
	
	static bool keys[256];
	static bool specialKeys[256];
};


#endif /* defined(__NeheGL__nehe34__) */

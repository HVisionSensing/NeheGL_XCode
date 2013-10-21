//
//  nehe37.h
//  NeheGL
//
//  Created by Andong Li on 10/20/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe37__
#define __NeheGL__nehe37__


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

// A Structure To Hold An OpenGL Matrix
struct MATRIX37{
    float Data[16];	// We Use [16] Due To OpenGL's Matrix Format
};

// A Structure To Hold A Single Vector
struct VECTOR37{
    float X, Y, Z;	// The Components Of The Vector
};

// A Structure To Hold A Single Vertex
struct VERTEX37{
    VECTOR37 Nor;	// Vertex Normal
    VECTOR37 Pos;	// Vertex Position
};

// A Structure To Hold A Single Polygon
struct POLYGON37{
    VERTEX37 Verts[3];	// Array Of 3 VERTEX Structures
};

class NEHE37{
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
	
	static bool outlineDraw;
	static bool outlineSmooth;
	static float outlineColor[3];
	static float outlineWidth;
	static VECTOR37 lightAngle;
	static bool lightRotate;
	static float modelAngle;
	static bool modelRotate;
	static POLYGON37* polyData;
	static int polyNum;
	static GLuint shaderTexture[1];
	
	static bool ReadMesh();
	static float DotProduct (VECTOR37 &V1, VECTOR37 &V2);
	static float Magnitude (VECTOR37 &V);
	static void Normalize (VECTOR37 &V);
	static void RotateVector (MATRIX37 &M, VECTOR37 &V, VECTOR37 &D);
		
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe37__) */

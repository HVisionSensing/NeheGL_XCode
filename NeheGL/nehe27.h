//
//  nehe27.h
//  NeheGL
//
//  Created by Andong Li on 10/6/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe27__
#define __NeheGL__nehe27__

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

// Definition Of "INFINITY" For Calculating The Extension Vector For The Shadow Volume
#define INFINITY_SHADOW 100
typedef float GLvector4f[4];	// Typedef's For VMatMult Procedure
typedef float GLmatrix16f[16];	// Typedef's For VMatMult Procedure

// vertex in 3d-coordinate system
struct sPoint{
	float x, y, z;
};

// plane equation
struct sPlaneEq{
	float a, b, c, d;
};

// structure describing an object's face
struct sPlane{
	unsigned int p[3];
	sPoint normals[3];
	unsigned int neigh[3];
	sPlaneEq PlaneEq;
	bool visible;
};

// object structure
struct glObject{
	GLuint nPlanes, nPoints;
	sPoint points[100];
	sPlane planes[200];
};


class NEHE27{
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
	
	static int ReadObject(const char *st, glObject *o);
	static void SetConnectivity(glObject *o);
	static void CalcPlane(glObject o, sPlane *plane);
	static void DrawGLObject(glObject o);
	static void CastShadow(glObject *o, float *lp);
	static void VMatMult(GLmatrix16f M, GLvector4f v);
	static int InitGLObjects();
	static void DrawGLRoom();
	
	static glObject obj;
	static GLfloat xrot;	// X Rotation
	static GLfloat yrot;	// Y Rotation
	static GLfloat xspeed;	// X Rotation Speed
	static GLfloat yspeed;	// Y Rotation Speed
	
	static GLfloat LightPos[];	// Light Position
	static GLfloat LightAmb[];	// Ambient Light Values
	static GLfloat LightDif[];	// Diffuse Light Values
	static GLfloat LightSpc[];	// Specular Light Values
	
	static GLfloat MatAmb[];	// Material - Ambient Values
	static GLfloat MatDif[];	// Material - Diffuse Values
	static GLfloat MatSpc[];	// Material - Specular Values
	static GLfloat MatShn[];	// Material - Shininess
	
	static float ObjPos[];	// Object Position
	
	static GLUquadricObj* q;	// Quadratic For Drawing A Sphere
	static GLfloat SpherePos[];
	
	static bool keys[256];
	static bool specialKeys[256];
};

#endif /* defined(__NeheGL__nehe27__) */

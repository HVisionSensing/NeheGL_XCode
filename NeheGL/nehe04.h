//
//  nehe04.h
//  NeheGL
//
//  Created by Andong Li on 8/30/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef NeheGL_nehe04_h
#define NeheGL_nehe04_h

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "utils.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class NEHE04{
public:
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	
	static GLfloat rtri;	// Rotate angle For The Triangle
	static GLfloat rquad;	// Rotate angle For The Quad
};

#endif

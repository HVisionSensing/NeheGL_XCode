//
//  nehe01.h
//  NeheGL
//
//  Created by Andong Li on 8/15/12.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//  implement NeHe tutorials lesson 1
//

#ifndef NEHE01_H
#define NEHE01_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class NEHE01{
public:
	static const char* TITLE;
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
};

#endif//NEHE01_H
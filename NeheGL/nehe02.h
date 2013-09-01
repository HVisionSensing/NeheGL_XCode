//
//  nehe02.h
//  NeheGL
//
//  Created by Andong Li on 8/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//	implement NeHe tutorials lesson 1
//

#ifndef NEHE02_H
#define NEHE02_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class NEHE02{
public:
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
};

#endif//NEHE02_H


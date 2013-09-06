//
//  nehe03.h
//  NeheGL
//
//  Created by Andong Li on 8/30/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef NeheGL_nehe03_h
#define NeheGL_nehe03_h

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class NEHE03{
public:
	static const char* TITLE;
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
};

#endif

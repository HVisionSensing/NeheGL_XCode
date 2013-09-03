//
//  utils.h
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__utils__
#define __NeheGL__utils__

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
using namespace std;

class TextUtils{
public:
	static void drawText(float x, float y, void* font, const char* str);
};

#endif /* defined(__NeheGL__utils__) */

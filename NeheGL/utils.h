//
//  utils.h
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//	Compute accurate FPS by averaging one frame processing
//	time of a certina amout
//

#ifndef __NeheGL__utils__
#define __NeheGL__utils__

#include <iostream>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <CoreFoundation/CoreFoundation.h>
using namespace std;

class Utils{
public:
	//draw bitmap text on screen with given location
	static void drawText(float x, float y, void* font, const char* str);
	
	//render stroke characters on screen
	static void strokeText(void* font, const char* str);
	
	//translate input relative directory into absolute directory
	static char* getAbsoluteDir(const char* relativeDir);
};

#endif /* defined(__NeheGL__utils__) */

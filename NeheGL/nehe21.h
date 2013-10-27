//
//  nehe21.h
//  NeheGL
//
//  Created by Andong Li on 9/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe21__
#define __NeheGL__nehe21__


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


// A Structure For Our Player
struct object{
    int fx, fy;	// Fine Movement Position
    int x, y;	// Current Player Position
    float spin;	// Spin Direction
};

struct Timer{
	int startTime;
	int elapseTime;
};

class NEHE21{
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
	static bool LoadGLTextures(const char* dir);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
	
	static bool vline[11][10];	// Keeps Track Of Verticle Lines
	static bool hline[10][11];	// Keeps Track Of Horizontal Lines
	static bool ap;
	static bool filled;
	static bool gameover;
	static bool anti;	// Anti-analis
	static int loop1;	// Generic Loop1
	static int loop2;	// Generic Loop2
	static int delay;	// Enemy Delay
	static int adjust;	// Speed Adjustment For Really Slow Video Cards
	static int lives;	// Player Lives
	static int level;	// Internal Game Level
	static int level2;	// Displayed Game Level
	static int stage;	// Game Stage
	
	static object player;	// Player Information
	static object enemy[9];	// Enemy Information
	static object hourglass;	// Hourglass Information
	
	static int steps[6];	// Stepping Values For Slow Video Adjustment
	
	static GLuint texture[2];
	static GLuint base;	// Display list pointer
	
	static void TimerInit();
	static int TimerGetTime();
	static Timer timer;
	
	static bool keys[256];
	static bool specialKeys[256];
};



#endif /* defined(__NeheGL__nehe21__) */

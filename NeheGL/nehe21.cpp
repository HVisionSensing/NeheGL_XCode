//
//  nehe21.cpp
//  NeheGL
//
//  Created by Andong Li on 9/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe21.h"


const char* NEHE21::TITLE = "NEHE21";

GLfloat NEHE21::sleepTime = 0.0f;
int NEHE21::frameCounter = 0;
int NEHE21::currentTime = 0;
int NEHE21::lastTime = 0;
char NEHE21::FPSstr[15]  = "Calculating...";


bool NEHE21::vline[11][10] = {};	//all set to false
bool NEHE21::hline[10][11] = {};	//all set to false
bool NEHE21::ap = false;
bool NEHE21::filled = false;
bool NEHE21::gameover = false;
bool NEHE21::anti = true;
int NEHE21::loop1 = 0;
int NEHE21::loop2 = 0;
int NEHE21::delay = 0;
int NEHE21::adjust=3;
int NEHE21::lives=5;
int NEHE21::level=1;
int NEHE21::level2=1;
int NEHE21::stage=1;

int NEHE21::steps[6]={ 1, 2, 4, 5, 10, 20 };

GLuint texture[2] = {0};
GLuint base = 0;	// Display list pointer

Timer NEHE21::timer;	// just for declaration

bool NEHE21::keys[256] = {};	//all set to false
bool NEHE21::specialKeys[256] = {};	//all set to false

GLvoid NEHE21::ReSizeGLScene(GLsizei width, GLsizei height){
	
	// Prevent A Divide By Zero By
    if(height==0)
    {
        height=1;
    }
	
	// Reset The Current Viewport
    glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void NEHE21::TimerInit(){
	memset(&timer, 0, sizeof(Timer));
	
	timer.startTime = glutGet(GLUT_ELAPSED_TIME);
}

int NEHE21::TimerGetTime(){
	timer.elapseTime = glutGet(GLUT_ELAPSED_TIME) - timer.startTime;
	return timer.elapseTime;
}

bool NEHE21::LoadGLTextures(const char* dir){
	
	return true;
}

GLvoid NEHE21::InitGL(){
	
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}


GLvoid NEHE21::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE21::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE21::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE21::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE21::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE21::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE21::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
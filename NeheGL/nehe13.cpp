//
//  nehe13.cpp
//  NeheGL
//
//  Created by Andong Li on 9/12/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe13.h"

const char* NEHE13::TITLE = "NEHE13";

GLfloat NEHE13::cnt1 = 0.0f;
GLfloat NEHE13::cnt2 = 0.0f;

GLfloat NEHE13::sleepTime = 0.0f;
int NEHE13::frameCounter = 0;
int NEHE13::currentTime = 0;
int NEHE13::lastTime = 0;
char NEHE13::FPSstr[15]  = "Calculating...";

GLvoid NEHE13::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLvoid NEHE13::InitGL(){
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}

GLvoid NEHE13::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	char str[128];
	
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-1.0f);
	// Pulsing Colors Based On Text Position
	glColor3f(1.0f*float(cos(cnt1)),1.0f*float(sin(cnt2)),
			  1.0f-0.5f*float(cos(cnt1+cnt2)));
	sprintf(str, "Active OpenGL Text With NeHe - %7.2f", cnt1);
	Utils::drawText(-0.45f+0.05f*float(cos(cnt1)), 0.32f*float(sin(cnt2)), GLUT_BITMAP_HELVETICA_18, str);
	
	//draw fps on screen
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glutSwapBuffers();
	
	cnt1 += 0.051f;
	cnt2 += 0.005f;
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE13::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE13::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}


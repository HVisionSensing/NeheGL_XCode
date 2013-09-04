//
//  nehe04.cpp
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe04.h"

GLfloat NEHE04::rtri = 0.0f;
GLfloat NEHE04::rquad = 0.0f;
GLfloat NEHE04::sleepTime = 0.0f;

int NEHE04::frameCounter = 0;
int NEHE04::currentTime = 0;
int NEHE04::lastTime = 0;
char NEHE04::FPSstr[15]  = "Calculating...";

GLvoid NEHE04::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE04::InitGL(){
	
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

GLvoid NEHE04::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glLoadIdentity();
	glTranslatef(-1.5f,0.0f,-6.0f);
	glRotatef(rtri,0.0f,1.0f,0.0f);	// Rotate The Triangle On The Y axis
	glBegin(GL_TRIANGLES);	// Drawing Using Triangles
	glColor3f(1.0f,0.0f,0.0f);	// Red
    glVertex3f( 0.0f, 1.0f, 0.0f);	// Top
	glColor3f(0.0f,1.0f,0.0f);	//Green
    glVertex3f(-1.0f,-1.0f, 0.0f);	// Bottom Left
	glColor3f(0.0f,0.0f,1.0f);	// Blue
    glVertex3f( 1.0f,-1.0f, 0.0f);	// Bottom Right
	glEnd();
	
	glLoadIdentity();
	glTranslatef(1.5f,0.0f,-6.0f);
	glRotatef(rquad,1.0f,0.0f,0.0f);	// Rotate The Quad On The X axis
	glColor3f(0.5f,0.5f,1.0f);
	glBegin(GL_QUADS);	// Draw A Quad
	glVertex3f(-1.0f, 1.0f, 0.0f);	// Top Left
	glVertex3f( 1.0f, 1.0f, 0.0f);	// Top Right
	glVertex3f( 1.0f,-1.0f, 0.0f);	// Bottom Right
	glVertex3f(-1.0f,-1.0f, 0.0f);	// Bottom Left
    glEnd();
	
	rtri += 0.2f;
	rquad -= 0.15f;
	
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	TextUtils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glutSwapBuffers();
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE04::UpdateScene(int flag){
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS) - (endTime-startTime))/1000.0;
    sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
		
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE04::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}


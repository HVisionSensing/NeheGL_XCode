//
//  nehe14.cpp
//  NeheGL
//
//  Created by Andong Li on 9/12/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe14.h"

const char* NEHE14::TITLE = "NEHE14";

GLfloat NEHE14::rot = 0.0f;

GLfloat NEHE14::sleepTime = 0.0f;
int NEHE14::frameCounter = 0;
int NEHE14::currentTime = 0;
int NEHE14::lastTime = 0;
char NEHE14::FPSstr[15]  = "Calculating...";

GLvoid NEHE14::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE14::InitGL(){
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
}

GLvoid NEHE14::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	char str[128];
	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-15.0f);						// Move One Unit Into The Screen
	glRotatef(rot,1.0f,0.0f,0.0f);					// Rotate On The X Axis
	glRotatef(rot*1.5f,0.0f,1.0f,0.0f);				// Rotate On The Y Axis
	glRotatef(rot*1.4f,0.0f,0.0f,1.0f);				// Rotate On The Z Axis
    glScalef(0.005, 0.005, 0.0);
	
	glColor3f(1.0f*float(cos(rot/20.0f)),
			  1.0f*float(sin(rot/25.0f)),
			  1.0f-0.5f*float(cos(rot/17.0f)));
 	sprintf(str, "NeHe - %3.2f",rot/50);
	
	glPushMatrix();
	Utils::strokeText(GLUT_STROKE_ROMAN, str);
	glPopMatrix();
	
	
	//draw fps on screen
	glLoadIdentity ();
	glDisable(GL_LIGHTING);
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_LIGHTING);
	
	glutSwapBuffers();
	rot += 0.5f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE14::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE14::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

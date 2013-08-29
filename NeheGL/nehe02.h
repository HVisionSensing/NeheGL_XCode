//
//  nehe02.cpp
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
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	const int WINDOW_POSITION_X = 100;
	const int WINDOW_POSITION_Y = 100;
	const char* WINDOW_TITLE = "Nehe02";
	
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	GLvoid InitGL();
	GLvoid DrawGLScene();
};

GLvoid NEHE02::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE02::InitGL(){
	
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

GLvoid NEHE02::DrawGLScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
	glTranslatef(-1.5f,0.0f,-6.0f);
	glBegin(GL_TRIANGLES);	// Drawing Using Triangles
    glVertex3f( 0.0f, 1.0f, 0.0f);	// Top
    glVertex3f(-1.0f,-1.0f, 0.0f);	// Bottom Left
    glVertex3f( 1.0f,-1.0f, 0.0f);	// Bottom Right
	glEnd();
	
	
	glTranslatef(3.0f,0.0f,0.0f);
	glBegin(GL_QUADS);	// Draw A Quad
	glVertex3f(-1.0f, 1.0f, 0.0f);	// Top Left
	glVertex3f( 1.0f, 1.0f, 0.0f);	// Top Right
	glVertex3f( 1.0f,-1.0f, 0.0f);	// Bottom Right
	glVertex3f(-1.0f,-1.0f, 0.0f);	// Bottom Left
    glEnd();

	glutSwapBuffers();
}


#endif//NEHE02_H


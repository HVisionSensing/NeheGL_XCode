//
//  nehe01.cpp
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe01.h"

GLvoid NEHE01::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE01::InitGL(){
	
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

GLvoid NEHE01::DrawGLScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glutSwapBuffers();
}

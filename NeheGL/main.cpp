//
//  main.cpp
//  NeheGL
//
//  Created by Andong Li on 8/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
// entry function of this project
//

#include "nehe01.h"
#include "nehe02.h"

//change class here to view different lesson
NEHE02* renderer = new NEHE02();

GLvoid displayFunc(){
	renderer->DrawGLScene();
}

GLvoid resizeFunc(GLsizei width, GLsizei height){
	renderer->ReSizeGLScene(width, height);
}

int main(int argc,  char * argv[]){
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (renderer->WINDOW_WIDTH, renderer->WINDOW_HEIGHT);
    glutInitWindowPosition (renderer->WINDOW_POSITION_X, renderer->WINDOW_POSITION_Y);
    glutCreateWindow (renderer->WINDOW_TITLE);
	
    renderer->InitGL();
	
    glutDisplayFunc(displayFunc);
	glutReshapeFunc(resizeFunc);
	
    glutMainLoop();
	
    return EXIT_SUCCESS;
	
}
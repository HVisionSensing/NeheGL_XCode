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

//just change here to change renderer class and window's title
#define RENDERER NEHE02
#define TITLE "NeHe02"

int main(int argc,  char * argv[]){
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (100,100);
    glutCreateWindow (TITLE);
	
	RENDERER::InitGL();
	
    glutDisplayFunc(RENDERER::DrawGLScene);
	glutReshapeFunc(RENDERER::ReSizeGLScene);
	
    glutMainLoop();
	
    return EXIT_SUCCESS;
	
}
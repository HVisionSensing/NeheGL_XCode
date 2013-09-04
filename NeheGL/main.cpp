//
//  main.cpp
//  NeheGL
//
//  Created by Andong Li on 8/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
// entry function of this project
//

#include "headers.h"

//just change here to change renderer class and window's title
#define RENDERER NEHE05
#define TITLE "NeHe05"

int main(int argc,  char * argv[]){
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (100,100);
    glutCreateWindow (TITLE);
	
	RENDERER::InitGL();
	
    glutDisplayFunc(RENDERER::DrawGLScene);
	glutReshapeFunc(RENDERER::ReSizeGLScene);
	glutTimerFunc(0, RENDERER::UpdateScene, 0);
    glutMainLoop();
	
    return EXIT_SUCCESS;
	
}

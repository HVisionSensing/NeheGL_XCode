//
//  main.cpp
//  NeheGL
//
//  Created by Andong Li on 8/28/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
// entry function of this project
//

#include "headers.h"

//just change here to change renderer
#define RENDERER NEHE34

int main(int argc,  char * argv[]){
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (100,100);
    glutCreateWindow (RENDERER::TITLE);
	
	RENDERER::InitGL();
	
    glutDisplayFunc(RENDERER::DrawGLScene);
	glutReshapeFunc(RENDERER::ReSizeGLScene);
	glutTimerFunc(0, RENDERER::UpdateScene, 0);
	//handle keyboard actions
	glutKeyboardFunc(RENDERER::KeyboardFuction);
	glutKeyboardUpFunc(RENDERER::KeyboardUpFuction);
	glutSpecialFunc(RENDERER::KeySpecialFuction);
	glutSpecialUpFunc(RENDERER::KeySpecialUpFuction);
	
    glutMainLoop();
	
    return EXIT_SUCCESS;
	
}

//
//  main.cpp
//  NeheGL
//
//  Created by Andong Li on 8/13/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
}
int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("Xcode Glut Demo");
    glutDisplayFunc(display);
    glutMainLoop();
}
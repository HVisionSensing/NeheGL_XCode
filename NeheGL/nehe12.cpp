//
//  nehe12.cpp
//  NeheGL
//
//  Created by Andong Li on 9/11/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe12.h"

const char* NEHE12::TITLE = "NEHE12";

GLfloat NEHE12::sleepTime = 0.0f;
int NEHE12::frameCounter = 0;
int NEHE12::currentTime = 0;
int NEHE12::lastTime = 0;
char NEHE12::FPSstr[15]  = "Calculating...";

GLuint NEHE12::texture[1] = {0};
GLfloat NEHE12::xrot = 0.0f;
GLfloat NEHE12::yrot = 0.0f;

GLuint  NEHE12::box = 0;
GLuint  NEHE12::top = 0;
GLuint  NEHE12::xloop = 0;
GLuint  NEHE12::yloop = 0;

bool NEHE12::specialKeys[256] = {};	//all set to false

GLfloat NEHE12::boxcol[5][3] = {
    // Bright:  Red, Orange, Yellow, Green, Blue
    {1.0f,0.0f,0.0f},
	{1.0f,0.5f,0.0f},
	{1.0f,1.0f,0.0f},
	{0.0f,1.0f,0.0f},
	{0.0f,1.0f,1.0f}
};

GLfloat NEHE12::topcol[5][3] = {
    // Dark:  Red, Orange, Yellow, Green, Blue
    {0.5f,0.0f,0.0f},
	{0.5f,0.25f,0.0f},
	{0.5f,0.5f,0.0f},
	{0.0f,0.5f,0.0f},
	{0.0f,0.5f,0.5f}
};

GLvoid NEHE12::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE12::LoadGLTextures(const char* dir){
	
	/* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir(dir),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[0] == 0){
        return false;
	}
	
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
    return true;
}

GLvoid NEHE12::BuildLists(){
	box=glGenLists(2);	// Building Two Lists
	
	glNewList(box,GL_COMPILE);	// New Compiled box Display List
	glBegin(GL_QUADS);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	// End first list
	glEndList();
	
	top=box+1;
	glNewList(top,GL_COMPILE);
	glBegin(GL_QUADS);
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glEnd();
	glEndList();
}

GLvoid NEHE12::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/cube.bmp")){
        cout<<"Fail to load textures"<<endl;
    }
	
	BuildLists();
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_LIGHT0);	// Quick And Dirty Lighting
	glEnable(GL_LIGHTING);	// Enable Lighting
	glEnable(GL_COLOR_MATERIAL);	// Enable Material Coloring
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	
}

GLvoid NEHE12::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	for(yloop=1;yloop<6;yloop++){
		for(xloop=0;xloop<yloop;xloop++){
			glLoadIdentity();
			// Position The Cubes On The Screen
			glTranslatef(1.4f+(float(xloop)*2.8f)-(float(yloop)*1.4f),
						 ((6.0f-float(yloop))*2.4f)-7.0f,-20.0f);
			glRotatef(45.0f-(2.0f*yloop)+xrot,1.0f,0.0f,0.0f);
			glRotatef(45.0f+yrot,0.0f,1.0f,0.0f);
			glColor3fv(boxcol[yloop-1]);
			glCallList(box);
			glColor3fv(topcol[yloop-1]);
			glCallList(top);
		}
	}
		
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
	if (specialKeys[GLUT_KEY_UP]){
		xrot -= 0.2f;
	}
	if (specialKeys[GLUT_KEY_DOWN]){
		xrot += 0.2f;
	}
	if (specialKeys[GLUT_KEY_LEFT]){
		yrot -= 0.2f;
	}
	if (specialKeys[GLUT_KEY_RIGHT]){
		yrot += 0.2f;
	}

	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE12::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE12::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

GLvoid NEHE12::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE12::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

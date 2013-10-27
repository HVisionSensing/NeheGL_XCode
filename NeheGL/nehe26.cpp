//
//  nehe26.cpp
//  NeheGL
//
//  Created by Andong Li on 10/6/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe26.h"


const char* NEHE26::TITLE = "NEHE26";

GLfloat NEHE26::sleepTime = 0.0f;
int NEHE26::frameCounter = 0;
int NEHE26::currentTime = 0;
int NEHE26::lastTime = 0;
char NEHE26::FPSstr[15]  = "Calculating...";

GLfloat NEHE26::LightAmb[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat NEHE26::LightDif[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat NEHE26::LightPos[] = {4.0f, 4.0f, 6.0f, 1.0f};

GLUquadricObj* NEHE26::q = NULL;
GLfloat NEHE26::xrot =  0.0f;
GLfloat NEHE26::yrot =  0.0f;
GLfloat NEHE26::xrotspeed =  0.0f;
GLfloat NEHE26::yrotspeed =  0.0f;
GLfloat NEHE26::zoom = -7.0f;
GLfloat NEHE26::height =  2.0f;
GLuint NEHE26::texture[3] = {0};

bool NEHE26::keys[256] = {};	//all set to false
bool NEHE26::specialKeys[256] = {};	//all set to false

GLvoid NEHE26::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
	//gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool NEHE26::LoadGLTextures(const char* dir, int index){
	
	/* load an image file directly as a new OpenGL texture */
    texture[index] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir(dir),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[index] == 0){
        return false;
	}
	
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	return true;
}

GLvoid NEHE26::InitGL(){
	
	glGenTextures(3, &texture[0]);	// Create Three Textures
	
	if(!LoadGLTextures("NeheGL/img/envwall.jpg", 0)){
        cout<<"Fail to load textures"<<endl;
    }
	
	if(!LoadGLTextures("NeheGL/img/ball.jpg", 1)){
        cout<<"Fail to load textures"<<endl;
    }
	
	if(!LoadGLTextures("NeheGL/img/envroll.jpg", 2)){
        cout<<"Fail to load textures"<<endl;
    }
	
	glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
    glClearColor(0.2f, 0.5f, 1.0f, 1.0f);	// Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glClearStencil(0);	// Clear The Stencil Buffer To 0
    glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);	// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glEnable(GL_TEXTURE_2D);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);	// Set The Ambient Lighting For Light0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);	// Set The Diffuse Lighting For Light0
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);	// Set The Position For Light0
	
	glEnable(GL_LIGHT0);	// Enable Light 0
	glEnable(GL_LIGHTING);	// Enable Lighting
	
	q = gluNewQuadric();	// Create A New Quadratic
    gluQuadricNormals(q, GL_SMOOTH);	// Generate Smooth Normals For The Quad
    gluQuadricTexture(q, GL_TRUE);	// Enable Texture Coords For The Quad
	
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	
}

// Draw Our Ball
void NEHE26::DrawObject()
{
    glColor3f(1.0f, 1.0f, 1.0f);	// Set Color To White
    glBindTexture(GL_TEXTURE_2D, texture[1]);	// Select Texture 2 (1)
    gluSphere(q, 0.35f, 32, 16);	// Draw First Sphere
	glBindTexture(GL_TEXTURE_2D, texture[2]);	// Select Texture 3 (2)
    glColor4f(1.0f, 1.0f, 1.0f, 0.4f);	// Set Color To White With 40% Alpha
    glEnable(GL_BLEND);	// Enable Blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);	// Set Blending Mode To Mix Based On SRC Alpha
    glEnable(GL_TEXTURE_GEN_S);	// Enable Sphere Mapping
    glEnable(GL_TEXTURE_GEN_T);	// Enable Sphere Mapping
	
    gluSphere(q, 0.35f, 32, 16);	// Draw Another Sphere Using New Texture
	// Textures Will Mix Creating A MultiTexture Effect (Reflection)
    glDisable(GL_TEXTURE_GEN_S);	// Disable Sphere Mapping
    glDisable(GL_TEXTURE_GEN_T);	// Disable Sphere Mapping
    glDisable(GL_BLEND);	// Disable Blending
}

// Draws The Floor
void NEHE26::DrawFloor(){
    glBindTexture(GL_TEXTURE_2D, texture[0]);	// Select Texture 1 (0)
    glBegin(GL_QUADS);	// Begin Drawing A Quad
	glNormal3f(0.0, 1.0, 0.0);	// Normal Pointing Up
	glTexCoord2f(0.0f, 1.0f);	// Bottom Left Of Texture
	glVertex3f(-2.0, 0.0, 2.0);	// Bottom Left Corner Of Floor
	
	glTexCoord2f(0.0f, 0.0f);	// Top Left Of Texture
	glVertex3f(-2.0, 0.0,-2.0);	// Top Left Corner Of Floor
	
	glTexCoord2f(1.0f, 0.0f);	// Top Right Of Texture
	glVertex3f( 2.0, 0.0,-2.0);	// Top Right Corner Of Floor
	
	glTexCoord2f(1.0f, 1.0f);	// Bottom Right Of Texture
	glVertex3f( 2.0, 0.0, 2.0);	// Bottom Right Corner Of Floor
    glEnd();	// Done Drawing The Quad
}

GLvoid NEHE26::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Clip Plane Equations
    double eqr[] = {0.0f,-1.0f, 0.0f, 0.0f};
	
	glLoadIdentity();
	glTranslatef(0.0f, -0.6f, zoom);
	
	glColorMask(0,0,0,0);	// Set Color Mask
	
	glEnable(GL_STENCIL_TEST);	// Enable Stencil Buffer For "marking" The Floor
	glStencilFunc(GL_ALWAYS, 1, 1);	// Always Passes, 1 Bit Plane, 1 As Mask
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	// We Set The Stencil Buffer To 1 Where We Draw Any Polygon
	
	// Keep If Test Fails, Keep If Test Passes But Buffer Test Fails
	// Replace If Test Passes
	glDisable(GL_DEPTH_TEST);	// Disable Depth Testing
	DrawFloor();	// Draw The Floor (Draws To The Stencil Buffer)
	
	
	// We Only Want To Mark It In The Stencil Buffer
	glEnable(GL_DEPTH_TEST);	// Enable Depth Testing
	glColorMask(1,1,1,1);	// Set Color Mask to TRUE, TRUE, TRUE, TRUE
	glStencilFunc(GL_EQUAL, 1, 1);	// We Draw Only Where The Stencil Is 1
	// (I.E. Where The Floor Was Drawn)
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);	// Don't Change The Stencil Buffer
	glEnable(GL_CLIP_PLANE0);	// Enable Clip Plane For Removing Artifacts
	// (When The Object Crosses The Floor)
	glClipPlane(GL_CLIP_PLANE0, eqr);	// Equation For Reflected Objects
	glPushMatrix();	// Push The Matrix Onto The Stack
    glScalef(1.0f, -1.0f, 1.0f);	// Mirror Y Axis
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);	// Set Up Light0
    glTranslatef(0.0f, height, 0.0f);	// Position The Object
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);	// Rotate Local Coordinate System On X Axis
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);	// Rotate Local Coordinate System On Y Axis
    DrawObject();	// Draw The Sphere (Reflection)
	
	glPopMatrix();	// Pop The Matrix Off The Stack
	glDisable(GL_CLIP_PLANE0);	// Disable Clip Plane For Drawing The Floor
	glDisable(GL_STENCIL_TEST);	// We Don't Need The Stencil Buffer Any More (Disable)
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);                // Set Up Light0 Position
	glEnable(GL_BLEND);	// Enable Blending (Otherwise The Reflected Object Wont Show)
	glDisable(GL_LIGHTING);	// Since We Use Blending, We Disable Lighting
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);	// Set Color To White With 80% Alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending Based On Source Alpha And 1 Minus Dest Alpha
	DrawFloor();                                // Draw The Floor To The Screen
	
	glEnable(GL_LIGHTING);	// Enable Lighting
	glDisable(GL_BLEND);	// Disable Blending
	glTranslatef(0.0f, height, 0.0f);	// Position The Ball At Proper Height
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);	// Rotate On The X Axis
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);	// Rotate On The Y Axis
	DrawObject();	// Draw The Ball
	
	//draw FPS text
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	glutSwapBuffers();
	
	xrot += xrotspeed;
    yrot += yrotspeed;
	
	// handle keyboard input
	if(specialKeys[GLUT_KEY_RIGHT]) yrotspeed += 0.08f;
    if(specialKeys[GLUT_KEY_LEFT])  yrotspeed -= 0.08f;
    if(specialKeys[GLUT_KEY_DOWN])  xrotspeed += 0.08f;
    if(specialKeys[GLUT_KEY_UP])    xrotspeed -= 0.08f;
	
    if(keys['a']) zoom +=0.05f;
    if(keys['z']) zoom -=0.05f;
	
    if(specialKeys[GLUT_KEY_PAGE_UP]) height +=0.03f;
    if(specialKeys[GLUT_KEY_PAGE_DOWN])  height -=0.03f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE26::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE26::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE26::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE26::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE26::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE26::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
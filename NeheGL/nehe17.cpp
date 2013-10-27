//
//  nehe17.cpp
//  NeheGL
//
//  Created by Andong Li on 9/15/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe17.h"


const char* NEHE17::TITLE = "NEHE17";

GLfloat NEHE17::sleepTime = 0.0f;
int NEHE17::frameCounter = 0;
int NEHE17::currentTime = 0;
int NEHE17::lastTime = 0;
char NEHE17::FPSstr[15]  = "Calculating...";

GLuint NEHE17::texture[2] = {0,0};
GLuint NEHE17::base = 0;
GLfloat NEHE17::cnt1 = 0.0f;
GLfloat NEHE17::cnt2 = 0.0f;


GLvoid NEHE17::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE17::LoadGLTextures(const char* dir, int textureID){
	
	/* load an image file directly as a new OpenGL texture */
    texture[textureID] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir(dir),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[textureID] == 0){
        return false;
	}
	
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[textureID]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
    return true;
}

GLvoid NEHE17::buildFont(){
	int i;
	float cx;
	float cy;
	
	base = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	for (i=0 ; i<256 ; i++) {
		cx = float(i%16)/16.0f;
		cy = float(i/16)/16.0f;
		glNewList(base+i,GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(cx,1-cy-0.0625f);
		glVertex2i(0,0);
		glTexCoord2f(cx+0.0625f,1-cy-0.0625f);
		glVertex2i(16,0);
		glTexCoord2f(cx+0.0625f,1-cy);
		glVertex2i(16,16);
		glTexCoord2f(cx,1-cy);
		glVertex2i(0,16);
		glEnd();
		glTranslated(10,0,0);
		glEndList();
	}
}

GLvoid NEHE17::InitGL(){
	if(!LoadGLTextures("NeheGL/img/Font.bmp", 0)){
        cout<<"Fail to load font texture"<<endl;
    }
	
	if(!LoadGLTextures("NeheGL/img/Bumps.bmp", 1)){
        cout<<"Fail to load bumps texture"<<endl;
    }
	
	buildFont();
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Select The Type Of Blending
	
}

GLvoid NEHE17::glPrint(GLint x, GLint y, const char *string, int set){
	if(set > 1){
		set = 1;
	}
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,640,0,480,-100,100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);	// translate to right position on screen
	glListBase(base-32+(128*set));
	glCallLists((int)(strlen(string)),GL_BYTE,string);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

GLvoid NEHE17::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(0.0f,0.0f,-5.0f);
	glRotatef(45.0f,0.0f,0.0f,1.0f);
	glRotatef(cnt1*30.0f,1.0f,1.0f,0.0f);
	glDisable(GL_BLEND);
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f,0.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2d(1.0f,0.0f);
	glVertex2f( 1.0f, 1.0f);
	glTexCoord2d(1.0f,1.0f);
	glVertex2f( 1.0f,-1.0f);
	glTexCoord2d(0.0f,1.0f);
	glVertex2f(-1.0f,-1.0f);
	glEnd();
	glRotatef(90.0f,1.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f,0.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2d(1.0f,0.0f);
	glVertex2f( 1.0f, 1.0f);
	glTexCoord2d(1.0f,1.0f);
	glVertex2f( 1.0f,-1.0f);
	glTexCoord2d(0.0f,1.0f);
	glVertex2f(-1.0f,-1.0f);
	glEnd();
	glEnable(GL_BLEND);
	
	glLoadIdentity();
	glColor3f(1.0f*float(cos(cnt1)),1.0f*float(sin(cnt2)),1.0f-0.5f*float(cos(cnt1+cnt2)));
	glPrint(int((280+250*cos(cnt1))),int(235+200*sin(cnt2)),"NeHe",0);
	
	glColor3f(1.0f*float(sin(cnt2)),1.0f-0.5f*float(cos(cnt1+cnt2)),1.0f*float(cos(cnt1)));
	glPrint(int((280+230*cos(cnt2))),int(235+200*sin(cnt1)),"OpenGL",1);
	
	glColor3f(0.0f,0.0f,1.0f);
	glPrint(int(240+200*cos((cnt2+cnt1)/5)),2,"Andong Li",0);
	glColor3f(1.0f,1.0f,1.0f);
	glPrint(int(242+200*cos((cnt2+cnt1)/5)),2,"Andong Li",0);
		
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
		
	glutSwapBuffers();
	
	cnt1 += 0.01f;
	cnt2 += 0.0081f;
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE17::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE17::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
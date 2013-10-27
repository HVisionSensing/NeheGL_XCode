//
//  nehe09.cpp
//  NeheGL
//
//  Created by Andong Li on 9/6/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe09.h"

const char* NEHE09::TITLE = "NEHE09";

GLfloat NEHE09::sleepTime = 0.0f;
int NEHE09::frameCounter = 0;
int NEHE09::currentTime = 0;
int NEHE09::lastTime = 0;
char NEHE09::FPSstr[15]  = "Calculating...";

GLuint NEHE09::texture[1] = {0};

bool NEHE09::twinkle = false;
bool NEHE09::tp = false;

GLfloat	NEHE09::zoom=-15.0f;
GLfloat NEHE09::tilt=90.0f;
GLfloat	NEHE09::spin = 0.0f;
GLuint NEHE09::loop = 0;

stars NEHE09::star[num];	//only for declaration

bool NEHE09::keys[256] = {};	//all set to false
bool NEHE09::specialKeys[256] = {};	//all set to false

GLvoid NEHE09::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE09::LoadGLTextures(const char* dir){
	
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

GLvoid NEHE09::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/star.bmp")){
        cout<<"Fail to load textures"<<endl;
    }
	
	glEnable(GL_TEXTURE_2D);	// Enable Texture Mapping
    glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	// Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Set The Blending Function For Translucency
    glEnable(GL_BLEND);	// Enable Blending
	
	//initial stars
	for(loop=0; loop<num; loop++){
		star[loop].angle=0.0f;
		star[loop].dist=(float(loop)/num)*5.0f;
		star[loop].r=rand()%256;
		star[loop].g=rand()%256;
		star[loop].b=rand()%256;
	}
	
}

GLvoid NEHE09::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	for (loop=0; loop<num; loop++){
		glLoadIdentity();
        glTranslatef(0.0f,0.0f,zoom);
        glRotatef(tilt,1.0f,0.0f,0.0f);
		
		//move stars
		glRotatef(star[loop].angle,0.0f,1.0f,0.0f);
		glTranslatef(star[loop].dist,0.0f,0.0f);
		//cancel star rotate and translate
		glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);
		glRotatef(-tilt,1.0f,0.0f,0.0f);
		
		if(twinkle){
			glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}
		
		glRotatef(spin,0.0f,0.0f,1.0f);
		glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		
		spin+=0.01f;
		star[loop].angle+=float(loop)/num;
		star[loop].dist-=0.01f;
		
		//update stars status
		if (star[loop].dist<0.0f){
            star[loop].dist+=5.0f;
            star[loop].r=rand()%256;
            star[loop].g=rand()%256;
            star[loop].b=rand()%256;
        }
	}
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
	
	if(keys['t'] && !tp){
		tp=TRUE;
		twinkle=!twinkle;
	}
	if(!keys['t']){
		tp=FALSE;
	}
	
	
	// handle key actions
	if(specialKeys[GLUT_KEY_PAGE_UP]){
		zoom -= 0.2f;
	}
	if(specialKeys[GLUT_KEY_PAGE_DOWN]){
		zoom += 0.2f;
	}
	
	if (specialKeys[GLUT_KEY_UP]){
		tilt -= 0.5f;
	}
	if (specialKeys[GLUT_KEY_DOWN]){
		tilt += 0.5f;
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE09::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE09::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE09::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE09::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE09::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE09::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

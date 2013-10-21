//
//  nehe38.cpp
//  NeheGL
//
//  Created by Andong Li on 10/20/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe38.h"


const char* NEHE38::TITLE = "NEHE38";

GLfloat NEHE38::sleepTime = 0.0f;
int NEHE38::frameCounter = 0;
int NEHE38::currentTime = 0;
int NEHE38::lastTime = 0;
char NEHE38::FPSstr[15]  = "Calculating...";

GLuint NEHE38::texture[3] = {0};
object38 NEHE38::obj[50];	// just for declaration

bool NEHE38::keys[256] = {};	//all set to false
bool NEHE38::specialKeys[256] = {};	//all set to false

GLvoid NEHE38::ReSizeGLScene(GLsizei width, GLsizei height){
	
	// Prevent A Divide By Zero By
    if(height==0){
        height=1;
    }
	
	// Reset The Current Viewport
    glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Calculate The Aspect Ratio Of The Window
    //gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void NEHE38::SetObject(int loop){
	obj[loop].tex=rand()%3;
	obj[loop].x=rand()%34-17.0f;
	obj[loop].y=18.0f;
	obj[loop].z=-((rand()%30000/1000.0f)+10.0f);
	obj[loop].spinzi=(rand()%10000)/5000.0f-1.0f;
	obj[loop].flap=0.0f;
	obj[loop].fi=0.05f+(rand()%100)/1000.0f;
	obj[loop].yi=0.001f+(rand()%1000)/10000.0f;
}

bool NEHE38::LoadGLTextures(const char* dir, int index){
	
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


GLvoid NEHE38::InitGL(){
	
	if(!LoadGLTextures("NeheGL/img/Butterfly1.bmp", 0)){
		cout<<"Fail to load texture!"<<endl;
	}
	
	if(!LoadGLTextures("NeheGL/img/Butterfly2.bmp", 1)){
		cout<<"Fail to load texture!"<<endl;
	}
	
	if(!LoadGLTextures("NeheGL/img/Butterfly3.bmp", 2)){
		cout<<"Fail to load texture!"<<endl;
	}
	
	
	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth (1.0f);
	glDepthFunc (GL_LEQUAL);
	glDisable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	for (int loop=0; loop<50; loop++){
		SetObject(loop);
	}
	
}


GLvoid NEHE38::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	for (int loop=0; loop<50; loop++){
		glLoadIdentity ();
		glBindTexture(GL_TEXTURE_2D, texture[obj[loop].tex]);
		glTranslatef(obj[loop].x,obj[loop].y,obj[loop].z);
		glRotatef(45.0f,1.0f,0.0f,0.0f);
		glRotatef((obj[loop].spinz),0.0f,0.0f,1.0f);
		
		glBegin(GL_TRIANGLES);
		// First Triangle														    _____
		glTexCoord2f(1.0f,1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);				//	(2)|    / (1)
		glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, obj[loop].flap);	//	   |  /
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);				//	(3)|/
		
		// Second Triangle
		glTexCoord2f(1.0f,1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);				//	       /|(1)
		glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);				//	     /  |
		glTexCoord2f(1.0f,0.0f); glVertex3f( 1.0f,-1.0f, obj[loop].flap);	//	(2)/____|(3)
		
		glEnd();
		
		obj[loop].y-=obj[loop].yi;	// Move Object Down The Screen
		obj[loop].spinz+=obj[loop].spinzi;	// Increase Z Rotation By spinzi
		obj[loop].flap+=obj[loop].fi;	// Increase flap Value By fi
		
		// Is Object Off The Screen?
		if (obj[loop].y<-18.0f){
			SetObject(loop);	// If So, Reassign New Values
		}
		
		// Time To Change Flap Direction?
		if ((obj[loop].flap>1.0f) || (obj[loop].flap<-1.0f)){
			obj[loop].fi=-obj[loop].fi;	// Change Direction By Making fi = -fi
		}
	}
	
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-1.08f,-0.8f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE38::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE38::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE38::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE38::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE38::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE38::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
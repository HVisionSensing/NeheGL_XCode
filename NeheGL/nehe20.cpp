//
//  nehe20.cpp
//  NeheGL
//
//  Created by Andong Li on 9/27/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe20.h"

const char* NEHE20::TITLE = "NEHE20";

GLfloat NEHE20::sleepTime = 0.0f;
int NEHE20::frameCounter = 0;
int NEHE20::currentTime = 0;
int NEHE20::lastTime = 0;
char NEHE20::FPSstr[15]  = "Calculating...";


GLuint NEHE20::texture[5] = {0};
bool NEHE20::masking = true;
bool NEHE20::scene = true;
bool NEHE20::mp = false;
bool NEHE20::sp = false;
GLuint NEHE20::loop = 0;
GLfloat NEHE20::roll = 0.0;


bool NEHE20::keys[256] = {};	//all set to false
bool NEHE20::specialKeys[256] = {};	//all set to false

GLvoid NEHE20::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE20::LoadGLTextures(const char* dir, int index){
	
	
	int imageWidth, imageHeight;
	
	//load image, using SOIL
	unsigned char* image = SOIL_load_image(Utils::getAbsoluteDir(dir),&imageWidth,&imageHeight,0,0);
	if(image == NULL){
		return false;
	}
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	
	//free loaded texture image
	SOIL_free_image_data(image);
	
    return true;

	
}

GLvoid NEHE20::InitGL(){
	
	glGenTextures(5, &texture[0]);	// Create Five Textures
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/Logo.bmp", 0)){
        cout<<"Fail to load textures"<<endl;
    }
	if(!LoadGLTextures("NeheGL/img/Mask1.bmp", 1)){
        cout<<"Fail to load textures"<<endl;
    }
	if(!LoadGLTextures("NeheGL/img/Image1.bmp", 2)){
        cout<<"Fail to load textures"<<endl;
    }
	if(!LoadGLTextures("NeheGL/img/Mask2.bmp", 3)){
        cout<<"Fail to load textures"<<endl;
    }
	if(!LoadGLTextures("NeheGL/img/Image2.bmp", 4)){
        cout<<"Fail to load textures"<<endl;
    }
	
	
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
	
}


GLvoid NEHE20::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw logo
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-2.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // Select Our Logo Texture
	glBegin(GL_QUADS);
    glTexCoord2f(0.0f, -roll+0.0f);
	glVertex3f(-1.1f, -1.1f,  0.0f);	// Bottom Left
    glTexCoord2f(3.0f, -roll+0.0f);
	glVertex3f( 1.1f, -1.1f,  0.0f);	// Bottom Right
    glTexCoord2f(3.0f, -roll+3.0f);
	glVertex3f( 1.1f,  1.1f,  0.0f);	// Top Right
    glTexCoord2f(0.0f, -roll+3.0f);
	glVertex3f(-1.1f,  1.1f,  0.0f);	// Top Left
	glEnd();
	
	
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	if(masking){
		glBlendFunc(GL_DST_COLOR,GL_ZERO);
	}
	
	//select which scene to draw
	if(scene){
		glTranslatef(0.0f,0.0f,-1.0f);
		glRotatef(roll*360,0.0f,0.0f,1.0f);
		if(masking){
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.1f, -1.1f,  0.0f);  // Bottom Left
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f( 1.1f, -1.1f,  0.0f);  // Bottom Right
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f( 1.1f,  1.1f,  0.0f);  // Top Right
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.1f,  1.1f,  0.0f);  // Top Left
			glEnd();
		}
		glBlendFunc(GL_ONE, GL_ONE);	// Copy Image 2 Color To The Screen
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.1f, -1.1f,  0.0f);  // Bottom Left
        glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.1f, -1.1f,  0.0f);  // Bottom Right
        glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.1f,  1.1f,  0.0f);  // Top Right
        glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.1f,  1.1f,  0.0f);  // Top Left
		glEnd();
	}
	else{
		if(masking){
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glBegin(GL_QUADS);
			glTexCoord2f(roll+0.0f, 0.0f);
			glVertex3f(-1.1f, -1.1f,  0.0f); // Bottom Left
			glTexCoord2f(roll+4.0f, 0.0f);
			glVertex3f( 1.1f, -1.1f,  0.0f); // Bottom Right
			glTexCoord2f(roll+4.0f, 4.0f);
			glVertex3f( 1.1f,  1.1f,  0.0f); // Top Right
			glTexCoord2f(roll+0.0f, 4.0f);
			glVertex3f(-1.1f,  1.1f,  0.0f); // Top Left
			glEnd();
		}
		glBlendFunc(GL_ONE, GL_ONE);	// Copy Image 1 Color To The Screen
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
        glTexCoord2f(roll+0.0f, 0.0f);
		glVertex3f(-1.1f, -1.1f,  0.0f); // Bottom Left
        glTexCoord2f(roll+4.0f, 0.0f);
		glVertex3f( 1.1f, -1.1f,  0.0f); // Bottom Right
        glTexCoord2f(roll+4.0f, 4.0f);
		glVertex3f( 1.1f,  1.1f,  0.0f); // Top Right
        glTexCoord2f(roll+0.0f, 4.0f);
		glVertex3f(-1.1f,  1.1f,  0.0f); // Top Left
		glEnd();
	}
	glEnable(GL_DEPTH_TEST);                        // Enable Depth Testing
	glDisable(GL_BLEND);
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	 
	glutSwapBuffers();
	
	// handle key actions
	if(keys[' '] && !sp){
		sp=TRUE;
		scene=!scene;
	}
	if(!keys[' ']){
		sp=FALSE;
	}
	if(keys['m'] && !mp){
		mp=TRUE;
		masking=!masking;
	}
	if (!keys['m']){
		mp=FALSE;
	}
	
	roll+=0.002f;
    if(roll>1.0f){
        roll-=1.0f;
    }
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE20::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE20::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE20::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE20::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE20::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE20::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
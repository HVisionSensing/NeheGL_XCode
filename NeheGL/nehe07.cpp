//
//  nehe07.cpp
//  NeheGL
//
//  Created by Andong Li on 9/5/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe07.h"

const char* NEHE07::TITLE = "NEHE07";

GLfloat NEHE07::sleepTime = 0.0f;
int NEHE07::frameCounter = 0;
int NEHE07::currentTime = 0;
int NEHE07::lastTime = 0;
char NEHE07::FPSstr[15]  = "Calculating...";

GLuint NEHE07::texture[3] = {0,0,0};
GLuint NEHE07::filter = 0;	//use first filter as default
GLfloat NEHE07::xrot = 0.0f;
GLfloat NEHE07::yrot = 0.0f;
GLfloat NEHE07::xspeed = 0.0f;
GLfloat NEHE07::yspeed = 0.0f;
GLfloat NEHE07::z = -5.0f;

GLfloat NEHE07::LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat NEHE07::LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat NEHE07::LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };

bool NEHE07::light = false;
bool NEHE07::lp = false;
bool NEHE07::fp = false;

bool NEHE07::keys[256] = {};	//all set to false
bool NEHE07::specialKeys[256] = {};	//all set to false

GLvoid NEHE07::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE07::LoadGLTextures(const char* dir){
	
	int imageWidth, imageHeight;
	
	//load image, using SOIL
	unsigned char* image = SOIL_load_image(Utils::getAbsoluteDir(dir),&imageWidth,&imageHeight,0,0);
	if(image == NULL){
		return false;
	}
	
	glGenTextures(3, &texture[0]);	// Create Three Textures
	
    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	//free loaded texture image
	SOIL_free_image_data(image);
	
    return true;
}

GLvoid NEHE07::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/crate.png")){
        cout<<"Fail to load textures"<<endl;
    }
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);	// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);	// Enable Light One
	
}

GLvoid NEHE07::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,z);	// Translate Into/Out Of The Screen By z
	glRotatef(xrot,1.0f,0.0f,0.0f);	// Rotate On The X Axis By xrot
	glRotatef(yrot,0.0f,1.0f,0.0f);	// Rotate On The Y Axis By yrot
	
	glBindTexture(GL_TEXTURE_2D, texture[filter]);	// Select Our Texture
	
	glBegin(GL_QUADS);
	// Front Face
    glNormal3f( 0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);                  // Normal Pointing Away From Viewer
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
    // Bottom Face
    glNormal3f( 0.0f,-1.0f, 0.0f);                  // Normal Pointing Down
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
	glEnd();
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	if (light){
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
	xrot += xspeed;	// Add xspeed To xrot
    yrot += yspeed;	// Add yspeed To yrot
	
	// handle key actions
	if(keys['l'] && !lp){
		lp=TRUE;
		light=!light;
		if (!light){
			glDisable(GL_LIGHTING);
		}
		else{
			glEnable(GL_LIGHTING);
		}
	}
	if(!keys['l']){
		lp=FALSE;
	}
	if(keys['f'] && !fp){
		fp=TRUE;
		filter = (filter+ 1) % 3;
	}
	if(!keys['f']){
		fp=FALSE;
	}
	
	if(specialKeys[GLUT_KEY_PAGE_UP]){
		z-=0.02f;
	}
	if(specialKeys[GLUT_KEY_PAGE_DOWN]){
		z+=0.02f;
	}
	
	if (specialKeys[GLUT_KEY_UP]){
		xspeed-=0.01f;
	}
	if (specialKeys[GLUT_KEY_DOWN]){
		xspeed+=0.01f;
	}
	if (specialKeys[GLUT_KEY_RIGHT]){
		yspeed+=0.01f;
	}
	if (specialKeys[GLUT_KEY_LEFT]){
		yspeed-=0.01f;
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE07::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE07::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE07::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE07::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE07::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE07::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

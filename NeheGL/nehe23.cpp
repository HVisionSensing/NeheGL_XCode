//
//  nehe23.cpp
//  NeheGL
//
//  Created by Andong Li on 10/4/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe23.h"


const char* NEHE23::TITLE = "NEHE23";

GLfloat NEHE23::sleepTime = 0.0f;
int NEHE23::frameCounter = 0;
int NEHE23::currentTime = 0;
int NEHE23::lastTime = 0;
char NEHE23::FPSstr[15]  = "Calculating...";

GLuint NEHE23::texture[6] = {0};
GLuint NEHE23::filter = 0;	//use first filter as default
GLfloat NEHE23::xrot = 0.0f;
GLfloat NEHE23::yrot = 0.0f;
GLfloat NEHE23::xspeed = 0.0f;
GLfloat NEHE23::yspeed = 0.0f;
GLfloat NEHE23::z = -5.0f;

int NEHE23::part1 = 0;
int NEHE23::part2 = 0;
int NEHE23::p1 = 0;
int NEHE23::p2 = 1;

GLUquadricObj* NEHE23::quadratic = NULL;
GLuint  NEHE23::object = 0;

GLfloat NEHE23::LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat NEHE23::LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat NEHE23::LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };

bool NEHE23::light = false;
bool NEHE23::lp = false;
bool NEHE23::fp = false;
bool NEHE23::sp = false;

bool NEHE23::keys[256] = {};	//all set to false
bool NEHE23::specialKeys[256] = {};	//all set to false

GLvoid NEHE23::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE23::LoadGLTextures(){
	
	glGenTextures(6, &texture[0]);	// Create Six Textures
		
    texture[0] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/bg.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[0] == 0){
        return false;
	}
	
    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	
	texture[2] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/bg.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[2] == 0){
        return false;
	}
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	texture[4] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/bg.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[4] == 0){
        return false;
	}
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

	texture[1] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/reflect.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[1] == 0){
        return false;
	}
	
    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	
	texture[3] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/reflect.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[3] == 0){
        return false;
	}
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	texture[5] = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir("NeheGL/img/reflect.png"),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	 );
	
    if(texture[5] == 0){
        return false;
	}
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	
    return true;
}

GLvoid NEHE23::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures()){
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
	
	quadratic=gluNewQuadric();	// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);	// Create Texture Coords
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set The Texture Generation Mode For S To Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set The Texture Generation Mode For T To Sphere Mapping
	
}

GLvoid NEHE23::glDrawCube(){
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back Face
	glNormal3f( 0.0f, 0.0f,-0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glNormal3f( 0.0f, 0.5f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom Face
	glNormal3f( 0.0f,-0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right Face
	glNormal3f( 0.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left Face
	glNormal3f(-0.5f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

GLvoid NEHE23::DrawGLScene(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
    glTranslatef(0.0f,0.0f,z);
	
    glEnable(GL_TEXTURE_GEN_S);	// Enable Texture Coord Generation For S
    glEnable(GL_TEXTURE_GEN_T);	// Enable Texture Coord Generation For T
	
    glBindTexture(GL_TEXTURE_2D, texture[filter+(filter+1)]);	// This Will Select A Sphere Map
    glPushMatrix();
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
    switch(object)
    {
		case 0:
			glDrawCube();
			break;
		case 1:
			glTranslatef(0.0f,0.0f,-1.5f);                  // Center The Cylinder
			gluCylinder(quadratic,1.0f,1.0f,3.0f,32,32);            // A Cylinder With A Radius Of 0.5 And A Height Of 2
			break;
		case 2:
			gluSphere(quadratic,1.3f,32,32);                // Sphere With A Radius Of 1 And 16 Longitude/Latitude Segments
			break;
		case 3:
			glTranslatef(0.0f,0.0f,-1.5f);                  // Center The Cone
			gluCylinder(quadratic,1.0f,0.0f,3.0f,32,32);            // Cone With A Bottom Radius Of .5 And Height Of 2
			break;
    };
	
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S);                        // Disable Texture Coord Generation ( NEW )
    glDisable(GL_TEXTURE_GEN_T);                        // Disable Texture Coord Generation ( NEW )
	
    glBindTexture(GL_TEXTURE_2D, texture[filter*2]);            // This Will Select The BG Texture ( NEW )
    glPushMatrix();
	glTranslatef(0.0f, 0.0f, -24.0f);
	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-13.3f, -10.0f,  10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.3f, -10.0f,  10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.3f,  10.0f,  10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-13.3f,  10.0f,  10.0f);
	glEnd();
	
    glPopMatrix();
	
	
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
	
	if (keys[' '] && !sp){
		sp=TRUE;
		object++;
		if(object>3)
			object=0;
	}
	if (!keys[' ']){
		sp=FALSE;
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
GLvoid NEHE23::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE23::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE23::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE23::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE23::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE23::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
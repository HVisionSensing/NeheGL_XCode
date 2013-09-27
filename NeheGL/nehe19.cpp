//
//  nehe19.cpp
//  NeheGL
//
//  Created by Andong Li on 9/26/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe19.h"

const char* NEHE19::TITLE = "NEHE19";

GLfloat NEHE19::sleepTime = 0.0f;
int NEHE19::frameCounter = 0;
int NEHE19::currentTime = 0;
int NEHE19::lastTime = 0;
char NEHE19::FPSstr[15]  = "Calculating...";

GLuint NEHE19::col = 0;
GLuint NEHE19::delay = 0;
GLuint NEHE19::texture[1] = {0};

bool NEHE19::sp = false;
bool NEHE19::rp = false;
bool NEHE19::rainbow = true;
GLfloat NEHE19::xspeed = 0.0f;
GLfloat NEHE19::yspeed = 0.0f;
GLfloat NEHE19::zoom = -40.0;
GLfloat NEHE19::slowdown = 2.0f;

Particle NEHE19::particles[MAX_PARTICLES];	//just declare here
GLfloat NEHE19::colors[12][3] = {
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};


bool NEHE19::keys[256] = {};	//all set to false
bool NEHE19::specialKeys[256] = {};	//all set to false

GLvoid NEHE19::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
	// Look deeper into the screen from 100 to 200
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool NEHE19::LoadGLTextures(const char* dir){
	
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

GLvoid NEHE19::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/Particle.png")){
        cout<<"Fail to load textures"<<endl;
    }
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Type Of Blending To Perform
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	
	// Initialize All The Textures
	for (int loop=0;loop<MAX_PARTICLES;loop++){
		particles[loop].active=true;	// Make All The Particles Active
		particles[loop].life=1.0f;	// Give All The Particles Full Life
		particles[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		//particles[loop].r=colors[(int)(loop*(12.0/MAX_PARTICLES))][0];
		//particles[loop].g=colors[(int)(loop*(12.0/MAX_PARTICLES))][1];
		//particles[loop].b=colors[(int)(loop*(12.0/MAX_PARTICLES))][2];
		particles[loop].r=colors[(loop*(12/MAX_PARTICLES))][0];
		particles[loop].g=colors[(loop*(12/MAX_PARTICLES))][1];
		particles[loop].b=colors[(loop*(12/MAX_PARTICLES))][2];
		particles[loop].xi=float((rand()%50)-26.0f)*10.0f;
		particles[loop].yi=float((rand()%50)-25.0f)*10.0f;
		particles[loop].zi=float((rand()%50)-25.0f)*10.0f;
		particles[loop].xg=0.0f;
        particles[loop].yg=-0.8f;
        particles[loop].zg=0.0f;
	}
}


GLvoid NEHE19::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	for (int loop=0;loop<MAX_PARTICLES;loop++){
		if (particles[loop].active){
			float x = particles[loop].x;
			float y = particles[loop].y;
			float z = particles[loop].z+zoom;
			glColor4f(particles[loop].r, particles[loop].g, particles[loop].b, particles[loop].life);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
			glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
			glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
			glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
			glEnd();
			particles[loop].x+=particles[loop].xi/(slowdown*1000);	// Move On The X Axis By X Speed
			particles[loop].y+=particles[loop].yi/(slowdown*1000);	// Move On The Y Axis By Y Speed
			particles[loop].z+=particles[loop].zi/(slowdown*1000);	// Move On The Z Axis By Z Speed
			particles[loop].xi+=particles[loop].xg;	// Take Pull On X Axis Into Account
			particles[loop].yi+=particles[loop].yg;	// Take Pull On Y Axis Into Account
			particles[loop].zi+=particles[loop].zg;	// Take Pull On Z Axis Into Account
			particles[loop].life-=particles[loop].fade;       // Reduce Particles Life By 'Fade'
			// If Particle Is Burned Out
			if (particles[loop].life<0.0f){
				particles[loop].life=1.0f;	// Give It New Life
				particles[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
				particles[loop].x=0.0f;	// Center On X Axis
				particles[loop].y=0.0f;	// Center On Y Axis
				particles[loop].z=0.0f;	// Center On Z Axis
				particles[loop].xi=xspeed+float((rand()%60)-32.0f);	// X Axis Speed And Direction
				particles[loop].yi=yspeed+float((rand()%60)-30.0f);	// Y Axis Speed And Direction
				particles[loop].zi=float((rand()%60)-30.0f);	// Z Axis Speed And Direction
				particles[loop].r=colors[col][0];	// Select Red From Color Table
				particles[loop].g=colors[col][1];	// Select Green From Color Table
				particles[loop].b=colors[col][2];	// Select Blue From Color Table
			}
		}
		
		// handle key actions
		if(keys['8'] && (particles[loop].yg<1.5f)){
			particles[loop].yg+=0.01f;
		}
		if(keys['2'] && (particles[loop].yg>-1.5f)){
			particles[loop].yg -= 0.01f;
		}
		if(keys['6'] && (particles[loop].xg<1.5f)){
			particles[loop].xg+=0.01f;
		}
		if(keys['4'] && (particles[loop].xg>-1.5f)){
			particles[loop].xg-=0.01f;
		}
		if (keys['\t']) {
			particles[loop].x = 0.0f;
			particles[loop].y = 0.0f;
			particles[loop].z = 0.0f;
			particles[loop].xi = float((rand()%50)-26.0f)*10.0f;
			particles[loop].yi = float((rand()%50)-25.0f)*10.0f;
			particles[loop].zi = float((rand()%50)-25.0f)*10.0f;
		}
	}
		
	//draw FPS text
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glutSwapBuffers();
	
	// handle key actions
	if (keys['+'] && slowdown > 1.0f){
		slowdown -= 0.01f;
	}
	if (keys['-'] && slowdown < 4.0f){
		slowdown += 0.01f;
	}
	if ((keys[' '] && !sp) || (rainbow && (delay>25))){
		if (keys[' ']) rainbow=false;
		sp=true;
		delay=0;
		col = (col + 1) % 12;
	}
	if (!keys[' ']){
		sp=false;
	}
	if (keys[GLUT_KEY_PAGE_UP]){
		zoom += 0.1f;
	}
	if (keys[GLUT_KEY_PAGE_DOWN]){
		zoom -= 0.1f;
	}
	if (keys[GLUT_KEY_UP] && (yspeed < 200)){
		yspeed += 1.0f;
	}
	if (keys[GLUT_KEY_DOWN] && (yspeed > -200)){
		yspeed -= 1.0f;
	}
	if (keys[GLUT_KEY_RIGHT] && (xspeed < 200)){
		xspeed += 1.0f;
	}
	if (keys[GLUT_KEY_LEFT] && (xspeed > -200)){
		xspeed -= 1.0f;
	}
	if(keys['\r'] && !rp){
		rp = true;
		rainbow=!rainbow;
	}
	if(!keys['\r']){
		rp=false;
	}
	delay++;
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE19::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE19::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE19::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE19::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE19::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE19::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
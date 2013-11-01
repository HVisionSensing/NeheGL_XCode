//
//  nehe39.cpp
//  NeheGL
//
//  Created by Andong Li on 10/27/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe39.h"


const char* NEHE39::TITLE = "NEHE39";

GLfloat NEHE39::sleepTime = 0.0f;
int NEHE39::frameCounter = 0;
int NEHE39::currentTime = 0;
int NEHE39::lastTime = 0;
char NEHE39::FPSstr[15]  = "Calculating...";


ConstantVelocity* NEHE39::constantVelocity = new ConstantVelocity();
MotionUnderGravitation* NEHE39::motionUnderGravitation = new MotionUnderGravitation(Vector3D_PHY(0.0f, -9.81f, 0.0f));
MassConnectedWithSpring* NEHE39::massConnectedWithSpring = new MassConnectedWithSpring(2.0f);
float NEHE39::slowMotionRatio = 10.0f;
float NEHE39::timeElapsed = 0;

clock_t NEHE39::current_time = 0;
clock_t NEHE39::previous_time = 0;

bool NEHE39::keys[256] = {};	//all set to false
bool NEHE39::specialKeys[256] = {};	//all set to false

GLvoid NEHE39::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,100.0f);
	//gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


bool NEHE39::LoadGLTextures(const char* dir){
	
	return true;
}

GLvoid NEHE39::InitGL(){
	
	
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


GLvoid NEHE39::DrawGLScene(){
	
	// Reset The Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Drawing The Coordinate Plane
	// We Will Draw Horizontal And Vertical Lines With A Space Of 1 Meter Between Them.
	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	
	// Draw The Vertical Lines
	for (float x = -20; x <= 20; x += 1.0f){
		glVertex3f(x, 20, 0);
		glVertex3f(x,-20, 0);
	}
	
	// Draw The Horizontal Lines
	for (float y = -20; y <= 20; y += 1.0f){
		glVertex3f( 20, y, 0);
		glVertex3f(-20, y, 0);
	}
	
	glEnd();
	
	// Draw All Masses In constantVelocity Simulation
	glColor3ub(255, 0, 0);
	int a;
	for (a = 0; a < constantVelocity->numOfMasses; ++a){
		Mass_PHY* mass = constantVelocity->getMass(a);
		Vector3D_PHY* pos = &mass->pos;
		
		glTranslatef(pos->x, pos->y + 1, pos->z);
		Utils::drawText(0.0f,0.0f, GLUT_BITMAP_HELVETICA_12,"Mass with constant vel");
		glTranslatef(-pos->x, -pos->y - 1, -pos->z);
		
		glPointSize(4);
		glBegin(GL_POINTS);
		glVertex3f(pos->x, pos->y, pos->z);
		glEnd();
	}
	
	
	// Draw All Masses In motionUnderGravitation Simulation
	glColor3ub(255, 255, 0);
	for (a = 0; a < motionUnderGravitation->numOfMasses; ++a){
		Mass_PHY* mass = motionUnderGravitation->getMass(a);
		Vector3D_PHY* pos = &mass->pos;
		
		glTranslatef(pos->x, pos->y + 1, pos->z);
		Utils::drawText(0.0f,0.0f, GLUT_BITMAP_HELVETICA_12,"Motion under gravitation");
		glTranslatef(-pos->x, -pos->y - 1, -pos->z);
		
		glPointSize(4);
		glBegin(GL_POINTS);
		glVertex3f(pos->x, pos->y, pos->z);
		glEnd();
	}
	
	// Draw All Masses In massConnectedWithSpring Simulation
	glColor3ub(0, 255, 0);
	for (a = 0; a < massConnectedWithSpring->numOfMasses; ++a){
		Mass_PHY* mass = massConnectedWithSpring->getMass(a);
		Vector3D_PHY* pos = &mass->pos;
		
		glTranslatef(pos->x, pos->y + 1, pos->z);
		Utils::drawText(0.0f,0.0f, GLUT_BITMAP_HELVETICA_12,"Mass connected with spring");
		glTranslatef(-pos->x, -pos->y - 1, -pos->z);
		
		glPointSize(8);
		glBegin(GL_POINTS);
		glVertex3f(pos->x, pos->y, pos->z);
		glEnd();
		
		// Draw A Line From The Mass Position To Connection Position To Represent The Spring
		glBegin(GL_LINES);
		glVertex3f(pos->x, pos->y, pos->z);
		pos = &massConnectedWithSpring->connectionPos;
		glVertex3f(pos->x, pos->y, pos->z);
		glEnd();
	}
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	
	
	// Draw sys info text
	glColor3ub(255, 255, 255);
	char buf[50];
	sprintf(buf, "Time elapsed (seconds): %.2f", timeElapsed);
	Utils::drawText(-0.5f,0.38f, GLUT_BITMAP_HELVETICA_12, buf);
	sprintf(buf, "Slow motion ratio: %.2f", slowMotionRatio);
	Utils::drawText(-0.5f,0.355f, GLUT_BITMAP_HELVETICA_12, buf);
	
	Utils::drawText(-0.5f,0.33f, GLUT_BITMAP_HELVETICA_12, "Press [up] for normal motion");
	Utils::drawText(-0.5f,0.305f, GLUT_BITMAP_HELVETICA_12, "Press [down] for slow motion");
	
	glutSwapBuffers();
	
	// handle keyboard input
	if(specialKeys[GLUT_KEY_DOWN]){
		slowMotionRatio = 10.0f;
	}
    if(specialKeys[GLUT_KEY_UP]){
		slowMotionRatio = 1.0f;
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE39::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
	
	float dt = sleepTime/1000.0;
	dt /= slowMotionRatio;
	timeElapsed += dt;
	float maxPossible_dt = 0.1f;
	int numOfIterations = (int)(dt / maxPossible_dt) + 1;
	if (numOfIterations != 0)
		dt = dt / numOfIterations;
	
	for (int a = 0; a < numOfIterations; ++a){
		constantVelocity->operate(dt);
		motionUnderGravitation->operate(dt);
		massConnectedWithSpring->operate(dt);
	}
	
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE39::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE39::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE39::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE39::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE39::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
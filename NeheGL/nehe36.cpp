//
//  nehe36.cpp
//  NeheGL
//
//  Created by Andong Li on 10/19/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe36.h"

const char* NEHE36::TITLE = "NEHE36";

GLfloat NEHE36::sleepTime = 0.0f;
int NEHE36::frameCounter = 0;
int NEHE36::currentTime = 0;
int NEHE36::lastTime = 0;
char NEHE36::FPSstr[15]  = "Calculating...";

float NEHE36::angle = 0.0f;
float NEHE36::vertexes[4][3] = {0};
float NEHE36::normal[3] = {0};
GLuint NEHE36::BlurTexture = 0;

bool NEHE36::keys[256] = {};	//all set to false
bool NEHE36::specialKeys[256] = {};	//all set to false

GLvoid NEHE36::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
    gluPerspective(50.0f,(GLfloat)width/(GLfloat)height,5.0f,2000.0f);
	//gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLuint NEHE36::EmptyTexture(){
    GLuint txtnumber;
    unsigned int* data;
	
    data = (unsigned int*)new GLuint[((128 * 128)* 4 * sizeof(unsigned int))];
	memset(data,0,((128 * 128)* 4 * sizeof(unsigned int)));
	
    glGenTextures(1, &txtnumber);
    glBindTexture(GL_TEXTURE_2D, txtnumber);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 128, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
    delete [] data;
	
    return txtnumber;
}

// Reduces A Normal Vector (3 Coordinates)
void NEHE36::ReduceToUnit(float vector[3]){
    float length;
    length = (float)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));
	
    if(length == 0.0f)
        length = 1.0f;
	
    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
}

void NEHE36::calcNormal(float v[3][3], float out[3]){
	
	float v1[3],v2[3];
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;
	
    v1[x] = v[0][x] - v[1][x];
    v1[y] = v[0][y] - v[1][y];
    v1[z] = v[0][z] - v[1][z];
    
    v2[x] = v[1][x] - v[2][x];
    v2[y] = v[1][y] - v[2][y];
    v2[z] = v[1][z] - v[2][z];
    
    out[x] = v1[y]*v2[z] - v1[z]*v2[y];
    out[y] = v1[z]*v2[x] - v1[x]*v2[z];
    out[z] = v1[x]*v2[y] - v1[y]*v2[x];
	
    ReduceToUnit(out);
}

void NEHE36::ProcessHelix(){
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat phi;
    GLfloat theta;
    GLfloat v,u;
    GLfloat r;
    int twists = 5;
	
    GLfloat glfMaterialColor[]={0.4f,0.2f,0.8f,1.0f};
    GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	
    glLoadIdentity();
    gluLookAt(0, 5, 50, 0, 0, 0, 0, 1, 0);
	
    glPushMatrix();
	
    glTranslatef(0,0,-50);
    glRotatef(angle/2.0f,1,0,0);
    glRotatef(angle/3.0f,0,1,0);
	
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,glfMaterialColor);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
	r = 1.5f;
	
    glBegin(GL_QUADS);
    for(phi=0; phi <= 360; phi+=20.0)
    {
        for(theta=0; theta<=360*twists; theta+=20.0)
        {
            v=(phi/180.0f*3.142f);
            u=(theta/180.0f*3.142f);
			
            x=float(cos(u)*(2.0f+cos(v) ))*r;
            y=float(sin(u)*(2.0f+cos(v) ))*r;
            z=float((( u-(2.0f*3.142f)) + sin(v) ) * r);
			
            vertexes[0][0]=x;
            vertexes[0][1]=y;
            vertexes[0][2]=z;
			
            v=(phi/180.0f*3.142f);
            u=((theta+20)/180.0f*3.142f);
			
            x=float(cos(u)*(2.0f+cos(v) ))*r;
            y=float(sin(u)*(2.0f+cos(v) ))*r;
            z=float((( u-(2.0f*3.142f)) + sin(v) ) * r);
			
            vertexes[1][0]=x;
            vertexes[1][1]=y;
            vertexes[1][2]=z;
			
            v=((phi+20)/180.0f*3.142f);
            u=((theta+20)/180.0f*3.142f);
			
            x=float(cos(u)*(2.0f+cos(v) ))*r;
            y=float(sin(u)*(2.0f+cos(v) ))*r;
            z=float((( u-(2.0f*3.142f)) + sin(v) ) * r);
			
            vertexes[2][0]=x;
            vertexes[2][1]=y;
            vertexes[2][2]=z;
			
            v=((phi+20)/180.0f*3.142f);
            u=((theta)/180.0f*3.142f);
			
            x=float(cos(u)*(2.0f+cos(v) ))*r;
            y=float(sin(u)*(2.0f+cos(v) ))*r;
            z=float((( u-(2.0f*3.142f)) + sin(v) ) * r);
			
            vertexes[3][0]=x;
            vertexes[3][1]=y;
            vertexes[3][2]=z;
			
            calcNormal(vertexes,normal);
			
            glNormal3f(normal[0],normal[1],normal[2]);
			
            glVertex3f(vertexes[0][0],vertexes[0][1],vertexes[0][2]);
            glVertex3f(vertexes[1][0],vertexes[1][1],vertexes[1][2]);
            glVertex3f(vertexes[2][0],vertexes[2][1],vertexes[2][2]);
            glVertex3f(vertexes[3][0],vertexes[3][1],vertexes[3][2]);
        }
    }
    glEnd();
	
    glPopMatrix();
}

// Set Up An Ortho View
void NEHE36::ViewOrtho(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, 640 , 480 , 0, -1, 1 );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

// Set Up A Perspective View
void NEHE36::ViewPerspective(){
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}

void NEHE36::RenderToTexture(){
    glViewport(0,0,128,128);
	
    ProcessHelix();
	
    glBindTexture(GL_TEXTURE_2D,BlurTexture);
	
    // Copy Our ViewPort To The Blur Texture (From 0,0 To 128,128... No Border)
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, 128, 128, 0);
	
    glClearColor(0.0f, 0.0f, 0.5f, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glViewport(0 , 0,640 ,480);
}

void NEHE36::DrawBlur(int times, float inc){
    float spost = 0.0f;
    float alphainc = 0.9f / times;
    float alpha = 0.2f;
	
    // Disable AutoTexture Coordinates
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
	
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,BlurTexture);
    ViewOrtho();
	
    alphainc = alpha / times;
	glBegin(GL_QUADS);
	for (int num = 0;num < times;num++){
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		glTexCoord2f(0+spost,1-spost);
		glVertex2f(0,0);
		
		glTexCoord2f(0+spost,0+spost);
		glVertex2f(0,480);
		
		glTexCoord2f(1-spost,0+spost);
		glVertex2f(640,480);
		
		glTexCoord2f(1-spost,1-spost);
		glVertex2f(640,0);
		
		spost += inc;
		alpha = alpha - alphainc;
	}
    glEnd();
	
    ViewPerspective();
	
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
}

GLvoid NEHE36::InitGL(){
	
	BlurTexture = EmptyTexture();
	
	glEnable(GL_DEPTH_TEST);
	
	GLfloat global_ambient[4]={0.2f, 0.2f,  0.2f, 1.0f};
	GLfloat light0pos[4]=     {0.0f, 5.0f, 10.0f, 1.0f};
	GLfloat light0ambient[4]= {0.2f, 0.2f,  0.2f, 1.0f};
	GLfloat light0diffuse[4]= {0.3f, 0.3f,  0.3f, 1.0f};
	GLfloat light0specular[4]={0.8f, 0.8f,  0.8f, 1.0f};
	
	GLfloat lmodel_ambient[]= {0.2f,0.2f,0.2f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glShadeModel(GL_SMOOTH);
	
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5);
	
}


GLvoid NEHE36::DrawGLScene(){
	
    glClearColor(0.0f, 0.0f, 0.0f, 0.5);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	RenderToTexture();
	ProcessHelix();
	DrawBlur(25,0.02f);
	
	
	//draw FPS text
	GLint matrixMode;
	GLboolean lightingOn;
	lightingOn= glIsEnabled(GL_LIGHTING);
	if (lightingOn) glDisable(GL_LIGHTING);
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);
	computeFPS();
	Utils::drawText(-0.98f,-0.98f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	if (lightingOn) glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers();
	
	angle += 0.5f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE36::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE36::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE36::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE36::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE36::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE36::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
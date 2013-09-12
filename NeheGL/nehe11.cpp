//
//  nehe11.cpp
//  NeheGL
//
//  Created by Andong Li on 9/11/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe11.h"

const char* NEHE11::TITLE = "NEHE11";

GLfloat NEHE11::sleepTime = 0.0f;
int NEHE11::frameCounter = 0;
int NEHE11::currentTime = 0;
int NEHE11::lastTime = 0;
char NEHE11::FPSstr[15]  = "Calculating...";

GLuint NEHE11::texture[1] = {0};
GLfloat NEHE11::xrot = 0.0f;
GLfloat NEHE11::yrot = 0.0f;
GLfloat NEHE11::zrot = 0.0f;

float NEHE11::points[45][45][3] = {0};
int NEHE11::wiggle_count = 0;
GLfloat NEHE11::hold = 0.0f;

GLvoid NEHE11::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE11::LoadGLTextures(const char* dir){
	
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

GLvoid NEHE11::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/tim.png")){
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
	
	
	glPolygonMode( GL_BACK, GL_FILL );	// Back Face Is Filled In
	glPolygonMode( GL_FRONT, GL_LINE );	// Front Face Is Drawn With Lines
	
	// Loop Through The X Plane
	for(int x=0; x<45; x++)
	{
		// Loop Through The Y Plane
		for(int y=0; y<45; y++)
		{
			// Apply The Wave To Our Mesh
			points[x][y][0]=float((x/5.0f)-4.5f);
			points[x][y][1]=float((y/5.0f)-4.5f);
			points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}
	
}

GLvoid NEHE11::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int x, y;
    float float_x, float_y, float_xb, float_yb;
	
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-12.0f);
	
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;
			glTexCoord2f( float_x, float_y);    // Bottom Left
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );
			
			glTexCoord2f( float_x, float_yb );  // Top Left
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );
			
			glTexCoord2f( float_xb, float_yb ); // Top Right
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );
			
			glTexCoord2f( float_xb, float_y );  // Bottom Right
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();
	
	if( wiggle_count == 2 ){
		for( y = 0; y < 45; y++ ){
			hold=points[0][y][2];
			for( x = 0; x < 44; x++){
				// Current Wave Value Equals Value To The Right
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;
		}
		wiggle_count = 0;
	}
	wiggle_count++;
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
	xrot+=0.3f;
    yrot+=0.2f;
    zrot+=0.4f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE11::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

void NEHE11::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

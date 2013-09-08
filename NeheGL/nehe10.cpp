//
//  nehe10.cpp
//  NeheGL
//
//  Created by Andong Li on 9/7/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe10.h"

const char* NEHE10::TITLE = "NEHE10";

GLfloat NEHE10::sleepTime = 0.0f;
int NEHE10::frameCounter = 0;
int NEHE10::currentTime = 0;
int NEHE10::lastTime = 0;
char NEHE10::FPSstr[15]  = "Calculating...";

const GLfloat NEHE10::piover180 = 0.0174532925f;
GLfloat NEHE10::walkbias = 0.0f;
GLfloat NEHE10::walkbiasangle = 0.0f;
GLfloat NEHE10::heading = 0.0f;
GLfloat NEHE10::xpos = 0.0f;
GLfloat NEHE10::zpos = 0.0f;
GLfloat NEHE10::yrot = 0.0f;
GLfloat NEHE10::lookupdown = 0.0f;
GLfloat	NEHE10::z = 0.0f;

GLuint NEHE10::texture[3] = {0,0,0};
GLuint NEHE10::filter = 0;	//use first filter as default
bool NEHE10::bp = false;
bool NEHE10::fp = false;
bool NEHE10::blend = false;

bool NEHE10::keys[256] = {};	//all set to false
bool NEHE10::specialKeys[256] = {};	//all set to false

Sector NEHE10::sector;	// just for declare

GLvoid NEHE10::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE10::LoadGLTextures(const char* dir){
	
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

bool NEHE10::setupWorld(const char* fileDir){
	
	FILE *filein;
    filein = fopen(Utils::getAbsoluteDir(fileDir), "rt");
	// fail to load file, just return
	if(filein == NULL){
		return false;
	}
	
	int numtriangles;
	char oneline[255];	// represent one line
	float x, y, z, u, v;
	
	readstr(filein,oneline);	//read first line
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);
	
	sector.triangles = new Triangle[numtriangles];
	sector.numTriangles = numtriangles;
	
	// loop all triangles
	for(int triloop = 0; triloop < numtriangles; triloop++){
		// loop all vertices in a triangle
		for (int vertloop = 0; vertloop < 3; vertloop++){
			readstr(filein,oneline);	//read a new line
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector.triangles[triloop].vertex[vertloop].x = x;
			sector.triangles[triloop].vertex[vertloop].y = y;
			sector.triangles[triloop].vertex[vertloop].z = z;
			sector.triangles[triloop].vertex[vertloop].u = u;
			sector.triangles[triloop].vertex[vertloop].v = v;
		}
	}
	
	fclose(filein);
	return true;
}

void NEHE10::readstr(FILE *f,char *string){
	do{
        fgets(string, 255, f);
    }while((string[0] == '/') || (string[0] == '\n') || (string[0] == '\r'));
    return;
}

GLvoid NEHE10::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/mud.png")){
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
	
	glColor4f(1.0f,1.0f,1.0f,0.5f);	// Full Brightness, 50% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Blending Function
	
	// load the world by reading from file
	if(!setupWorld("NeheGL/model/world.txt")){
		cout<<"Fail to load world file, and setup"<<endl;
	}
}

GLvoid NEHE10::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-0.25f;
	GLfloat sceneroty = 360.0f - yrot;
	
	int numtriangles;
	
	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	
	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[filter]);
	
	numtriangles = sector.numTriangles;
	
	
	// Process Each Triangle
	for (int loop_m = 0; loop_m < numtriangles; loop_m++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		x_m = sector.triangles[loop_m].vertex[0].x;
		y_m = sector.triangles[loop_m].vertex[0].y;
		z_m = sector.triangles[loop_m].vertex[0].z;
		u_m = -sector.triangles[loop_m].vertex[0].u;
		v_m = -sector.triangles[loop_m].vertex[0].v;
		glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
		
		x_m = sector.triangles[loop_m].vertex[1].x;
		y_m = sector.triangles[loop_m].vertex[1].y;
		z_m = sector.triangles[loop_m].vertex[1].z;
		u_m = -sector.triangles[loop_m].vertex[1].u;
		v_m = -sector.triangles[loop_m].vertex[1].v;
		glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
		
		x_m = sector.triangles[loop_m].vertex[2].x;
		y_m = sector.triangles[loop_m].vertex[2].y;
		z_m = sector.triangles[loop_m].vertex[2].z;
		u_m = -sector.triangles[loop_m].vertex[2].u;
		v_m = -sector.triangles[loop_m].vertex[2].v;
		glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
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
	
	glutSwapBuffers();
	
	
	if(keys['b'] && !bp){
		bp=TRUE;
		blend = !blend;
		if(blend){
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}
		else{
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
	}
	if(!keys['b']){
		bp=FALSE;
	}
	
	if(keys['f'] && !fp){
		fp=TRUE;
		filter = (filter+ 1) % 3;
	}
	if(!keys['f']){
		fp=FALSE;
	}
	
	// handle key actions
	if(specialKeys[GLUT_KEY_PAGE_UP]){
		z -= 0.02f;
		lookupdown -= 1.0f;
	}
	if(specialKeys[GLUT_KEY_PAGE_DOWN]){
		z += 0.02f;
		lookupdown += 1.0f;
	}
	
	if (specialKeys[GLUT_KEY_UP]){
		xpos -= (float)sin(heading*piover180) * 0.05f;
		zpos -= (float)cos(heading*piover180) * 0.05f;
		if (walkbiasangle >= 359.0f)
		{
			walkbiasangle = 0.0f;
		}
		else
		{
			walkbiasangle+= 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
	}
	if (specialKeys[GLUT_KEY_DOWN]){
		xpos += (float)sin(heading*piover180) * 0.05f;
		zpos += (float)cos(heading*piover180) * 0.05f;
		if (walkbiasangle <= 1.0f)
		{
			walkbiasangle = 359.0f;
		}
		else
		{
			walkbiasangle-= 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
	}
	if (specialKeys[GLUT_KEY_LEFT]){
		heading += 1.0f;
		yrot = heading;
	}
	if (specialKeys[GLUT_KEY_RIGHT]){
		heading -= 1.0f;
		yrot = heading;
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE10::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE10::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE10::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE10::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE10::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE10::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}

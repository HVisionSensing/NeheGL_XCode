//
//  nehe25.cpp
//  NeheGL
//
//  Created by Andong Li on 10/5/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe25.h"

const char* NEHE25::TITLE = "NEHE25";

GLfloat NEHE25::sleepTime = 0.0f;
int NEHE25::frameCounter = 0;
int NEHE25::currentTime = 0;
int NEHE25::lastTime = 0;
char NEHE25::FPSstr[15]  = "Calculating...";

GLfloat NEHE25::xrot = 0.0f;
GLfloat NEHE25::yrot = 0.0f;
GLfloat NEHE25::zrot = 0.0f;
GLfloat NEHE25::xspeed = 0.0f;
GLfloat NEHE25::yspeed = 0.0f;
GLfloat NEHE25::zspeed = 0.0f;
GLfloat NEHE25::cx = 0.0f;
GLfloat NEHE25::cy = 0.0f;
GLfloat NEHE25::cz = -15.0f;

int NEHE25::maxver = 0;
OBJECT NEHE25::morph1;
OBJECT NEHE25::morph2;
OBJECT NEHE25::morph3;
OBJECT NEHE25::morph4;
OBJECT NEHE25::helper;
OBJECT* NEHE25::sour = NULL;
OBJECT* NEHE25::dest = NULL;

int NEHE25::key = 1;
int NEHE25::step = 0;
int NEHE25::steps = 200;
bool NEHE25::morph = false;


bool NEHE25::keys[256] = {};	//all set to false
bool NEHE25::specialKeys[256] = {};	//all set to false

GLvoid NEHE25::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE25::LoadGLTextures(const char* dir){
	
	return true;
}

// Allocate Memory For Each Object
void NEHE25::objallocate(OBJECT *k,int n){
    k->points=(VERTEX*)malloc(sizeof(VERTEX)*n);
}

// Frees The Object (Releasing The Memory)
void NEHE25::objfree(OBJECT *k){
    free(k->points);
}

void NEHE25::readstr(FILE *f,char *string){
	do{
        fgets(string, 255, f);	// Gets A String Of 255 Chars Max From f (File)
    } while ((string[0] == '/') || (string[0] == '\n'));	// Read Again If Line Has Comment Or Is Blank
}

// Loads Object From File (name)
void NEHE25::objload(const char *name,OBJECT *k)
{
    int ver;	// Will Hold Vertice Count
    float rx,ry,rz;	// Hold Vertex X, Y & Z Position
    FILE *filein;	// Filename To Open
    char oneline[255];	// Holds One Line Of Text (255 Chars Max)
	
    filein = fopen(Utils::getAbsoluteDir(name), "rt");	// Opens The File For Reading Text In Translated Mode
	if(filein == NULL){
		cout<<"fail to load model!"<<endl;
	}
	// CTRL Z Symbolizes End Of File In Translated Mode
    readstr(filein,oneline);	// Jumps To Code That Reads One Line Of Text From The File
    sscanf(oneline, "Vertices: %d\n", &ver);	// Scans Text For "Vertices: ".  Number After Is Stored In ver
    k->verts=ver;	// Sets Objects verts Variable To Equal The Value Of ver
    objallocate(k,ver);	// Jumps To Code That Allocates Ram To Hold The Object
	
	for (int i=0;i<ver;i++)	// Loops Through The Vertices
	{
		readstr(filein,oneline);	// Reads In The Next Line Of Text
		sscanf(oneline, "%f %f %f", &rx, &ry, &rz);	// Searches For 3 Floating Point Numbers, Store In rx,ry & rz
		k->points[i].x = rx;	// Sets Objects (k) points.x Value To rx
        k->points[i].y = ry;	// Sets Objects (k) points.y Value To ry
        k->points[i].z = rz;	// Sets Objects (k) points.z Value To rz
    }
    fclose(filein);	// Close The File
	
    if(ver>maxver) maxver=ver;	// If ver Is Greater Than maxver Set maxver Equal To ver
}

// Calculates Movement Of Points During Morphing
VERTEX NEHE25::calculate(int i)
{
    VERTEX a;	// Temporary Vertex Called a
    a.x=(sour->points[i].x-dest->points[i].x)/steps;	// a.x Value Equals Source x - Destination x Divided By Steps
    a.y=(sour->points[i].y-dest->points[i].y)/steps;	// a.y Value Equals Source y - Destination y Divided By Steps
    a.z=(sour->points[i].z-dest->points[i].z)/steps;	// a.z Value Equals Source z - Destination z Divided By Steps
    return a;
}

GLvoid NEHE25::InitGL(){
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// Set The Blending Function For Translucency
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
    glClearDepth(1.0);	// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);	// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
    glShadeModel(GL_SMOOTH);	// Enables Smooth Color Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	objload("NeheGL/model/sphere.txt",&morph1);	// Load The First Object Into morph1 From File sphere.txt
	objload("NeheGL/model/torus.txt",&morph2);	// Load The Second Object Into morph2 From File torus.txt
	objload("NeheGL/model/tube.txt",&morph3);	// Load The Third Object Into morph3 From File tube.txt
	objallocate(&morph4,486);	// Manually Reserver Ram For A 4th 468 Vertice Object (morph4)
	for(int i=0;i<486;i++)	// Loop Through All 468 Vertices
	{
		morph4.points[i].x=((float)(rand()%14000)/1000)-7;	//morph4 x Point Becomes A Random Float Value From -7 to 7
		morph4.points[i].y=((float)(rand()%14000)/1000)-7;	//morph4 y Point Becomes A Random Float Value From -7 to 7
		morph4.points[i].z=((float)(rand()%14000)/1000)-7;	//morph4 z Point Becomes A Random Float Value From -7 to 7
	}
	
	objload("NeheGL/model/sphere.txt",&helper);	// Load sphere.txt Object Into Helper (Used As Starting Point)
    sour=dest=&morph1;	// Source & Destination Are Set To Equal First Object (morph1)
	
}


GLvoid NEHE25::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();	// Reset The View
    glTranslatef(cx,cy,cz);	// Translate To The Current Position
    glRotatef(xrot,1,0,0);	// Rotate On The X Axis By xrot
    glRotatef(yrot,0,1,0);	// Rotate On The Y Axis By yrot
    glRotatef(zrot,0,0,1);	// Rotate On The Z Axis By zrot
	
    xrot+=xspeed;
	yrot+=yspeed;
	zrot+=zspeed;
	
    GLfloat tx,ty,tz;                               // Temp X, Y & Z Variables
    VERTEX q;                                   // Holds Returned Calculated Values For One Vertex
	
	glBegin(GL_POINTS);
    for(int i=0;i<morph1.verts;i++)                      // Loop Through All The Verts Of morph1 (All Objects Have
    {                                   // The Same Amount Of Verts For Simplicity, Could Use maxver Also)
        if(morph) q=calculate(i); else q.x=q.y=q.z=0;           // If morph Is True Calculate Movement Otherwise Movement=0
        helper.points[i].x-=q.x;                    // Subtract q.x Units From helper.points[i].x (Move On X Axis)
        helper.points[i].y-=q.y;                    // Subtract q.y Units From helper.points[i].y (Move On Y Axis)
        helper.points[i].z-=q.z;                    // Subtract q.z Units From helper.points[i].z (Move On Z Axis)
        tx=helper.points[i].x;                      // Make Temp X Variable Equal To Helper's X Variable
        ty=helper.points[i].y;                      // Make Temp Y Variable Equal To Helper's Y Variable
        tz=helper.points[i].z;                      // Make Temp Z Variable Equal To Helper's Z Variable
		glColor3f(0,1,1);                       // Set Color To A Bright Shade Of Off Blue
        glVertex3f(tx,ty,tz);                       // Draw A Point At The Current Temp Values (Vertex)
        glColor3f(0,0.5f,1);                        // Darken Color A Bit
        tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;                // Calculate Two Positions Ahead
        glVertex3f(tx,ty,tz);                       // Draw A Second Point At The Newly Calculate Position
        glColor3f(0,0,1);                       // Set Color To A Very Dark Blue
        tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;                // Calculate Two More Positions Ahead
        glVertex3f(tx,ty,tz);                       // Draw A Third Point At The Second New Position
    }                                   // This Creates A Ghostly Tail As Points Move
	glEnd();
	
	// If We're Morphing And We Haven't Gone Through All 200 Steps Increase Our Step Counter
    // Otherwise Set Morphing To False, Make Source=Destination And Set The Step Counter Back To Zero.
    if(morph && step<=steps){
		step++;
	}
	else{
		morph=FALSE;
		sour=dest;
		step=0;
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
	
	//handle keyboard input
	if(specialKeys[GLUT_KEY_PAGE_UP])	zspeed+=0.01f;
	if(specialKeys[GLUT_KEY_PAGE_DOWN])	zspeed-=0.01f;
	if(specialKeys[GLUT_KEY_DOWN])	xspeed+=0.01f;
	if(specialKeys[GLUT_KEY_UP])	xspeed-=0.01f;
	if(specialKeys[GLUT_KEY_RIGHT])	yspeed+=0.01f;
	if(specialKeys[GLUT_KEY_LEFT])	yspeed-=0.01f;
	
	if(keys['q'])	cz-=0.01f;
	if(keys['z'])	cz+=0.01f;
	if(keys['w'])	cy+=0.01f;
	if(keys['s'])	cy-=0.01f;
	if(keys['d'])	cx+=0.01f;
	if(keys['a'])	cx-=0.01f;
	
	if (keys['1'] && (key!=1) && !morph){
		key=1;                      // Sets key To 1 (To Prevent Pressing 1 2x In A Row)
		morph=TRUE;                 // Set morph To True (Starts Morphing Process)
		dest=&morph1;                   // Destination Object To Morph To Becomes morph1
	}
	if (keys['2'] && (key!=2) && !morph){
		key=2;                      // Sets key To 2 (To Prevent Pressing 2 2x In A Row)
		morph=TRUE;                 // Set morph To True (Starts Morphing Process)
		dest=&morph2;                   // Destination Object To Morph To Becomes morph2
	}
	if (keys['3'] && (key!=3) && !morph){
		key=3;                      // Sets key To 3 (To Prevent Pressing 3 2x In A Row)
		morph=TRUE;                 // Set morph To True (Starts Morphing Process)
		dest=&morph3;                   // Destination Object To Morph To Becomes morph3
	}
	if (keys['4'] && (key!=4) && !morph){
		key=4;                      // Sets key To 4 (To Prevent Pressing 4 2x In A Row)
		morph=TRUE;                 // Set morph To True (Starts Morphing Process)
		dest=&morph4;                   // Destination Object To Morph To Becomes morph4
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE25::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE25::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE25::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE25::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE25::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE25::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
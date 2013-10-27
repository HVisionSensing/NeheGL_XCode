//
//  nehe28.cpp
//  NeheGL
//
//  Created by Andong Li on 10/7/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe28.h"

const char* NEHE28::TITLE = "NEHE28";

GLfloat NEHE28::sleepTime = 0.0f;
int NEHE28::frameCounter = 0;
int NEHE28::currentTime = 0;
int NEHE28::lastTime = 0;
char NEHE28::FPSstr[15]  = "Calculating...";

GLfloat NEHE28::rotz = 0.0f;
BEZIER_PATCH NEHE28::mybezier;	// just for declaration
bool NEHE28::showCPoints=true;
int NEHE28::divs = 7;

bool NEHE28::keys[256] = {};	//all set to false
bool NEHE28::specialKeys[256] = {};	//all set to false

GLvoid NEHE28::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE28::LoadGLTextures(const char* dir, GLuint* texPntr){
	
	glGenTextures(1, texPntr);
	
	/* load an image file directly as a new OpenGL texture */
    *texPntr = SOIL_load_OGL_texture
	(
	 Utils::getAbsoluteDir(dir),
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 SOIL_FLAG_INVERT_Y
	 );
	
    if(*texPntr == 0){
        return false;
	}
	
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, *texPntr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
    return true;
}

// Adds 2 Points. Don't Just Use '+' ;)
POINT_3D NEHE28::pointAdd(POINT_3D p, POINT_3D q){
    p.x += q.x;
	p.y += q.y;
	p.z += q.z;
    return p;
}

// Multiplies A Point And A Constant. Don't Just Use '*'
POINT_3D NEHE28::pointTimes(double c, POINT_3D p){
    p.x *= c;
	p.y *= c;
	p.z *= c;
    return p;
}

// Function For Quick Point Creation
POINT_3D NEHE28::makePoint(double a, double b, double c){
    POINT_3D p;
    p.x = a;
	p.y = b;
	p.z = c;
    return p;
}

// Calculates 3rd Degree Polynomial Based On Array Of 4 Points
// And A Single Variable (u) Which Is Generally Between 0 And 1
POINT_3D NEHE28::Bernstein(float u, POINT_3D *p){
    POINT_3D a, b, c, d, r;
	
    a = pointTimes(pow(u,3), p[0]);
    b = pointTimes(3*pow(u,2)*(1-u), p[1]);
    c = pointTimes(3*u*pow((1-u),2), p[2]);
    d = pointTimes(pow((1-u),3), p[3]);
	
    r = pointAdd(pointAdd(a, b), pointAdd(c, d));
	
    return r;
}

// Generates A Display List Based On The Data In The Patch
// And The Number Of Divisions
GLuint NEHE28::genBezier(BEZIER_PATCH patch, int divs){
    int u = 0;
	int v;
    float py;
	float px;
	float pyold;
    GLuint drawlist = glGenLists(1);	// Make The Display List
    POINT_3D temp[4];
    POINT_3D *last = (POINT_3D*)malloc(sizeof(POINT_3D)*(divs+1));
	// Array Of Points To Mark The First Line Of Polys
	
    if (patch.dlBPatch != 0)	// Get Rid Of Any Old Display Lists
        glDeleteLists(patch.dlBPatch, 1);
	
    temp[0] = patch.anchors[0][3];	// The First Derived Curve (Along X-Axis)
    temp[1] = patch.anchors[1][3];
    temp[2] = patch.anchors[2][3];
    temp[3] = patch.anchors[3][3];
	
    for (v=0;v<=divs;v++) {                      // Create The First Line Of Points
        px = ((float)v)/((float)divs);              // Percent Along Y-Axis
		// Use The 4 Points From The Derived Curve To Calculate The Points Along That Curve
        last[v] = Bernstein(px, temp);
    }
	
    glNewList(drawlist, GL_COMPILE);                // Start A New Display List
    glBindTexture(GL_TEXTURE_2D, patch.texture);            // Bind The Texture
	
    for (u=1;u<=divs;u++) {
        py    = ((float)u)/((float)divs);           // Percent Along Y-Axis
        pyold = ((float)u-1.0f)/((float)divs);          // Percent Along Old Y Axis
		
        temp[0] = Bernstein(py, patch.anchors[0]);      // Calculate New Bezier Points
        temp[1] = Bernstein(py, patch.anchors[1]);
        temp[2] = Bernstein(py, patch.anchors[2]);
        temp[3] = Bernstein(py, patch.anchors[3]);
		
        glBegin(GL_TRIANGLE_STRIP);             // Begin A New Triangle Strip
		
        for (v=0;v<=divs;v++) {
            px = ((float)v)/((float)divs);          // Percent Along The X-Axis
			
            glTexCoord2f(pyold, px);            // Apply The Old Texture Coords
            glVertex3d(last[v].x, last[v].y, last[v].z);    // Old Point
			
            last[v] = Bernstein(px, temp);          // Generate New Point
            glTexCoord2f(py, px);               // Apply The New Texture Coords
            glVertex3d(last[v].x, last[v].y, last[v].z);    // New Point
        }
		
        glEnd();                        // END The Triangle Strip
    }
	
    glEndList();                            // END The List
	
    free(last);                         // Free The Old Vertices Array
    return drawlist;                        // Return The Display List
}

void NEHE28::initBezier() {
    mybezier.anchors[0][0] = makePoint(-0.75,   -0.75,  -0.50); // Set The Bezier Vertices
    mybezier.anchors[0][1] = makePoint(-0.25,   -0.75,   0.00);
    mybezier.anchors[0][2] = makePoint( 0.25,   -0.75,   0.00);
    mybezier.anchors[0][3] = makePoint( 0.75,   -0.75,  -0.50);
    mybezier.anchors[1][0] = makePoint(-0.75,   -0.25,  -0.75);
    mybezier.anchors[1][1] = makePoint(-0.25,   -0.25,   0.50);
    mybezier.anchors[1][2] = makePoint( 0.25,   -0.25,   0.50);
    mybezier.anchors[1][3] = makePoint( 0.75,   -0.25,  -0.75);
    mybezier.anchors[2][0] = makePoint(-0.75,    0.25,   0.00);
    mybezier.anchors[2][1] = makePoint(-0.25,    0.25,  -0.50);
    mybezier.anchors[2][2] = makePoint( 0.25,    0.25,  -0.50);
    mybezier.anchors[2][3] = makePoint( 0.75,    0.25,   0.00);
    mybezier.anchors[3][0] = makePoint(-0.75,    0.75,  -0.50);
    mybezier.anchors[3][1] = makePoint(-0.25,    0.75,  -1.00);
    mybezier.anchors[3][2] = makePoint( 0.25,    0.75,  -1.00);
    mybezier.anchors[3][3] = makePoint( 0.75,    0.75,  -0.50);
    mybezier.dlBPatch = NULL;                   // Go Ahead And Initialize This To NULL
}

GLvoid NEHE28::InitGL(){
	
	//give the relative directory of image under current project folder
	if(!LoadGLTextures("NeheGL/img/NeHe.png", &(mybezier.texture))){
        cout<<"Fail to load textures"<<endl;
    }
	
	initBezier();
	mybezier.dlBPatch = genBezier(mybezier, divs);
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.05f, 0.05f, 0.05f, 0.5f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}


GLvoid NEHE28::DrawGLScene(){
	
	int i, j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();                       // Reset The Current Modelview Matrix
    glTranslatef(0.0f,0.0f,-4.0f);                  // Move Left 1.5 Units And Into The Screen 6.0
    glRotatef(-75.0f,1.0f,0.0f,0.0f);
    glRotatef(rotz,0.0f,0.0f,1.0f);                 // Rotate The Triangle On The Z-Axis
	
    glCallList(mybezier.dlBPatch);                  // Call The Bezier's Display List
	// This Need Only Be Updated When The Patch Changes
	
    if (showCPoints) {                      // If Drawing The Grid Is Toggled On
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f,0.0f,0.0f);
        for(i=0;i<4;i++) {                   // Draw The Horizontal Lines
            glBegin(GL_LINE_STRIP);
            for(j=0;j<4;j++)
                glVertex3d(mybezier.anchors[i][j].x, mybezier.anchors[i][j].y, mybezier.anchors[i][j].z);
            glEnd();
        }
        for(i=0;i<4;i++) {                   // Draw The Vertical Lines
            glBegin(GL_LINE_STRIP);
            for(j=0;j<4;j++)
                glVertex3d(mybezier.anchors[j][i].x, mybezier.anchors[j][i].y, mybezier.anchors[j][i].z);
            glEnd();
        }
        glColor3f(1.0f,1.0f,1.0f);
        glEnable(GL_TEXTURE_2D);
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
	
	// handle keyboard input
	if (specialKeys[GLUT_KEY_LEFT])  rotz -= 0.8f;       // Rotate Left ( NEW )
	if (specialKeys[GLUT_KEY_RIGHT]) rotz += 0.8f;       // Rotate Right ( NEW )
	if (specialKeys[GLUT_KEY_UP]) {              // Resolution Up ( NEW )
		divs++;
		mybezier.dlBPatch = genBezier(mybezier, divs);  // Update The Patch
		specialKeys[GLUT_KEY_UP] = FALSE;
	}
	if (specialKeys[GLUT_KEY_DOWN] && divs > 1) {     // Resolution Down ( NEW )
		divs--;
		mybezier.dlBPatch = genBezier(mybezier, divs);  // Update The Patch
		specialKeys[GLUT_KEY_DOWN] = FALSE;
	}
	if (keys[' ']) {
		showCPoints = !showCPoints;
		keys[' '] = FALSE;
	}
	
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE28::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE28::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE28::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE28::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE28::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE28::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
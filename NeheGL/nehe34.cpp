//
//  nehe34.cpp
//  NeheGL
//
//  Created by Andong Li on 10/17/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe34.h"


const char* NEHE34::TITLE = "NEHE34";

GLfloat NEHE34::sleepTime = 0.0f;
int NEHE34::frameCounter = 0;
int NEHE34::currentTime = 0;
int NEHE34::lastTime = 0;
char NEHE34::FPSstr[15]  = "Calculating...";

bool NEHE34::bRender = true;
BYTE NEHE34::g_HeightMap[MAP_SIZE*MAP_SIZE] = {0};
float NEHE34::scaleValue = 0.15f;

bool NEHE34::keys[256] = {};	//all set to false
bool NEHE34::specialKeys[256] = {};	//all set to false

GLvoid NEHE34::ReSizeGLScene(GLsizei width, GLsizei height){
	
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
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,500.0f);
	//gluPerspective(80.0f,(GLfloat)width/(GLfloat)height, 1.0, 5000.0f);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void NEHE34::LoadRawFile(const char* strName, int nSize, BYTE *pHeightMap){
	FILE *pFile = NULL;

	
    pFile = fopen(Utils::getAbsoluteDir(strName), "rb");

    if ( pFile == NULL ){
        return;
    }
	
	fread( pHeightMap, 1, nSize, pFile );
	
    int result = ferror( pFile );
	
    if (result){
        cout<<"Failed To Get Data!"<<endl;
    }

    fclose(pFile);
}

GLvoid NEHE34::InitGL(){
	
	
	// Enable Texture Mapping
	//glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	LoadRawFile("NeheGL/img/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);
	
}

int NEHE34::Height(BYTE *pHeightMap, int X, int Y){
	int x = X % MAP_SIZE;	// Error Check Our x Value
    int y = Y % MAP_SIZE;	// Error Check Our y Value
	
    if(!pHeightMap) return 0;	// Make Sure Our Data Is Valid
	return pHeightMap[x + (y * MAP_SIZE)];	// Index Into Our Height Array And Return The Height
}

// This Sets The Color Value For A Particular Index
void NEHE34::SetVertexColor(BYTE *pHeightMap, int x, int y){
    if(!pHeightMap) return;
	
    float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);
	
    // Assign This Blue Shade To The Current Vertex
    glColor3f(0.0f, 0.0f, fColor );
}

void NEHE34::RenderHeightMap(BYTE pHeightMap[]){
    int X = 0, Y = 0;
    int x, y, z;
	
    if(!pHeightMap) return;
	
	if(bRender)
		glBegin( GL_QUADS );
	else
		glBegin( GL_LINES );
	
	for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE )
		for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE )
		{
			x = X;
			y = Height(pHeightMap, X, Y );
			z = Y;
			
			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);
			
			x = X;
			y = Height(pHeightMap, X, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;
			
			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);
			
			x = X + STEP_SIZE;
			y = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;
			
			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);
			
			x = X + STEP_SIZE;
			y = Height(pHeightMap, X + STEP_SIZE, Y );
			z = Y;
			
			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);
		}
	glEnd();
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	// Reset The Color
}

GLvoid NEHE34::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(212, 60, 194,  186, 55, 171,  0, 1, 0);
	
	glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);
	RenderHeightMap(g_HeightMap);
	
	
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
	
	if (specialKeys[GLUT_KEY_UP]) {
		scaleValue += 0.001f;
	}
	if (specialKeys[GLUT_KEY_DOWN]) {
		scaleValue -= 0.001f;
	}
	if (keys['b']) {
		bRender = !bRender;
		keys['b'] = false;
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE34::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE34::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE34::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE34::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE34::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE34::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
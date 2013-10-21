//
//  nehe37.cpp
//  NeheGL
//
//  Created by Andong Li on 10/20/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe37.h"

const char* NEHE37::TITLE = "NEHE37";

GLfloat NEHE37::sleepTime = 0.0f;
int NEHE37::frameCounter = 0;
int NEHE37::currentTime = 0;
int NEHE37::lastTime = 0;
char NEHE37::FPSstr[15]  = "Calculating...";


bool NEHE37::outlineDraw = true;
bool NEHE37::outlineSmooth = false;
float NEHE37::outlineColor[3] = { 0.0f, 0.0f, 0.0f };
float NEHE37::outlineWidth = 3.0f;
VECTOR37 NEHE37::lightAngle;
bool NEHE37::lightRotate = false;
float NEHE37::modelAngle  = 0.0f;
bool NEHE37::modelRotate = false;
POLYGON37* NEHE37::polyData   = NULL;
int NEHE37::polyNum = 0;
GLuint NEHE37::shaderTexture[1];


bool NEHE37::keys[256] = {};	//all set to false
bool NEHE37::specialKeys[256] = {};	//all set to false

GLvoid NEHE37::ReSizeGLScene(GLsizei width, GLsizei height){
	
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


bool NEHE37::ReadMesh (){
	
    FILE *In = fopen (Utils::getAbsoluteDir("NeheGL/model/Model.txt"), "rb");
    if (!In)
        return FALSE;
    fread (&polyNum, sizeof (int), 1, In);
    polyData = new POLYGON37[polyNum];
    fread (&polyData[0], sizeof (POLYGON37) * polyNum, 1, In);
    fclose (In);
    return true;
}

float NEHE37::DotProduct (VECTOR37 &V1, VECTOR37 &V2){
    return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

float NEHE37::Magnitude (VECTOR37 &V){
    return sqrtf (V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

void NEHE37::Normalize (VECTOR37 &V){
    float M = Magnitude (V);
	
    if (M != 0.0f){
        V.X /= M;
        V.Y /= M;
        V.Z /= M;
    }
}

void NEHE37::RotateVector (MATRIX37 &M, VECTOR37 &V, VECTOR37 &D){
    D.X = (M.Data[0] * V.X) + (M.Data[4] * V.Y) + (M.Data[8]  * V.Z);
    D.Y = (M.Data[1] * V.X) + (M.Data[5] * V.Y) + (M.Data[9]  * V.Z);
    D.Z = (M.Data[2] * V.X) + (M.Data[6] * V.Y) + (M.Data[10] * V.Z);
}

GLvoid NEHE37::InitGL(){
	
	char Line[255];	// Storage For 255 Characters
	float shaderData[32][3];	// Storage For The 96 Shader Values
	FILE *In = NULL;
	
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glClearColor (0.7f, 0.7f, 0.7f, 0.0f);
	glClearDepth (1.0f);
	
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glDepthFunc (GL_LESS);
	
	glShadeModel (GL_SMOOTH);
	glDisable (GL_LINE_SMOOTH);
	glEnable (GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	
	In = fopen (Utils::getAbsoluteDir("NeheGL/model/Shader.txt"), "r");	// Open The Shader File
	
	if(In){
		for (int i = 0; i < 32; i++){
			if (feof (In))
				break;
			fgets (Line, 255, In);
			shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = atof (Line);
		}
		fclose (In);
	}
	else{
		cout<<"Fail to load shader!"<<endl;
	}
	
	glGenTextures (1, &shaderTexture[0]);
	glBindTexture (GL_TEXTURE_1D, shaderTexture[0]);
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage1D (GL_TEXTURE_1D, 0, GL_RGB, 32, 0, GL_RGB , GL_FLOAT, shaderData);
	lightAngle.X = 0.0f;
	lightAngle.Y = 0.0f;
	lightAngle.Z = 1.0f;
	Normalize (lightAngle);
	
	if(!ReadMesh ()){
		cout<<"Fail to load model!"<<endl;
	}
}


GLvoid NEHE37::DrawGLScene(){
	
	float TmpShade;
	MATRIX37 TmpMatrix;
	VECTOR37 TmpVector, TmpNormal;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// Check To See If We Want Anti-Aliased Lines
	if(outlineSmooth){
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable (GL_LINE_SMOOTH);
	}
	else{
		glDisable (GL_LINE_SMOOTH);
	}
	
	glTranslatef (0.0f, 0.0f, -2.0f);
	glRotatef (modelAngle, 0.0f, 1.0f, 0.0f);
	
	glGetFloatv (GL_MODELVIEW_MATRIX, TmpMatrix.Data);
	
	// Cel-Shading Code
	glEnable (GL_TEXTURE_1D);
	glBindTexture (GL_TEXTURE_1D, shaderTexture[0]);
	
	glColor3f (1.0f, 1.0f, 1.0f);
	
	glBegin (GL_TRIANGLES);
	
    for (int i = 0; i < polyNum; i++){
        for (int j = 0; j < 3; j++){
            TmpNormal.X = polyData[i].Verts[j].Nor.X;
            TmpNormal.Y = polyData[i].Verts[j].Nor.Y;
            TmpNormal.Z = polyData[i].Verts[j].Nor.Z;
			RotateVector (TmpMatrix, TmpNormal, TmpVector);
			Normalize (TmpVector);
			TmpShade = DotProduct (TmpVector, lightAngle);
			if (TmpShade < 0.0f)
				TmpShade = 0.0f;
			glTexCoord1f (TmpShade);
            glVertex3fv (&polyData[i].Verts[j].Pos.X);
        }
    }
	
	glEnd ();
	glDisable (GL_TEXTURE_1D);
	
	
	// Outline Code
	if (outlineDraw){
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
		
		glPolygonMode (GL_BACK, GL_LINE);
		glLineWidth (outlineWidth);
		
		glCullFace (GL_FRONT);
		
		glDepthFunc (GL_LEQUAL);
		
		glColor3fv (&outlineColor[0]);
		
		glBegin (GL_TRIANGLES);
		
        for (int i = 0; i < polyNum; i++){
            for (int j = 0; j < 3; j++){
                glVertex3fv (&polyData[i].Verts[j].Pos.X);
            }
        }
		
		glEnd ();
		glDepthFunc (GL_LESS);
        glCullFace (GL_BACK);
        glPolygonMode (GL_BACK, GL_FILL);
		
        glDisable (GL_BLEND);
    }
	
	/*
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	*/
	 
	glutSwapBuffers();
	
	//handle key input
    if(specialKeys[GLUT_KEY_DOWN]){
		outlineWidth--;
        specialKeys[GLUT_KEY_DOWN] = FALSE;
	}
    if(specialKeys[GLUT_KEY_UP]){
		outlineWidth++;
        specialKeys[GLUT_KEY_UP] = FALSE;
	}
	
    if(keys[' ']){
		modelRotate = !modelRotate;	// Toggle Model Rotation On/Off
        keys[' '] = false;
	}
    if(keys['1']){
		outlineDraw = !outlineDraw;	// Toggle Outline Drawing On/Off
        keys['1'] = false;
	}
	
	if(keys['2']){
		outlineSmooth = !outlineSmooth;	// Toggle Anti-Aliasing On/Off
        keys['2'] = false;
	}
	
	if (modelRotate)
        modelAngle += 1.0f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE37::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE37::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE37::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE37::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE37::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE37::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
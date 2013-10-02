//
//  nehe22.cpp
//  NeheGL
//
//  Created by Andong Li on 9/29/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe22.h"


const char* NEHE22::TITLE = "NEHE22";

GLfloat NEHE22::sleepTime = 0.0f;
int NEHE22::frameCounter = 0;
int NEHE22::currentTime = 0;
int NEHE22::lastTime = 0;
char NEHE22::FPSstr[15]  = "Calculating...";

bool NEHE22::multitextureSupported=false;
bool NEHE22::useMultitexture=true;
GLint NEHE22::maxTexelUnits=1;

#ifdef _WIN32
	PFNGLMULTITEXCOORD1FARBPROC NEHE22::glMultiTexCoord1fARB = NULL;
	PFNGLMULTITEXCOORD2FARBPROC NEHE22::glMultiTexCoord2fARB = NULL;
	PFNGLMULTITEXCOORD3FARBPROC NEHE22::glMultiTexCoord3fARB = NULL;
	PFNGLMULTITEXCOORD4FARBPROC NEHE22::glMultiTexCoord4fARB = NULL;
	PFNGLACTIVETEXTUREARBPROC   NEHE22::glActiveTextureARB = NULL;
	PFNGLCLIENTACTIVETEXTUREARBPROC NEHE22::glClientActiveTextureARB = NULL;
#endif

GLuint NEHE22::filter=1;
GLuint NEHE22::texture[3] = {0};
GLuint NEHE22::bump[3] = {0};
GLuint NEHE22::invbump[3] = {0};
GLuint NEHE22::glLogo = 0;
GLuint NEHE22::multiLogo = 0;
GLfloat NEHE22::LightAmbient[] = { 0.2f, 0.2f, 0.2f};
GLfloat NEHE22::LightDiffuse[] = { 1.0f, 1.0f, 1.0f};
GLfloat NEHE22::LightPosition[] = { 0.0f, 0.0f, 2.0f};
GLfloat NEHE22::Gray[] = { 0.5f, 0.5f, 0.5f, 1.0f};
GLfloat NEHE22::data[] = {
    // FRONT FACE
    0.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    1.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, +1.0f,
    0.0f, 1.0f,     -1.0f, +1.0f, +1.0f,
    // BACK FACE
    1.0f, 0.0f,     -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f,     -1.0f, +1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, -1.0f,
    // Top Face
    0.0f, 1.0f,     -1.0f, +1.0f, -1.0f,
    0.0f, 0.0f,     -1.0f, +1.0f, +1.0f,
    1.0f, 0.0f,     +1.0f, +1.0f, +1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    // Bottom Face
    1.0f, 1.0f,     -1.0f, -1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, -1.0f, -1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    1.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    // Right Face
    1.0f, 0.0f,     +1.0f, -1.0f, -1.0f,
    1.0f, 1.0f,     +1.0f, +1.0f, -1.0f,
    0.0f, 1.0f,     +1.0f, +1.0f, +1.0f,
    0.0f, 0.0f,     +1.0f, -1.0f, +1.0f,
    // Left Face
    0.0f, 0.0f,     -1.0f, -1.0f, -1.0f,
    1.0f, 0.0f,     -1.0f, -1.0f, +1.0f,
    1.0f, 1.0f,     -1.0f, +1.0f, +1.0f,
    0.0f, 1.0f,     -1.0f, +1.0f, -1.0f
};

bool NEHE22::keys[256] = {};	//all set to false
bool NEHE22::specialKeys[256] = {};	//all set to false

bool NEHE22::isInString(char *string, const char *search) {
    int pos=0;
    size_t maxpos=strlen(search)-1;
    size_t len=strlen(string);
    char *other;
    for (int i=0; i<len; i++) {
        if ((i==0) || ((i>1) && string[i-1]=='\n')) {            // New Extension Begins Here!
            other=&string[i];
            pos=0;                          // Begin New Search
            while (string[i]!='\n') {               // Search Whole Extension-String
                if (string[i]==search[pos]) pos++;      // Next Position
                if ((pos>maxpos) && string[i+1]=='\n') return true;  // We Have A Winner!
                i++;
            }
        }
    }
    return false;                               // Sorry, Not Found!
}

bool NEHE22::initMultitexture() {
    char *extensions;
    extensions=strdup((char *) glGetString(GL_EXTENSIONS));	// Fetch Extension String
    size_t len=strlen(extensions);
    for (int i=0; i<len; i++)	// Separate It By Newline Instead Of Blank
        if (extensions[i]==' ') extensions[i]='\n';
	
    if (isInString(extensions,"GL_ARB_multitexture")	// Is Multitexturing Supported?
        && __ARB_ENABLE	// Override Flag
        //&& isInString(extensions,"GL_EXT_texture_env_combine")	// texture-environment-combining supported?
		&& isInString(extensions,"GL_ARB_texture_env_combine")	// texture-environment-combining supported?
		){
        glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
#ifdef _WIN32
        glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
        glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
        glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
        glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
        glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
        glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
#endif
        return true;
    }
	
    useMultitexture=false;	// We Can't Use It If It Isn't Supported!
    return false;
}

void NEHE22::initLights() {
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	// Load Light-Parameters into GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

GLvoid NEHE22::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE22::LoadGLTextures(){
    char *alpha=NULL;
	
	int imageWidth, imageHeight;
	unsigned char* Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/Base.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}
	
	glGenTextures(3, &texture[0]);	// Create Three Textures
	
	// Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, Image);

	//free loaded texture image
	SOIL_free_image_data(Image);
	Image = NULL;
	
	// create bump texture
	Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/Bump.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}

	glPixelTransferf(GL_RED_SCALE,0.5f);	// Scale RGB By 50%, So That We Have Only
    glPixelTransferf(GL_GREEN_SCALE,0.5f);	// Half Intenstity
    glPixelTransferf(GL_BLUE_SCALE,0.5f);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);	// No Wrapping, Please!
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glGenTextures(3, &bump[0]);	// Create Three Textures
	
	// Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	
	//create invert bump
	for (int i=0; i<3*imageWidth*imageHeight; i++){
		// Invert The Bumpmap
        Image[i] = 255 - Image[i];
	}
	
	glGenTextures(3, &invbump[0]);	// Create Three Textures
	
	// Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	//free loaded texture image
	SOIL_free_image_data(Image);
	Image = NULL;
	
	
	// create OpenGL logo texture
	int a;
	Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/OpenGL_ALPHA.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}
	alpha=new char[4*imageWidth*imageHeight];
	// Create Memory For RGBA8-Texture
	for (a=0; a < imageWidth * imageHeight; a++){
		// Pick Only Red Value As Alpha!
		alpha[4*a+3]=Image[a*3];
	}
	Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/OpenGL.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}
	for (a=0; a<imageWidth*imageHeight; a++) {
		alpha[4*a]=Image[a*3];		// R
		alpha[4*a+1]=Image[a*3+1];	// G
		alpha[4*a+2]=Image[a*3+2];	// B
	}
	
	glGenTextures(1, &glLogo);
	// Create Linear Filtered RGBA8-Texture
	glBindTexture(GL_TEXTURE_2D, glLogo);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);
	delete[] alpha;
	alpha = NULL;
	SOIL_free_image_data(Image);
	Image = NULL;
	
	// create text logo texture
	Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/Multi_On_Alpha.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}
	alpha=new char[4*imageWidth*imageHeight];
	// Create Memory For RGBA8-Texture
	for (a=0; a < imageWidth * imageHeight; a++){
		// Pick Only Red Value As Alpha!
		alpha[4*a+3]=Image[a*3];
	}
	Image = SOIL_load_image(Utils::getAbsoluteDir("NeheGL/img/Multi_On.bmp"),&imageWidth,&imageHeight,0,0);
	if(Image == NULL){
		return false;
	}
	for (a=0; a<imageWidth*imageHeight; a++) {
		alpha[4*a]=Image[a*3];		// R
		alpha[4*a+1]=Image[a*3+1];	// G
		alpha[4*a+2]=Image[a*3+2];	// B
	}
	glGenTextures(1, &multiLogo);
	// Create Linear Filtered RGBA8-Texture
	glBindTexture(GL_TEXTURE_2D, multiLogo);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);
	delete[] alpha;
	alpha = NULL;
	SOIL_free_image_data(Image);
	Image = NULL;
	
	
	return true;
}

GLvoid NEHE22::InitGL(){
	
	initMultitexture();
	initLights();
	
	if(!LoadGLTextures()){
        cout<<"Fail to load textures"<<endl;
    }
	
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


GLvoid NEHE22::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	
	//draw FPS text
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	
	glutSwapBuffers();
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE22::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE22::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE22::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE22::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE22::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE22::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
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

GLfloat NEHE22::xrot = 0.0f;
GLfloat NEHE22::yrot = 0.0f;
GLfloat NEHE22::z = -5.0f;
GLfloat NEHE22::xspeed = 0.0f;
GLfloat NEHE22::yspeed = 0.0f;
bool NEHE22::emboss = false;
bool NEHE22::bumps = true;

bool NEHE22::multitextureSupported=false;
bool NEHE22::useMultitexture=true;
GLint NEHE22::maxTexelUnits=1;

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
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,Gray);
	glGenTextures(3, &bump[0]);	// Create Three Textures
	
	// Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, bump[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, bump[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, bump[2]);
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
    glBindTexture(GL_TEXTURE_2D, invbump[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, invbump[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
	
	// Create MipMapped Filtered Texture
    glBindTexture(GL_TEXTURE_2D, invbump[2]);
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

void NEHE22::doCube () {
    int i;
    glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	for (i=0; i<4; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);
	for (i=4; i<8; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	for (i=8; i<12; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);
	for (i=12; i<16; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
	// Right Face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	for (i=16; i<20; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	for (i=20; i<24; i++) {
		glTexCoord2f(data[5*i],data[5*i+1]);
		glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
	}
    glEnd();
}

// Calculates v=vM, M Is 4x4 In Column-Major, v Is 4dim. Row (i.e. "Transposed")
void NEHE22::VMatMult(GLfloat *M, GLfloat *v){
    GLfloat res[3];
    res[0]=M[ 0]*v[0]+M[ 1]*v[1]+M[ 2]*v[2]+M[ 3]*v[3];
    res[1]=M[ 4]*v[0]+M[ 5]*v[1]+M[ 6]*v[2]+M[ 7]*v[3];
    res[2]=M[ 8]*v[0]+M[ 9]*v[1]+M[10]*v[2]+M[11]*v[3];
    v[0]=res[0];
    v[1]=res[1];
    v[2]=res[2];
    v[3]=M[15];	// Homogenous Coordinate
}

// Sets Up The Texture-Offsets
// n : Normal On Surface. Must Be Of Length 1
// c : Current Vertex On Surface
// l : Lightposition
// s : Direction Of s-Texture-Coordinate In Object Space (Must Be Normalized!)
// t : Direction Of t-Texture-Coordinate In Object Space (Must Be Normalized!)
void NEHE22::SetUpBumps(GLfloat *n, GLfloat *c, GLfloat *l, GLfloat *s, GLfloat *t) {
    GLfloat v[3];	// Vector From Current Position To Light
    GLfloat lenQ;	// Used To Normalize
    // Calculate v From Current Vertex c To Lightposition And Normalize v
    v[0]=l[0]-c[0];
    v[1]=l[1]-c[1];
    v[2]=l[2]-c[2];
    lenQ=(GLfloat) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    v[0]/=lenQ;
    v[1]/=lenQ;
    v[2]/=lenQ;
    // Project v Such That We Get Two Values Along Each Texture-Coordinate Axis
    c[0]=(s[0]*v[0]+s[1]*v[1]+s[2]*v[2])*MAX_EMBOSS;
    c[1]=(t[0]*v[0]+t[1]*v[1]+t[2]*v[2])*MAX_EMBOSS;
}

void NEHE22::doLogo(){
    // MUST CALL THIS LAST!!!, Billboards The Two Logos
    glDepthFunc(GL_ALWAYS);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D,glLogo);
    glBegin(GL_QUADS);
	glTexCoord2f(0.0f,-0.0f);    glVertex3f(0.23f, -0.4f,-1.0f);
	glTexCoord2f(1.0f,-0.0f);    glVertex3f(0.53f, -0.4f,-1.0f);
	glTexCoord2f(1.0f,-1.0f);    glVertex3f(0.53f, -0.25f,-1.0f);
	glTexCoord2f(0.0f,-1.0f);    glVertex3f(0.23f, -0.25f,-1.0f);
    glEnd();
    if (useMultitexture) {
        glBindTexture(GL_TEXTURE_2D,multiLogo);
        glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);	glVertex3f(-0.53f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(-0.33f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,-1.0f);	glVertex3f(-0.33f, -0.3f,-1.0f);
		glTexCoord2f(0.0f,-1.0f);	glVertex3f(-0.53f, -0.3f,-1.0f);
        glEnd();
    }
	glDepthFunc(GL_LEQUAL);
}

bool NEHE22::doMesh1TexelUnits() {
    GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};	// Holds Current Vertex
    GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};	// Normalized Normal Of Current Surface
    GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};	// s-Texture Coordinate Direction, Normalized
    GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};	// t-Texture Coordinate Direction, Normalized
    GLfloat l[4];	// Holds Our Lightposition To Be Transformed Into Object Space
    GLfloat Minv[16];	// Holds The Inverted Modelview Matrix To Do So
    int i;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	
    // Build Inverse Modelview Matrix First. This Substitutes One Push/Pop With One glLoadIdentity();
    // Simply Build It By Doing All Transformations Negated And In Reverse Order
    glLoadIdentity();
    glRotatef(-yrot,0.0f,1.0f,0.0f);
    glRotatef(-xrot,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-z);
    glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,z);
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
	
    // Transform The Lightposition Into Object Coordinates:
    l[0]=LightPosition[0];
    l[1]=LightPosition[1];
    l[2]=LightPosition[2];
    l[3]=1.0f;                              // Homogenous Coordinate
    VMatMult(Minv,l);
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	doCube();
	glBindTexture(GL_TEXTURE_2D,invbump[filter]);
	glBlendFunc(GL_ONE,GL_ONE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	
	glBegin(GL_QUADS);
    // Front Face
    n[0]=0.0f;
    n[1]=0.0f;
    n[2]=1.0f;
    s[0]=1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=0; i<4; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Back Face
    n[0]=0.0f;
    n[1]=0.0f;
    n[2]=-1.0f;
    s[0]=-1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=4; i<8; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Top Face
    n[0]=0.0f;
    n[1]=1.0f;
    n[2]=0.0f;
    s[0]=1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=0.0f;
    t[2]=-1.0f;
    for (i=8; i<12; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Bottom Face
    n[0]=0.0f;
    n[1]=-1.0f;
    n[2]=0.0f;
    s[0]=-1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=0.0f;
    t[2]=-1.0f;
    for (i=12; i<16; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Right Face
    n[0]=1.0f;
    n[1]=0.0f;
    n[2]=0.0f;
    s[0]=0.0f;
    s[1]=0.0f;
    s[2]=-1.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=16; i<20; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Left Face
    n[0]=-1.0f;
    n[1]=0.0f;
    n[2]=0.0f;
    s[0]=0.0f;
    s[1]=0.0f;
    s[2]=1.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=20; i<24; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
	glEnd();
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
    yrot+=yspeed;
    if (xrot>360.0f) xrot-=360.0f;
    if (xrot<0.0f) xrot+=360.0f;
    if (yrot>360.0f) yrot-=360.0f;
    if (yrot<0.0f) yrot+=360.0f;
	
    /* LAST PASS: Do The Logos! */
    doLogo();
    return true;
}

bool NEHE22::doMesh2TexelUnits() {
    GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};	// Holds Current Vertex
    GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};	// Normalized Normal Of Current Surface
    GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};	// s-Texture Coordinate Direction, Normalized
    GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};	// t-Texture Coordinate Direction, Normalized
    GLfloat l[4];	// Holds Our Lightposition To Be Transformed Into Object Space
    GLfloat Minv[16];	// Holds The Inverted Modelview Matrix To Do So
    int i;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	
    // Build Inverse Modelview Matrix First. This Substitutes One Push/Pop With One glLoadIdentity();
    // Simply Build It By Doing All Transformations Negated And In Reverse Order
    glLoadIdentity();
    glRotatef(-yrot,0.0f,1.0f,0.0f);
    glRotatef(-xrot,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-z);
    glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,z);
	
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
	
    // Transform The Lightposition Into Object Coordinates:
    l[0]=LightPosition[0];
    l[1]=LightPosition[1];
    l[2]=LightPosition[2];
    l[3]=1.0f;	// Homogenous Coordinate
    VMatMult(Minv,l);
	
	// TEXTURE-UNIT #0
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
	
	// TEXTURE-UNIT #1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, invbump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
	
	
	// General Switches
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	
	glBegin(GL_QUADS);
    // Front Face
    n[0]=0.0f;	n[1]=0.0f;	n[2]=1.0f;
    s[0]=1.0f;	s[1]=0.0f;	s[2]=0.0f;
    t[0]=0.0f;	t[1]=1.0f;	t[2]=0.0f;
    for (i=0; i<4; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Back Face
    n[0]=0.0f;	n[1]=0.0f;	n[2]=-1.0f;
    s[0]=-1.0f;	s[1]=0.0f;	s[2]=0.0f;
    t[0]=0.0f;	t[1]=1.0f;	t[2]=0.0f;
    for (i=4; i<8; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Top Face
    n[0]=0.0f;
    n[1]=1.0f;
    n[2]=0.0f;
    s[0]=1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=0.0f;
    t[2]=-1.0f;
    for (i=8; i<12; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Bottom Face
    n[0]=0.0f;
    n[1]=-1.0f;
    n[2]=0.0f;
    s[0]=-1.0f;
    s[1]=0.0f;
    s[2]=0.0f;
    t[0]=0.0f;
    t[1]=0.0f;
    t[2]=-1.0f;
    for (i=12; i<16; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Right Face
    n[0]=1.0f;
    n[1]=0.0f;
    n[2]=0.0f;
    s[0]=0.0f;
    s[1]=0.0f;
    s[2]=-1.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=16; i<20; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
    // Left Face
    n[0]=-1.0f;
    n[1]=0.0f;
    n[2]=0.0f;
    s[0]=0.0f;
    s[1]=0.0f;
    s[2]=1.0f;
    t[0]=0.0f;
    t[1]=1.0f;
    t[2]=0.0f;
    for (i=20; i<24; i++) {
        c[0]=data[5*i+2];
        c[1]=data[5*i+3];
        c[2]=data[5*i+4];
        SetUpBumps(n,c,l,s,t);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
        glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
    }
	glEnd();
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
    yrot+=yspeed;
    if (xrot>360.0f) xrot-=360.0f;
    if (xrot<0.0f) xrot+=360.0f;
    if (yrot>360.0f) yrot-=360.0f;
    if (yrot<0.0f) yrot+=360.0f;
	
    /* LAST PASS: Do The Logos! */
    doLogo();
    return true;
}

bool NEHE22::doMeshNoBumps() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,z);
	
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
	
    if (useMultitexture) {
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0_ARB);
    }
	
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,texture[filter]);
    glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
    glEnable(GL_LIGHTING);
    doCube();
	
    xrot+=xspeed;
    yrot+=yspeed;
    if (xrot>360.0f) xrot-=360.0f;
    if (xrot<0.0f) xrot+=360.0f;
    if (yrot>360.0f) yrot-=360.0f;
    if (yrot<0.0f) yrot+=360.0f;
	
    /* LAST PASS: Do The Logos! */
    doLogo();
    return true;
}

GLvoid NEHE22::InitGL(){
	
	multitextureSupported=initMultitexture();
	
	if(!LoadGLTextures()){
        cout<<"Fail to load textures"<<endl;
    }
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);
	
	// clear background as black
	glClearColor(0.0f,0.0f,0.0f,0.5f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// want the best perspective correction to be done
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	initLights();
}


GLvoid NEHE22::DrawGLScene(){
	
	if (bumps) {
        if (useMultitexture && maxTexelUnits>1){
            doMesh2TexelUnits();
		}
        else{
			doMesh1TexelUnits();
		}
	}
	
    else{
		doMeshNoBumps();
	}
	
	//draw FPS text
	GLint matrixMode;
	GLboolean lightingOn;
	lightingOn= glIsEnabled(GL_LIGHTING);        /* lighting on? */
	if (lightingOn) glDisable(GL_LIGHTING);
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);  /* matrix mode? */
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_COLOR_BUFFER_BIT);       /* save current colour */
	
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.98f,-0.98f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	if (lightingOn) glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glEnable(GL_DEPTH_TEST);
	
	
	glutSwapBuffers();
	
	// handle keyboard input
	if (keys['e']){
		keys['e']=false;
		emboss=!emboss;
	}
	if (keys['m']){
		keys['m']=false;
		useMultitexture=((!useMultitexture) && multitextureSupported);
	}
	if (keys['b']){
		keys['b']=false;
		bumps=!bumps;
	}
	
	if (keys['f']){
		keys['f']=false;
		filter++;
		filter%=3;
	}
	if(specialKeys[GLUT_KEY_PAGE_UP]){
		z-=0.02f;
	}
	if(specialKeys[GLUT_KEY_PAGE_DOWN]){
		z+=0.02f;
	}
	
	if (specialKeys[GLUT_KEY_UP]){
		xspeed-=0.01f;
	}
	if (specialKeys[GLUT_KEY_DOWN]){
		xspeed+=0.01f;
	}
	if (specialKeys[GLUT_KEY_RIGHT]){
		yspeed+=0.01f;
	}
	if (specialKeys[GLUT_KEY_LEFT]){
		yspeed-=0.01f;
	}
	
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
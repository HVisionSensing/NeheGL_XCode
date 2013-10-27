//
//  nehe24.cpp
//  NeheGL
//
//  Created by Andong Li on 10/5/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe24.h"


const char* NEHE24::TITLE = "NEHE24";

GLfloat NEHE24::sleepTime = 0.0f;
int NEHE24::frameCounter = 0;
int NEHE24::currentTime = 0;
int NEHE24::lastTime = 0;
char NEHE24::FPSstr[15]  = "Calculating...";

int NEHE24::scroll = 0;
int NEHE24::maxtokens = 0;
int NEHE24::swidth = 0;
int NEHE24::sheight = 0;

TextureImage NEHE24::textures[1];	// just for declaration
GLuint NEHE24::base = 0;

bool NEHE24::keys[256] = {};	//all set to false
bool NEHE24::specialKeys[256] = {};	//all set to false

GLvoid NEHE24::ReSizeGLScene(GLsizei width, GLsizei height){
	
	swidth=width;	// Set Scissor Width To Window Width
    sheight=height;	// Set Scissor Height To Window Height
    if (height==0)                                  // Prevent A Divide By Zero By
    {
        height=1;
    }
    glViewport(0,0,width,height);	// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
    glLoadIdentity();	// Reset The Projection Matrix
    glOrtho(0.0f,640,480,0.0f,-1.0f,1.0f);	// Create Ortho 640x480 View (0,0 At Top Left)
    glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
    glLoadIdentity();
}

bool NEHE24::LoadTGA(TextureImage *texture, const char *filename) {
	
	GLubyte TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
    GLubyte TGAcompare[12];	// Used To Compare TGA Header
    GLubyte header[6];	// First 6 Useful Bytes From The Header
    GLuint bytesPerPixel;	// Holds Number Of Bytes Per Pixel Used In The TGA File
    GLuint imageSize;	// Used To Store The Image Size When Setting Aside Ram
    GLuint temp;	// Temporary Variable
    GLuint type=GL_RGBA;	// Set The Default GL Mode To RBGA (32 BPP)
	
	FILE *file = fopen(Utils::getAbsoluteDir(filename), "rb");	// Open The TGA File
	
	if( file==NULL ||                               // Does File Even Exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||  // Are There 12 Bytes To Read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0       ||  // Does The Header Match What We Want?
	   fread(header,1,sizeof(header),file)!=sizeof(header))            // If So Read Next 6 Header Bytes
	{
		if (file == NULL)                           // Did The File Even Exist? *Added Jim Strong*
			return false;                           // Return False
		else
		{
			fclose(file);                           // If Anything Failed, Close The File
			return false;                           // Return False
		}
	}
	
	texture->width  = header[1] * 256 + header[0];	// Determine The TGA Width  (highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];	// Determine The TGA Height (highbyte*256+lowbyte)
	
	if( texture->width   <=0  ||                      // Is The Width Less Than Or Equal To Zero
	   texture->height  <=0  ||                      // Is The Height Less Than Or Equal To Zero
	   (header[4]!=24 && header[4]!=32))                   // Is The TGA 24 or 32 Bit?
	{
		fclose(file);                               // If Anything Failed, Close The File
		return false;                               // Return False
	}
	
	texture->bpp = header[4];	// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel   = texture->bpp/8;	// Divide By 8 To Get The Bytes Per Pixel
	imageSize   = texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	
	texture->imageData=(GLubyte *)malloc(imageSize);	// Reserve Memory To Hold The TGA Data
	
	if( texture->imageData==NULL ||                      // Does The Storage Memory Exist?
	   fread(texture->imageData, 1, imageSize, file)!=imageSize)        // Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)	// Was Image Data Loaded
			free(texture->imageData);	// If So, Release The Image Data
		
		fclose(file);	// Close The File
		return false;	// Return False
	}
	
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)              // Loop Through The Image Data
	{                                       // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];                      // Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];            // Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;                    // Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	
	fclose (file);	// Close The File
	
	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);                        // Generate OpenGL texture IDs
	
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);                 // Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);       // Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       // Linear Filtered
	
	if (texture[0].bpp==24)                             // Was The TGA 24 Bits
	{
		type=GL_RGB;                                // If So Set The 'type' To GL_RGB
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);
	
    return true;
}

// Build Our Font Display List
GLvoid NEHE24::buildFont(){
    base=glGenLists(256);                               // Creating 256 Display Lists
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);                // Select Our Font Texture
    for (int loop1=0; loop1<256; loop1++)                        // Loop Through All 256 Lists
    {
        float cx=float(loop1%16)/16.0f;                     // X Position Of Current Character
        float cy=float(loop1/16)/16.0f;                     // Y Position Of Current Character
		
        glNewList(base+loop1,GL_COMPILE);                   // Start Building A List
		glBegin(GL_QUADS);                      // Use A Quad For Each Character
		glTexCoord2f(cx,1.0f-cy-0.0625f);           // Texture Coord (Bottom Left)
		glVertex2d(0,16);                   // Vertex Coord (Bottom Left)
		glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);       // Texture Coord (Bottom Right)
		glVertex2i(16,16);                  // Vertex Coord (Bottom Right)
		glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);        // Texture Coord (Top Right)
		glVertex2i(16,0);                   // Vertex Coord (Top Right)
		glTexCoord2f(cx,1.0f-cy-0.001f);            // Texture Coord (Top Left)
		glVertex2i(0,0);                    // Vertex Coord (Top Left)
		glEnd();                            // Done Building Our Quad (Character)
		glTranslated(14,0,0);                       // Move To The Right Of The Character
        glEndList();                                // Done Building The Display List
    }                                       // Loop Until All 256 Are Built
}

// Delete The Font From Memory
GLvoid NEHE24::KillFont(){
    glDeleteLists(base,256);	// Delete All 256 Display Lists
}

GLvoid NEHE24::glPrint(GLint x, GLint y, int set, const char *fmt, ...){
    char text[1024];	// Holds Our String
    va_list ap;	// Pointer To List Of Arguments
	
    if (fmt == NULL)	// If There's No Text
        return;	// Do Nothing
	
    va_start(ap, fmt);	// Parses The String For Variables
	vsprintf(text, fmt, ap);	// And Converts Symbols To Actual Numbers
    va_end(ap);	// Results Are Stored In Text
	
    if (set>1)	// Did User Choose An Invalid Character Set?
    {
        set=1;	// If So, Select Set 1 (Italic)
    }
	
    glEnable(GL_TEXTURE_2D);	// Enable Texture Mapping
    glLoadIdentity();	// Reset The Modelview Matrix
    glTranslated(x,y,0);	// Position The Text (0,0 - Top Left)
    glListBase(base-32+(128*set));	// Choose The Font Set (0 or 1)
	
    glScalef(1.0f,2.0f,1.0f);	// Make The Text 2X Taller
	
    glCallLists((int)(strlen(text)),GL_UNSIGNED_BYTE, text);	// Write The Text To The Screen
    glDisable(GL_TEXTURE_2D);	// Disable Texture Mapping
}

GLvoid NEHE24::InitGL(){
	
	// Load The Font Texture
	if (!LoadTGA(&textures[0],"NeheGL/img/Font.tga")){
        cout<<"Fail to load texture"<<endl;
    }
	
    buildFont();	// Build The Font
	
    glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	// Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);	// Select Our Font Texture
	
}


GLvoid NEHE24::DrawGLScene(){
	
	char* token;	// Storage For Our Token
    int cnt=0;	// Local Counter Variable
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1.0f,0.5f,0.5f);	// Set Color To Bright Red
    glPrint(10,16,1,"Renderer");	// Display Renderer
    glPrint(40,48,1,"Vendor");	// Display Vendor Name
    glPrint(26,80,1,"Version");	// Display Version
	
	glColor3f(1.0f,0.7f,0.4f);                          // Set Color To Orange
	glPrint(130,16,1,(char *)glGetString(GL_RENDERER));             // Display Renderer
	glPrint(130,48,1,(char *)glGetString(GL_VENDOR));               // Display Vendor Name
	glPrint(130,80,1,(char *)glGetString(GL_VERSION));              // Display Version
	
	glColor3f(0.5f,0.5f,1.0f);                          // Set Color To Bright Blue
	glPrint(192,432,1,"NeHe Productions");                      // Write NeHe Productions At The Bottom Of The Screen
	
	glLoadIdentity();                               // Reset The ModelView Matrix
	glColor3f(1.0f,1.0f,1.0f);                          // Set The Color To White
	glBegin(GL_LINE_STRIP);                             // Start Drawing Line Strips (Something New)
    glVertex2d(639,417);                            // Top Right Of Bottom Box
    glVertex2d(  0,417);                            // Top Left Of Bottom Box
    glVertex2d(  0,480);                            // Lower Left Of Bottom Box
    glVertex2d(639,480);                            // Lower Right Of Bottom Box
    glVertex2d(639,128);                            // Up To Bottom Right Of Top Box
	glEnd();                                    // Done First Line Strip
	glBegin(GL_LINE_STRIP);                             // Start Drawing Another Line Strip
    glVertex2d(  0,128);                            // Bottom Left Of Top Box
    glVertex2d(639,128);                            // Bottom Right Of Top Box
    glVertex2d(639,  1);                            // Top Right Of Top Box
    glVertex2d(  0,  1);                            // Top Left Of Top Box
    glVertex2d(  0,417);                            // Down To Top Left Of Bottom Box
	glEnd();                                    // Done Second Line Strip
	
	glScissor(1 ,int(0.135416f*sheight),swidth-2,int(0.597916f*sheight));   // Define Scissor Region
	glEnable(GL_SCISSOR_TEST);                          // Enable Scissor Testing
	
	char* text=(char*)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);     // Allocate Memory For Our Extension String
	strcpy (text,(char *)glGetString(GL_EXTENSIONS));               // Grab The Extension List, Store In Text
	
	
	token = strtok(text, " ");									                    // Parse 'text' For Words, Seperated By " " (spaces)
	while(token != NULL)										                    // While The Token Isn't NULL
	{
		cnt++;												// Increase The Counter
		if(cnt > maxtokens)									// Is 'maxtokens' Less Than 'cnt'
			maxtokens=cnt;									    // If So, Set 'maxtokens' Equal To 'cnt'
		glColor3f(0.5f, 1.0f, 0.5f);							// Set Color To Bright Green
		glPrint(0, 96 + (cnt*32) - scroll, 0, "%i", cnt);		// Print Current Extension Number
		glColor3f(1.0f, 1.0f, 0.5f);							// Set Color To Yellow
		glPrint(50, 96 + (cnt*32) - scroll, 0, token);		// Print The Current Token (Parsed Extension Name)
		token = strtok(NULL, " ");							// Search For The Next Token
	}
	
	glDisable(GL_SCISSOR_TEST);	// Disable Scissor Testing
	free(text);
	
	//draw FPS text
	GLint matrixMode;
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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers();
	
	// handle keyboard input
	if (specialKeys[GLUT_KEY_UP] && (scroll>0)){
		scroll-=2;	// Decrease 'scroll' Moving Screen Down
	}
	if (specialKeys[GLUT_KEY_DOWN] && (scroll<32*(maxtokens-9)))
	{
		scroll+=2;	// Increase 'scroll' Moving Screen Up
	}
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE24::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE24::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE24::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE24::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE24::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE24::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
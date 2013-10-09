//
//  nehe29.cpp
//  NeheGL
//
//  Created by Andong Li on 10/8/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe29.h"

const char* NEHE29::TITLE = "NEHE29";

GLfloat NEHE29::sleepTime = 0.0f;
int NEHE29::frameCounter = 0;
int NEHE29::currentTime = 0;
int NEHE29::lastTime = 0;
char NEHE29::FPSstr[15]  = "Calculating...";

GLfloat NEHE29::xrot = 0.0f;
GLfloat NEHE29::yrot = 0.0f;
GLfloat NEHE29::zrot = 0.0f;

GLuint NEHE29::texture[1] = {0};

P_TEXTURE_IMAGE NEHE29::t1 = NULL;
P_TEXTURE_IMAGE NEHE29::t2 = NULL;


bool NEHE29::keys[256] = {};	//all set to false
bool NEHE29::specialKeys[256] = {};	//all set to false


// Allocate An Image Structure And Inside Allocate Its Memory Requirements
P_TEXTURE_IMAGE NEHE29::AllocateTextureBuffer( GLint w, GLint h, GLint f){
    P_TEXTURE_IMAGE ti=NULL;	// Pointer To Image Struct
    unsigned char *c=NULL;	// Pointer To Block Memory For Image
	ti = (P_TEXTURE_IMAGE)malloc(sizeof(TEXTURE_IMAGE));	// One Image Struct Please
	
	if( ti != NULL ) {
		ti->width  = w;	// Set Width
		ti->height = h;	// Set Height
		ti->format = f;	// Set Format
		c = (unsigned char *)malloc( w * h * f);
		if ( c != NULL ) {
			ti->data = c;
		}
		else {
			cout<<"Fail to allocate texture memory!"<<endl;
			return NULL;
		}
	}
	else
    {
        cout<<"Fail to allocate texture memory!"<<endl;
        return NULL;
    }
    return ti;
}

// Free Up The Image Data
void NEHE29::DeallocateTexture(P_TEXTURE_IMAGE t)
{
    if(t)
    {
        if(t->data)
        {
            free(t->data);                           // Free Its Image Buffer
        }
		
        free(t);                                // Free Itself
    }
}

// Read A .RAW File In To The Allocated Image Buffer Using data In The Image Structure Header.
// Flip The Image Top To Bottom.  Returns 0 For Failure Of Read, Or Number Of Bytes Read.
int NEHE29::ReadTextureData(const char *filename, P_TEXTURE_IMAGE buffer){
    FILE *f;
    int i,j,k,done=0;
    int stride = buffer->width * buffer->format;                  // Size Of A Row (Width * Bytes Per Pixel)
    unsigned char *p = NULL;
	
    f = fopen(Utils::getAbsoluteDir(filename), "rb");                          // Open "filename" For Reading Bytes
    if( f != NULL )                                 // If File Exists
    {
		for( i = buffer->height-1; i >= 0 ; i-- )             // Loop Through Height (Bottoms Up - Flip Image)
		{
			p = buffer->data + (i * stride );
			for ( j = 0; j < buffer->width ; j++ )                // Loop Through Width
			{
				for ( k = 0 ; k < buffer->format-1 ; k++, p++, done++ )
				{
					*p = fgetc(f);                  // Read Value From File And Store In Memory
				}
				*p = 255; p++;                      // Store 255 In Alpha Channel And Increase Pointer
			}
		}
		fclose(f);                              // Close The File
	}
	else                                        // Otherwise
    {
        cout<<"Fail to load texture image!"<<endl;
    }
    return done;                                    // Returns Number Of Bytes Read In
}

void NEHE29::BuildTexture (P_TEXTURE_IMAGE tex){
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex->width, tex->height, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);
}

void NEHE29::Blit( P_TEXTURE_IMAGE src, P_TEXTURE_IMAGE dst, int src_xstart, int src_ystart, int src_width, int src_height, int dst_xstart, int dst_ystart, int blend, int alpha){
    int i,j,k;
    unsigned char *s, *d;                               // Source & Destination
	
    // Clamp Alpha If Value Is Out Of Range
    if( alpha > 255 ) alpha = 255;
    if( alpha < 0 ) alpha = 0;
	
    // Check For Incorrect Blend Flag Values
    if( blend < 0 ) blend = 0;
    if( blend > 1 ) blend = 1;
	d = dst->data + (dst_ystart * dst->width * dst->format);           // Start Row - dst (Row * Width In Pixels * Bytes Per Pixel)
	s = src->data + (src_ystart * src->width * src->format);           // Start Row - src (Row * Width In Pixels * Bytes Per Pixel)
	
	for (i = 0 ; i < src_height ; i++ )                      // Height Loop
	{
		s = s + (src_xstart * src->format);                  // Move Through Src Data By Bytes Per Pixel
		d = d + (dst_xstart * dst->format);                  // Move Through Dst Data By Bytes Per Pixel
		for (j = 0 ; j < src_width ; j++ )                   // Width Loop
		{
			for( k = 0 ; k < src->format ; k++, d++, s++)         // "n" Bytes At A Time
            {
                if (blend)                      // If Blending Is On
					*d = ( (*s * alpha) + (*d * (255-alpha)) ) >> 8;  // Multiply Src Data*alpha Add Dst Data*(255-alpha)
                else                            // Keep in 0-255 Range With >> 8
					*d = *s;                        // No Blending Just Do A Straight Copy
            }
        }
        d = d + (dst->width - (src_width + dst_xstart))*dst->format;      // Add End Of Row
        s = s + (src->width - (src_width + src_xstart))*src->format;      // Add End Of Row
    }
}

GLvoid NEHE29::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE29::InitGL(){
	
	t1 = AllocateTextureBuffer( 256, 256, 4 );	// Get An Image Structure
    if (ReadTextureData("NeheGL/img/monitor.raw",t1)==0){
		cout<<"Fail to load image!"<<endl;
    }
	
    t2 = AllocateTextureBuffer( 256, 256, 4 );	// Second Image Structure
    if (ReadTextureData("NeheGL/img/gl.raw",t2)==0){
        cout<<"Fail to load image!"<<endl;
    }
	
	
	// Image To Blend In, Original Image,
	//Src Start X & Y, Src Width & Height,
	//Dst Location X & Y, Blend Flag, Alpha Value
	Blit(t2,t1,127,127,128,128,64,64,1,127);	// Call The Blitter Routine
	
	BuildTexture (t1);	// Load The Texture Map Into Texture Memory
	
    DeallocateTexture(t1);	// Clean Up Image Memory Because Texture Is
    DeallocateTexture(t2);	// In GL Texture Memory Now
	
    glEnable(GL_TEXTURE_2D);	// Enable Texture Mapping
	
    glShadeModel(GL_SMOOTH);	// Enables Smooth Color Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
    glClearDepth(1.0);	// Enables Clearing Of The Depth Buffer
    glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
    glDepthFunc(GL_LESS);	// The Type Of Depth Test To Do
	
}


GLvoid NEHE29::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();                               // Reset The View
    glTranslatef(0.0f,0.0f,-5.0f);
	
    glRotatef(xrot,1.0f,0.0f,0.0f);
    glRotatef(yrot,0.0f,1.0f,0.0f);
    glRotatef(zrot,0.0f,0.0f,1.0f);
	
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	
    glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Right Face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
	
    
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
GLvoid NEHE29::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE29::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE29::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE29::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE29::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE29::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
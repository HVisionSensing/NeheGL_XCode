//
//  nehe33.cpp
//  NeheGL
//
//  Created by Andong Li on 10/16/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe33.h"

const char* NEHE33::TITLE = "NEHE33";

GLfloat NEHE33::sleepTime = 0.0f;
int NEHE33::frameCounter = 0;
int NEHE33::currentTime = 0;
int NEHE33::lastTime = 0;
char NEHE33::FPSstr[15]  = "Calculating...";

TGAHeader NEHE33::tgaheader;	// just declare
TGA NEHE33::tga;	//just declare
GLubyte NEHE33::uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte NEHE33::cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

float NEHE33::spin = 0.0f;
Texture NEHE33::texture[2];	//just declare

bool NEHE33::keys[256] = {};	//all set to false
bool NEHE33::specialKeys[256] = {};	//all set to false

GLvoid NEHE33::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

bool NEHE33::LoadUncompressedTGA(Texture * texture, const char * filename, FILE * fTGA){
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0){
		if(fTGA != NULL){
			fclose(fTGA);
		}
		return false;
	}
	
	texture->width  = tga.header[1] * 256 + tga.header[0];	// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];	// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];	// Determine the bits per pixel
	tga.Width		= texture->width;	// Copy width into local structure
	tga.Height		= texture->height;	// Copy height into local structure
	tga.Bpp			= texture->bpp;	// Copy BPP into local structure
	
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32))){
		if(fTGA != NULL){
			fclose(fTGA);
		}
		return false;
	}
	
	if(texture->bpp == 24)
		texture->type	= GL_RGB;
	else
		texture->type	= GL_RGBA;
	
	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);
	
	if(texture->imageData == NULL){
		fclose(fTGA);
		return false;
	}
	
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize){
		if(texture->imageData != NULL){
			free(texture->imageData);
		}
		fclose(fTGA);
		return false;
	}
	
	// Byte Swapping Optimized By Steve Thomas
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}
	
	fclose(fTGA);
	return true;
}

bool NEHE33::LoadCompressedTGA(Texture * texture, const char * filename, FILE * fTGA){
	
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0){
		if(fTGA != NULL){
			fclose(fTGA);
		}
		return false;
	}
	
	texture->width  = tga.header[1] * 256 + tga.header[0];
	texture->height = tga.header[3] * 256 + tga.header[2];
	texture->bpp	= tga.header[4];
	tga.Width		= texture->width;
	tga.Height		= texture->height;
	tga.Bpp			= texture->bpp;
	
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32))){
		if(fTGA != NULL){
			fclose(fTGA);
		}
		return false;
	}
	
	if(texture->bpp == 24)
		texture->type	= GL_RGB;
	else
		texture->type	= GL_RGBA;
	
	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);
	
	if(texture->imageData == NULL){
		fclose(fTGA);
		return false;
	}
	
	GLuint pixelcount	= tga.Height * tga.Width;
	GLuint currentpixel	= 0;
	GLuint currentbyte	= 0;
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);
	
	do
	{
		GLubyte chunkheader = 0;
		
		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0){
			if(fTGA != NULL){
				fclose(fTGA);
			}
			if(texture->imageData != NULL){
				free(texture->imageData);
			}
			return false;
		}
		
		if(chunkheader < 128){
			
			chunkheader++;
			for(short counter = 0; counter < chunkheader; counter++){
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel){

					if(fTGA != NULL){
						fclose(fTGA);
					}
					
					if(colorbuffer != NULL){
						free(colorbuffer);
					}
					
					if(texture->imageData != NULL){
						free(texture->imageData);
					}
					
					return false;
				}
				
				texture->imageData[currentbyte		] = colorbuffer[2];
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];
				
				if(tga.bytesPerPixel == 4){
					texture->imageData[currentbyte + 3] = colorbuffer[3];
				}
				
				currentbyte += tga.bytesPerPixel;
				currentpixel++;
				
				if(currentpixel > pixelcount){
					
					if(fTGA != NULL){
						fclose(fTGA);
					}
					
					if(colorbuffer != NULL){
						free(colorbuffer);
					}
					
					if(texture->imageData != NULL){
						free(texture->imageData);
					}
					
					return false;
				}
			}
		}
		else{
			chunkheader -= 127;
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel){
				
				if(fTGA != NULL){
					fclose(fTGA);
				}
				
				if(colorbuffer != NULL){
					free(colorbuffer);
				}
				
				if(texture->imageData != NULL){
					free(texture->imageData);
				}
				
				return false;
			}
			
			for(short counter = 0; counter < chunkheader; counter++){
				
				texture->imageData[currentbyte		] = colorbuffer[2];
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];
				
				if(tga.bytesPerPixel == 4){
					texture->imageData[currentbyte + 3] = colorbuffer[3];
				}
				
				currentbyte += tga.bytesPerPixel;
				currentpixel++;
				
				if(currentpixel > pixelcount){
					
					if(fTGA != NULL){
						fclose(fTGA);
					}
					
					if(colorbuffer != NULL){
						free(colorbuffer);
					}
					
					if(texture->imageData != NULL){
						free(texture->imageData);
					}
					
					return false;
				}
			}
		}
	}while(currentpixel < pixelcount);
	
	fclose(fTGA);
	return true;
}

bool NEHE33::LoadTGA(Texture * texture, const char * filename){
	FILE * fTGA;	// Declare File Pointer
	fTGA = fopen(filename, "rb");	// Open File For Reading
	
	if(fTGA == NULL){
		return false;
	}
	
	// Attempt To Read The File Header
	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0){
		return false;
	}
	
	// If The File Header Matches The Uncompressed Header
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0){
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	
	// If The File Header Matches The Compressed Header
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0){
		LoadCompressedTGA(texture, filename, fTGA);
	}
	else{
		return false;
	}
	return true;
}

bool NEHE33::LoadGLTextures(){
	
	// Load The Bitmap, Check For Errors.
	if (LoadTGA(&texture[0], Utils::getAbsoluteDir("NeheGL/img/Uncompressed.tga")) &&
		LoadTGA(&texture[1], Utils::getAbsoluteDir("NeheGL/img/Compressed.tga"))){
		
		for (int loop=0; loop<2; loop++){
			glGenTextures(1, &texture[loop].texID);
			glBindTexture(GL_TEXTURE_2D, texture[loop].texID);
			glTexImage2D(GL_TEXTURE_2D, 0, texture[loop].bpp / 8, texture[loop].width, texture[loop].height, 0, texture[loop].type, GL_UNSIGNED_BYTE, texture[loop].imageData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
			if (texture[loop].imageData){
				free(texture[loop].imageData);
			}
		}
		return true;
	}
	return false;
}


GLvoid NEHE33::InitGL(){
	
	if (!LoadGLTextures()){
		cout<<"Fail to load texture!"<<endl;
	}
	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}


GLvoid NEHE33::DrawGLScene(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-10.0f);
	
	for (int loop=0; loop<20; loop++){
		glPushMatrix();
		glRotatef(spin+loop*18.0f,1.0f,0.0f,0.0f);
		glTranslatef(-2.0f,2.0f,0.0f);
		
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(2.0f,0.0f,0.0f);
		glRotatef(spin+loop*36.0f,0.0f,1.0f,0.0f);
		glTranslatef(1.0f,0.0f,0.0f);
		
		glBindTexture(GL_TEXTURE_2D, texture[1].texID);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
		glPopMatrix();
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
	
	spin+=0.05f;
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE33::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE33::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE33::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE33::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE33::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE33::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
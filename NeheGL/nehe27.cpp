//
//  nehe27.cpp
//  NeheGL
//
//  Created by Andong Li on 10/6/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "nehe27.h"


const char* NEHE27::TITLE = "NEHE27";

GLfloat NEHE27::sleepTime = 0.0f;
int NEHE27::frameCounter = 0;
int NEHE27::currentTime = 0;
int NEHE27::lastTime = 0;
char NEHE27::FPSstr[15]  = "Calculating...";

glObject NEHE27::obj;	// just for declaration
GLfloat NEHE27::xrot =  0.0f;
GLfloat NEHE27::yrot =  0.0f;
GLfloat NEHE27::xspeed =  0.0f;
GLfloat NEHE27::yspeed =  0.0f;

GLfloat NEHE27::LightPos[] = { 0.0f, 5.0f,-4.0f, 1.0f};
GLfloat NEHE27::LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};
GLfloat NEHE27::LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};
GLfloat NEHE27::LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};

GLfloat NEHE27::MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat NEHE27::MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};
GLfloat NEHE27::MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat NEHE27::MatShn[] = {0.0f};

GLfloat NEHE27::ObjPos[] = {-2.0f,-2.0f,-5.0f};

GLUquadricObj* NEHE27::q = NULL;
GLfloat NEHE27::SpherePos[] = {-4.0f,-5.0f,-6.0f};


bool NEHE27::keys[256] = {};	//all set to false
bool NEHE27::specialKeys[256] = {};	//all set to false


int NEHE27::ReadObject(const char *st, glObject *o){
	FILE *file;
	unsigned int i;
	
	file = fopen(Utils::getAbsoluteDir(st), "r");
	if (!file) return FALSE;
	//points
	fscanf(file, "%d", &(o->nPoints));
	for (i=1;i<=o->nPoints;i++){
		fscanf(file, "%f", &(o->points[i].x));
		fscanf(file, "%f", &(o->points[i].y));
		fscanf(file, "%f", &(o->points[i].z));
	}
	//planes
	fscanf(file, "%d", &(o->nPlanes));
	for (i=0;i<o->nPlanes;i++){
		fscanf(file, "%d", &(o->planes[i].p[0]));
		fscanf(file, "%d", &(o->planes[i].p[1]));
		fscanf(file, "%d", &(o->planes[i].p[2]));
		
		fscanf(file, "%f", &(o->planes[i].normals[0].x));
		fscanf(file, "%f", &(o->planes[i].normals[0].y));
		fscanf(file, "%f", &(o->planes[i].normals[0].z));
		fscanf(file, "%f", &(o->planes[i].normals[1].x));
		fscanf(file, "%f", &(o->planes[i].normals[1].y));
		fscanf(file, "%f", &(o->planes[i].normals[1].z));
		fscanf(file, "%f", &(o->planes[i].normals[2].x));
		fscanf(file, "%f", &(o->planes[i].normals[2].y));
		fscanf(file, "%f", &(o->planes[i].normals[2].z));
	}
	return true;
}

void NEHE27::SetConnectivity(glObject *o){
	unsigned int p1i, p2i, p1j, p2j;
	unsigned int P1i, P2i, P1j, P2j;
	unsigned int i,j,ki,kj;
	
	for(i=0;i<o->nPlanes-1;i++)
		for(j=i+1;j<o->nPlanes;j++)
			for(ki=0;ki<3;ki++)
				if(!o->planes[i].neigh[ki]){
					for(kj=0;kj<3;kj++){
						p1i=ki;
						p1j=kj;
						p2i=(ki+1)%3;
						p2j=(kj+1)%3;
						
						p1i=o->planes[i].p[p1i];
						p2i=o->planes[i].p[p2i];
						p1j=o->planes[j].p[p1j];
						p2j=o->planes[j].p[p2j];
						
						P1i=((p1i+p2i)-abs((int)(p1i-p2i)))/2;
						P2i=((p1i+p2i)+abs((int)(p1i-p2i)))/2;
						P1j=((p1j+p2j)-abs((int)(p1j-p2j)))/2;
						P2j=((p1j+p2j)+abs((int)(p1j-p2j)))/2;
						
						if((P1i==P1j) && (P2i==P2j)){  //they are neighbours
							o->planes[i].neigh[ki] = j+1;
							o->planes[j].neigh[kj] = i+1;
						}
					}
				}
}

void NEHE27::CalcPlane(glObject o, sPlane *plane){
	sPoint v[4];
	int i;
	
	for (i=0;i<3;i++){
		v[i+1].x = o.points[plane->p[i]].x;
		v[i+1].y = o.points[plane->p[i]].y;
		v[i+1].z = o.points[plane->p[i]].z;
	}
	plane->PlaneEq.a = v[1].y*(v[2].z-v[3].z) + v[2].y*(v[3].z-v[1].z) + v[3].y*(v[1].z-v[2].z);
	plane->PlaneEq.b = v[1].z*(v[2].x-v[3].x) + v[2].z*(v[3].x-v[1].x) + v[3].z*(v[1].x-v[2].x);
	plane->PlaneEq.c = v[1].x*(v[2].y-v[3].y) + v[2].x*(v[3].y-v[1].y) + v[3].x*(v[1].y-v[2].y);
	plane->PlaneEq.d =-( v[1].x*(v[2].y*v[3].z - v[3].y*v[2].z) +
						v[2].x*(v[3].y*v[1].z - v[1].y*v[3].z) +
						v[3].x*(v[1].y*v[2].z - v[2].y*v[1].z) );
}

void NEHE27::DrawGLObject(glObject o){
	unsigned int i, j;
	
	glBegin(GL_TRIANGLES);
	for (i=0; i<o.nPlanes; i++){
		for (j=0; j<3; j++){
			glNormal3f(o.planes[i].normals[j].x,
					   o.planes[i].normals[j].y,
					   o.planes[i].normals[j].z);
			glVertex3f(o.points[o.planes[i].p[j]].x,
					   o.points[o.planes[i].p[j]].y,
					   o.points[o.planes[i].p[j]].z);
		}
	}
	glEnd();
}

void NEHE27::CastShadow(glObject *o, float *lp){
	unsigned int	i, j, k, jj;
	unsigned int	p1, p2;
	sPoint			v1, v2;
	float			side;
	
	//set visual parameter
	for (i=0;i<o->nPlanes;i++){
		// chech to see if light is in front or behind the plane (face plane)
		side =	o->planes[i].PlaneEq.a*lp[0]+
		o->planes[i].PlaneEq.b*lp[1]+
		o->planes[i].PlaneEq.c*lp[2]+
		o->planes[i].PlaneEq.d*lp[3];
		if (side >0) o->planes[i].visible = TRUE;
		else o->planes[i].visible = FALSE;
	}
	
 	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
	
	// first pass, stencil operation decreases stencil value
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	for (i=0; i<o->nPlanes;i++){
		if (o->planes[i].visible)
			for (j=0;j<3;j++){
				k = o->planes[i].neigh[j];
				if ((!k) || (!o->planes[k-1].visible)){
					// here we have an edge, we must draw a polygon
					p1 = o->planes[i].p[j];
					jj = (j+1)%3;
					p2 = o->planes[i].p[jj];
					
					//calculate the length of the vector
					v1.x = (o->points[p1].x - lp[0])*100;
					v1.y = (o->points[p1].y - lp[1])*100;
					v1.z = (o->points[p1].z - lp[2])*100;
					
					v2.x = (o->points[p2].x - lp[0])*100;
					v2.y = (o->points[p2].y - lp[1])*100;
					v2.z = (o->points[p2].z - lp[2])*100;
					
					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(o->points[p1].x,
							   o->points[p1].y,
							   o->points[p1].z);
					glVertex3f(o->points[p1].x + v1.x,
							   o->points[p1].y + v1.y,
							   o->points[p1].z + v1.z);
					
					glVertex3f(o->points[p2].x,
							   o->points[p2].y,
							   o->points[p2].z);
					glVertex3f(o->points[p2].x + v2.x,
							   o->points[p2].y + v2.y,
							   o->points[p2].z + v2.z);
					glEnd();
				}
			}
	}
	
	// second pass, stencil operation increases stencil value
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	for (i=0; i<o->nPlanes;i++){
		if (o->planes[i].visible)
			for (j=0;j<3;j++){
				k = o->planes[i].neigh[j];
				if ((!k) || (!o->planes[k-1].visible)){
					// here we have an edge, we must draw a polygon
					p1 = o->planes[i].p[j];
					jj = (j+1)%3;
					p2 = o->planes[i].p[jj];
					
					//calculate the length of the vector
					v1.x = (o->points[p1].x - lp[0])*100;
					v1.y = (o->points[p1].y - lp[1])*100;
					v1.z = (o->points[p1].z - lp[2])*100;
					
					v2.x = (o->points[p2].x - lp[0])*100;
					v2.y = (o->points[p2].y - lp[1])*100;
					v2.z = (o->points[p2].z - lp[2])*100;
					
					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(o->points[p1].x,
							   o->points[p1].y,
							   o->points[p1].z);
					glVertex3f(o->points[p1].x + v1.x,
							   o->points[p1].y + v1.y,
							   o->points[p1].z + v1.z);
					
					glVertex3f(o->points[p2].x,
							   o->points[p2].y,
							   o->points[p2].z);
					glVertex3f(o->points[p2].x + v2.x,
							   o->points[p2].y + v2.y,
							   o->points[p2].z + v2.z);
					glEnd();
				}
			}
	}
	
	glFrontFace(GL_CCW);
	glColorMask(1, 1, 1, 1);
	
	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.1f, 0.1f,-0.10f);
	glVertex3f(-0.1f,-0.1f,-0.10f);
	glVertex3f( 0.1f, 0.1f,-0.10f);
	glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);
}

void NEHE27::VMatMult(GLmatrix16f M, GLvector4f v){
	GLfloat res[4];	// Hold Calculated Results
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];	// Results Are Stored Back In v[]
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];	// Homogenous Coordinate
}

int NEHE27::InitGLObjects()
{
	if (!ReadObject("NeheGL/model/Object2.txt", &obj))			// Read Object2 Into obj
	{
		return false;									// If Failed Return False
	}
	
	SetConnectivity(&obj);								// Set Face To Face Connectivity
	
	for (unsigned int i=0;i<obj.nPlanes;i++)			// Loop Through All Object Planes
		CalcPlane(obj, &(obj.planes[i]));				// Compute Plane Equations For All Faces
	
	return true;										// Return True
}

void NEHE27::DrawGLRoom(){
	
	glBegin(GL_QUADS);									// Begin Drawing Quads
	// Floor
	glNormal3f(0.0f, 1.0f, 0.0f);					// Normal Pointing Up
	glVertex3f(-10.0f,-10.0f,-20.0f);				// Back Left
	glVertex3f(-10.0f,-10.0f, 20.0f);				// Front Left
	glVertex3f( 10.0f,-10.0f, 20.0f);				// Front Right
	glVertex3f( 10.0f,-10.0f,-20.0f);				// Back Right
	// Ceiling
	glNormal3f(0.0f,-1.0f, 0.0f);					// Normal Point Down
	glVertex3f(-10.0f, 10.0f, 20.0f);				// Front Left
	glVertex3f(-10.0f, 10.0f,-20.0f);				// Back Left
	glVertex3f( 10.0f, 10.0f,-20.0f);				// Back Right
	glVertex3f( 10.0f, 10.0f, 20.0f);				// Front Right
	// Front Wall
	glNormal3f(0.0f, 0.0f, 1.0f);					// Normal Pointing Away From Viewer
	glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Left
	glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Left
	glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Right
	glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Right
	// Back Wall
	glNormal3f(0.0f, 0.0f,-1.0f);					// Normal Pointing Towards Viewer
	glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Right
	glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Right
	glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Left
	glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Left
	// Left Wall
	glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
	glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Front
	glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Front
	glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Back
	glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Back
	// Right Wall
	glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
	glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Back
	glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Back
	glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Front
	glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Front
	glEnd();											// Done Drawing Quads
}

GLvoid NEHE27::ReSizeGLScene(GLsizei width, GLsizei height){
	
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

GLvoid NEHE27::InitGL(){
	
	
	if (!InitGLObjects()){
		cout<<"fail to load model"<<endl;
	}
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Stencil Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Set Light1 Position
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);			// Set Light1 Ambience
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);			// Set Light1 Diffuse
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);		// Set Light1 Specular
	glEnable(GL_LIGHT1);								// Enable Light1
	glEnable(GL_LIGHTING);								// Enable Lighting
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);			// Set Material Ambience
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);			// Set Material Diffuse
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);		// Set Material Specular
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);		// Set Material Shininess
	
	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling
	glClearColor(0.1f, 1.0f, 0.5f, 1.0f);				// Set Clear Color (Greenish Color)
	
	q = gluNewQuadric();								// Initialize Quadratic
	gluQuadricNormals(q, GL_SMOOTH);					// Enable Smooth Normal Generation
	gluQuadricTexture(q, GL_FALSE);						// Disable Auto Texture Coords
	
}


GLvoid NEHE27::DrawGLScene(){
	
    GLmatrix16f Minv;
	GLvector4f wlp, lp;
	
	// Clear Color Buffer, Depth Buffer, Stencil Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadIdentity();									// Reset Modelview Matrix
	glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into Screen 20 Units
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Position Light1
	glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);	// Position The Sphere
	gluSphere(q, 1.5f, 32, 16);							// Draw A Sphere
	
	// calculate light's position relative to local coordinate system
	// dunno if this is the best way to do it, but it actually works
	// if u find another aproach, let me know ;)
	
	// we build the inversed matrix by doing all the actions in reverse order
	// and with reverse parameters (notice -xrot, -yrot, -ObjPos[], etc.)
	glLoadIdentity();									// Reset Matrix
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);					// Rotate By -yrot On Y Axis
	glRotatef(-xrot, 1.0f, 0.0f, 0.0f);					// Rotate By -xrot On X Axis
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix (Stores In Minv)
	lp[0] = LightPos[0];								// Store Light Position X In lp[0]
	lp[1] = LightPos[1];								// Store Light Position Y In lp[1]
	lp[2] = LightPos[2];								// Store Light Position Z In lp[2]
	lp[3] = LightPos[3];								// Store Light Direction In lp[3]
	VMatMult(Minv, lp);									// We Store Rotated Light Vector In 'lp' Array
	glTranslatef(-ObjPos[0], -ObjPos[1], -ObjPos[2]);	// Move Negative On All Axis Based On ObjPos[] Values (X, Y, Z)
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
	wlp[0] = 0.0f;										// World Local Coord X To 0
	wlp[1] = 0.0f;										// World Local Coord Y To 0
	wlp[2] = 0.0f;										// World Local Coord Z To 0
	wlp[3] = 1.0f;
	VMatMult(Minv, wlp);								// We Store The Position Of The World Origin Relative To The
	// Local Coord. System In 'wlp' Array
	lp[0] += wlp[0];									// Adding These Two Gives Us The
	lp[1] += wlp[1];									// Position Of The Light Relative To
	lp[2] += wlp[2];									// The Local Coordinate System
	
	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					// Set Color To An Orange
	glLoadIdentity();									// Reset Modelview Matrix
	glTranslatef(0.0f, 0.0f, -20.0f);					// Zoom Into The Screen 20 Units
	DrawGLRoom();										// Draw The Room
	glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);		// Position The Object
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// Spin It On The X Axis By xrot
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);					// Spin It On The Y Axis By yrot
	DrawGLObject(obj);									// Procedure For Drawing The Loaded Object
	CastShadow(&obj, lp);								// Procedure For Casting The Shadow Based On The Silhouette
	
	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					// Set Color To Purplish Blue
	glDisable(GL_LIGHTING);								// Disable Lighting
	glDepthMask(GL_FALSE);								// Disable Depth Mask
	glTranslatef(lp[0], lp[1], lp[2]);					// Translate To Light's Position
	// Notice We're Still In Local Coordinate System
	gluSphere(q, 0.2f, 16, 8);							// Draw A Little Yellow Sphere (Represents Light)
	glEnable(GL_LIGHTING);								// Enable Lighting
	glDepthMask(GL_TRUE);								// Enable Depth Mask
	
	
	//draw FPS text
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity ();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.8f,0.8f,0.8f);//set text color
	computeFPS();
	Utils::drawText(-0.54f,-0.4f, GLUT_BITMAP_HELVETICA_12, FPSstr);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	glutSwapBuffers();
	
	xrot += xspeed;
	yrot += yspeed;
	
	// handle keyboard input
	
	// Spin Object
	if (specialKeys[GLUT_KEY_LEFT])	yspeed -= 0.1f;
	if (specialKeys[GLUT_KEY_RIGHT])	yspeed += 0.1f;
	if (specialKeys[GLUT_KEY_UP])	xspeed -= 0.1f;
	if (specialKeys[GLUT_KEY_DOWN])	xspeed += 0.1f;
	
	// Adjust Light's Position
	if (keys['l']) LightPos[0] += 0.05f;	// 'L' Moves Light Right
	if (keys['j']) LightPos[0] -= 0.05f;	// 'J' Moves Light Left
	if (keys['i']) LightPos[1] += 0.05f;	// 'I' Moves Light Up
	if (keys['k']) LightPos[1] -= 0.05f;	// 'K' Moves Light Down
	if (keys['o']) LightPos[2] += 0.05f;	// 'O' Moves Light Toward Viewer
	if (keys['u']) LightPos[2] -= 0.05f;	// 'U' Moves Light Away From Viewer
	
	// Adjust Ball's Position
	if (keys['d']) SpherePos[0] += 0.05f;	// 'D' Move Ball Right
	if (keys['a']) SpherePos[0] -= 0.05f;	// 'A' Move Ball Left
	if (keys['w']) SpherePos[1] += 0.05f;	// 'W' Move Ball Up
	if (keys['s']) SpherePos[1] -= 0.05f;	// 'S' Move Ball Down
	if (keys['e']) SpherePos[2] += 0.05f;	// 'E' Move Ball Toward Viewer
	if (keys['q']) SpherePos[2] -= 0.05f;	// 'Q' Move Ball Away From Viewer
	
	// Adjust Object's Position
	if (keys['h']) ObjPos[0] += 0.05f;			// 'H' Move Object Right
	if (keys['f']) ObjPos[0] -= 0.05f;			// 'F' Move Object Left
	if (keys['t']) ObjPos[1] += 0.05f;			// 'T' Move Object Up
	if (keys['g']) ObjPos[1] -= 0.05f;			// 'G' Move Object Down
	if (keys['r']) ObjPos[2] += 0.05f;			// 'R' Move Object Toward Viewer
	if (keys['y']) ObjPos[2] -= 0.05f;			// 'Y' Move Object Away From Viewer
	
}

/*
 This function is used to limit FPS for smooth animation
 */
GLvoid NEHE27::UpdateScene(int flag){
	
	clock_t startTime = clock();
    glutPostRedisplay();
    clock_t endTime = clock();
	
	//compute sleep time in millesecond
    float sleepTime =  ((CLOCKS_PER_SEC/EXPECT_FPS)-(endTime-startTime))/1000.0;
    //sleepTime = floor(sleepTime+0.5);
    sleepTime < 0 ? sleepTime = 0 : NULL;
	
    glutTimerFunc(sleepTime, UpdateScene, flag);
	
}

GLvoid NEHE27::KeyboardFuction(unsigned char key, int x, int y){
	keys[key] = true;
}

GLvoid NEHE27::KeyboardUpFuction(unsigned char key, int x, int y){
	keys[key] = false;
}

GLvoid NEHE27::KeySpecialFuction(int key, int x, int y){
	specialKeys[key] = true;
}

GLvoid NEHE27::KeySpecialUpFuction(int key, int x, int y){
	specialKeys[key] = false;
}

void NEHE27::computeFPS(){
	frameCounter++;
	currentTime=glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - lastTime > FPS_UPDATE_CAP) {
		sprintf(FPSstr,"FPS: %4.2f",frameCounter*1000.0/(currentTime-lastTime));
		lastTime = currentTime;
		frameCounter = 0;
	}
}
//
//  nehe31.h
//  NeheGL
//
//  Created by Andong Li on 10/13/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#ifndef __NeheGL__nehe31__
#define __NeheGL__nehe31__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "utils.h"
#include "SOIL.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
using namespace std;


#define PACK_STRUCT	__attribute__((packed))

//typedef unsigned char byte;
typedef char byte;
typedef unsigned short word;

// File header
struct MS3DHeader
{
	char m_ID[10];
	int m_version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	byte m_flags;
	float m_vertex[3];
	char m_boneID;
	byte m_refCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	word m_flags;
	word m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	byte m_smoothingGroup;
	byte m_groupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;	// 0.0f - 128.0f
    float m_transparency;	// 0.0f - 1.0f
    byte m_mode;	// 0, 1, 2 is unused now
    char m_texture[128];
    char m_alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	byte m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	word m_numRotationKeyframes;
	word m_numTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;

#undef PACK_STRUCT

struct Vertex31{
    char m_boneID;  // For Skeletal Animation
    float m_location[3];
};

struct Triangle31{
    float m_vertexNormals[3][3];
    float m_s[3], m_t[3];
    int m_vertexIndices[3];
};

struct Mesh31{
    int m_materialIndex;
    int m_numTriangles;
    int *m_pTriangleIndices;
};

struct Material31{
	float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
	float m_shininess;
	GLuint m_texture;
	char *m_pTextureFilename;
};

class NEHE31{
public:
	
	static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
	static GLvoid InitGL();
	static GLvoid DrawGLScene();
	static GLvoid UpdateScene(int flag);
	static GLvoid KeyboardFuction(unsigned char key, int x, int y);
	static GLvoid KeyboardUpFuction(unsigned char key, int x, int y);
	static GLvoid KeySpecialFuction(int key, int x, int y);
	static GLvoid KeySpecialUpFuction(int key, int x, int y);
	
	static const char* TITLE;
	const static int EXPECT_FPS = 60;	// expect FPS during rendering
	const static int FPS_UPDATE_CAP = 100; // time period for updating FPS
	
private:
	static GLfloat sleepTime;	//delay time for limiting FPS
	static void computeFPS();
	static GLuint LoadGLTextures(const char* dir);
	static int frameCounter;
	static int currentTime;
	static int lastTime;
	static char FPSstr[15];
		
	static bool keys[256];
	static bool specialKeys[256];
	
	static GLuint texture;
	static GLfloat yrot;
	
	// Vertices Used
	static int m_numVertices;
	static Vertex31* m_pVertices;
	
	// Triangles Used
	static int m_numTriangles;
	static Triangle31* m_pTriangles;
	
	// Meshes Used
	static int m_numMeshes;
	static Mesh31* m_pMeshes;
	
	//	Materials used
	static int m_numMaterials;
	static Material31 *m_pMaterials;
	
	static bool loadModelData(const char *filename);
};



#endif /* defined(__NeheGL__nehe31__) */

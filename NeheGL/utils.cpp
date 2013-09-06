//
//  utils.cpp
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "utils.h"

void Utils::drawText(float x, float y, void* font, const char* str) {
	
    glRasterPos2f(x,y);
	
	size_t len = strlen(str);
	
    for (int i = 0; i < len; i++){
        glutBitmapCharacter(font, str[i]);
    }
	
}

/*
 you should change build directory to your current project directory in xcode
 preference setting
 how to change (for Xcode 4):
 Xcode->preferences->Locations->Drived Data->Advanced->Build Location->Legacy
 */
char* Utils::getAbsoluteDir(const char *relativeDir){
	CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	
    char* path = new char[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        cout<<"Fail to get current build directory!"<<endl;
    }
    CFRelease(resourcesURL);
    chdir(path);
	
	char* currentBundle = strtok(path,"build");
	char* absoluteDir = new char[strlen(currentBundle)+strlen(relativeDir)+1];
	strcpy(absoluteDir, currentBundle);
	strcat(absoluteDir, relativeDir);
	
	delete path;
	path = NULL;
	
	return absoluteDir;
}


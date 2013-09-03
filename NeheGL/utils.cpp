//
//  utils.cpp
//  NeheGL
//
//  Created by Andong Li on 8/31/13.
//  Copyright (c) 2013 Andong Li. All rights reserved.
//

#include "utils.h"

void TextUtils::drawText(float x, float y, void* font, const char* str) {
	
    glRasterPos2f(x,y);
	
	size_t len = strlen(str);
	
    for (int i = 0; i < len; i++){
        glutBitmapCharacter(font, str[i]);
    }
	
}
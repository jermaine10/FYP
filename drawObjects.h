#pragma once
#ifndef DRAWOBJECTS_H
#define DRAWOBJECTS_H
#include "texture.h"
#include "globalVar.h"


// object rendering was done here
// too bulky to implement in the main although it should be there

 void draw3Dobjects(globalVar datastruct);
void drawAxis();
void selectionDraw(int, int, int, int);


#endif
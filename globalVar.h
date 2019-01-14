#pragma once
#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#define NULL 0
#define LINES 501
#define RECTANGLE 502
#define TRIANGLE 2
#define CUBES 503



#include "GL/glut.h"
#include "shapes.h"
#include "texture.h"
#include <fstream>
#include <iomanip>
#include "Graph.h"


//data structure of everything including some global variables used across class
class globalVar {
public:
	 Graph verticesList;

	 std::vector<GLint> texture;
	 std::vector<lines> collectiveLinesList;
	 std::vector<points> collectivePointList;
	 std::vector<faces> faceList;

	 globalVar();



};
extern std::vector<shapes> shapearray;
extern std::vector<lines> previewLineList;
extern std::vector<lines> selectionList;
extern std::vector<faces> selectedFace;
extern GLdouble posX, posY, posZ;
extern int xOrigin, yOrigin; // x and y position of the screen.

extern bool wireflag;
#endif
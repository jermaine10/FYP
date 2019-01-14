#pragma once
#include <stdlib.h>
#include <math.h>
#include "GL/glut.h"
#include <sstream>
#include <sstream>
#include <tuple> 
#include <string>
#include <Windows.h>
#include "GL/glui.h"
#include "Graph.h"
#include "3dmaths.h"
#include "drawObjects.h"
#include "texture.h"
#include "globalVar.h"


#define NORMALMODE 400
#define VIEWCHANGINGMODE 401
#define DRAWINGMODE		402
#define OBJECTCHANGINGMODE 403
#define MOVINGMODE 404
#define POINTMOVEMODE 405
#define ROTATEMODE 406
#define SCALEMODE 407
#define COPYINGMODE 408
#define RESETID 100
#define CMDSUBMIT 101

#define ENABLE_ID            300
#define DISABLE_ID           301
#define SHOW_ID              302
#define HIDE_ID              303
#define LISTBOXCOLOR	 305
#define LISTBOXTEXTURE	306
#define OPENTEXTURE		307

#define WIREFRAMEVIEW 0
#define APPEARANCEVIEW 1

int curColorId=0;


int newX = 0, newY = 0;
static int pointID = 0;
int currDrawMode = LINES;
int currtexture = 0;

//#### GUI
int drawFlag = 0, viewFlag = 0, moveFlag = 0, copyFlag = 0, normalFlag = 1, scaleFlag = 0, rotateFlag = 0;
int lineFlag = 1, rectangleFlag = 0, cubesFlag = 0;

GLUI * subwindow1 = 0;
GLUI * subwindow2 = 0;
GLUI * subwindow3 = 0;
GLUI_Listbox *texturelist;
GLUI_Listbox *colorlist;

int mainWindow = 0;
GLUI *commandGUIWINDOW = 0, *glui;
GLUI_CommandLine *commandLineGUI;

//#### GUI ENDS

globalVar datastruct;

//@@@@@@@@@@  DISPLAY AND CAMERA VARIABLES  @@@@@@@@@@
GLdouble modelview[16];
int zFar = 1000;
int axisPos;

GLint viewport[4];
GLdouble projection[16];
float zoomfactor = 5.0f;
int rotX = 0, rotY = 0;
float deltaMove = 0;
float cdx = 0.0f, cdz = -1.0f, cdy = 0.0f;
float cx = 0.0f, cz = zFar / 2, cy = 0.0f;
float cenX = 0.0f, cenY = 0.0f, cenZ = 0.0f;
GLfloat hratio, wratio;
float deltaAngleY = 0.0f;
float deltaAngleX = 0.0f;
int ww, wh;
int zNear = 1;
bool rotate = true;

//@@@@@@@@@@ DISPLAY AND CAMERA VARIABLES @@@@@@


// OBJECT MANIPULATION AND COORDINATES
GLdouble tx = 0, ty = 0, tz = 0;
int objcount = 0;
static int count;
GLfloat r = 1.0, g = 1.0, b = 1.0;
float thickness = 2.0f;
int currentMode = NORMALMODE;
bool shift = false, alt = false, ctrl = false, middleBut = false;
std::vector<points> flaggedPoints;

points referencePoints;
int rotOriginX, rotOriginy;
double angleOrigin;
std::vector<std::tuple<int, int, points>> tempFlagPoints;
points pointsFlaggedNear;
points flaggedPointsMove;
std::pair<int, int> flagged2Dpoints; // to draw yellow boxes

std::string textbox = "test";
GLUI_TextBox *moo;

									 //DATA STRUCTURE



//texture


void calcposition(int, int);

void mouseMove(int, int);

void draw2Dobject();

void resetModelView();

void createObject(int, points &, points &, std::vector<lines>&,bool);

void updateText(int);

void mouseButton(int, int, int, int);

void moveAllSelected(float, float, float, float, float, float);

void snapFunct(int, int);

void setFlag();

void keys(unsigned char, int, int);

void init();

void modeControl(int control);

void createFuncControl();

void createGUI();

void changeSize(int, int);

void reload();

void renderScene(void);

void displayInfo();

void checkActiveShift();

void createGUI();

void setTextBox(std::string);

void defaultMode();

void filterSelection();

bool tryMovePoints();

void idleFunc(void);

void computeZPos(float);

void reCalcPoints();

bool insideBoundingBox(lines l);

bool isInsideBoundingPlane(faces f);

bool intersect(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);

void computeSelection();

void computeYpos(float);


bool comparePoints(lines l1, lines l2);


void moveObjects(points);

void scaleObject(float);

void scaleObjectByFactor(float);

void moveObjectPoints(points,bool);

void copyObject(points);

void getAxis();

void splittingLines();

void creatingFaces();

void changeMode(int);

void changeDrawMode(int);

void commandControl(GLUI_Control* control);

void checkPoints(points&);

void changecolor(GLfloat, GLfloat, GLfloat);

void pressKey(int, int, int);

void releaseKey(int, int, int);

bool faceLooping(points originPoint, points currentPoint, std::vector<lines> tempFace);

void mouseMovePassive(int, int);

void moveCameras(int, int);



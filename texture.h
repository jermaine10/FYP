#pragma once
#include "openfile.h"
#include <iostream>
#include "GL\SOIL.h"
#include "GL\glut.h"
#include <vector>
#include "globalVar.h"
#include <string>

// texture of the object

int loadTextureBox();
std::vector<int> loadDefaultTexture();
	int loadTexture(TCHAR*);
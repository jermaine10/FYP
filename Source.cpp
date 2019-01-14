#include <math.h>
#include "Source.h"
// start of shapes data structure

// ############ START OF RENDERING #######################
// #######################################################
// ######################################################
void changeSize(int w, int h) {
	ww = w;
	wh = h;
	if (ww <= wh) {
		hratio = (GLfloat)ww / (GLfloat)wh;
		wratio = 1;
	}
	else {
		hratio = 1;
		wratio = (GLfloat)wh / (GLfloat)ww;
	}

	if (w == 0)
		w = 1;
	glViewport(0, 0, (GLsizei)ww, (GLsizei)wh);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glutPostRedisplay();

}



void reload() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-hratio* zoomfactor, hratio* zoomfactor, -wratio * zoomfactor, wratio * zoomfactor, zNear, zFar);

	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void renderScene(void) {
	if (deltaMove) {
		computeZPos(deltaMove);
		glutPostRedisplay();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reload();
	gluLookAt(cx, cy, cz,
		cenX, cenY, cenZ,
		0.0f, 1.0f, 0.0f);
	glRotatef(rotX, 1.0f, 0, 0);
	glRotatef(rotY, 0, 1.0f, 0);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(cx, cy, zFar, cenX, cenY, cenZ, 0.0f, 1.0f, 0.0f);
	glRotatef(rotX, 1.0f, 0, 0);
	glRotatef(rotY, 0, 1.0f, 0);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
	drawAxis();

	draw3Dobjects(datastruct);
	displayInfo();
	//#######################################################################3
	//#######################################################################
	reload();
	glutSwapBuffers();
	glutPostRedisplay();
	subwindow1->sync_live();
	Sleep(10);
}

void displayInfo() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, ww, 0.0, wh);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 150);
	draw2Dobject();
	if (currentMode == NORMALMODE) {
		selectionDraw(xOrigin,yOrigin,newX,newY);
		reCalcPoints();
	}
	std::stringstream oss;
	oss << " POS X : " << posX << " POS Y : " << posY << " POS : z " << posZ;
	std::string tempString = oss.str();
	glColor3f(0, 0, 0);

	for (int i = 0; i < tempString.size(); i++) {

		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, tempString[i]);
	}
	glEnable(GL_DEPTH_TEST);
}

void resetModelView() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-hratio* zoomfactor, hratio* zoomfactor, -wratio * zoomfactor, wratio * zoomfactor, zNear, zFar);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cx, cy, (cz),
		cenX, cenY, cenZ,
		0.0f, 1.0f, 0.0f);
	glRotatef(rotX, 1.0f, 0, 0);
	glRotatef(rotY, 0, 1.0f, 0);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

}





//misc utilities

void checkActiveShift() {
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		compareDistance(posX,posY,posZ,tx,ty,tz);
	}
}


// From here on its object manipulation
bool tryMovePoints() {
	if (flagged2Dpoints.first == -999999)
		return false;
	return true;
}


bool insideBoundingBox(lines l) {
	GLdouble x1, y1, z1, x2, y2, z2;
	GLdouble temp;
	gluProject(l.getFirst().getX(), l.getFirst().getY(), l.getFirst().getZ(), modelview, projection, viewport, &x1, &y1, &z1);
	gluProject(l.getSecond().getX(), l.getSecond().getY(), l.getSecond().getZ(), modelview, projection, viewport, &x2, &y2, &z2);
	if (newX < xOrigin) {
		temp = newX;
		newX = xOrigin;
		xOrigin = temp;
	}if (newY < yOrigin) {
		temp = newY;
		newY = yOrigin;
		yOrigin = temp;
	}
	if ((xOrigin <= x1 && x1 <= newX) && (yOrigin <= y1&&y1 <= newY) ||
		(xOrigin <= x2&& x2 <= newX) && (yOrigin <= y2 &&y2 <= newY))
		return true;

	if (intersection2D(x1, y1, x2, y2, xOrigin, yOrigin, xOrigin, newY) ||
		intersection2D(x1, y1, x2, y2, xOrigin, yOrigin, newX, yOrigin) ||
		intersection2D(x1, y1, x2, y2, newX, yOrigin, newX, newY) ||
		intersection2D(x1, y1, x2, y2, xOrigin, newY, newX, newY))
		return true;

	return false;
}


void computeSelection() {
	if (fabs(newX - xOrigin) < 2)
		xOrigin = newX + 5;
	if (fabs(newY - yOrigin) < 2)
		yOrigin = newY + 5;
	flaggedPoints.clear();
	selectionList.clear();
	selectedFace.clear();
	for (lines l : datastruct.collectiveLinesList) {
		if (insideBoundingBox(l)) {
			selectionList.push_back(l);
			flaggedPoints.push_back(l.getFirst());
			flaggedPoints.push_back(l.getSecond());

		}
	}
	for (faces f : datastruct.faceList) {

		if (isInsideBoundingPlane(f,datastruct.collectivePointList,posX,posY,posZ)) {
			selectedFace.push_back(f);
		}
	}
}



bool comparePoints(lines l1, lines l2) {
	std::vector<float> test, test2;
	l1.getMaxMinInNum(test);
	l2.getMaxMinInNum(test2);

	if (test[0] > test2[3])
		return false;
	return true;
}

void moveObjects(points p)
{
	points temp;
	for (lines& l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
			l.moveAllCoordinates(p.getX(), p.getY(), p.getZ());
			temp = l.getFirst();
			checkPoints(temp);
			l.setFirst(temp);
			temp = l.getSecond();
			checkPoints(temp);
			l.setSecond(temp);
			datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());

		}
	}
}

void scaleObject(float scaleFactor) {
	points temp;
	for (lines& l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
			l.moveAllCoordinates(-tx, -ty, -tz);
			l.scaleObjectByFactor(scaleFactor);
			l.moveAllCoordinates(tx, ty, tz);
			temp = l.getFirst();
			checkPoints(temp);
			l.setFirst(temp);
			temp = l.getSecond();
			checkPoints(temp);
			l.setSecond(temp);
			datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());
		}
	}

}
void scaleObjectByFactor(float scaleFactor) { //scale object by reference
	for (lines& l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			l.scaleObjectByFactor(scaleFactor);

		}
	}

}
void rotateObjectIn3D(double angle, points referencePoint) {//rotating angle by angle reference and axis
	points temp;
	for (lines& l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
			l.rotate3D(axisPos, angle, referencePoint);
			temp = l.getFirst();
			checkPoints(temp);
			l.setFirst(temp);
			temp = l.getSecond();
			checkPoints(temp);
			l.setSecond(temp);
			datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());

		}
	}
}


void moveObjectPoints(points p,bool flag) {// move every points connected to a specific object
										//flag is used to check if its a preview or if it should be saved
	points temp;
	std::vector<points> tempcoordinates;
	for (lines l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			tempcoordinates.push_back(l.getFirst());
			tempcoordinates.push_back(l.getSecond());
		}
	}
	for (points p2 : tempcoordinates) {
		points temp = p2;
		temp.moveXYZ(p.getX(), p.getY(), p.getZ());
		if (flag) {
			checkPoints(temp);
			for (lines& l : datastruct.collectiveLinesList) {
				if (l.coordinatesExist(p2)) {
					datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
					l.moveCoordinates(p2, temp);
					if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
						points temp2 = l.returnOther(p2);
						l.moveCoordinates(temp2, (temp2 + p));
					}
					datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());

				}
			}
		}
		else {
			for (lines& l : previewLineList) {
				l.moveCoordinates(p2, temp);
				if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
					points temp2 = l.returnOther(p2);
					l.moveCoordinates(temp2, (temp2 + p));
				}
			}
		}

	}
}
void copyObject(points p)
{
	std::vector<lines> templines;
	for (lines l : datastruct.collectiveLinesList) {
		if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
			templines.push_back(l);
		}
	}
	moveObjects(p);
	for (lines l : templines) {
		datastruct.collectiveLinesList.push_back(l);
		datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());
	}
}
void getAxis() {
	axisPos = compareDistance(posX, posY, posZ, tx, ty, tz);
}




void changecolor(GLfloat tr, GLfloat tg, GLfloat tb) {
	if (selectionList.size() != 0) {
		for (lines& l : datastruct.collectiveLinesList) {
			if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {

				l.setRGB(tr, tg, tb);
			}
		}
		
	}
	if (selectedFace.size() != 0) {
		for (faces& f : datastruct.faceList) {
			if (std::find(selectedFace.begin(), selectedFace.end(), f) != selectedFace.end()) {
				f.setLineColor(tr, tg, tb);
			}	
		}
	}
}


/*
################################

*/

//cameras
void moveCameras(int x, int y) {

	deltaAngleY = (viewport[3] - y - yOrigin) * 0.001f;
	deltaAngleX = (x - xOrigin) *0.001f;

	if (rotate) {
		// update deltaAngleY
		cenY += deltaAngleY;
		cy += deltaAngleY;

		cenX += deltaAngleX;
		cx += deltaAngleX;
	}
	else {
		int tx1 = xOrigin - x;
		int ty1 = yOrigin - y;
		rotY = (rotY + abs(tx1) / 50) % 360;
		rotX = (rotX + abs(ty1) / 50) % 360;

	}
	reload();
}

void computeZPos(float deltaMove) {
	cz += deltaMove * cdz * 0.1f;
}


// most important in translating 2D to 3D
// used with Z buffer so it knows if there is object blocking the ray
// also using ray detection
void calcposition(int x, int y) {
	float winX, winY, winZ;
	winX = (float)x;
	winY = (float)viewport[3] - (float)y - 1;
	if ((winX - 2) <= flagged2Dpoints.first && flagged2Dpoints.first <= (winX + 2) &&
		(winY - 2) <= flagged2Dpoints.second && flagged2Dpoints.second <= (winY + 2)) {
		posX = pointsFlaggedNear.getX();
		posY = pointsFlaggedNear.getY();
		posZ = pointsFlaggedNear.getZ();

	}
	else {
		glReadPixels(winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		int wiZ = winZ;
		if (wiZ < 1) 
			resetModelView(); //ray casting
			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	}
}


//some simple pointer help to make drawing easier
void draw2Dobject() {
	GLdouble xval, yval, zval;
	for (points p : flaggedPoints) {
		glPolygonMode(GL_FRONT, GL_FILL);
		gluProject(p.getX(), p.getY(), p.getZ(), modelview, projection, viewport, &xval, &yval, &zval);
		glColor3f(0, 0, 1);
		glRectf(xval - 2, yval - 2, xval + 2, yval + 2);
	}
	if (flagged2Dpoints.first != -999999) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 1, 0);
		glRectf(flagged2Dpoints.first - 4, flagged2Dpoints.second - 4, flagged2Dpoints.first + 4, flagged2Dpoints.second + 4);
	}
}


//snap function  also to make drawing easy
void check2Dpointers(int x, int y) {
	int tempX = x, tempY = viewport[3] - y - 1;
	std::find_if(tempFlagPoints.begin(), tempFlagPoints.end(), [&tempX, &tempY](const std::tuple<int, int, points> p) {
		if ((std::get<0>(p) - 2 <= tempX && tempX <= std::get<0>(p) + 2) && (std::get<1>(p) - 2 <= tempY && tempY <= std::get<1>(p) + 2)) {
			flagged2Dpoints = std::make_pair(tempX, tempY);
			pointsFlaggedNear = std::get<2>(p);
			return true;
		}

		flagged2Dpoints = std::make_pair(-999999, -999999);
		return false;
	});
}


void reCalcPoints() {
	tempFlagPoints.clear();
	double tempx2d, tempy2d, test;
	for (lines l : datastruct.collectiveLinesList) {
		points p1 = l.getFirst();
		points p2 = l.getSecond();
		points tempp = (p2 + p1) / 2;
		gluProject(p1.getX(), p1.getY(), p1.getZ(), modelview, projection, viewport, &tempx2d, &tempy2d, &test);
		tempFlagPoints.push_back(std::make_tuple(tempx2d, tempy2d, p1));
		gluProject(p2.getX(), p2.getY(), p2.getZ(), modelview, projection, viewport, &tempx2d, &tempy2d, &test);
		tempFlagPoints.push_back(std::make_tuple(tempx2d, tempy2d, p2));
		gluProject(tempp.getX(), tempp.getY(), tempp.getZ(), modelview, projection, viewport, &tempx2d, &tempy2d, &test);
		tempFlagPoints.push_back(std::make_tuple(tempx2d, tempy2d, tempp));
	}

}



//#############            OBJECTS CREATION        #################

// making sure points dont double 
//used with everything that involves with creating and modifying points
void checkPoints(points& p1) {
	int pos = std::find(datastruct.collectivePointList.begin(), datastruct.collectivePointList.end(), p1) - datastruct.collectivePointList.begin();
	if (pos >= datastruct.collectivePointList.size()) {

		p1.setID(pointID);
		pointID++;
		datastruct.collectivePointList.push_back(p1);
	}
	else {


		p1.setID(datastruct.collectivePointList[pos].getID());

	}
}




// object Creation, bool is used to indicate if the program should preserve it in the data structure.
void createObject(int cond, points& p1, points& p2, std::vector<lines>& dataArray,bool flag) {
	switch (cond) {
	case LINES: {
	
		if (flag) {
			checkPoints(p1);
			checkPoints(p2);

		}	
		if (!(std::find(dataArray.begin(), dataArray.end(), lines(p1, p2)) != dataArray.end())) {
			lines l(p1, p2);
			l.setRGB(r, g, b);
			if ( flag== true) {
				datastruct.verticesList.addEdge(p1.getID(),p2.getID());
			}
			dataArray.insert(dataArray.end(), l);

		}
	}
		break;
	case RECTANGLE: {
		int xFlag = 0;
		if (currDrawMode == RECTANGLE)
			xFlag=  limitMovement(p1, p2);
		points p3;
		points p4;
		
		if (xFlag == 1) {
			 p3= points(p1.getX(), p2.getY(), p1.getZ());
			 p4 = points(p2.getX(), p1.getY(), p2.getZ());

		}
		else {
			 p3 = points(p1.getX(), p2.getY(), p2.getZ());
			 p4 = points(p2.getX(), p1.getY(), p1.getZ());

		}
		std::vector<lines> linelist;
		bool iscollective = false;
		if (flag == true) {
			iscollective = true;
			checkPoints(p1);
			checkPoints(p2);
			checkPoints(p3);
			checkPoints(p4);
		}
		std::vector<points> pointlist = { p1,p2,p3,p4 };

		for (int i = 0; i < 2; i++) {
			linelist.push_back(lines(pointlist[i], pointlist[2]));
			linelist.push_back(lines(pointlist[i], pointlist[3]));
			if (iscollective) {
				datastruct.verticesList.addEdge(pointlist[i].getID(), pointlist[2].getID());
				datastruct.verticesList.addEdge(pointlist[i].getID(), pointlist[3].getID());
			}
		}
		dataArray.insert(dataArray.end(), linelist.begin(), linelist.end());

	}
					break;

	case CUBES: {
	
		std::vector<points> temppoints;
		float tempx[] = { p1.getX(),p2.getX() };
		float tempy[] = { p1.getY(),p2.getY() };
		float tempz[] = { p1.getZ(),p2.getZ() };

		int counter = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					points temppoint(tempx[i], tempy[j], tempz[k]);
					if (flag) 
						checkPoints(temppoint);
					temppoints.push_back(temppoint);
				}
			}
		}

		for (int i = 0; i < (temppoints.size() - 1); i++) {
			for (int j = i + 1; j < temppoints.size(); j++) {
				// c = getx = getx
				// b = gety = gety
				// a = get z = getz
				if (((temppoints[i].getX() == temppoints[j].getX())
					&&
					((temppoints[i].getY() == temppoints[j].getY())
						||
						(temppoints[i].getZ() == temppoints[j].getZ()))
					) ||
					(
						((temppoints[i].getY() == temppoints[j].getY())
							&&
							(temppoints[i].getZ() == temppoints[j].getZ()))
						)
					) {
					lines templine(temppoints[i], temppoints[j]);
					if (flag) {
						datastruct.verticesList.addEdge(temppoints[i].getID(), temppoints[j].getID());
					}
					dataArray.push_back(templine);
				}
			}

		}
	}
				break;
	}

}

// Face creation, using some path finding	algorithm to find paths between lines to form a face
void creatingFaces() {

	std::vector < std::vector<int>> faceLoops = datastruct.verticesList.BFS();
	datastruct.faceList.erase(std::remove_if(datastruct.faceList.begin(), datastruct.faceList.end(), [&, faceLoops](faces f) {
		if (std::find(faceLoops.begin(), faceLoops.end(), f.getPointList()) == faceLoops.end())
			return true;
		return false;

	}), datastruct.faceList.end());
	for (std::vector<int> linelists : faceLoops) {
		if (std::find(datastruct.faceList.begin(), datastruct.faceList.end(), linelists) == datastruct.faceList.end()) {
			std::vector<points> pointlist;
			for (int i : linelists) {
				points px = datastruct.collectivePointList.at(i);
				pointlist.push_back(px);
			}

			std::vector<lines> linelist;
			for (int i = 0; i < pointlist.size(); i++) {
				if (i < pointlist.size() - 1) {
					linelist.push_back(lines(pointlist[i], pointlist[i + 1]));
				}
				else if (i == pointlist.size() - 1) {
					linelist.push_back(lines(pointlist[i], pointlist[0]));
				}

			}

			faces f(linelist);
			if (linelists.size() == 4) {
				f.setTextCoord(4);

			}
			else if (linelist.size() == 3) {
				f.setTextCoord(3);
			}
			f.setAppearanceID(currtexture);
			f.r = r;
			f.g = g;
			f.b = b;
			f.setPoint(linelists);
			datastruct.faceList.push_back(f);

		}
	}
}


void splittingLines() {
	std::vector<lines> tempCollectiveLinesList = datastruct.collectiveLinesList;
	for (int i = 0; i < datastruct.collectiveLinesList.size(); i++) {
		bool test4 = false;
		GLfloat tr, tb, tg, tr2, tb2, tg2;
		tr = tempCollectiveLinesList[i].getRed();
		tb = tempCollectiveLinesList[i].getBlue();
		tg = tempCollectiveLinesList[i].getGreen();
		for (int j = i; j < datastruct.collectiveLinesList.size(); j++) {
			if (i != j) { // test to reduce calculation

				points interSect = points();

				if (comparePoints(datastruct.collectiveLinesList[i], datastruct.collectiveLinesList[j])) { // again simplifying calculation times
					if (intersection3D(tempCollectiveLinesList[i], tempCollectiveLinesList[j], interSect)) {
						checkPoints(interSect);
						lines newLines(tempCollectiveLinesList[i].getSecond(), interSect);
						lines newLines2(tempCollectiveLinesList[i].getFirst(), interSect);
						lines newLines3(tempCollectiveLinesList[j].getSecond(), interSect);
						lines newLines4(tempCollectiveLinesList[j].getFirst(), interSect);
						newLines.setRGB(tr, tg, tb);
						tempCollectiveLinesList.push_back(newLines);
						datastruct.verticesList.addEdge(tempCollectiveLinesList[i].getSecond().getID(), interSect.getID());
						tr2 = tempCollectiveLinesList[j].getRed();
						tb2 = tempCollectiveLinesList[j].getBlue();
						tg2 = tempCollectiveLinesList[j].getGreen();
						newLines2.setRGB(tr, tg, tb);
						datastruct.verticesList.addEdge(tempCollectiveLinesList[i].getFirst().getID(), interSect.getID());
						tempCollectiveLinesList.push_back(newLines2);
						newLines3.setRGB(tr2, tg2, tb2);
						newLines4.setRGB(tr2, tg2, tb2);
						tempCollectiveLinesList.push_back(newLines3);
						datastruct.verticesList.addEdge(tempCollectiveLinesList[j].getSecond().getID(), interSect.getID());
						tempCollectiveLinesList.push_back(newLines4);
						datastruct.verticesList.addEdge(tempCollectiveLinesList[j].getFirst().getID(), interSect.getID());
						datastruct.verticesList.removeEdge(tempCollectiveLinesList[j].getFirst().getID(), tempCollectiveLinesList[j].getSecond().getID());
						datastruct.verticesList.removeEdge(tempCollectiveLinesList[i].getFirst().getID(), tempCollectiveLinesList[i].getSecond().getID());
						tempCollectiveLinesList.erase(std::remove(tempCollectiveLinesList.begin(), tempCollectiveLinesList.end(), tempCollectiveLinesList[j]), tempCollectiveLinesList.end());
						tempCollectiveLinesList.erase(std::remove(tempCollectiveLinesList.begin(), tempCollectiveLinesList.end(), tempCollectiveLinesList[i]), tempCollectiveLinesList.end());
						tempCollectiveLinesList[i];
						datastruct.collectiveLinesList = tempCollectiveLinesList;
						test4 = true;

					}
				}
			}

		}

	}
}
//#############            OBJECTS CREATION ENDED        #################


 //###### INPUT HANDLERS

//mouse event handler
void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			if (currentMode == NORMALMODE) {
				computeSelection();
				xOrigin = 0;
				yOrigin = 0;
				newX = 0;
				newY = 0;
			}
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = viewport[3] - y;

			switch (currentMode) {
			case NORMALMODE: {
				if (tryMovePoints()) {
					for (lines l : datastruct.collectiveLinesList) {
						if (l.coordinatesExist(pointsFlaggedNear)) {
							selectionList.push_back(l);
						}
					}
					flaggedPointsMove = pointsFlaggedNear;
					changeMode(POINTMOVEMODE);
					setTextBox("Select point to resize the line");
				}
				else {
					newY = yOrigin;
					newX = xOrigin;
					tx = posX;
					ty = posY;
					tz = posZ;
				}
			}
				 break;

			case POINTMOVEMODE: {
			
				if (count == 0) {
					points temp(posX,posY,posZ);
					checkPoints(temp);

					for (lines& l : datastruct.collectiveLinesList) {
						if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()){
						datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
						l.moveCoordinates(flaggedPointsMove, temp);
						datastruct.verticesList.addEdge(l.getFirst().getID(), l.getSecond().getID());
						}

					}

				}
				defaultMode();
			}
								break;

			case SCALEMODE:
			{
				if (count == 0) {
					tx = posX;
					ty = posY;
					tz = posZ;
					count++;
				}
				else {
					float scaleFactor = sqrt((posX - tx)*(posX - tx) + (posY - ty)*(posY - ty) + (posZ - tz)*(posZ - tz));
					scaleObject(scaleFactor);
					//to do scale object
					defaultMode();

					reCalcPoints();
				}
			}
			break;
			case COPYINGMODE: {
				if (count == 0) {
					tx = posX;
					ty = posY;
					tz = posZ;
					count++;
				}
				else {
					tx = posX - tx;
					ty = posY - ty;
					tz = posZ - tz;
					copyObject(points(tx, ty, tz));
					defaultMode();

				}
			}break;
			case ROTATEMODE: {
				if (count == 0) {
					tx = posX;
					ty = posY;
					tz = posZ;
					referencePoints = points(tx, ty, tz);
					count++;

					rotOriginX = x;
					rotOriginy = viewport[3] - y;
					setTextBox("set reference point for rotate");

					break;
				}
				else if (count == 1) {
					angleOrigin = atan2(((viewport[3] - y) - rotOriginy), (x - rotOriginX));
					getAxis(); // getting the axis by the position
					count++;
					setTextBox("set angle of rotation");

					break;

				}
				else if (count == 2) {

					double angle = atan2(((viewport[3] - y) - rotOriginy), (x - rotOriginX)) - angleOrigin;
					rotateObjectIn3D(angle, referencePoints);
					count = 0;
					defaultMode();
					reCalcPoints();
					break;
				}


			}
							 break;
			case DRAWINGMODE: {
				if (count == 0) {
					setTextBox("select position to create object");
					tx = posX;
					ty = posY;
					tz = posZ;
					count++;
				}
				else {
					checkActiveShift();
					points p1(tx, ty, tz);
					points p2(posX, posY, posZ);
			
					createObject(currDrawMode, p1, p2, datastruct.collectiveLinesList,true);

					reCalcPoints();
					defaultMode();


				}
			}
							  break;
			case MOVINGMODE: {
				if (count == 0) {
					setTextBox("set distance of moving");
					tx = posX;
					ty = posY;
					tz = posZ;
					count++;
				}
				else {
					checkActiveShift();
					tx = posX - tx;
					ty = posY - ty;
					tz = posZ - tz;
				
					setTextBox(("Point entered is X : " + std::to_string(tx) + " Y : " + std::to_string(ty) + " Z : " + std::to_string(tz)));
					if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
						moveObjectPoints(points(tx,ty,tz),true);
					}
					else {
						moveObjects(points(tx, ty, tz));
					}
					defaultMode();
					reCalcPoints();

				}
			}
							 break;
			}
		}

	}
	else if (button == 3) {
		if (state == GLUT_DOWN) {
			zoomfactor += 0.03f;
			reCalcPoints();
		}
		else deltaMove = 0.0f;
	}
	else if (button == 4) {
		if (state == GLUT_DOWN) {
			if (zoomfactor > 0.2f)
				zoomfactor -= 0.03f;
			reCalcPoints();

		}
		else deltaMove = 0.0f;
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			middleBut = true;
			xOrigin = x;
			yOrigin = viewport[3] - y;
			newX = x;
			newY = viewport[3] - y;

		}
		else {
			middleBut = false;
			xOrigin = -1;
			yOrigin = -1;
			newX = -1;
			newY = -1;

		}
	}
	reload();
	setFlag();
}

//mouse moving when button is clicked
void mouseMove(int x, int y) {
	if (currentMode == VIEWCHANGINGMODE || middleBut == true) {
		moveCameras(x, y);
	}
	else {
		newX = x;
		newY = viewport[3] - y;
	}
	tx = posX;
	ty = posY;
	tz = posZ;
	reload();
	glutPostRedisplay();
}

//mouse moving when no button is pressed
void mouseMovePassive(int x, int y) { // mouse move when not clicking
	check2Dpointers(x, y);
	calcposition(x, y);

	previewLineList.clear();

	switch (currentMode) {
	case DRAWINGMODE:
		if (count != 0) {
			checkActiveShift();
			points p1(tx, ty, tz);
			points p2(posX, posY, posZ);
			createObject(currDrawMode, p1, p2, previewLineList, false);

		}
		break;

	case MOVINGMODE: {
		if (count != 0) {
			GLdouble tx2 = posX - tx;
			GLdouble ty2 = posY - ty;
			GLdouble tz2 = posZ - tz;
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
				previewLineList = datastruct.collectiveLinesList;
				moveObjectPoints(points(tx2, ty2, tz2), false);

			}
			else {
				previewLineList = selectionList;
				for (lines& l : previewLineList) {
					l.moveAllCoordinates(tx2, ty2, tz2);
				}
			}
		}
	}
					 break;

	case POINTMOVEMODE: {
		previewLineList = selectionList;
		points temp(posX, posY, posZ);
		for (lines& l : previewLineList) {
			l.moveCoordinates(flaggedPointsMove, temp);
		}
		//todo
	}
						break;

	case SCALEMODE: {
		if (count != 0) {
			previewLineList = selectionList;

			for (lines& l : previewLineList) {
				float scaleFactor = sqrt((posX - tx)*(posX - tx) + (posY - ty)*(posY - ty) + (posZ - tz)*(posZ - tz));
				l.moveAllCoordinates(-tx, -ty, -tz);
				l.scaleObjectByFactor(scaleFactor);
				l.moveAllCoordinates(tx, ty, tz);

			}
		}
	}
					break;
	case ROTATEMODE: {
		if (count == 2) {
			previewLineList = selectionList;
			double angle = atan2(((viewport[3] - y) - rotOriginy), (x - rotOriginX)) - angleOrigin;
			for (lines& l : previewLineList) {
				l.rotate3D(axisPos, angle, referencePoints);
			}
		}
	}
					 break;

	}

	renderScene();

}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

// KEYBOARD HANDLER FOR THE SOFTWARE
void keys(unsigned char key, int x, int y) {
		std::cout << key << std::endl;

		switch (key) {
		case 'c': {
			if (currentMode != COPYINGMODE)
				changeMode(COPYINGMODE);
			else
				changeMode(NORMALMODE);

		}
				  break;
		case 'z'://zoom mode
		{
			if (currentMode != VIEWCHANGINGMODE)
				changeMode(VIEWCHANGINGMODE);
			else
				changeMode(NORMALMODE);
		}
		break;
		case 'd'://drawing mode
		{
			if (currentMode != DRAWINGMODE)
				changeMode(DRAWINGMODE);
			else
				changeMode(NORMALMODE);
		}
		break;
		case 'r': {
			if (currentMode != ROTATEMODE)
				changeMode(ROTATEMODE);
			else
				changeMode(NORMALMODE);
		}
				  break;

		case 's': {
			if (currentMode != SCALEMODE)
				changeMode(SCALEMODE);
			else
				changeMode(NORMALMODE);
		}
				  break;

		case 'm'://moving mode
		{
			if (currentMode != MOVINGMODE)
				changeMode(MOVINGMODE);

			else
				changeMode(NORMALMODE);
		}
		break;
		case '1'://change rotate
		{
			if (!rotate) {
				rotate = true;
			}
			else
				rotate = false;
		}
		break;
		case '2': //zoom +
		{
			zoomfactor += 10;
			reCalcPoints();

		}
		break;
		case '3': //zoom-
		{
			zoomfactor -= 10;
			if (zoomfactor <= 0) {
				zoomfactor = 0.1;
				reCalcPoints();

			}
			int counts = 0;

		}
		break;
		case '4': {
			createObject(LINES, points(0, 0, 0), points(10, 10, 0), datastruct.collectiveLinesList, true);


		}break;
		case '5': {
			createObject(LINES, points(0, 10, 0), points(10, 0, 0), datastruct.collectiveLinesList, true);
			createObject(LINES, points(0, 10, 0), points(0, 0, 0), datastruct.collectiveLinesList, true);
			splittingLines();
			creatingFaces();

		}break;
		case'!':
		{
			wireflag = false;

		}
		break;
		case'#':
		{
			wireflag = true;
		}
		break;

		case'`': {
			std::cout << "total line" << datastruct.collectiveLinesList.size() << std::endl;

			std::cout << "total faces" << datastruct.faceList.size() << std::endl;
		}
				 break;


		case 27: {
			changeMode(NORMALMODE);
			flaggedPoints.clear();
			selectionList.clear();

		}
				 break;
		case 127: {
			for (lines l : selectionList) {
				datastruct.collectiveLinesList.erase(std::remove(datastruct.collectiveLinesList.begin(), datastruct.collectiveLinesList.end(), l), datastruct.collectiveLinesList.end());
				datastruct.verticesList.removeEdge(l.getFirst().getID(), l.getSecond().getID());
			}
			creatingFaces();

			flaggedPoints.clear();
			selectionList.clear();

		} // esc key
				  break;
		case '6':
		{
			changeDrawMode(LINES);
		}
		break;
		case '7':
		{
			changeDrawMode(RECTANGLE);
		}
		break;
		case '8':
		{
			changeDrawMode(CUBES);
		}
		break;
		}
		setFlag();
	

}

//FROM HERE ON IS GUI AND MODE SWITCHING
void setFlag() {
	drawFlag = 0;
	viewFlag = 0;
	moveFlag = 0;
	normalFlag = 0;
	rotateFlag = 0;
	scaleFlag = 0;
	rectangleFlag = 0;
	lineFlag = 0;
	cubesFlag = 0;
	copyFlag = 0;
	if (currentMode == NORMALMODE)
		normalFlag = 1;
	if (currentMode == VIEWCHANGINGMODE)
		viewFlag = 1;
	if (currentMode == MOVINGMODE)
		moveFlag = 1;
	if (currentMode == DRAWINGMODE)
		drawFlag = 1;
	if (currentMode == SCALEMODE)
		scaleFlag = 1;
	if (currentMode == ROTATEMODE)
		rotateFlag = 1;
	if (currentMode == COPYINGMODE)
		copyFlag = 1;

	if (currDrawMode == LINES)
		lineFlag = 1;
	else if (currDrawMode == RECTANGLE)
		rectangleFlag = 1;
	else if (currDrawMode == CUBES)
		cubesFlag = 1;
}

void changeMode(int mode) {
	currentMode = mode;
	updateText(mode);
	posX = tx, posY = ty, posZ = tz;
	newX = 0, yOrigin = 0;
	count = 0;
	setFlag();
}
void updateText(int mode) {
	std::string s;
	switch (mode) {
	case DRAWINGMODE:
		s = "Entering draw mode, please enter values or click the screen";
		break;
	case VIEWCHANGINGMODE:
		s = "press 1 to enable rotating, mouse click on a point to rotate view";
		break;
	case SCALEMODE:
		s = " select point for reference / enter the command line for default reference";
		break;
	case ROTATEMODE:
		s = "Select point as rotation origin";
		break;
	}
	setTextBox(s);
}
void changeDrawMode(int drawMode) {
	currDrawMode = drawMode;
	setFlag();

}
void changeDefaultColor(int colorID) {
	if (colorID == 0) { r = 1.0f, g = 1.0f, b = 1.0f; }
	if (colorID == 1) { r = 1.0f, g = 0.0f, b = 0.0f; }
	if (colorID == 2) { r = 0.0f, g = 1.0f, b = 0.0f; }
	if (colorID == 3) { r = 0.0f, g = 0.0f, b = 1.0f; }
	if (colorID == 4) { r = 0.0f, g = 1.0f, b = 1.0f; }
	if (colorID == 5) { r = 1.0f, g = 0.0f, b = 1.0f; }
	if (colorID == 6) { r = 1.0f, g = 1.0f, b = 0.0f; }
	changecolor(r, g, b);

}
void changeTexture(int textureId){
	if (selectedFace.size() != 0) {
		
		for (faces& f : datastruct.faceList) {
			if (std::find(selectedFace.begin(), selectedFace.end(), f) != selectedFace.end()) {
				f.setAppearanceID(textureId);
			}

		}
	}
}
void setTextBox(std::string s) {
	textbox = s + "\n" + textbox;
	moo->set_text(textbox.c_str());
}
void modeControl(int control)
{
	if(((control == LINES  )|| (control == RECTANGLE ) ||(control == CUBES)))
		changeDrawMode(control);
	else if (control == LISTBOXTEXTURE)
		changeTexture(currtexture);
	else if (control == LISTBOXCOLOR)
		changeDefaultColor(curColorId);
	else
		changeMode(control);
}
void defaultMode() {
	flaggedPoints.clear();
	previewLineList.clear();
	selectionList.clear();
	splittingLines();
	creatingFaces();
	changeMode(NORMALMODE);

}

	//handles the bottom side of the gui 
	//the cmd will allow user to enter basic command

void commandControl(GLUI_Control* control) {

	if (control->get_id() == RESETID) {
		textbox = "";
		setTextBox("");
	}
	if (control->get_id() == OPENTEXTURE) {
			int csd = loadTextureBox();
			if (csd <= 0) {
				setTextBox("Failed to load file");
			}
			else {
				datastruct.texture.push_back(csd);
				texturelist->add_item(csd - 1, std::to_string(csd - 1).c_str());
			}
	}
	if (control == commandLineGUI || control->get_id() == CMDSUBMIT) {
		std::string text = commandLineGUI->get_text();
		std::istringstream  ssDRAW(text);
		printf("command : %s\n", commandLineGUI->get_text());
		if (text == "draw") {
			changeMode(DRAWINGMODE);
			setTextBox("Enter number in terms of x y z  or choose a point in the screen");

		}
		else if (text == "rotate") {
			changeMode(ROTATEMODE);
			setTextBox("Enter origin of rotation");

		}
		else if (text == "copy") {
			changeMode(COPYINGMODE);
			setTextBox("Enter distance to copy object");

		}
		else if (text == "move") {
			changeMode(MOVINGMODE);
			setTextBox("Enter distance to move object");
		}
		else if (text == "scale") {
			changeMode(SCALEMODE);
			setTextBox("Enter scalar factor :");
		}
		else if (text == "rectangle") {
			currDrawMode = RECTANGLE;
			setTextBox("drawing mode has changed to Rectangle");
		}
		else if (text == "cube") {
			currDrawMode = CUBES;
			setTextBox("drawing mode has changed to Cubes");

		}
		else if (text == "lines") {
			currDrawMode = LINES;
			setTextBox("drawing mode has changed to lines");

		}
		else if (text == "resetview") {
			setTextBox("viewReset");

			cx = 0.0f, cz = zFar / 2, cy = 0.0f;
			cdx = 0.0f, cdz = -1.0f, cdy = 0.0f;
			cenX = 0.0f, cenY = 0.0f, cenZ = 0.0f;
			rotX = 0;
			rotY = 0;
		}
		else if (text == "view") {
			changeMode(VIEWCHANGINGMODE);
			setTextBox("enter RotX RotY of the viewport");
		}
		else {
			switch (currentMode) {
			case NORMALMODE:
				setTextBox("enter the command (draw,scale,cube,lines,rotate)");
				break;
			case VIEWCHANGINGMODE:{
					bool isNumber = true;
					double arr[10];
					int i = 0;
					while (ssDRAW.good()) {
						if (ssDRAW >> arr[i]) {
							i++;
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter point of origin in the format of X Y Z");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
							isNumber = false;
							break;
						}
					}if (isNumber&& (i == 2) &&!(ssDRAW.good())) {
						rotX = arr[0];
						rotY = arr[1];
					}
				}
			  break;
			case DRAWINGMODE: {
				bool isNumber = true;
				double arr[10];
				int i = 0;
				while (ssDRAW.good() && i < 3) {
					if (ssDRAW >> arr[i]) {
						i++;
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
						setTextBox("enter point of origin in the format of X Y Z");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						isNumber = false;
						break;
					}
				}if (ssDRAW.good() || i!=3) {
					setTextBox("please enter in the format of x y and z");
					isNumber = false;

				}
				if (isNumber) {
					switch (count) {
					case 0: {
						setTextBox(("Point entered is X : " + std::to_string(arr[1]) + " Y : " + std::to_string(arr[2]) + " Z : " + std::to_string(arr[2])));
						tx = arr[0];
						ty = arr[1];
						tz = arr[2];
						count++;
						setTextBox("enter point next point in the format of x y z :\n");

					}break;
					case 1: {
						posX = arr[0] + tx;
						posY = arr[1] + ty;
						posZ = arr[2] + tz;
						points p1(tx, ty, tz);
						checkPoints(p1);
						points p2(posX, posY, posZ);
						checkPoints(p2);
						createObject(currDrawMode, p1, p2, datastruct.collectiveLinesList, true);
						defaultMode();


					}
							break;

					}
				}//end of switch node;


			}break;

			case COPYINGMODE: {
				if (selectionList.empty()) {
					setTextBox("no object to copy");
				}
				else {
					bool isNumber = true;
					double arr[3];
					int i = 0;
					while (ssDRAW.good() && i < 3) {
						if (ssDRAW >> arr[i]) {
							i++;
						}
						else {
						ErrorMessage:
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter distance to move in the format of X Y Z");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
							isNumber = false;
							break;
						}

					}
					if (isNumber&& i == 3 &&!(ssDRAW.good()))
						copyObject(points(arr[0], arr[1], arr[2]));
					else
						setTextBox("enter point of origin in the format of X Y Z");
					break;

				}
			}
							  break;

			case SCALEMODE: {
				if (selectionList.empty()) {
					setTextBox("no object to scale");
				}
				else {
					double arr;
					if (ssDRAW >> arr) {
						if (ssDRAW.eof()) {
							setTextBox("object is scaled by factor of :" + std::to_string(arr));
							tx = 0, ty = 0, tz = 0;
							scaleObject(arr);


						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter scalar factor s");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						}

					}
				}
				defaultMode();
			}

				break;
			case ROTATEMODE:
			{
				bool isNumber = true;
				if (count == 0) {
					if (selectionList.empty()) {
						setTextBox("no object to rotate");
						break;
					}
					int i = 0;
					double arr[3];
					while (ssDRAW.good() && i < 3) {
						if (ssDRAW >> arr[i]) {
							i++;
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter point of origin in the format of X Y Z");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
							isNumber = false;
							break;
						}
					}
					if (isNumber && i==3 && !(ssDRAW.good())) {
						tx = arr[0];
						ty = arr[1];
						tz = arr[2];
						count++;
						setTextBox("enter angle of rotation");

					}
					else
						setTextBox("enter origin of rotation in the form of x y z");
						break;

				}
				else {
					double arr;
					if (ssDRAW >> arr) {
						if (ssDRAW.eof()) {
							axisPos = 1;
							rotateObjectIn3D(arr, points(tx, ty, tz));
							defaultMode();
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter rotation R");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						}
					}
				}
				defaultMode();
			}
				break;
			case MOVINGMODE: {
				if (selectionList.empty()) {
					setTextBox("no object to scale");
				}
				else {
					bool isNumber = true;
					double arr[3];
					int i = 0;
					while (ssDRAW.good() && i < 3) {
						if (ssDRAW >> arr[i]) {
							i++;
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
							setTextBox("enter point of origin in the format of X Y Z");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
							isNumber = false;
							break;
						}
					
					}
					if (isNumber&& i ==3 && !(ssDRAW.good())) {
						moveObjects(points(arr[0], arr[1], arr[2]));
					}
					else
						setTextBox("enter point of origin in the format of X Y Z");
					break;

				}
				defaultMode();
			}
				break;
			}
		}
	}
}
void createFuncControl() {
	GLUI_Panel *operationPanel = subwindow1->add_panel("Operations");
	subwindow1->add_checkbox_to_panel(operationPanel, "Normal", &normalFlag, NORMALMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Draw", &drawFlag, DRAWINGMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Move", &moveFlag, MOVINGMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "View", &viewFlag, VIEWCHANGINGMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Scale", &scaleFlag, SCALEMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Rotate", &rotateFlag, ROTATEMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Copy", &copyFlag, COPYINGMODE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Line", &lineFlag, LINES, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Rectangle", &rectangleFlag, RECTANGLE, modeControl);
	subwindow1->add_checkbox_to_panel(operationPanel, "Cube", &cubesFlag, CUBES, modeControl);



	commandLineGUI = new GLUI_CommandLine(
		subwindow2, "Command (draw , scale rotate,...):", NULL, -1, commandControl);
	commandLineGUI->set_w(500);  /** Widen 'command line' control **/
	GLUI_Panel *panel = new GLUI_Panel(subwindow2, "", GLUI_PANEL_NONE);
	subwindow2->set_main_gfx_window(mainWindow);
	new GLUI_Button(panel, "Clear History", RESETID, commandControl);
	new GLUI_Column(panel, false);
	new GLUI_Button(panel, "SUBMIT COMMAND", CMDSUBMIT, commandControl);
	moo = new GLUI_TextBox(panel, true);
	moo->set_text(textbox.c_str());
	moo->set_h(50);
	moo->set_w(700);
	moo->disable();
	std::string string_list[] = { "white","red","green","blue","cyan","magenta","yellow" };
	 GLUI_Panel *operationPanel2 = subwindow1->add_panel("Properties");
	texturelist = new GLUI_Listbox(operationPanel2, "texture : ", &currtexture, LISTBOXTEXTURE, modeControl);
	colorlist = new GLUI_Listbox(operationPanel2, "Color:", &curColorId, LISTBOXCOLOR, modeControl);

	for (int i = 0; i <7; i++) {
		colorlist->add_item(i, string_list[i].c_str());
	}

	for (int i = 0; i < datastruct.texture.size(); i++) {
		std::string s = std::to_string(i);
		texturelist->add_item(i, s.c_str());

	}
}
void createMenubar() {
	GLUI_Panel* testglui;
	testglui = subwindow3->add_panel("test2", GLUI_PANEL_RAISED);
	subwindow3->add_button_to_panel(testglui, "add Texture", OPENTEXTURE, commandControl);
	subwindow3->add_column_to_panel(testglui, true);

}
void createGUI() {


	createFuncControl();
	createMenubar();

}
// GUI ENDED




// GLUT INIT

void init() {
	for (int i : loadDefaultTexture()) {
		datastruct.texture.push_back(i);
	}
	glShadeModel(GL_SMOOTH);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	flagged2Dpoints = std::make_pair(-999999, -999999);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv); // Initialize GLUT  
	glutInitDisplayMode(GLUT_SINGLE); // Set up a basic display buffer (only single buffered for now)  
	glutInitWindowSize(800, 480); // Set the width and height of the window  
	glutInitWindowPosition(100, 100); // Set the position of the window  
	mainWindow = glutCreateWindow("House Projects"); // Set the title for the window  




	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMovePassive);
	glutKeyboardFunc(keys);

	init();
	subwindow1 = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);
	subwindow2 = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_BOTTOM);
	subwindow3 = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_TOP);
	createGUI();

	glutMainLoop();
	return 1;
}

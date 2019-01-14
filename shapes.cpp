#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "shapes.h"
#include "faces.h"

shapes::shapes()
{
}

shapes::shapes(int count) {
	ID = count;
	name = "object" + ID;
	fill = false;

}

void shapes::changeFaceAppearance(faces f1, int aid) {
	faces tempface = f1;
	tempface.setAppearanceID(aid);
	std::replace(facelist.begin(), facelist.end(), f1, tempface);
}

void shapes::setAllAppearance(int aid){
	for (faces& f : facelist) {
		f.setAppearanceID(aid);
	}
}

int shapes::getID()
{
	return ID;
}

int shapes::getType()
{
	return objectType;
}

void shapes::moveAllFaces(float x, float y, float z)
{
	for (faces& tempface : facelist) {
		tempface.moveFaces(x, y, z);
	}
}

void shapes::addFace(faces f){
	facelist.push_back(f);

}

void shapes::setAllfacesColor(GLfloat r, GLfloat g, GLfloat b)
{// for now setting appearance not yet implemented
	for (faces& tempface : facelist) {
		tempface.setLineColor(r, g, b);
	}
}

void shapes::moveSingleFaces(points p, GLfloat newX, GLfloat newY, GLfloat newZ){
	points newP(newX, newY, newZ);
	for (faces& f: facelist) {
		f.moveFacesByPoints(p, newX, newY, newZ);
	}
}

void shapes::moveAllRelatedFaces(points refpoint, GLfloat newX, GLfloat newY, GLfloat newZ){
	for (faces& f : facelist) {
		f.resizeLine(refpoint, newX, newY, newZ);

	}


}

bool shapes::getFill(){
	return this->fill;
}

void shapes::setFill(){	
	this->fill = !this->fill;

}

std::vector<points> shapes::getAllPoints()
{
	std::vector<points> pointlist;
	for (faces f : facelist) {
		std::vector<points> tempPoints = f.getAllLinePoints();
		pointlist.insert(pointlist.end(), tempPoints.begin(), tempPoints.end());
	}
	pointlist.erase(unique(pointlist.begin(), pointlist.end()), pointlist.end());
	return pointlist;

}

std::vector<lines> shapes::getLinesList(){
	std::vector<lines> linesResult;
	for (faces f : facelist) {
		for (lines l : f.getLineList()) {
			linesResult.insert(linesResult.end(), l);
		}
	}
	return linesResult;
}

std::vector<faces> shapes::getFacelist(){
	return facelist;
}


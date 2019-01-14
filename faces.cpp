#include "faces.h"
#include <algorithm>



void faces::addLines(lines l){
	linelist.insert(linelist.end(), l);
}


void faces::removeLines(lines l){
	linelist.erase(std::remove(linelist.begin(), linelist.end(), l), linelist.end());
	
}


void faces::moveFaces(float x, float y, float z){
	for (int i = 0; i < linelist.size();i++) {
		lines& tempLines = linelist[i];
		tempLines.moveAllCoordinates(x, y, z);
	}
}

void faces::moveFacesByPoints(points p , float x,  float y, float z){
	points newpoint(x, y, z);
	for (lines& l : linelist) {
		l.moveCoordinates(p, newpoint);
	}
}
void faces::resizeLine(points refpoint, double newX, double newY, double newZ){
	for (lines& l : linelist) {
		l.resizePoint(refpoint, newX, newY, newZ);

	}

}
std::vector<lines> faces::getLineList()
{
	return linelist;
}

void faces::setLineColor(double r, double g, double b){
	this->r = r;
	this->g = g;
	this->b = b;
}

int faces::getAppearance()
{
	return appearanceID;
}

void faces::rearrangeLines(){
	lines l = linelist.front();
	for (lines& lt : linelist) {
		if (!(lt == l)) {
			if (!(l.getCoordinates().back() == lt.getCoordinates().front())) {
				lt.rearrangePoints();
				l = lt;
			}
		}
	}
}

void faces::setTextCoord(int numberOfVer){

	if (numberOfVer == 3) {
	textureCoord.push_back(std::pair<double, double>(0, 0));
	textureCoord.push_back(std::pair<double, double>(0.5, 1));
	textureCoord.push_back(std::pair<double, double>(1,0));
	}
	else if (numberOfVer == 4) {

		textureCoord.push_back(std::pair<double, double>(1,0));
		textureCoord.push_back(std::pair<double, double>(0, 0));
		textureCoord.push_back(std::pair<double, double>(0, 1));
		textureCoord.push_back(std::pair<double, double>(1, 1));


	}

}

std::vector<std::pair<double, double>> faces::getTextureCoord()
{
	return this->textureCoord;
}

bool faces::operator==(const faces &Ref) const
{
	return (Ref.pointlist == this->pointlist);
}

faces::faces()
{
}
faces::faces(std::vector<int> pointlist) {
	this->pointlist = pointlist;

}
faces::faces(std::vector<lines> linelist)
{
	this->linelist = linelist;
}
//
//bool faces::operator==(const faces & Ref) const{
//	if (this->linelist.size() != Ref.linelist.size())
//		return false;
//	for (int i = 0; i < Ref.linelist.size();i++) {
//		if ((std::find(this->linelist.begin(), this->linelist.end(), Ref.linelist[i]) != this->linelist.end()))
//			return false;
//	}
//	return true;
//}

std::vector<int> faces::getPointList()
{
	return this->pointlist;
}

void faces::setPoint(std::vector<int> pointlist)
{
	this->pointlist = pointlist;
}

void faces::setAppearanceID(int aid){
	appearanceID = aid;
}

std::vector<points> faces::getAllLinePoints(){
	std::vector<points> allpoints;
	for (lines& tempLines: linelist) {
		std::vector<points> tempPoints = tempLines.getCoordinates();
		for (points& p : tempPoints) {
			allpoints.insert(allpoints.end(), tempPoints.begin(), tempPoints.end());
		}
		double midX= (tempPoints.front().getX() + tempPoints.back().getX()) / 2,
			midY = (tempPoints.front().getY() + tempPoints.back().getY()) / 2,
			midZ = (tempPoints.front().getZ() + tempPoints.back().getZ()) / 2;
		points tempP(midX, midY, midZ);
		allpoints.insert(allpoints.end(), tempP);
	}
	allpoints.erase(unique(allpoints.begin(), allpoints.end()), allpoints.end());

	return allpoints;
}


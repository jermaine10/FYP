#pragma once
#include "lines.h"
class faces {

private:
	std::vector<lines> linelist; //list of lines in 1 faces.
	std::vector<int> pointlist;
	std::vector<std::pair<double, double>> textureCoord;
	int appearanceID;// :: TODO find out how to link external appearance
	// ########## TODO ... SOME DATA TYPE TO SAVE APPEARANCE.
public:
	faces();
	faces(std::vector<int>);
	faces(std::vector<lines> linelist);
	
	bool operator== (const faces&) const;
	float r, g, b;
	std::vector<int> getPointList();
	void setPoint(std::vector<int>);
	void setAppearanceID(int);
	std::vector<points> getAllLinePoints(); //returning all the points
	void addLines(lines);  // adding lines to faces, used to control the faces.
	void removeLines(lines);  
	void moveFaces(float ,float,float); //moving the faces4
	void moveFacesByPoints(points, float, float, float);
	void resizeLine(points, double, double, double);
	std::vector<lines> getLineList();
	void setLineColor(double, double, double);
	int getAppearance();
	void rearrangeLines();
	void setTextCoord(int);
	std::vector<std::pair<double, double>> getTextureCoord();

};
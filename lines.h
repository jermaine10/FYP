#pragma once
#include "points.h"
#include <vector>

class lines {
private:
	float red, green, blue;//rgb for the lines
	std::vector<points>coordinates;	 // coordinates of the lines (x,y,z)

public:
	lines(points&, points&); // used when initializing the lines
	bool operator == (const lines &Ref) const;
	bool operator ==(const std::vector<points> &Ref) const {
		return (Ref.front() == this->coordinates.front() || Ref.back() == this->coordinates.back() ||
			Ref.back() == this->coordinates.front() || Ref.front() == this->coordinates.back());
	}
	const std::vector<points> getCoordinates() const;
	lines();
	float getRed();
	float getGreen();
	float getBlue();
	void setRGB(float, float, float);
	void setFirst(points);
	void setSecond(points);
	points returnOther(points);
	points* getMaxMin();
	void resizePoint(points, float, float, float);
	bool coordinatesExist(points);
	void moveCoordinates(points , points);
	// used to resize one point of the lines
	void moveAllCoordinates(float, float, float); // used to move the lines
	points getFirst();
	void getMaxMinInNum(std::vector<float>&);
	points getSecond();
	void rearrangePoints();
	void rotate3D(int, double, points);
	float getDistance(points, points);
	void scaleObjectByFactor(float);
	void rotate3DFromCenter(double);
};
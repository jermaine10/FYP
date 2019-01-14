#include "lines.h"
#include <algorithm>


const std::vector<points> lines::getCoordinates() const
{
	return(this->coordinates);
}
lines::lines() {
}

float lines::getRed()
{
	return this->red;
}
float lines::getGreen()
{
	return this->green;
}
float lines::getBlue()
{
	return this->blue;
}

void lines::setRGB(float r, float g, float b) {
	this->red = r, this->green = g, this->blue = b;
}
void lines::setFirst(points p)
{
	coordinates.front() = p;
}
void lines::setSecond(points p)
{
	coordinates.back() = p;
}
points lines::returnOther(points p) {
	if (coordinates.front() == p)
		return coordinates.back();
	else
		return coordinates.front();
}
points* lines::getMaxMin() {
	double maxX = coordinates.front().getX(),
		maxY = coordinates.front().getY(),
		maxZ = coordinates.front().getZ(),
		minX = coordinates.back().getX(),
		minY = coordinates.back().getY(),
		minZ = coordinates.back().getZ();
	double temp;
	points* result;
	points info[2];
	result = info;
	if (maxX < minX) {
		temp = maxX;
		maxX = minX;
		minX = temp;
	}if (maxY < minY) {
		temp = maxY;
		maxY = minY;
		minY = temp;
	}	if (maxZ < minZ) {
		temp = maxZ;
		maxZ = minZ;
		minZ = temp;
	}
	info[0] = points(minX, minY, minZ);
	info[1] = points(maxX, maxY, maxZ);
	return result;
}
void lines::resizePoint(points refPoint, float newX, float newY, float newZ) {
	for (points& p : coordinates) {
		if (p.getX() == refPoint.getX()) {
			p.resizePoint(newX, p.getY(), p.getZ());
		}if (p.getY() == refPoint.getY()) {
			p.resizePoint(p.getX(), newY, p.getZ());
		}if (p.getZ() == refPoint.getZ()) {
			p.resizePoint(p.getX(), p.getY(), newZ);
		}
	}
}
bool lines::coordinatesExist(points tempPoint)
{
	bool flag = false;
	for (points p : coordinates) {
		if (p == tempPoint) {
			flag = true;
		}
	}
	return flag;
}
lines::lines(points& point1, points& point2) {
	coordinates = { point1,point2 };
	setRGB(0.5f, 1.0f, 1.0f); //default color
}

bool lines::operator==(const lines & Ref) const
{
	if (this->coordinates == Ref.coordinates)
		return true;
	if ((this->coordinates.front() == Ref.coordinates.back()) && this->coordinates.back() == Ref.coordinates.back())
		return true;
		
	return false;
		
}


void lines::moveCoordinates(points p1, points p2) {
	for (points& p : coordinates) {
		if (p == p1) {
			p = p2;
		}
	}
}

void lines::moveAllCoordinates(float x, float y, float z) {
	for (int i = 0; i < coordinates.size(); i++) {
		points& tempPoints = coordinates[i];
		tempPoints.moveXYZ(x, y, z);
		coordinates[i] = tempPoints;
	}

}
points lines::getFirst()
{
	return coordinates.front();
}
void lines::getMaxMinInNum(std::vector<float>& results) {

	results = { std::min(getFirst().getX(),getSecond().getX()),
		std::min(getFirst().getY(),getSecond().getY()),
		std::min(getFirst().getZ(),getSecond().getZ()),
		std::max(getFirst().getX(),getSecond().getX()),
		std::max(getFirst().getY(),getSecond().getY()),
		std::max(getFirst().getZ(),getSecond().getZ())
	};
}
points lines::getSecond()
{
	return coordinates.back();
}
void lines::rearrangePoints() {
	points p = coordinates.front();
	coordinates.front() = coordinates.back();
	coordinates.back() = p;
}


void lines::rotate3DFromCenter(double angle)
{
	points referencePoint = getFirst() - getSecond();
	for (points& p : coordinates) { // z axis rotation
		p = referencePoint - p;
		float xMove = (cos(angle) * p.getX()) - (sin(angle) * p.getY());
		float yMove = (sin(angle) * p.getX()) + (cos(angle) * p.getY());


		p.resizePoint(xMove, yMove, p.getZ());

		p = p + referencePoint;

	}
}
void lines::rotate3D(int axis, double angle, points referencePoint) {
	switch (axis) {
	case 1:
		for (points& p : coordinates) { // z axis rotation
			p = referencePoint - p;
			float xMove = (cos(angle) * p.getX()) - (sin(angle) * p.getY());
			float yMove = (sin(angle) * p.getX()) + (cos(angle) * p.getY());


			p.resizePoint(xMove, yMove, p.getZ());

			p = p + referencePoint;

		}
		break;
	case 2:
		for (points& p : coordinates) {
			p = referencePoint - p;
			float zMove = (p.getZ() * cos(angle)) - (p.getX() * sin(angle));
			float xMove = (p.getZ() * sin(angle)) + (p.getX() * cos(angle));
			p.resizePoint(xMove, p.getY(), zMove);
			p = p + referencePoint;
		}
		//rotate in y axis
		break;
	case 3:
		for (points& p : coordinates) {
			p = referencePoint - p;
			float yMove = p.getY() * cos(angle) - p.getZ() * sin(angle);
			float zMove = p.getY() * sin(angle) + p.getZ() * cos(angle);
			p.resizePoint(p.getX(), yMove, zMove);
			p = p + referencePoint;
		}		//rotate in z axis
		break;

	}

}
float lines::getDistance(points p1, points p2) {
	return (sqrt(pow((p2.getX() - p1.getX()), 2) +
		pow((p2.getY() - p1.getY()), 2) +
		pow(p2.getZ() - p1.getZ(), 2)));

}
void lines::scaleObjectByFactor(float scaleFactor) {
	for (points& p : coordinates) {
		p = p * scaleFactor;
	}

}
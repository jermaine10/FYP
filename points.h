#pragma once
#include <stdlib.h>
#include <iostream>

// the most important class
// matrix of vectors used in almost everything
// overloadd most operator to make matrices calculation easier

class points {
private:
	int id;
public:
	float x, y, z;

	bool operator == (const points &Ref) const;
	bool operator == (const int &Ref) const;
	int getID();
	bool operator !=(const points &Ref) const;
	points operator- (const points &Ref) const;
	points operator+ (const points &Ref) const;
	points operator* (const points &Ref) const;
	points operator* (const float &Ref) const;
	points operator/ (const float &Ref) const;
	points();
	points(float, float, float);
	float getX(), getY(), getZ();
	void setID(int);
	void moveXYZ(float, float, float);
	void resizePoint(float, float, float);
	void setX(float), setY(float), setZ(float);


};
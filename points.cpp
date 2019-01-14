#include "points.h"


bool points::operator==(const points & Ref) const
{

	return(
		(fabs(this->x - Ref.x) <= 0.001)
		&& (fabs(this->y - Ref.y) <= 0.001) &&
		(fabs(this->z - Ref.z) <= 0.001));
}

bool points::operator==(const int & Ref) const
{
	return (Ref == this->id);
}

int points::getID()
{
	return this->id;
}

bool points::operator!=(const points & Ref) const
{
	return(this->x != Ref.x || this->y != Ref.y || this->z != Ref.z);
}

points points::operator-(const points & Ref) const
{
	return points(Ref.x - this->x, Ref.y - this->y, Ref.z - this->z);
}

points points::operator+(const points & Ref) const
{
	return points(Ref.x + this->x, Ref.y + this->y, Ref.z + this->z);
}

points points::operator*(const points & Ref) const
{
	return points(Ref.x * this->x, Ref.y * this->y, Ref.z * this->z);
}
points points::operator*(const float & Ref) const
{
	return points(Ref * this->x, Ref* this->y, Ref * this->z);
}

points points::operator/(const float & Ref) const
{
	return points(this->x/Ref,this->y/Ref,this->z/Ref);
}

points::points() {
	x = -999999, y = -999999, z = -999999;
}

points::points(float newX, float newY, float newZ) {
	x = newX, y = newY, z = newZ;
}

float points::getX() {
	return x;
}

float points::getY() {
	return y;
}

float points::getZ() {
	return z;
}

void points::setID(int ID)
{
	this->id = ID;
}

void points::moveXYZ(float newX, float newY, float newZ) {
	this->x += newX;
	this->y += newY;
	this->z += newZ;

}

void points::resizePoint(float newX, float newY, float newZ) {
	this->x = newX;
	this->y = newY;
	this->z = newZ;
}

void points::setX(float x)
{
	this->x = x;
}

void points::setY(float y)
{
	this->y = y;
}

void points::setZ(float z)
{
	this->z = z;
}

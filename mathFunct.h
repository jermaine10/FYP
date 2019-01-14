#pragma once

double dot(points& p1, points& p2) {
	return p1.getX()*p2.getX() + p1.getY()*p2.getY() + p1.getZ() + p2.getZ();

}
inline points cross(points& p1, points& p2) {
	return	points(p1.getY()*p2.getZ() - p2.getY()*p1.getZ()
		, p1.getZ()*p2.getX() - p2.getZ()*p1.getX()
		, p1.getX()*p2.getY() - p2.getX()*p1.getY());
}
inline double norm2(points& p1) {
	return (p1.getX() * p1.getX() + p1.getY() *p1.getY() + p1.getZ() * p1.getZ());
}
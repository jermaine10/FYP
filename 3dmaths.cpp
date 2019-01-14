
#include "3dmaths.h"
inline double dot(points p1, points p2) {
	return ((p1.getX() * p2.getX()) + (p1.getY() *p2.getY()) + (p1.getZ() *p2.getZ()));

}
inline double norm2(points& p1) {
	return (p1.getX() * p1.getX() + p1.getY() *p1.getY() + p1.getZ() * p1.getZ());
}

 points crossProd(points p1, points p2) {
	 double d = (p1.getX() * p2.getY());
	 double d2 = p1.getY() * p2.getX();
	return points(
		(p1.getY() * p2.getZ()) - (p1.getZ() * p2.getY()),
		(p1.getZ() * p2.getX()) - (p1.getX() * p2.getZ()),
		(p1.getX() * p2.getY()) - (p1.getY() * p2.getX())


		);
}
 std::pair<points, points> findMaxMIn(std::vector<points> calculatePoints) {
	
	 double maxX= -999999, maxY = -999999, maxZ = -999999;
	 double minZ =999999, minY = 999999, minX = 999999;
	 for (points p : calculatePoints) {
		 if (p.getX() > maxX)
			 maxX = p.getX();
		 if (p.getY() > maxY)
			 maxY = p.getY();
		 if (p.getZ() > maxZ)
			 maxZ = p.getZ();
		 if (p.getX() < minX)
			 minX = p.getX();
		 if (p.getY() < minY)
			 minY = p.getY();
		 if (p.getZ() < minZ)
			 minZ = p.getZ();
	 }
	 return std::pair<points, points>(points(minX,minY, minZ), points(maxX, maxY, maxZ));

 }
int limitMovement(points& p1, points& p2)
{
	double distanceX = fabs(p1.getX() - p2.getX());
	double distanceY = fabs(p1.getY() - p2.getY());
	double distanceZ = fabs(p1.getZ() - p2.getZ());
	double distanceM = std::min({ distanceX ,distanceY,distanceZ });
	if (distanceX == distanceM) {
		p2.setX(p1.getX());
		return 1;

	}
	else if (distanceY == distanceM) {
		p2.setY(p1.getY());


	}
	else if (distanceZ == distanceM) {
		p2.setZ(p1.getZ());


	}
	return 0;

}
bool isInsideBoundingPlane(faces f,std::vector<points> pointList,double px,double py,double pz) {

	std::vector<int> planePoint = f.getPointList();
	std::vector<points> temppoint;
	for (int i : planePoint) {
		temppoint.push_back(pointList.at(i));
	}
	points currPoint(px, py, pz);
	std::pair<points, points> maxmin = findMaxMIn(temppoint);
	points p = maxmin.first;
	points p2 = maxmin.second;

	if (((p.getZ()) > (currPoint.getZ() + 0.01)) ||
		((p.getX() > currPoint.getX() + 0.01)) ||
		((p.getY() > currPoint.getY() + 0.01)) ||
		((p2.getZ()) < (currPoint.getZ() - 0.01)) ||
		((p2.getX() < currPoint.getX() - 0.01)) ||
		((p2.getY() < currPoint.getY() - 0.01))


		) {
		return false;
	}
	return true;

}

bool intersection2D(double x1, double y1, double x2, double y2, double tx1, double ty1, double tx2, double ty2) {
	double cenX1 = x1 - x2;
	double cenY1 = y1 - y2;

	if (cenX1 == 0 ) {
		if ((tx1 <= x1 && x1 <= tx2) &&
			std::min(y1, y2) < ty1 &&
			ty1 < std::max(y1, y2)
			)
			return true;
		return false;
	}
	if (cenY1 == 0) {
		if ((ty1 <= y1 && y1 <= ty2) &&
			std::min(x1, x2) < tx1 &&
			tx1 < std::max(x1, x2)
			)
			return true;
		return false;
	}
	double cenX2 = tx1 - tx2;
	double cenY2 = ty1 - ty2;

	
	double grad = cenX1*cenY2 - cenX2*cenY1;
	if (fabs(grad) < 0.01)
		return false;
	float iX = ((x1*y2 - y1*x2) * cenX2 - cenX1 * (tx1*ty2 - ty1*tx2)) / grad;
	float iY = ((x1*y2 - y1*x2) * cenY2 - cenY1 * (tx1*ty2 - ty1*tx2)) / grad;
	
	if ((iX ) < std::min(x1, x2) || (iX) > std::max(x1, x2) ||
		(iX) < std::min(tx1, tx2) || (iX ) > std::max(tx1, tx2))
		return false;
	if ((iY) < std::min(y1, y2) || (iY ) > std::max(y1, y2) ||
		(iY) < std::min(ty1, ty2) || (iY ) > std::max(ty1, ty2))return false;
	return true;

}


bool intersection3D(lines l1, lines l2, points& interSect) {

	points p13 = l2.getFirst() - l1.getFirst();
	points p43 = l2.getSecond() - l2.getFirst();
	if (norm2(p43)*norm2(p43) < 0.01) {
		return false;
	}
	points p21 = l1.getSecond() - l1.getFirst();
	if (norm2(p21)*norm2(p21) < 0.01) {
		return false;
	}
	double d1 = dot(p13, p43);
	double d2 = dot(p43, p21);
	double d3 = dot(p13, p21);
	double d4 = dot(p43, p43);
	double d5 = dot(p21, p21);
	double denom = d5 * d4 - d2 * d2;
	if (abs(denom) < 0.01) 
		return false;
	
	double numer = d1 * d2 - d3 * d4;
	double mua = numer / denom;
	double mub = (d1 + d2 * (mua)) / d4;

	interSect = (l1.getFirst() + (points(mua, mua, mua) * p21));
	points p7 =( l2.getFirst() + (points(mub, mub, mub) * p43));

	if (!((fabs(p7.getX() - interSect.getX())) <= 0.1 &&
		(fabs(p7.getY() - interSect.getY())) <= 0.1 &&
		(fabs(p7.getZ() - interSect.getZ())) <= 0.1
		))
		return false;
	points p5 = l2.getMaxMin()[0];
	points p6 = l2.getMaxMin()[1];

	if (!((p5.getY() <= (interSect.getY()+0.01) && (interSect.getY()-0.01) <= p6.getY()) &&
		(p5.getX() <= (interSect.getX()+0.01) && (interSect.getX()-0.01) <= p6.getX()) &&
		(p5.getZ() <= (interSect.getZ()+0.01) && (interSect.getZ()-0.01) <= p6.getZ())))
		return false;



	points p3 = l1.getMaxMin()[0];
	points p4 = l1.getMaxMin()[1];



	if ((l1.coordinatesExist(interSect)) && (l2.coordinatesExist(interSect)))
		return false;

	if ((p3.getY() <= interSect.getY() && interSect.getY() <= p4.getY()) &&
		(p3.getX() <= interSect.getX() && interSect.getX() <= p4.getX()) &&
		(p3.getZ() <= interSect.getZ() && interSect.getZ() <= p4.getZ()))
		return true;



	return false;
}

int compareDistance(double& posX, double& posY, double& posZ, double tx, double ty, double tz) {

	double distanceX = fabs(posX - tx);
	double distanceY = fabs(posY - ty);
	double distanceZ = fabs(posZ - tz);
	double distanceD = std::max({ distanceX, distanceY, distanceZ });
	if (distanceD == distanceX) {
		posY = ty;
		posZ = tz;
	}
	else if (distanceD == distanceZ) {
		posX = tx;
		posY = ty;
	}
	else {
		posX = tx;
		posZ = tz;
	}
	double distanceM = std::min({ distanceX, distanceY, distanceZ });
	if (distanceM == distanceX) {
		return 3;
	}
	else if (distanceM == distanceZ) {
		return 1;
	}
	else if (distanceM == distanceY) {
		return 2;
	}
}

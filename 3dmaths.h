#pragma once
#ifndef _3DMATHS_H
#define _3DMATHS_H

// MOST OF THE MATHS ARE DONE HERE
// includes intersection of 2d ->3d ->plane

#include "faces.h"
#include <algorithm>
#include <math.h>
#include <utility>

int limitMovement(points&, points&);
bool isInsideBoundingPlane(faces f, std::vector<points> pointList, double px, double py, double pz);
bool intersection2D(double x1, double y1, double x2, double y2, double tx1, double ty1, double tx2, double ty2);
inline double dot(points p1, points p2);
inline double norm2(points& p1);
bool intersection3D(lines l1, lines l2, points& interSect);
int compareDistance(double& , double& , double& , double, double, double);
std::pair<points, points> findMaxMIn(std::vector<points> calculatePoints);
points crossProd(points p1, points p2);

#endif
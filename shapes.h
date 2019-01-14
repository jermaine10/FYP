#include <stdlib.h>

#include <stdlib.h>
#include "GL/glut.h"
#include <iostream>
#include "faces.h"

#include <algorithm>

class shapes {
private:
	int ID;
	std::string name;
	std::vector<faces> facelist;
	int objectType;
	bool fill;


public:
	shapes();
	shapes(int);
	bool operator == (const shapes &Ref) const
	{
		return(this->facelist == Ref.facelist);
	}
	void createShape(int, float, float, float, float, float, float, GLfloat, GLfloat, GLfloat); 
	void changeFaceAppearance(faces, int);// changing a single face to other appearance .
	void setAllAppearance(int);
	int getID();
	int getType();
	void moveAllFaces(float, float, float); // moving the shape together
	void addFace(faces fl);
	void setAllfacesColor(GLfloat, GLfloat, GLfloat); // change the appearance of the whole shape
	void moveSingleFaces(points,GLfloat,GLfloat,GLfloat); // resize function of a single object
	void moveAllRelatedFaces(points, GLfloat, GLfloat, GLfloat);
	bool getFill();
	void setFill();
	std::vector<points> getAllPoints(); // getting all the points on the objects
	std::vector<lines> getLinesList(); //.............
	std::vector<faces> getFacelist();// getting facelist for easier access in the object

};


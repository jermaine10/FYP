#include "drawObjects.h"
 void draw3Dobjects(globalVar datastruct)
{
	{

		for (lines l : previewLineList) {
			points p = l.getFirst();
			points p2 = l.getSecond();
			glBegin(GL_LINES);
			glColor3f(l.getRed(), l.getGreen(), l.getBlue());
			glVertex3f(p.getX(), p.getY(), p.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());
			glEnd();
		}
		for (lines l : datastruct.collectiveLinesList) {
			if (std::find(selectionList.begin(), selectionList.end(), l) != selectionList.end()) {
				glPushAttrib(GL_ENABLE_BIT);
				glLineStipple(3, 0xAAAA);
				glEnable(GL_LINE_STIPPLE);

				points p = l.getFirst();
				points p2 = l.getSecond();
				glBegin(GL_LINES);
				glColor3f(l.getRed(), l.getGreen(), l.getBlue());
				glVertex3f(p.getX(), p.getY(), p.getZ());
				glVertex3f(p2.getX(), p2.getY(), p2.getZ());
				glEnd();

				glPopAttrib();
			}
			else {
				points p = l.getFirst();
				points p2 = l.getSecond();
				glBegin(GL_LINES);
				glColor3f(l.getRed(), l.getGreen(), l.getBlue());
				glVertex3f(p.getX(), p.getY(), p.getZ());
				glVertex3f(p2.getX(), p2.getY(), p2.getZ());
				glEnd();


			}
		}

		if (!wireflag);
		glEnable(GL_TEXTURE_2D);
		for (faces f : datastruct.faceList) {
			if (std::find(selectedFace.begin(), selectedFace.end(), f) == selectedFace.end()) {
				if (f.getPointList().size() <= 4) {
					int aid = f.getAppearance();
					glBindTexture(GL_TEXTURE_2D, datastruct.texture.at(aid));
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


				}
				if (wireflag)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
				int textureCounter = 0;
				std::vector<std::pair<double, double>> texCoord = f.getTextureCoord();
				glBegin(GL_POLYGON);
				for (int i : f.getPointList()) {
					if (f.getPointList().size() <= 4) {
						glTexCoord2f(texCoord[textureCounter].first, texCoord[textureCounter].second);
					}
					points p = datastruct.collectivePointList.at(i);
					glColor3f(f.r, f.g, f.b);
					glVertex3f(p.getX(), p.getY(), p.getZ());
					textureCounter++;


				}


				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				for (int i : f.getPointList()) {

					points p = datastruct.collectivePointList.at(i);
					glColor3f(f.r, f.g, f.b);
					glVertex3f(p.getX(), p.getY(), p.getZ());

				}


				glEnd();
			}
			
		}
		
		glDisable(GL_TEXTURE_2D);
	}
}
void drawAxis() {
	glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -1000);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, -1000, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-1000, 0, 0);
	glEnd();

	glPopAttrib();


	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();
}


void selectionDraw(int xOrigin, int yOrigin, int newX, int newY) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex2f(xOrigin, yOrigin);
	glVertex2f(xOrigin, newY);
	glVertex2f(newX, newY);
	glVertex2f(newX, yOrigin);
	glEnd();
}

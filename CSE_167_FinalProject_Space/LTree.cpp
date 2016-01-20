//Source (public Github repo): https://github.com/paluka/L-Systems-OpenGL
//Made a few changes to create better looking trees

#include "GLee.h"

#include "LTree.h"
#include <iostream>
using namespace std;

LTree::LTree()
{
	trees = new vector<string>();
	altColor = false;
	length = 0.125;
	lineWidth = 5;
	depth = 0;
	treeCode = "X";

	for (int i = 0; i <= DEPTH; i++)
		expand();
}

void LTree::expand()
{
	string token = "";
	for (int i = 0; i < treeCode.length(); i++)
	{
		float n = (float)rand() / RAND_MAX;
		token = treeCode.at(i);

		if (token.compare("D") == 0)
		{
			treeCode.replace(i, 1, "DD");
			i = i + 1;
		}

		//Added more rules for greater variability
		else if (token.compare("X") == 0)
		{
			if (n < 0.1)
				treeCode.replace(i, 1, "D[LXV]D[RXV]LX");

			else if (n < 0.2)
				treeCode.replace(i, 1, "D[RXV]D[LXV]RX");

			else if (n < 0.3)
				treeCode.replace(i, 1, "D[LXV]D[RXV]RX");

			else if (n < 0.4)
				treeCode.replace(i, 1, "D[RXV]D[LXV]LX");

			else if (n < 0.5)
				treeCode.replace(i, 1, "D[LLXV]D[RXV]LLX");

			else if (n < 0.6)
				treeCode.replace(i, 1, "D[RRXV]D[LXV]RRX");

			else if (n < 0.7)
				treeCode.replace(i, 1, "D[RXV]D[LLXV]RX");

			else if (n < 0.8)
				treeCode.replace(i, 1, "D[LXV]D[RRXV]LX");

			else if (n < 0.9)
				treeCode.replace(i, 1, "D[VXV]D[VXV]X");

			else
				treeCode.replace(i, 1, "D[XV]D[XV]X");

			i = i + 13;
		}
	}
	trees->push_back(treeCode);
}

void LTree::draw()
{
	glDisable(GL_LIGHTING);
	string token = "";
	string treeCode = trees->at(depth);

	for (int i = 0; i < treeCode.length(); i++)
	{
		token = treeCode.at(i);

		if (token.compare("D") == 0 || token.compare("X") == 0)
			drawBranch();
		
		else if (token.compare("[") == 0)
			pushMatrix();
		
		else if (token.compare("]") == 0)
			popMatrix();
		
		else if (token.compare("V") == 0)
			drawLeaf();
		
		else if (token.compare("R") == 0)
			rotateRight();
		
		else if (token.compare("L") == 0)
			rotateLeft();
	}

	if (depth < DEPTH)
		depth++;
	
	glEnable(GL_LIGHTING);
}

void LTree::drawBranch()
{	
	//Set branch color / width
	glColor3f(.42, .28, 0);

	glLineWidth(lineWidth);

	//Draw branch
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glEnd();

	//length = tree height
	glTranslatef(0, length, 0);
	glPopAttrib();
}

void LTree::drawLeaf()
{
	//Leaves now alternate between two colors
	if (altColor)
		glColor3f(1, .8, .8);
	else
		glColor3f(.7, .28, .56);

	altColor = !altColor;

	//Draw leaf using triangles (individual leaf)
	//Also tried using cubes to add more volume... Looked a bit funky
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.1, 0, 0.15);
	glVertex3f(0, 0.5, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-0.1, 0, -0.15);
	glVertex3f(0, 0.5, 0);
	glEnd();
	glPopAttrib();
}

void LTree::rotateRight()
{
	glRotatef(-ANGLE, 1, 0, 0);
	glRotatef(ANGLE, 0, 1, 0);
	glRotatef(-ANGLE, 0, 0, 1);
}

void LTree::rotateLeft()
{
	glRotatef(ANGLE, 1, 0, 0);
	glRotatef(ANGLE, 0, 1, 0);
	glRotatef(ANGLE, 0, 0, 1);
}

void LTree::popMatrix()
{
	glPopMatrix();
	lineWidth += 1;

}

void LTree::pushMatrix()
{
	glPushMatrix();
	if (lineWidth > 0)
		lineWidth -= 1;
}
#ifndef _LTREE_H_
#define _LTREE_H_

#include "Globals.h"

class LTree
{
public:
	vector<string> *trees;
	bool altColor;
	float length, lineWidth, depth;
	string treeCode;

	LTree(void);
	void expand(void);
	void draw();
	void drawBranch();
	void drawLeaf();
	void rotateRight();
	void rotateLeft();
	void popMatrix();
	void pushMatrix();
};

#endif
#include "GLee.h"
#include "Globals.h"

#include "Sphere.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif


Sphere::Sphere(double radius, int slices, int stacks) : Drawable()
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
}

void Sphere::draw(DrawData& data)
{
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);


	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	//material.color = Color::red();
	//material.apply();
	gluSphere(sphere, radius, slices, stacks);

	glPopMatrix();
}


void Sphere::update(UpdateData& data)
{
    //
}

void Sphere::rotate()
{
	Matrix4 rotate;
	Vector3 axis;

	axis.set(1, 0, 0);
	rotate.makeRotateArbitrary(axis, -(3 * PI) / 5);
	this->toWorld = rotate * this->toWorld;

	axis.set(0, 1, 0);
	rotate.makeRotateArbitrary(axis, PI / 8);
	this->toWorld = rotate * this->toWorld;
}



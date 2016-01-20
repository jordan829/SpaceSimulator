#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	move_scale = 0.1;
	rotation = glm::vec3(0, 0, 0);;
	reset();
}

Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
	Vector3 zc = e - d;
	float zc_mag = zc.magnitude();
	zc = zc.multiply(1/zc_mag);

	Vector3 xc;
	xc = up.cross(zc);
	float xc_mag = xc.magnitude();
	xc = xc.multiply(1/xc_mag);

	Vector3 yc;
	yc = zc.cross(xc);

    //Use these axes and the e vector to create a camera matrix c
	c.set(xc[0], xc[1], xc[2], 0,
				yc[0], yc[1], yc[2], 0,
				zc[0], zc[1], zc[2], 0,
				e[0], e[1], e[2], 1);

    //Use c to solve for an inverse camera matrix ci
	Matrix4 R;
	Matrix4 Ri;
	Matrix4 Ti;
	R.set(xc[0], xc[1], xc[2], 0,
		  yc[0], yc[1], yc[2], 0,
		  zc[0], zc[1], zc[2], 0,
		  0, 0, 0, 1);

	Ri = R.transpose();

	Ti.set(1, 0, 0, 0,
		  0, 1, 0, 0,
		  0, 0, 1, 0,
		 -e[0], -e[1], -e[2], 1);

	ci = Ri * Ti;
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    update();
}

void Camera::reset()
{
	e.set(0.0, 0.0, 200.0);
	d.set(0.0, 0.0, 0.0);
	up.set(0.0, 1.0, 0.0);
	move_scale = 0.0;
	update();
	rotation = glm::vec3(0, 0, 0);
}

void Camera::rotate(Matrix4 trans){
	Vector3 lookAt = trans * (d - e);
	d = e + lookAt;
	up = trans * up;
	up.normalize();
	update();
}

void Camera::forward(){
	if (move_scale > 0.05)
	{
		Vector3 forward = (d - e).normalize().scale(move_scale);
		e = e + forward;
		d = d + forward;
		update();
	}
}

void Camera::backward(){
	Vector3 backward = (d - e).normalize().scale(-move_scale);
	e = e + backward;
	d = d + backward;
	update();
}

void Camera::left(){
	Vector3 left = up.cross((d - e).normalize()).scale(move_scale);
	e = e + left;
	d = d + left;
	update();
}

void Camera::right(){
	Vector3 right = up.cross((d - e).normalize()).scale(-move_scale);
	e = e + right;
	d = d + right;
	update();
}

void Camera::addYaw(float yaw)
{
	rotation.y += yaw;
}

void Camera::addRoll(float roll)
{
	rotation.z += roll;
}

void Camera::addPitch(float pitch)
{
	rotation.x += pitch;
}

void Camera::addRotation(const glm::quat& rot)
{
	glm::quat finalRotate = glm::quat(rotation) * rot;
	rotation = glm::eularAngles(finalRotate);
}

glm::vec3 Camera::GetRotation() const
{
	return rotation;
}
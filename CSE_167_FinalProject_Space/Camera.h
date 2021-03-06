#ifndef CSE167_Camera_h
#define CSE167_Camera_h

#include "Matrix4.h"
#include "Vector3.h"
#include "externals\glm-0.9.1\glm\glm.hpp"
#include "externals\glm-0.9.1\glm\gtc\quaternion.hpp"
#include "externals\glm-0.9.1\glm\gtx\quaternion.hpp"

class Camera
{
public:
    
    Vector3 e;   //Center of Projection
    Vector3 d;   //Look At
    Vector3 up;  //Up Vector ^

	Matrix4 c;   //Camera Matrix
	Matrix4 ci;  //Inverse of Camera Matrix

	glm::vec3   rotation;	// rotation of camera
    
	float move_scale;

    Camera(void);
    ~Camera(void);
    
    void update(void);
    
    Matrix4& getMatrix(void);
    Matrix4& getInverseMatrix(void);
    
    void set(Vector3& e, Vector3& d, Vector3& up);

	void reset(void);
    
	void rotate(Matrix4 trans);
	void forward();
	void backward();
	void left();
	void right();

	// Used to align particles with camera orientation
	void addYaw(float);
	void addRoll(float);
	void addPitch(float);
	void addRotation(const glm::quat&);
	glm::vec3 GetRotation() const;
};

#endif

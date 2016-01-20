#include "GLee.h"

#include <iostream>
#include <ctime>
#include <stdlib.h>
using namespace std;
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Skybox.h"
#include "Matrix4.h"
#include "Globals.h"
#include "Rasterizer.h"
#include "MatrixTransform.h"
#include "GeoSphere.h"
#include "GeoCube.h"
#include "GeoChest.h"
#include "GeoBall.h"
#include "ParticleEmitter.h"
#include "Timer.h"
#include "LTree.h"

////////////////////////////////////////////////////////////////////////
int Window::width  = 1920;   //Set window width in pixels here
int Window::height = 1080;   //Set window height in pixels here

bool leftClick = false;
bool rightClick = false;
bool zoom = false;
int mouseX = 0;
int mouseY = 0;

Vector3 lastPoint;

Matrix4 meh;	//Used for grid translation / scaling
Matrix4 bleh;	//Used for grid rotation

Skybox skybox(0);

float elapsedTime, lastElapsedTime;
bool hyperSpeedEnable = false;
float prevSpeed;
bool test = false;
bool show = true;
bool orbit = true;
bool celShade = true;
bool lightShade = true;
bool bumpMap = true;

ParticleEmitter thrusterL;
ParticleEffect thrusterEffectL(60000);

ParticleEmitter thrusterR;
ParticleEffect thrusterEffectR(60000);

ParticleEffect::ColorInterpolator colors;

ParticleEmitter star;
ParticleEffect starEffect(5000);

ParticleEmitter hyperSpeed;
ParticleEffect hyperSpeedEffect(1000);

ParticleEffect::ColorInterpolator colors1;

ParticleEmitter sample;
ParticleEffect sampleEffect(1);

LTree tree = LTree();
///////////////////////////////////////////////////////////////////////

void Window::initialize(void)
{
	//Default light
	Vector4 lightPos(0.0, 34999.0, 0.0, 1.0);
	Globals::light.position = lightPos;
	Globals::light.quadraticAttenuation = 0.0;
	Globals::light.linearAttenuation = 0.0;
	Globals::light.constantAttenuation = 1;
	Globals::light.specularColor = Color::white();
	Globals::light.diffuseColor = Color::white();;
	//Globals::light.bind(0);

	Globals::cel = new Shader("Assets/Shaders/celshading.vert", "Assets/Shaders/celshading.frag", true);

	Globals::bump = new Shader("Assets/Shaders/bump_map.vert", "Assets/Shaders/bump_map.frag", true);
	Globals::bumpy = new Texture("Assets/Textures/bump.ppm");

	Globals::lighting = new Shader("Assets/Shaders/texture.vert", "Assets/Shaders/texture.frag", true);
	Globals::planet = new Texture("Assets/Textures/planet.ppm");

	meh.identity();
	bleh.identity();

	glEnable(GL_NORMALIZE);

	//Set camera position
	Globals::camera.reset();

	//Initialize center planet
	Matrix4 trans;
	trans.makeTranslate(0, -50, 0);
	Globals::globe.toWorld = trans * Globals::globe.toWorld;

	//Initialize moon
	trans.makeTranslate(0, 100, 0);
	Globals::moon.toWorld = trans * Globals::moon.toWorld;

	//Initialize skybox
	skybox = Skybox(70000.0);
	Globals::ft = Texture("Assets/Textures/purplenebula_ft.ppm");	//Front face
	Globals::bk = Texture("Assets/Textures/purplenebula_bk.ppm");	//Back face
	Globals::lf = Texture("Assets/Textures/purplenebula_lf.ppm");	//Left face
	Globals::rt = Texture("Assets/Textures/purplenebula_rt.ppm");	//Right face
	Globals::up = Texture("Assets/Textures/purplenebula_up.ppm");	//Top face
	Globals::dn = Texture("Assets/Textures/purplenebula_dn.ppm");	//Bottom face

	//Initialize ship
	string filename = "Assets/Models/dark_fighter_6.obj";
	Globals::ship.parse(filename);
	Globals::ship.toWorld.identity();

	trans.makeTranslate(0, -10, -75);
	Globals::ship.toWorld = trans  * Globals::ship.toWorld;

	//Initialize thrusters (left)
	thrusterEffectL.toWorld = Globals::ship.toWorld;

	trans.makeTranslate(-3, -6, 25);
	thrusterEffectL.toWorld = trans * thrusterEffectL.toWorld;

	thrusterEffectL.LoadTexture("Assets/Textures/particle.dds");

	colors.AddValue(0.0f, glm::vec4(1, 1, 1, 1));		//white
	colors.AddValue(0.35f, glm::vec4(0, 1, 1, 1));	//cyan
	colors.AddValue(0.45f, glm::vec4(0, 0, 1, 1));	//blue

	thrusterEffectL.SetColorInterplator(colors);
	thrusterEffectL.move = false;
	thrusterEffectL.scale = 0.02;

	thrusterL.MinimumRadius = 0;
	thrusterL.MaximumRadius = 100;
	thrusterL.MinInclination = 0;
	thrusterL.MaxInclination = 6;	//Angle of cone
	thrusterL.MinAzimuth = 0;
	thrusterL.MaxAzimuth = 360;
	thrusterL.MinSpeed = 0;
	thrusterL.MaxSpeed = THRUSTER_SPEED;
	thrusterL.MinLifetime = 0;
	thrusterL.MaxLifetime = .3;

	thrusterEffectL.SetParticleEmitter(&thrusterL);
	thrusterEffectL.EmitParticles();

	//Initialize thrusters (right)
	thrusterEffectR.toWorld = Globals::ship.toWorld;

	trans.makeTranslate(3, -6, 25);
	thrusterEffectR.toWorld = trans * thrusterEffectR.toWorld;

	thrusterEffectR.LoadTexture("Assets/Textures/particle.dds");

	thrusterEffectR.SetColorInterplator(colors);
	thrusterEffectR.move = false;
	thrusterEffectR.scale = 0.02;

	thrusterR.MinimumRadius = 0;
	thrusterR.MaximumRadius = 100;
	thrusterR.MinInclination = 0;
	thrusterR.MaxInclination = 6;	//Angle of cone
	thrusterR.MinAzimuth = 0;
	thrusterR.MaxAzimuth = 360;
	thrusterR.MinSpeed = 0;
	thrusterR.MaxSpeed = THRUSTER_SPEED;
	thrusterR.MinLifetime = 0;
	thrusterR.MaxLifetime = .3;

	thrusterEffectR.SetParticleEmitter(&thrusterR);
	thrusterEffectR.EmitParticles();

	//Initialize stars
	starEffect.LoadTexture("Assets/Textures/particle.dds");

	colors1.AddValue(0.0f, glm::vec4(1, 1, 1, 1));		

	starEffect.SetColorInterplator(colors1);
	starEffect.move = true;
	
	star.MinimumRadius = 600;
	star.MaximumRadius = 900;
	star.MinInclination = 0;
	star.MaxInclination = 360;	//Angle of cone
	star.MinAzimuth = 0;
	star.MaxAzimuth = 360;
	star.MinSpeed = 0;
	star.MaxSpeed = 0;
	star.MinLifetime = 0;
	star.MaxLifetime = 15;

	starEffect.SetParticleEmitter(&star);
	starEffect.EmitParticles();

	//Initialize hyper speed effect
	hyperSpeedEffect.toWorld.identity();

	trans.makeTranslate(0, -30, -150);
	hyperSpeedEffect.toWorld = trans * hyperSpeedEffect.toWorld;

	hyperSpeedEffect.LoadTexture("Assets/Textures/particle.dds");

	hyperSpeedEffect.SetColorInterplator(colors1);
	hyperSpeedEffect.move = false;
	hyperSpeedEffect.scale = 0.3;

	hyperSpeed.MinimumRadius = 0;
	hyperSpeed.MaximumRadius = 1;
	hyperSpeed.MinInclination = 30;
	hyperSpeed.MaxInclination = 30;	//Angle of cone
	hyperSpeed.MinAzimuth = 0;
	hyperSpeed.MaxAzimuth = 360;
	hyperSpeed.MinSpeed = 500;
	hyperSpeed.MaxSpeed = 750;
	hyperSpeed.MinLifetime = 0;
	hyperSpeed.MaxLifetime = 2;

	hyperSpeedEffect.SetParticleEmitter(&hyperSpeed);
	hyperSpeedEffect.EmitParticles();

	//Initialize (for testing)
	sampleEffect.LoadTexture("Assets/Textures/particle.dds");

	ParticleEffect::ColorInterpolator colors2;

	colors2.AddValue(0.0f, glm::vec4(1, 1, 1, 1));

	sampleEffect.SetColorInterplator(colors2);
	sampleEffect.move = true;

	sample.MinimumRadius = 0;
	sample.MaximumRadius = 0;
	sample.MinInclination = 0;
	sample.MaxInclination = 360;	//Angle of cone
	sample.MinAzimuth = 0;
	sample.MaxAzimuth = 360;
	sample.MinSpeed = 0;
	sample.MaxSpeed = 0;
	sample.MinLifetime = 1500;
	sample.MaxLifetime = 1500;

	sample.Origin = glm::vec3(0, 5, 0);

	sampleEffect.SetParticleEmitter(&sample);
	sampleEffect.EmitParticles();
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	//Use timer to determine how much time has passed
	static Timer timer;

	//Determine 'age' of particles
	elapsedTime = timer.getTimePassed();

	thrusterEffectL.Update(elapsedTime);
	thrusterEffectR.Update(elapsedTime);
	starEffect.Update(elapsedTime);
	sampleEffect.Update(elapsedTime);
	hyperSpeedEffect.Update(elapsedTime);

	if (show)
		Globals::camera.forward();

	thrusterL.MaxSpeed = THRUSTER_SPEED * Globals::camera.move_scale;
	thrusterR.MaxSpeed = THRUSTER_SPEED * Globals::camera.move_scale;

	if (orbit)
	{
		Matrix4 orbit, trans;
		trans.makeTranslate(0, 50, 0);
		Globals::moon.toWorld = trans * Globals::moon.toWorld;

		orbit.makeRotateZ(PI / 300);
		Globals::moon.toWorld = orbit * Globals::moon.toWorld;

		trans.makeTranslate(0, -50, 0);
		Globals::moon.toWorld = trans * Globals::moon.toWorld;
	}

    Globals::updateData.dt = 1.0/60.0;	// 60 fps
    
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 100000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
	
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
	Globals::light.bind(0);

	// Draw particles
	if (Globals::camera.move_scale >= 0.05 && show)
	{
		thrusterEffectL.Render();
		thrusterEffectR.Render();
	}

	if (hyperSpeedEnable)
		hyperSpeedEffect.Render();

	else
	{
		glScalef(50, 50, 50);
		starEffect.Render();
		glScalef(.02, .02, .02);

		if (test)
		{
			glScalef(10, 10, 10);
			sampleEffect.Render();		//Test
			glScalef(.1, .1, .1);
		}

		// Draw skybox faces
		skybox.draw_ft(Globals::drawData);
		skybox.draw_bk(Globals::drawData);
		skybox.draw_lf(Globals::drawData);
		skybox.draw_rt(Globals::drawData);
		skybox.draw_up(Globals::drawData);
		skybox.draw_dn(Globals::drawData);

		// Center planet
		if (lightShade)
		{
			Globals::lighting->bind();
			glActiveTexture(GL_TEXTURE0);
			Globals::planet->bind();
			int normal_location = glGetUniformLocation(Globals::lighting->getPid(), "color_texture");
			glUniform1i(normal_location, 0);
			Globals::globe.draw(Globals::drawData);
			Globals::lighting->unbind();
			glActiveTexture(GL_TEXTURE0);
			Globals::planet->unbind();
		}
		else
		{
			Globals::planet->bind();
			Globals::globe.draw(Globals::drawData);
			Globals::planet->unbind();
		}

		if (bumpMap)
		{
			Globals::bump->bind();
			glActiveTexture(GL_TEXTURE1);
			Globals::bumpy->bind();
			int normal_location = glGetUniformLocation(Globals::bump->getPid(), "normal_texture");
			glUniform1i(normal_location, 1);
			Globals::moon.draw(Globals::drawData);
			Globals::bump->unbind();
			glActiveTexture(GL_TEXTURE1);
			Globals::bumpy->unbind();
			glActiveTexture(GL_TEXTURE0);
		}
		else
			Globals::moon.draw(Globals::drawData);

		//Draw Tree
		tree.draw();
	}
	
	// Draw ship
	if (show)
	{
		if (celShade)
		{
			glActiveTexture(GL_TEXTURE0);
			Globals::cel->bind();
			Globals::ship.draw(Globals::drawData);
			glActiveTexture(GL_TEXTURE0);
			Globals::cel->unbind();
		}
		else
			Globals::ship.draw(Globals::drawData);
	}

    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}

void Window::keyCallback(unsigned char key, int x, int y)
{
	Matrix4 trans;
	Vector3 axis;
	float temp;
	float billboardSens = .15;

	switch (key)
	{
	case 'w':	//Speed up (ship) / move forward (cam)
		if (show)
		{
			if (Globals::camera.move_scale <= 2.95)
				Globals::camera.move_scale += 0.05;
		}
		else
		{
			temp = Globals::camera.move_scale;
			Globals::camera.move_scale = 1;
			Globals::camera.forward();
			Globals::camera.move_scale = temp;
		}
		break;
	case 'a':	//Yaw left (ship) / move left (cam)
		if (show)
		{
			axis = Globals::camera.getMatrix() * Vector3(0, 1, 0);
			trans.makeRotateArbitrary(axis, PI / 120);
			Globals::camera.rotate(trans);
		}
		else
		{
			temp = Globals::camera.move_scale;
			Globals::camera.move_scale = 1;
			Globals::camera.left();
			Globals::camera.move_scale = temp;
		}
		break;
	case 's':	//Slow down (ship) / move backward (cam)
		if (show)
		{
			if (Globals::camera.move_scale >= 0.05)
				Globals::camera.move_scale -= 0.05;
		}
		else
		{
			temp = Globals::camera.move_scale;
			Globals::camera.move_scale = 1;
			Globals::camera.backward();
			Globals::camera.move_scale = temp;
		}
		break;
	case 'd':	//Yaw right (ship) / move right (cam)
		if (show)
		{
			axis = Globals::camera.getMatrix() * Vector3(0, 1, 0);
			trans.makeRotateArbitrary(axis, -PI / 120);
			Globals::camera.rotate(trans);
		}
		else
		{
			temp = Globals::camera.move_scale;
			Globals::camera.move_scale = 1;
			Globals::camera.right();
			Globals::camera.move_scale = temp;
		}
		break;

	////////////////////////////////////////////////////
	case '1':	//Toggle cel shading
		celShade = !celShade;
		break;
	case '2':	//Toggle per pixel illumination
		lightShade = !lightShade;
		break;
	case '3':	//Toggle bump mapping
		bumpMap = !bumpMap;
		break;
	case 't':	//Generate new tree
		tree = LTree();
		break;
	case 'r':	//Reset camera, ship, and particles
		Globals::camera.reset();
		hyperSpeedEnable = false;
		break;
	case 'x':	//Hyper speed
		hyperSpeedEnable = !hyperSpeedEnable;
		if (hyperSpeedEnable)
		{
			prevSpeed = Globals::camera.move_scale;
			Globals::camera.move_scale = 10;
		}
		else
		{
			if (prevSpeed < 0.05)
				Globals::camera.move_scale = 1;
			else
				Globals::camera.move_scale = prevSpeed;
		}
		break;
	case 'b':	//Display billboard particle (for testing)
		test = !test;
		break;
	case 'e':	//Toggle between ship flight / camera controls
		show = !show;
		break;
	case 'o':
		orbit = !orbit;
		break;
	case ' ':	//Stop ship movement
		Globals::camera.move_scale = 0;
		break;
	case 'q':   //Quit application
		exit(0);
		break;
	}
}

void Window::mouseFunc(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN)
		{
			leftClick = true;
			lastPoint = Vector3(x, -y, 0);
		}
		else
			leftClick = false;
	}
}

void Window::mouseMotion(int x, int y)
{
	Vector3 curPoint;
	Vector3 rot_axis;
	Matrix4 trans_mat;
	float sensitivity = .005;	// sets the sensitivity of the mouse movement, bigger the faster
	float billboardSens = .3;
	float movedX, movedY;

	curPoint = Vector3(x, -y, 0);

	Vector3 distance = (curPoint - lastPoint);

	//Inverted movement controls
	movedX = -distance[0];
	movedY = -distance[1];

	if (leftClick)
	{
		if (!show)
			movedY = -movedY;

		rot_axis = Globals::camera.getMatrix() * Vector3(1, 0, 0);
		trans_mat.makeRotateArbitrary(rot_axis, movedY * sensitivity);
		Globals::camera.rotate(trans_mat);
		Globals::camera.addPitch(movedY * billboardSens);

		if (show)
		{
			rot_axis = Globals::camera.getMatrix() * Vector3(0, 0, 1);
			trans_mat.makeRotateArbitrary(rot_axis, movedX * sensitivity);
			Globals::camera.rotate(trans_mat);
			Globals::camera.addRoll(movedX * billboardSens);
		}

		else
		{
			rot_axis = Globals::camera.getMatrix() * Vector3(0, 1, 0);
			trans_mat.makeRotateArbitrary(rot_axis, movedX * sensitivity);
			Globals::camera.rotate(trans_mat);
			Globals::camera.addYaw(movedX * billboardSens);
		}
		lastPoint = curPoint;
	}
}

//NOTE not used anymore
Vector3 Window::trackBallMapping(int width, int height, int mouseX, int mouseY)
{
	Vector3 v;
	float d;
	v[0] = (2.0 * mouseX - width) / width;
	v[1] = (height - 2.0 * mouseY) / height;
	v[2] = 0.0;
	d = sqrtf(v[0] * v[0] + v[1] * v[1]);
	d = (d<1.0) ? d : 1.0;
	v[2] = sqrtf(1.001 - d*d);
	return v.normalize();
}
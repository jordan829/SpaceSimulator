#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "House.h"
#include "OBJObject.h"
#include "Rasterizer.h"
#include "Sphere.h"
#include "Cone.h"
#include "Skybox.h"
#include "Shader.h"
#include "Texture.h"
#include "externals\glm-0.9.1\glm\glm.hpp"					//for Vector2


#define PI 3.14159265
#define DEPTH 6		//smaller depths will improve frame rate. larger makes trees look much better
#define ANGLE 20
#define THRUSTER_SCALE 10
#define THRUSTER_SPEED 5000

class Globals
{
    
public:
	static Shader* cel;
	static Shader* bump;
	static Shader* lighting;

    static Camera camera;

    static Cube cube;

	static Texture* bumpy;
	static Texture* planet;
	static Texture ft;
	static Texture bk;
	static Texture lf;
	static Texture rt;
	static Texture up;
	static Texture dn;
	static Texture trooper;
	static Texture chestplate;

	static bool drawChest;

    static Light light;
	static Light directional;
	static Light point;
	static Light spot;

	static Light* currentLight;

	static bool light_selected;
	static int light_id;

	static Sphere globe;
	static Sphere moon;
	static Cone cone;
	static Drawable* lightShape;

    static DrawData drawData;
    static UpdateData updateData;

	static House house;

	static OBJObject ship;
	static OBJObject frigate;
	static OBJObject bunny;
	static OBJObject dragon;
	static OBJObject bear;

	static Drawable* object;

	//static Rasterizer* rasterizer;

	static bool zbuf;
	static bool interp;
};

#endif

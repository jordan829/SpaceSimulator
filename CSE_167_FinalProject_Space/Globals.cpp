#include "GLee.h"

#include "Globals.h"
Shader* Globals::cel;
Shader* Globals::bump;
Shader* Globals::lighting;

//Camera Globals::camera = Camera();
Camera Globals::camera;

//Cube Globals::cube = Cube(10.0);
Cube Globals::cube(10.0);

Texture* Globals::bumpy;
Texture* Globals::planet;
Texture Globals::ft;
Texture Globals::bk;
Texture Globals::lf;
Texture Globals::rt;
Texture Globals::up;
Texture Globals::dn;
Texture Globals::trooper;
Texture Globals::chestplate;

bool Globals::drawChest = true;

//Light Globals::light = Light();
Light Globals::light;
Light Globals::directional= Light();
Light Globals::point = Light();
Light Globals::spot = Light();
Light* Globals::currentLight;

Sphere Globals::globe = Sphere(50, 100, 100);
Sphere Globals::moon = Sphere(10, 100, 100);
Cone Globals::cone(0.3, 1.0, 25, 25);
Drawable* Globals::lightShape;

bool Globals::light_selected = false;
int Globals::light_id;

//Rasterizer* Globals::rasterizer = new Rasterizer();

/* House object */
//House Globals::house = House();

/* OBJObjects */
OBJObject Globals::ship = OBJObject();
OBJObject Globals::frigate = OBJObject();
OBJObject Globals::bunny = OBJObject();
OBJObject Globals::dragon = OBJObject();
OBJObject Globals::bear = OBJObject();
/**************/

Drawable* Globals::object;

//DrawData Globals::drawData = DrawData();
//UpdateData Globals::updateData = UpdateData();
DrawData Globals::drawData;
UpdateData Globals::updateData;

bool Globals::zbuf = 0;
bool Globals::interp = 0;
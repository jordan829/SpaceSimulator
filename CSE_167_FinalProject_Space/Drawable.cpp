#include "GLee.h"

#include "Drawable.h"
#include "Window.h"

Drawable::Drawable()
{
    this->toWorld.identity();
}

Drawable::~Drawable()
{
    //
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}

void Drawable::myDraw(DrawData& data, bool mode)
{
	std::cout << "Warning: myDraw() is not implemented" << std::endl;
}
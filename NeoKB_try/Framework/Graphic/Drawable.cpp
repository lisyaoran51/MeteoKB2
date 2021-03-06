#include "Drawable.h"

#include <stdexcept>

using namespace Framework::Graphic;
using namespace std;


Drawable::Drawable()
{
	isInitialized = false;
}

Drawable::Drawable(int w, int h)
{
	width = w;
	height = h;
	graph = new Map(w, h);
	positionX = positionY = rotation = 0;
	anchor = Anchor::TopLeft;
	alpha = 1;
	transparency = 0;
	isMasked = false;
	depth = 0;
	isInitialized = true;
	isDrawable = true;
}

Drawable::~Drawable()
{
	if (isInitialized)
		delete graph;
}

int Drawable::Initialize(int w, int h)
{
	width = w;
	height = h;
	graph = new Map(w, h);
	positionX = positionY = rotation = 0;
	anchor = Anchor::TopLeft;
	alpha = 1;
	transparency = 0;
	isMasked = false;
	depth = 0;
	isInitialized = true;
	isDrawable = true;
	return 0;
}

int Drawable::Deactivate()
{
	isDrawable = false;
	return 0;
}

bool Drawable::GetIsDrawable()
{
	return isDrawable;
}

int Drawable::SetPositionX(int x)
{
	positionX = x;
	return 0;
}

int Drawable::SetPositionY(int y)
{
	positionY = y;
	return 0;
}

int Drawable::GetPositionX()
{
	return positionX;
}

int Drawable::GetPositionY()
{
	return positionY;
}

int Drawable::SetRotation(float r)
{
	rotation = r;
	return 0;
}

float Drawable::GetRotation()
{
	return rotation;
}

int Drawable::SetAnchor(Anchor a)
{
	anchor = a;
	return 0;
}

Anchor Drawable::GetAnchor()
{
	return anchor;
}

int Drawable::SetAlpha(float a)
{
	alpha = a;
	return 0;
}

float Drawable::GetAlpha()
{
	return alpha;
}

int Drawable::SetTransparency(float t)
{
	transparency = t;
	return 0;
}

float Drawable::GetTransparency()
{
	return transparency;
}

int Drawable::SetWidth(int w)
{
	// TODO: throw error 還不想寫
	return 0;
}

int Drawable::SetHeight(int h)
{
	// TODO: throw error 還不想寫
	return 0;
}

int Drawable::GetWidth()
{
	return width;
}

int Drawable::GetHeight()
{
	return height;
}

int Drawable::SetIsMasked(bool value)
{
	isMasked = value;
	return 0;
}

bool Drawable::GetIsMasked()
{
	return isMasked;
}

int Drawable::SetDepth(int d)
{
	depth = d;
	return 0;
}

int Drawable::GetDepth()
{
	return depth;
}

Map * Drawable::GetGraph()
{
	if(!isInitialized)
		throw logic_error("Map* Drawable::GetGraph() : the drawable hasn't been initialized.");

	return graph;
}

int Drawable::SetIsDrawable(bool value)
{
	isDrawable = value;
	return 0;
}

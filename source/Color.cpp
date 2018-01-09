#include "Color.h"
#include "Lerp.h"

const Color Color::white	= Color(255, 255, 255, 255);
const Color Color::black	= Color(0, 0, 0, 255);
const Color Color::red		= Color(255, 0, 0, 255);
const Color Color::green	= Color(0, 255, 0, 255);
const Color Color::blue		= Color(0, 0, 255, 255);
const Color Color::none		= Color(0, 0, 0, 0);


Color Color::Lerp(Color a, Color b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
	{
		Color result;
		result.r = (unsigned char)::Lerp(a.r, b.r, t);
		result.g = (unsigned char)::Lerp(a.g, b.g, t);
		result.b = (unsigned char)::Lerp(a.b, b.b, t);
		result.a = (unsigned char)::Lerp(a.a, b.a, t);
		return result;
	}
}

Color::Color(void) : Color(0,0,0,0)
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	setRGBA(r, g, b, a);
}

void Color::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


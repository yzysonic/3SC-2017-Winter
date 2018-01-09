#pragma once

class Color
{
public:
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color none;

	static Color Lerp(Color a, Color b, float t);

public:
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;

	Color(void);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);


	void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};
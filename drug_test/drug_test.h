#pragma once

#include "resource.h"

#include <Windows.h>

#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include "Interfaces.h"
#include <memory>
using std::unique_ptr;
//#include <gl/glaux.h>
#include <process.h>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
using std::_List_iterator;
using std::list;
using std::vector;
#pragma comment(lib, "OpenGL32.lib")
void addObject(Paintable* object);
void InitOpenGL();
void PaintOpenGL();
class GLCoord;
class Manipulatable;
class Paintable;
class Circle;
class Ring;
class MouseHandler;
class MouseManipulator;
class MousePullCircleManipulator;
class MouseHandlerState;
class WaitingMouseHandler;
class DruggingMouseHandler;
GLCoord getMouseCoord();
bool isLow(double d);
class GLCoord{
public:
	GLCoord() : x(0), y(0){}
	GLCoord(double x, double y) : x(x), y(y){}
	virtual double getX() const { return x; }
	virtual double getY() const { return y; }
	virtual void setX(double x){ this->x = x; }
	virtual void setY(double y){ this->y = y; }
	double getNormalX() const { return x; }
	double getNormalY() const { return y; }
protected:
	double x, y;
};
double sqr(double x);
double distance_p(const GLCoord& p1,const GLCoord& p2);
/*class GLCoordN : public GLCoord{
public:
	GLCoordN() : GLCoord(){}
	GLCoordN(double x, double y) : GLCoord(x, y){}
};
class GLCoordP : public GLCoord{
public:
	GLCoordP() : GLCoord(){}
	GLCoordP(double x, double y) : GLCoord(x, y){}
	virtual double getX(){ return x * 2.0 - 1 + 0.0001; }
	virtual double getY(){ return y * 2.0 - 1 + 0.0001; }
};
class GLCoord2TimesLower : public GLCoord{
public:
	GLCoord2TimesLower() : GLCoord(){}
	GLCoord2TimesLower(double x, double y) : GLCoord(x, y){}
	virtual double getX(){ return x / 2; }
	virtual double getY(){ return y / 2; }
};*/
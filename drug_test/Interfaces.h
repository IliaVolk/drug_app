#include "stdafx.h"
#pragma once
#include "drug_test.h"
#include <vector>
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


//rectangles
class Rectangle;
class RectangleMouseVerticalSideManipulator;
class RectangleMouseHorizontalSideManipulator;
class RectangleMouseCornerManipulator;
////
class MouseManipulator{
public:
	MouseManipulator(Manipulatable* manipulatable, const GLCoord& curM);
	virtual void manipulate(const GLCoord& point);
	MouseHandlerState* getMouseHandler();
protected:
	double dx;
	double dy;
	Manipulatable* manipulatable;
};

class MousePullCircleManipulator : public MouseManipulator{
public:
	MousePullCircleManipulator(Manipulatable* manipulatable,const GLCoord& curM);
	void manipulate(const GLCoord& point);
protected:
	Circle* circle;
};
class RectangleMouseVerticalSideManipulator : public MouseManipulator{
public:
	RectangleMouseVerticalSideManipulator(Manipulatable* manipulatable, const GLCoord& curM);
	void manipulate(const GLCoord& point);
protected:
	class MyRectangle* rectangle;
	double baseY;
};

class RectangleMouseHorizontalSideManipulator : public MouseManipulator{
public:
	RectangleMouseHorizontalSideManipulator(Manipulatable* manipulatable, const GLCoord& curM);
	void manipulate(const GLCoord& point);
protected:
	class MyRectangle* rectangle;
	double baseX;
};
class RectangleMouseCornerManipulator : public MouseManipulator{
public:
	RectangleMouseCornerManipulator(Manipulatable* manipulatable, const GLCoord& curM);
	void manipulate(const GLCoord& point);
protected:
	std::shared_ptr<RectangleMouseHorizontalSideManipulator> mx;//horizontal manipulator
	std::shared_ptr<RectangleMouseVerticalSideManipulator> my;//vertical manipulator
};
class Manipulatable{
public:
	~Manipulatable(){}
	Manipulatable();
	virtual MouseManipulator* checkPoint(const GLCoord& point){ return false; }
	GLCoord* getBasePoint(){ return basePoint.get(); }
protected:
	std::unique_ptr<GLCoord> basePoint;
};


class Paintable{
public:
	~Paintable(){}
	virtual void paint(){};
};



class MyRectangle : public Manipulatable, public Paintable{
public:
	MyRectangle(double x, double y, double w, double h, double cr, double cg, double cb);
	virtual void paint();
	virtual MouseManipulator* checkPoint(const GLCoord& point);
	friend MouseManipulator;
	friend RectangleMouseVerticalSideManipulator;
	friend RectangleMouseHorizontalSideManipulator;
	friend RectangleMouseCornerManipulator;
protected:
	bool isLow(double d){
		return abs(d) < 0.01;
	}
	double w; 
	double h;
	double color[3];
};


class Circle : public Manipulatable, public Paintable{
public:
	Circle(double x, double y, double radius, double cr, double cg, double cb);
	virtual void paint();
	void setR(double newR);
	virtual MouseManipulator* checkPoint(const GLCoord& point);
	friend MouseManipulator;
	friend MousePullCircleManipulator;
protected:
	double r;
	double rver;
	double color[3];
};
class Ring : public Circle{
public:
	Ring(double x, double y, double radius, double cr, double cg, double cb) :
		Circle(x, y, radius, cr, cg, cb), thickness(0.02) {}
	virtual void paint();
	
	virtual MouseManipulator* checkPoint(const GLCoord& point);
	friend MouseManipulator;
	friend MousePullCircleManipulator;
protected:
	double thickness;
};




class MouseHandlerState{
public:
	virtual void handleMouseLeaveWindow(MouseHandler*){}
	virtual void handleLeftMouseUp(MouseHandler*){}
	virtual void handleLeftMouseDown(MouseHandler*){}
	virtual void handleMouseMove(MouseHandler*){}
	
protected:
	
	MouseHandlerState(){}
};

extern HWND hWnd;
class MouseHandler{
public:
	MouseHandler();
	virtual void addObject(Manipulatable* object);
	virtual void handleMouseLeaveWindow(){ currentHandler->handleMouseLeaveWindow(this); }
	virtual void handleLeftMouseUp(){ currentHandler->handleLeftMouseUp(this); }
	virtual void handleLeftMouseDown(){ currentHandler->handleLeftMouseDown(this); }
	virtual void handleMouseMove(){	currentHandler->handleMouseMove(this);}
	
	void setCurrentHandler(std::shared_ptr<MouseHandlerState> newValue);
	std::vector<Manipulatable*>::iterator begin(){ return objects.begin(); }
	std::vector<Manipulatable*>::iterator end(){ return objects.end(); }
protected:
	std::vector<Manipulatable*> objects;
	std::shared_ptr<MouseHandlerState> currentHandler;
};
class WaitingMouseHandler : public MouseHandlerState{
public:
	//virtual void handleLeftMouseUp(MouseHandler*){}
	virtual void handleLeftMouseDown(MouseHandler*);
	//virtual void handleMouseMove(MouseHandler*){}
};

class DruggingMouseHandler : public MouseHandlerState{
public:
	DruggingMouseHandler(MouseManipulator* manipulator);
	virtual void handleMouseLeaveWindow(MouseHandler*);
	virtual void handleLeftMouseUp(MouseHandler*);
	//virtual void handleLeftMouseDown(MouseHandler*){}
	virtual void handleMouseMove(MouseHandler*);
private:
	std::shared_ptr<MouseManipulator> manipulator;
};
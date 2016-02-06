#include "stdafx.h"
#include "drug_test.h"

Manipulatable::Manipulatable() : basePoint(new GLCoord(0,0)){}
MouseHandler::MouseHandler() : currentHandler(new WaitingMouseHandler()){}
void MouseHandler::addObject(Manipulatable* m){
	//objects.push_back(std::shared_ptr<Manipulatable>(m));
	objects.push_back(m);
}

Circle::Circle(double x, double y, double radius, double cr, double cg, double cb){
	basePoint->setX(x);
	basePoint->setY(y);
	color[0] = cr;
	color[1] = cg;
	color[2] = cb;
	setR(radius);
}
void MouseManipulator::manipulate(const GLCoord& point){
	manipulatable->getBasePoint()->setX(point.getX() - dx);
	manipulatable->getBasePoint()->setY(point.getY() - dy);
}

RectangleMouseVerticalSideManipulator::RectangleMouseVerticalSideManipulator(
	Manipulatable* manipulatable, const GLCoord& curM) : 
	MouseManipulator(manipulatable, curM), baseY(curM.getY()){
	if (MyRectangle* r = dynamic_cast<MyRectangle*>(manipulatable)){
		rectangle = r;
	}
	else throw std::bad_exception("Needed a rectangle");
}

void RectangleMouseVerticalSideManipulator::manipulate(const GLCoord& point){
	GLCoord* basePoint = manipulatable->getBasePoint();
	if (baseY > basePoint->getY()){
		rectangle->h += point.getY() - baseY;
		basePoint->setY(point.getY() - rectangle->h / 2);
	}
	else {
		rectangle->h -= point.getY() - baseY;
		basePoint->setY(point.getY() + rectangle->h / 2);
	}
	rectangle->h = abs(rectangle->h);
	baseY = point.getY();
}

RectangleMouseHorizontalSideManipulator::RectangleMouseHorizontalSideManipulator(
	Manipulatable* manipulatable, const GLCoord& curM) : MouseManipulator(manipulatable, curM), baseX(curM.getX()){
	if (MyRectangle* r = dynamic_cast<MyRectangle*>(manipulatable)){
		rectangle = r;
	}
	else throw std::bad_exception("Needed a rectangle");
}

void RectangleMouseHorizontalSideManipulator::manipulate(const GLCoord& point){
	GLCoord* basePoint = manipulatable->getBasePoint();
	if (baseX < basePoint->getX()){
		rectangle->w -= point.getX() - baseX;
		basePoint->setX(point.getX() + rectangle->w / 2);
	}
	else {
		rectangle->w += point.getX() - baseX;
		basePoint->setX(point.getX() - rectangle->w / 2);
	}
	rectangle->w = abs(rectangle->w);
	baseX = point.getX();
}

RectangleMouseCornerManipulator::RectangleMouseCornerManipulator(
	Manipulatable* manipulatable, const GLCoord& curM):
	MouseManipulator(manipulatable, curM),
	mx(new RectangleMouseHorizontalSideManipulator(manipulatable, curM)),
	my(new RectangleMouseVerticalSideManipulator(manipulatable, curM))
	{}

void RectangleMouseCornerManipulator::manipulate(const GLCoord& point){
	mx->manipulate(point);
	my->manipulate(point);
}
void MousePullCircleManipulator::manipulate(const GLCoord& point){
	double x = point.getX() - circle->basePoint->getX();
	double y = (point.getY() - circle->basePoint->getY()) / (circle->rver / circle->r);

	circle->setR(sqrt(x*x + y*y));
}
void Circle::paint(){
	glColor3dv(color);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 25; i++)
	{
		float angle = 2.0 * 3.1415926 * float(i) / float(25);

		float dx = r * cosf(angle);
		float dy = rver * sinf(angle);
		double x = basePoint->getX() + dx;
		double y = basePoint->getY() + dy;
		glVertex2d(x,y);
		//glVertex2f(points[i].coordX, points[i].coordY);
	}
	glEnd();
}

MouseManipulator* Circle::checkPoint(const GLCoord& point){
	double x = basePoint->getX() - point.getX();
	double y = (basePoint->getY() - point.getY()) / (rver / r);
	double x_y = x*x + y*y;
	if (isLow(x_y - r*r)){
		return new MousePullCircleManipulator(this, point);
	}
	if ( x_y  < r*r){
		return new MouseManipulator(this, point);
	}
	return false;
}
using namespace std;
DruggingMouseHandler::DruggingMouseHandler(MouseManipulator* manipulator) : manipulator(manipulator){
}
void WaitingMouseHandler::handleLeftMouseDown(MouseHandler* base){
	for_each(base->begin(), base->end(), [&](Manipulatable* m){
		GLCoord curMouseCoord = getMouseCoord();
		if (MouseManipulator* manipulator = m->checkPoint(curMouseCoord)){
			base->setCurrentHandler (shared_ptr<MouseHandlerState>(
				manipulator->getMouseHandler()));
		}
	});
}
void DruggingMouseHandler::handleMouseMove(MouseHandler* base){
	InvalidateRect(hWnd, 0, 0);
	manipulator->manipulate(getMouseCoord());
}
void DruggingMouseHandler::handleLeftMouseUp(MouseHandler* base){
	base->setCurrentHandler(shared_ptr<MouseHandlerState>(
		new WaitingMouseHandler()));
}
void DruggingMouseHandler::handleMouseLeaveWindow(MouseHandler* base){
	base->setCurrentHandler(shared_ptr<MouseHandlerState>(
		new WaitingMouseHandler()));
}
MouseManipulator::MouseManipulator(Manipulatable* manipulatable, const  GLCoord& curM) :
		manipulatable(manipulatable){
	dx = curM.getX() - manipulatable->getBasePoint()->getX();
	dy = curM.getY() - manipulatable->getBasePoint()->getY();
}


MouseManipulator* Ring::checkPoint(const GLCoord& point){
	if (MouseManipulator* m = Circle::checkPoint(point)){
		double x = basePoint->getX() - point.getX();
		double y = (basePoint->getY() - point.getY()) / (rver / r);
		double R = r - thickness;
		if (x*x + y*y > R*R){
			return m;
		}
		return 0;
	}
	return 0;
}
void Ring::paint(){
	glColor3dv(color);
	//glBegin(GL_POLYGON);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 25; i++)
	{
		float angle = 2.0 * 3.1415926 * float(i) / float(25);

		float dx = r * cosf(angle);
		float dy = rver * sinf(angle);
		double x = basePoint->getX() + dx;
		double y = basePoint->getY() + dy;
		glVertex2d(x, y);
		//glVertex2f(points[i].coordX, points[i].coordY);
	}
	glEnd();
	//glColor4d(color[0], color[1], color[2], 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 25; i++)
	{
		float angle = -2.0 * 3.1415926 * float(i) / float(25);

		float dx = (r-thickness) * cosf(angle);
		float dy = (rver-thickness) * sinf(angle);
		double x = basePoint->getX() + dx;
		double y = basePoint->getY() + dy;
		glVertex2d(x, y);
		//glVertex2f(points[i].coordX, points[i].coordY);
	}
	glEnd();
}

MousePullCircleManipulator::MousePullCircleManipulator(Manipulatable* manipulatable,const GLCoord& curM) :
MouseManipulator(manipulatable, curM){
	if (Circle* c = dynamic_cast<Circle*>(manipulatable)){
		circle = c;
	}
	else throw std::bad_exception("Circle Manipulator needs Circle");
}

void Circle::setR(double newR) {
	r = newR;
	RECT wcoord;
	extern HWND hWnd;
	GetClientRect(hWnd, &wcoord);
	rver = r * wcoord.right / wcoord.bottom;
}
void MouseHandler::setCurrentHandler(std::shared_ptr<MouseHandlerState> newValue){
	currentHandler = newValue;
}
MouseHandlerState* MouseManipulator::getMouseHandler(){
	return new DruggingMouseHandler(this);
}


MyRectangle::MyRectangle(double x, double y, double w, double h, double cr, double cg, double cb)
:  w(w), h(h)
{
	color[0] = cr;
	color[1] = cg;
	color[2] = cb;
	basePoint->setX((x + w) / 2);
	basePoint->setY((y - h) / 2);

}

void MyRectangle::paint(){
	glColor3dv(color);
	glBegin(GL_QUADS);
	double x = basePoint->getX() - w / 2;
	double y = basePoint->getY() + h / 2;
	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y - h);
	glVertex2d(x, y - h);
	glEnd();
	glColor3d(1, 1, 1);
}

MouseManipulator* MyRectangle::checkPoint(const GLCoord& point){
	double x = basePoint->getX() - w / 2;
	double y = basePoint->getY() + h / 2;
	if (isLow(distance_p(point, GLCoord(x, y))) ||
		isLow(distance_p(point, GLCoord(x + w, y))) || 
		isLow(distance_p(point, GLCoord(x+w, y-h))) ||
		isLow(distance_p(point, GLCoord(x, y - h)))){
		return new RectangleMouseCornerManipulator(this, point);
		}
	double px = point.getX();
	double py = point.getY();
	double m = 0.008;
	bool xInRect = px > x-m && px < x + w+m;
	bool yInRect = py < y+m && py > y - h-m;
	if (xInRect && (isLow(py - y) || isLow(py - (y - h)))){
		return new RectangleMouseVerticalSideManipulator(this, point);
	}
	if (yInRect && (isLow(px - x)|| isLow(px - (x + w)))){
		return new RectangleMouseHorizontalSideManipulator(this, point);
	}
	/*if (xInRect){
		if (isLow(py - y)){
			return new RectangleMouseVerticalSideManipulator(this, point, y);
		}
		if (isLow(py - (y - h))){
			return new RectangleMouseVerticalSideManipulator(this, point, y - h);
		}
	}
	if (yInRect){
		if (isLow(px - x)){
			return new RectangleMouseHorizontalSideManipulator(this, point, x);
		}
		if (isLow(px - (x + w))){
			return new RectangleMouseHorizontalSideManipulator(this, point, x + w);
		}
	}*/
	if (xInRect && yInRect){
		return new MouseManipulator(this, point);
	}
	return 0;
}








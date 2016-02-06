#include "stdafx.h"
#include "drug_test.h"

HGLRC  hGLRC;              // ѕосто¤нный контекст рендеринга
HDC  hDC;              // ѕриватный контекст устройства GDI
extern HWND  hWnd;				// «десь будет хранитс¤ дескриптор окна

extern HINSTANCE  hInstance;              // «десь будет хранитс¤ дескриптор приложени¤
extern MouseHandler mouseHandler;

//vector<std::shared_ptr<Paintable> > paintable;
vector<Paintable*> paintable;
void addObject(Paintable* o){
	//paintable.push_back(std::shared_ptr<Paintable>(o));
	paintable.push_back(o);
	if (Manipulatable* om = dynamic_cast<Manipulatable*>(o)){
		mouseHandler.addObject(om);
	}
}
int time, dt;
void updating(void*params){
	while (hWnd){
		time = GetCurrentTime();
		InvalidateRect(hWnd, NULL, NULL);
		dt = GetCurrentTime() - time;
		if (dt < 10) Sleep(10 - dt);
	}
}
//class Paintable;

void InitOpenGL(){

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0

	};


	//ѕолучаем ссылку на поверхность где будем рисовать
	hDC = GetDC(hWnd);


	//задаем параметры точек
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);

	//задаем параметры точки
	SetPixelFormat(hDC, pixelFormat, &pfd);

	//сообщаем OpenGL, где будем рисовать
	hGLRC = wglCreateContext(hDC);

	//создаем контекст рисовани¤
	wglMakeCurrent(hDC, hGLRC);
	for (int i = 0; i < 20; i++)
	addObject(new MyRectangle(0, 0, 0.2, 0.2, i/20.0, 0, 0));
		addObject(new Circle(0.5, 0.5, 0.1, 0, 0, 0));
		addObject(new Circle(0, 0.5, 0.1, 0, 1, 0));
		addObject(new Circle(0.5, 0, 0.1, 0, 0, 1));
		addObject(new Circle(0, 0, 0.1, 1, 1, 0));
		addObject(new Circle(0, -0.5, 0.1, 1, 0, 0));
	/*addObject(new Ring(0.5, 0.5, 0.1, 0, 0, 0));
	addObject(new Ring(0, 0.5, 0.1, 0, 1, 0));
	addObject(new Ring(0.5, 0, 0.1, 0, 0, 1));
	addObject(new Ring(0, 0, 0.1, 1, 1, 0));
	addObject(new Ring(0, -0.5, 0.1, 1, 0, 0)); */
	//_beginthread(updating, 0, NULL);
	
}




GLCoord ConvertMouseCoord(POINT currentMouse){
	RECT windowSize;
	GetClientRect(hWnd, &windowSize);
	GLCoord mouseOpenGLCoord;
	float x = currentMouse.x;
	float h = windowSize.right;
	float ax = 2 * x / h - 1;
	float y = currentMouse.y;
	float w = windowSize.bottom;
	float ay = 2 * (w - y) / w - 1;

	mouseOpenGLCoord.setX(ax);
	mouseOpenGLCoord.setY(ay);
	return mouseOpenGLCoord;
}

void PaintOpenGL(){

	//устанавливаем очистку черным цветом
	glClearColor(1, 1, 1, 0);

	//glPushMatrix();


	glColor3f(1.0, 1.0, 1.0);
	
	//glPopMatrix();
	//new буффер (двойна¤ буферизаци¤)
	for_each(paintable.begin(), paintable.end(), [](/*std::shared_ptr<Paintable>&*/Paintable* p) {
		p->paint();
	});

		SwapBuffers(hDC);
	//очистить буфер точек и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glutSwapBuffers();

}
//class GLCoord;
GLCoord getMouseCoord(){
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);
	return ConvertMouseCoord(point);
	
	//InvalidateRect(hWnd, NULL, TRUE);
}
bool isLow(double d){
	return abs(d) < 0.0017;
}
double sqr(double x){ return x*x; }
double distance_p(const GLCoord& p1,const GLCoord& p2){
	return sqrt(sqr(p1.getNormalX() - p2.getNormalX()) + sqr(p1.getNormalY() - p2.getNormalY()));
}
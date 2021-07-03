#include "../../utilites/glwnd.h"
#include "CGUI.h"

INT Width, Height;

CGUI gui(800, 600);

void fn_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	gluOrtho2D(0.0, (double)Width, (double)Height, 0.0);

	//glColor3ub(255, 255, 255);
	//glRecti(10, 10, 100, 100);
	glEnableClientState(GL_VERTEX_ARRAY);
	gui.Draw();
	glDisableClientState(GL_VERTEX_ARRAY);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void fn_window_resize(HWND hWnd, int width, int height)
{
	if (!height)
		height = 1;

	Width = width;
	Height = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, width / (double)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gui.OnResize(width, height);
}

void fn_mousemove(HWND hWnd, int x, int y)
{
	gui.MouseMove(x, y);
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	gui.MouseClick(x, y, button);
}

void fn_charinput(HWND hWnd, char symbol)
{
}

//https://docs.microsoft.com/ru-ru/windows/win32/inputdev/wm-keydown
void fn_keydown(HWND hWnd, INT state, WPARAM wparam, LPARAM lparam)
{
	INT key = (INT)wparam;
	if (state == KEY_DOWN) {
		if (key == 27) //esc code
			exit(0); //close program
	}
}

//Add this GL functions
void fn_windowcreate(HWND hWnd)
{
	glClearColor(0.1, 0.1, 0.1, 1.f);

	class : public IButtonHandler {
	public:
		void OnMouseButtonDown(int id, int button, int state) {}
		bool OnClick(int id, CPoint &point) {
			printf("Button clicked!\n");
			return true;
		}
		bool OnHoverSet(int id) { return true; }
		bool OnHoverLoss(int id) { return true; }
	} buttons_handler;

	CCanvas *pcanvas = new CCanvas(100, 100, 300, 300, CColor3(40, 40, 40), CColor3(255, 255, 255));
	pcanvas->AddElement(new CCanvas(150, 150, 20, 20, CColor3(100, 100, 100), CColor3(255, 255, 255)));
	pcanvas->AddElement(new CButton("test button", 1, 10, 10, &buttons_handler));
	gui.AddElement(pcanvas);
}

void fn_windowclose(HWND hWnd)
{
	exit(0);
}

int main()
{
	create_window("Sample OpenGL Application", __FILE__ __TIME__,
		24,					  //Colors bits
		32,					  //Depth bits
		fn_draw,			  //Draw function
		fn_window_resize,	  //Window resize function
		fn_mousemove,		  //Mouse move function
		fn_mouseclick,		  //Mouse click function
		fn_charinput,		  //Char input function
		fn_keydown,			  //Keydown function
		fn_windowcreate,	  //Window create function
		fn_windowclose,		  //Window close function
		0,					  //Window position X
		0,					  //Window position Y
		800,				  //Window width
		600);				  //Window height
	return 0;
}
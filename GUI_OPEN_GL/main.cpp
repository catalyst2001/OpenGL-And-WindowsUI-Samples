/*
�������� �������

glColor3[ub] - UNSIGNED BYTE (unsigned char) �� ���� ���� ����� ���������� �� 0 �� 255
3 - ������ RGB. ���� �� ���� glColor4ub, ���� �� RGBA (� ���������� ������������)

i - integer
f - float
d - double
iv - integer array
fv - float array

glColor... - ������������� ���� ���� ������� ������
glEnableClientState(type) - �������� ����� ���������������� ��������, ����� ��� ��������� ������� ������, ������ � ���������� ���������
GL_VERTEX_ARRAY � ����� ������� ����� ������� �������

glDisableClientState(type) - ��������� ��� ��������
���� ��� ������� ������ ������, OPENGL ������� ��� ����� ������� ��������� �� ������ ������� ��������� ����������, � ������� glVertexPointer. ���� �������� GL_VERTEX_ARRAY ����� ��������, � ��������� �� ����� �������, �� �������� ����

glEnable(type) - �������� ����� ��������, ����� ��� �������� (GL_TEXTURE_2D, GL_DEPTH_TEST � ��)
glDisable(type) - ��������� ��������� ��������
glVertexPointer(���������� �����,  ��� ������, ���, ���������); - �������� opengl ���������� � ���, � ������ ����� ����� ������ ������
1 ���������� ����� - ���������� ���������� � ����� �������, ������������ ��� �������� ����������. ��� ������� ��� 2 (xy) ��� 3 (xyz)
2 ��� ������ - ������ ���� ������ ���������� ����� ������������. ���� ������ ������������ � 3D ��������,  ����� ������������ GL_FLOAT. ���� ������ ���������� � ��������� �������� (��� ��� ��������), ����� ���������� ��� GL_INT, ��� ������� ��� ������ ������ ���� ����� ��� INT
3 ��� - ������ �� ������ ��������, ����� ����� ���������� ������ � ���� ����� � �������� ���������. (�������� ���� ��������� � ������ ������� �������� ��������� ������, � ����� �������� ������ ������ ������� ����� ������������ ��� �����������) ��� ������� ����������� �� ������ ������. � ������� ��� �� ���������, ������ ��� � ���� ��� ������� �� �������� ��� �����.
4 ��������� - ���� �������� ��������� �������� ����� � ������, � �������� ������� �������� ��������� ������ ��� �����������

glDrawArrays(��� ���������, ������, �����) - ���������� ������, �� ����� ���������� ����������
1 ��� ��������� - GL_QUADS, GL_TRIANGLES � ���� �������� (� ���� "���� ���������� opengl")
2 ������ - ��������� ������ �������, � ������� ��������� �������� �����������
3 ����� - ������ ���������� ��������

glDrawElements ���������� ������� � ��������� �������
glDrawElements(��� ���������, ������, ��� ������� ��������, ��������� �������);
1 ��� ��������� - ������ ����
2 ������
3 ��� ������� �������� - GL_UNSIGNED_INT
4 ��������� ������ �������� - ������ ������� ����� ������� ������� ������ ��� �����������

glMatrixMode(type) - ������������� �� ��������� �������
GL_PROJECTION - ������� ��������
GL_MODELVIEW - ������� ����
��������� ������� �� ���� ������ ��� �� ����������� � �� ���� � ���

glPushMatrix() - ��������� ��������� � ������� glMatrixMode(), ������� � �����
glPopMatrix() - ��������������� ��������� ������� �� �����
glLoadIdentity() - ���������� ������� �� ��������� (��������� ������ ����)
*/

#include "main.h"

Cgui Gui;

CProgressBar * ppbs;
CButton * ppb;
CCheckBox * ppbb;
CTrackBar * bpp;



void fn_draw()
{
	ppbs->SetPercent(bpp->percent);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, vv);
	Gui.OnDraw();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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
	Gui.OnResizewindow(width, height);
}

void fn_mousemove(HWND hWnd, int x, int y)
{
	//printf("%Lf\n ", bpp->percent);
	Gui.OnMouseMove(x, y);
}

void fn_mouseclick(HWND hWnd, int x, int y, int button, int state)
{
	Gui.OnMouseClick(x, y, button, state);
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
	LoadTexture("notex.bmp", &g_texture);
	ppbs = new CProgressBar(100, 50, 120, 8, 66, { 255,255,255 }, { 246,145,205 });
	ppb = new CButton(100, 100, 120, 45, { 240, 240, 240 }, UI_BTN_TEXTURED, g_texture.texID);
	ppbb = new CCheckBox(100, 150, 15, 15, 0, { 100, 100, 100 });
	bpp = new CTrackBar(100, 200, 300, 17, { 0, 102, 204 });
	Gui.AddElement(ppb);
	Gui.AddElement(ppbb);
	Gui.AddElement(ppbs);
	Gui.AddElement(bpp);
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
		50,					  //Window position X
		50,					  //Window position Y
		Width,				  //Window width
		Height);				//Window height
	return 0;
}

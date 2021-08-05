/*
ПРЕФИКСЫ ФУНКЦИЙ

glColor3[ub] - UNSIGNED BYTE (unsigned char) ТО есть цвет будет задаваться от 0 до 255
3 - значит RGB. Если бы было glColor4ub, было бы RGBA (с установкой прозрачности)

i - integer
f - float
d - double
iv - integer array
fv - float array

glColor... - устанавливает цвет ниже лежащей фигуры
glEnableClientState(type) - включает более оптимизированные операции, такие как рендеринг массива вершин, цветов и текстурных координат
GL_VERTEX_ARRAY и далее найдешь рядом лежащие дефайны

glDisableClientState(type) - отключает эти операции
ЕСЛИ БЫЛ ВКЛЮЧЕН МАССИВ ВЕРШИН, OPENGL ОЖИДАЕТ ЧТО БУДЕТ ПЕРЕДАН УКАЗАТЕЛЬ НА ДАННЫЕ КОТОРЫЕ ТРЕБУЕТСЯ ОТОБРАЗИТЬ, С ПОМОЩЬЮ glVertexPointer. ЕСЛИ ОПЕРАЦИЯ GL_VERTEX_ARRAY БУДЕТ ВКЛЮЧЕНА, А УКАЗАТЕЛЬ НЕ БУДЕТ ПЕРЕДАН, ТЫ ПОЛУЧИШЬ КРАШ

glEnable(type) - включает общие операции, такие как текстуры (GL_TEXTURE_2D, GL_DEPTH_TEST и тд)
glDisable(type) - отключает указанную операцию
glVertexPointer(количество полей,  тип данных, шаг, указатель); - передает opengl информацию о том, с какого места брать данные вершин
1 количество полей - количество переменных в одной вершине, используемых для передачи информации. Как правило это 2 (xy) или 3 (xyz)
2 тип данных - какого типа данных информация будет передаваться. Если данные используются в 3D проекции,  лучше использовать GL_FLOAT. Если данные передаются в двумерной проекции (для гуи например), можно передавать тип GL_INT, при условии что массив вершин тоже имеет тип INT
3 ШАГ - отступ от начала элемента, чтобы брать информацию только с того места с которого требуется. (допустим есть структура в начале которой хранятся служебные данные, а далее хранятся данные вершин которые нужно использовать для отображения) Шаг поможет перешагнуть до нужных данных. Я никогда это не использую, потому что у меня это никогда не работало как нужно.
4 Указатель - сюда передаем указатель хранящий адрес в памяти, с которого следует начинать считывать данные для отображения

glDrawArrays(тип примитива, начало, конец) - отображает данные, по ранее переданной информации
1 тип примитива - GL_QUADS, GL_TRIANGLES и тому подобные (в гугл "типы примитивов opengl")
2 начало - начальный индекс вершины, с которой требуется начинать отображение
3 конец - индекс последнего элемента

glDrawElements Отображает вершины в указанном порядке
glDrawElements(тип примитива, размер, тип массива индексов, указатель вершины);
1 тип примитива - описал выше
2 размер
3 тип массива индексов - GL_UNSIGNED_INT
4 указатель массив индексов - массив который будет хранить порядок вершин для отображения

glMatrixMode(type) - переключается на выбранную матрицу
GL_PROJECTION - матрица проекции
GL_MODELVIEW - матрица вида
Остальное описать не могу потому что не использовал и не знаю о них

glPushMatrix() - сохраняет выбранную с помощью glMatrixMode(), матрицу в стеке
glPopMatrix() - восстанавливает выбранную матрицу из стека
glLoadIdentity() - сбрасывает матрицу на единичную (обнуление своего рода)
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

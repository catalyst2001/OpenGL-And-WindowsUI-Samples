#include "../utilites/glwnd.h"
#include "../utilites/texture.h"
#include "Cgui.h"


static Texture g_texture;

static CPointf vv[4] = { {0,0}, {1,0}, {1, 1}, {0, 1} };
INT Width = 800, Height = 600;


/*
all													(flag, id, int x, int y, int white, int height, collor, int m_x, int m_y)
button
trackbar											(float proc)
��� ����
����� ���� ���������� ����							(int num, char * all_text)
���� ��������										(float proc)




0 1
1 1
1 0
0 0

/editbox
*/


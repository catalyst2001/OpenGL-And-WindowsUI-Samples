#include "glwnd.h"
//#include "texture.h"
#include "FTFont.h"
#include "Cgui.h"
#include "СFont.h"


//static Texture g_texture;
extern CFontManager fontmanager;
extern СFont *p_font;

static CPointf vv[4] = { {0,0}, {1,0}, {1, 1}, {0, 1} };
INT Width = 800, Height = 600;


/*
all													(flag, id, int x, int y, int white, int height, collor, int m_x, int m_y)
button
trackbar											(float proc)
чек бокс
комбо бокс выпадающее меню							(int num, char * all_text)
бокс загрузки										(float proc)




0 1
1 1
1 0
0 0

/editbox
*/


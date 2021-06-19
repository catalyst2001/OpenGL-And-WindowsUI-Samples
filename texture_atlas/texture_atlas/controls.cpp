#include "controls.h"

HFONT g_hFont;

void OpenSelectColorDialog(HWND parent, color_t *color)
{
	CHOOSECOLORA cc;
	static COLORREF custColors[16];
	BYTE rr = 255;
	BYTE gg = 255;
	BYTE bb = 255;
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = parent;
	cc.rgbResult = RGB(rr, gg, bb);
	cc.lpCustColors = custColors;
	cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColorA(&cc)) {
		color->r = (int)GetRValue(cc.rgbResult);
		color->g = (int)GetGValue(cc.rgbResult);
		color->b = (int)GetBValue(cc.rgbResult);
	}
}

// Создаёт диалог выбора файла для открытия
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для выбора файлов
BOOL OpenFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter)
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window;
	ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrInitialDir = initialdir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	return GetOpenFileNameA(&ofn);
}

// Открыть диалог сохранения файла
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для сохранения файлов
BOOL SaveFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter)
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window;
	ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrInitialDir = initialdir;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	return GetSaveFileNameA(&ofn);
}

HWND CreateListView(HWND hParentWindow, INT id, DWORD dwFlags, INT x, INT y, INT width, INT height)
{
	HWND hLw = CreateWindowExA(NULL, WC_LISTVIEWA, "", WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_EDITLABELS|WS_EX_CLIENTEDGE, x, y, width, height, hParentWindow, (HMENU)id, NULL, NULL);
	//SendMessageA(hLw, WM_SETFONT, (WPARAM)g_hFont, (LPARAM)NULL);
	return hLw;
}

void ListView_InitCols(HWND hLw, lw_colnames_t *cols, INT numcols)
{
	LVITEMA lvitem = { 0 };
	LVCOLUMNA col = { 0 };
	for (int i = 0; i < numcols; ++i) {
		col.mask = LVCF_TEXT | LVCF_WIDTH;
		col.pszText = cols[i].colname;
		col.cx = cols[i].width;
		ListView_InsertColumn(hLw, i + 1, &col);
	}
}

BOOL ListView_AddItem(HWND hLw, LVITEMA *pitem, INT iItem, UINT iMask, INT Image, LPSTR colname)
{
	pitem->mask = iMask;
	pitem->lParam = pitem->iItem;
	pitem->iSubItem = 0;
	pitem->iImage = Image;
	pitem->pszText = colname;
	return ListView_InsertItem(hLw, pitem); //ListView_SetItem(hLw, pitem);
}

INT ListView_AddItemImaged(HWND hLw, LVITEMA *pitem, INT iItem, INT subItem, INT iImage, LPSTR colname)
{
	pitem->mask = LVIF_TEXT | LVIF_IMAGE;
	pitem->iItem = iItem;
	pitem->iSubItem = subItem;
	pitem->iImage = iImage;
	pitem->pszText = colname;
	return ListView_InsertItem(hLw, pitem);
}
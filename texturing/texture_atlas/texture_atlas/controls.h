#pragma once
#include <Windows.h>
#include <stdbool.h>
#include <commdlg.h>
#include <CommCtrl.h>
#pragma comment(lib,  "comctl32.lib")

extern HFONT g_hFont;

inline void ControlsSetFont(HFONT font) { g_hFont = font; }

inline HWND AddButton(HWND window, const char *name, int id, int pox, int posy, int width, int height)
{
	HWND button = CreateWindowExA(0, "button", name, WS_CHILD | WS_VISIBLE | WS_TABSTOP, pox, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(button, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return button;
}

inline HWND AddToggleButton(HWND window, const char *name, int id, int pox, int posy, int width, int height)
{
	HWND button = CreateWindowExA(0, "button", name, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_AUTOCHECKBOX, pox, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(button, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return button;
}

inline HWND AddRadioButton(HWND window, const char *name, int id, int pox, int posy)
{
	int width = 0, height = 20;
	width = strlen(name) * 5;
	if (width < 20)
		width = 20;
	HWND button = CreateWindowExA(0, "button", name, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_AUTOCHECKBOX, pox, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(button, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return button;
}

inline HWND AddCheckboxButton(HWND window, const char *name, int id, int pox, int posy, int width, int height)
{
	HWND button = CreateWindowExA(0, "button", name, WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX, pox, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(button, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return button;
}


inline HWND AddLabel(HWND window, const char *name, int id, int pox, int posy, int width, int height)
{
	HWND label = CreateWindowExA(0, "static", "", WS_VISIBLE | WS_CHILD, pox, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(label, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	SetWindowTextA(label, name);
	return label;
}

///// COMBOBOX
inline HWND AddCombobox(HWND window, int id, DWORD dwFlags, int posx, int posy, int width, int height)
{
	HWND combobox = CreateWindowExA(0, "combobox", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | dwFlags, posx, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(combobox, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return combobox;
}

inline void CboxAddItem(HWND cbbox, const char *itemname, int id) { SendMessageA(cbbox, CB_ADDSTRING, id, (LPARAM)(LPCTSTR)itemname); }
inline void CboxRemoveItem(HWND cbbox, int id) { SendMessageA(cbbox, CB_DELETESTRING, id, 0L); }
inline void CboxSelectItem(HWND cbbox, int id) { SendMessageA(cbbox, CB_SETCURSEL, id, 0L); }
inline int	CboxCurrentSelected(HWND cbbox) { return SendMessageA(cbbox, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0); }
inline char *CboxGetListboxText(HWND cbbox, INT itemidx, LPSTR listitem) { SendMessageA(cbbox, (UINT)CB_GETLBTEXT, (WPARAM)itemidx, (LPARAM)listitem); }

inline HWND AddEditbox(HWND window, const char *deftext, int id, int posx, int posy, int width, int height, int textlimit, bool breadonly, bool bpassword)
{
	//ES_READONLY, ES_PASSWORD
	long flag = (WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL);
	if (breadonly)
		flag |= ES_READONLY;
	if (bpassword)
		flag |= ES_PASSWORD;

	if (!deftext)
		deftext = "";

	HWND editbox = CreateWindowExA(WS_EX_CLIENTEDGE, "edit", deftext, flag, posx, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(editbox, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	SendMessageA(editbox, EM_LIMITTEXT, (WPARAM)textlimit, 0L);
	return editbox;
}
#define EditboxSetText(hedit, text) SetWindowTextA(hedit, text)
#define EditboxGetText(hedit, buffer, maxlen) GetWindowTextA(hedit, buffer, maxlen)

inline HWND AddListbox(HWND window, int id, int posx, int posy, int width, int height, bool bmultiselect)
{
	long flags = (WS_VISIBLE | WS_CHILD | WS_TABSTOP | LBS_NOTIFY | WS_VSCROLL | WS_HSCROLL | WS_BORDER);
	if (bmultiselect)
		flags |= (LBS_MULTIPLESEL | LBS_EXTENDEDSEL);

	HWND listbox = CreateWindowExA(0, "listbox", "", flags, posx, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(listbox, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return listbox;
}
#define LbAddItem(hlistbox, itemname) (int)SendMessageA(hlistbox, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)itemname)
#define LbSelectItem(hlistbox, itemidx) (int)SendMessageA(hlistbox, LB_SETCURSEL, (WPARAM)itemidx, 0L)
#define LbUnselectItem(hlb, idx) SendMessageA(hlb, LB_SETSEL, (WPARAM)FALSE, (LPARAM)(UINT)idx)
#define LbSetItemData(hlb, idx, data) SendMessageA(hlb, LB_SETITEMDATA, (WPARAM)idx, (LPARAM)data)
#define LbGetItemData(hlb, idx) SendMessageA(hlb, LB_GETITEMDATA, (WPARAM)idx, 0L);
#define LbGetItemText(hlb, idx, buffer) SendMessageA(hlb, LB_GETTEXT, (WPARAM)idx, (LPARAM)buffer)
#define LbItemIsSelected(hlb, idx) !!SendMessageA(hlb, LB_GETSEL, (WPARAM)idx, 0L)
#define LbRemoveItem(hlb, idx) SendMessageA(hlb, LB_DELETESTRING, (WPARAM)idx, (LPARAM)NULL);


#define ENABLEDITEM MF_ENABLED
#define DISABLEDITEM MF_GRAYED
#define PopupMenu CreatePopupMenu
#define MenuAddItem(hmenu, id, itemname) AppendMenuA(hmenu, MF_STRING, (UINT)id, itemname)
#define MenuAddSeparator(hmenu) AppendMenuA(hmenu, MF_SEPARATOR, 0, 0);
#define MenuItemState(hmenu, id, state) EnableMenuItem(hmenu, (UINT)id, MF_BYCOMMAND | state)
#define MenuItemDisable(hmenu, id) EnableMenuItem(hmenu, (UINT)id, MF_BYCOMMAND | MF_GRAYED)
#define MenuItemEnable(hmenu, id) EnableMenuItem(hmenu, (UINT)id, MF_BYCOMMAND | MF_ENABLED)
#define MenuSetCheck(hmenu, id, state) CheckMenuItem(hmenu, id, MF_BYCOMMAND | (state ? MF_CHECKED : MF_UNCHECKED))
#define MenuAddTab(hmenu, hchildmenu, tabname) AppendMenuA(hmenu, MF_POPUP, (UINT)hchildmenu, tabname);
inline void CMDisplayOnMousePos(HWND window, HMENU hmenu)
{
	POINT pt;
	GetCursorPos(&pt);
	ClientToScreen(window, &pt);
	TrackPopupMenu(hmenu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y, 0, window, NULL);
}


#define PBSetRange(hpb, minpercent, maxpercent) SendMessageA(hpb, PBM_SETRANGE, 0, MAKELPARAM(minpercent, maxpercent))
#define PBSetValue(hpb, percent) SendMessageA(hpb, PBM_SETPOS, (WPARAM)percent, 0L)
inline HWND AddProgressBar(HWND window, int posx, int posy, int width, int height, int minpercent, int maxpercent)
{
	HWND progressbar = CreateWindowExA(0, PROGRESS_CLASSA, "", WS_VISIBLE | WS_CHILD | PBS_SMOOTH,
		posx, posy, width, height, window, (HMENU)0, 0, 0);
	PBSetRange(progressbar, minpercent, maxpercent);
	SendMessageA(progressbar, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return progressbar;
}

#define TRACKBAR_VERTICAL TBS_VERT
#define TRACKBAR_HORIZONTAL TBS_HORZ

// Создаёт трекбар
// HWND AddTrackbar(HWND window, int id, int posx, int posy, int width, int height, long flag)
//    window - дескриптор родительского окна
//    id - ид контрола
//    posx - позиция по x
//    posy - позиция по y
//    width - ширина
//    height - высота
//    flag - флаги (TBS_VERT - вертикальный трекбар / TBS_HORZ - горизонтальный трекбар)
inline HWND AddTrackbar(HWND window, int id, int posx, int posy, int width, int height, long flag)
{
	if (!flag)
		flag |= TRACKBAR_HORIZONTAL;

	HWND trackbar = CreateWindowExA(0, TRACKBAR_CLASSA, "", WS_CHILD | WS_VISIBLE | flag, posx, posy, width, height, window, (HMENU)id, 0, 0);
	SendMessageA(trackbar, WM_SETFONT, (WPARAM)g_hFont, MAKELPARAM(TRUE, 0));
	return trackbar;
}

// Устанавливает диапазон
// TBSetRange(htb, minrange, maxrange)
//    htb - дескриптор контрола
//    minrange - минимальное значение
//    maxrange - максимальное значение
#define TBSetRange(htb, minrange, maxrange) SendMessageA(htb, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(minrange, maxrange))

// Устанавливает значение
// TBSetValue(htb, value)
//    htb - дескриптор трекбара
//    value - новое значение
#define TBSetValue(htb, value) SendMessageA(htb, TBM_SETPOS, (WPARAM)true, (LPARAM)value)

// TBSetLineSize(htb, value)
//    htb - дескриптор трекбара
//    value - новое значение
#define TBSetLineSize(htb, value) SendMessageA(htb, TBM_SETLINESIZE, 0, (LPARAM)value)

// TBSetPageSize(htb, value)
//    htb - дескриптор трекбара
//    value - новое значение
#define TBSetPageSize(htb, value) SendMessageA(htb, TBM_SETPAGESIZE, 0, (LPARAM)value);

// Получает значение трекбара
// TBGetValue(htb)
//    htb - дескриптор трекбара
#define TBGetValue(htb) (int)SendMessageA(htb, TBM_GETPOS, 0, 0L)

// Получает минимальный диапазон
// TBGetRangeMin(htb)
//    htb - дескриптор трекбара
#define TBGetRangeMin(htb) (int)SendMessageA(htb, TBM_GETRANGEMIN, 0, 0L)

// Получает максимальный диапазон
// TBGetRangeMax(htb)
//    htb - дескриптор трекбара
#define TBGetRangeMax(htb) (int)SendMessageA(htb, TBM_GETRANGEMAX, 0, 0L)

// Получает ?
// TBGetLineSize(htb)
//    htb - дескриптор трекбара
#define TBGetLineSize(htb) (int)SendMessageA(htb, TBM_GETLINESIZE, 0, 0L)

// Получает ?
// TBGetPageSize(htb)
//    htb - дескриптор трекбара
#define TBGetPageSize(htb) (int)SendMessageA(htb, TBM_GETPAGESIZE, 0, 0L)


// Open dialog for select colors
//	HWND parent - parent window
//	color_t *color - pointer to color var

typedef struct color {
	unsigned char r, g, b, a;
} color_t;

void OpenSelectColorDialog(HWND parent, color_t *color);

// Создаёт диалог выбора файла для открытия
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для выбора файлов
BOOL OpenFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);

// Открыть диалог сохранения файла
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для сохранения файлов
BOOL SaveFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);


//LISTVIEW
typedef struct lw_colnames_s {
	INT		width;
	LPSTR	colname;
} lw_colnames_t;

//LVS_SHOWSELALWAYS
HWND CreateListView(HWND hParentWindow, INT id, DWORD dwFlags, INT x, INT y, INT width, INT height);
void ListView_InitCols(HWND hLw, lw_colnames_t *cols, INT numcols);
BOOL ListView_AddItem(HWND hLw, LVITEMA *pitem, INT iItem, UINT iMask, INT Image, LPSTR colname);
INT ListView_AddItemImaged(HWND hLw, LVITEMA *pitem, INT iItem, INT subItem, INT iImage, LPSTR colname);
#pragma once
#include "gui.h"

/**
*	UI Canvas definitions
*/
#define UI_CANVAS 1

/**
*	UI Button definitions
*/
#define UI_BUTTON 2
#define UI_BUTTON_ACTION_CLICK 0x0
#define UI_BUTTON_ACTION_TOUCH 0x1

// UI button flags
#define BF_DRAWEDGE (1 << 0)
#define BF_DRAWTEXT (1 << 1)
#define BF_DRAWBACKGROUND (1 << 2)
#define BF_DRAWTOUCH (1 << 3)
#define BF_DRAWTEXTURE (1 << 4)
#define BF_DEFAULT (BF_DRAWEDGE|BF_DRAWTEXT|BF_DRAWBACKGROUND|BF_DRAWTOUCH)

// id - button unique id
// handle - handle the control
// action - ( UI_BUTTON_ACTION_CLICK / UI_BUTTON_ACTION_TOUCH )
// button - ( MLEFTBUTTON / MRIGHTBUTTON / MWHEELBUTTON / MBUTTONADT1 .. MBUTTONADT5 )
// state - ( BUTTONDOWN / BUTTONUP ). unused
typedef void(*ui_button_action_fn)(int id, ui_handle_t *handle, char action, char button, char state); //button notification function
typedef struct ui_button_s {
	bool b_touched;
	char *name;
	ui_rect_t rect;
	ui_color4_t edge_color;
	ui_color4_t rect_color;
	ui_color4_t rect_focus_color;
	ui_color4_t text_color;
	ui_button_action_fn button_callback;
} ui_button_t;

void ui_register_elements();
void ui_unregister_elements();

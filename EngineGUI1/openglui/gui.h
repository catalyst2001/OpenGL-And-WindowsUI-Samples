// -------------------------------------------------------------------------------------------------- //
//	Engine Graphics User Interface
//	Authors: Deryabin K., Tynin R.
//	Created: 30.01.2021
//	Modified: 19.06.2021
//	Copyright (c) 2021.
// -------------------------------------------------------------------------------------------------- //
//	CHANGELOG:
//		18.06.2021 - Changed all handlers
//		19.06.2021 - Buttons
//	
//
//	
//	
// -------------------------------------------------------------------------------------------------- //
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include "memutils.h"
#include "gui_primitives.h"
#pragma warning(disable:4996)

/**
*	UI Utilites
*/
typedef unsigned short ushort_t;
#define POINT_IN_RECT(rectx, recty, rectright, recttop, pointx, pointy) (rectx < pointx && recty < pointy && rectright > pointx && recttop > pointy)

#define UI_TRACKBAR 3

#define FL_BTNNORMAL (1 << 0)
#define FL_BTNCHECK (1 << 1)
#define FL_BTNRADIO (1 << 2)

#define FL_NOEDGE (1 << 3)
#define FL_RECTED (1 << 4)
#define FL_NOCENTER (1 << 5)
#define FL_NOTEXT (1 << 6)

/**
*	Mouse buttons
*/
#define MLEFTBUTTON 0x0
#define MRIGHTBUTTON 0x1
#define MWHEELBUTTON 0x2
#define MBUTTONADT1 0x3
#define MBUTTONADT2 0x4
#define MBUTTONADT3 0x5
#define MBUTTONADT4 0x6
#define MBUTTONADT5 0x7

/**
*	Button definitions
*/
#define BUTTONDOWN 0x0
#define BUTTONUP 0x1

typedef struct ui_point_s {
	int x, y;
} ui_point_t;

typedef struct ui_rect_s {
	ui_point_t p1, p2, p3, p4;
} ui_rect_t;

typedef struct ui_cliprect_s {
	int left, top, right, bottom;
} ui_cliprect_t;

typedef struct ui_color4_s {
	unsigned char r, g, b, a;
} ui_color4_t;

typedef struct ui_color3_s {
	unsigned char r, g, b;
} ui_color3_t;

#if defined(_M_IX86)
typedef unsigned long ui_param;
#elif defined(_M_X64) || defined(_M_AMD64)
typedef unsigned long long ui_param;
#endif
typedef struct ui_handle_s ui_handle_t;

/**
*	Registers a control message handler
*	ATTENTION: THE DRAWING PROCEDURE DOES NOT WORK IN THIS PROCESSOR, THERE IS A SEPARATE FUNCTION!
*
*	Processes:
*	 Creation
*	 Destruction
*	 Cursor movement
*	 Mouse clicks
*	 Pressing keyboard buttons
*	 and more...
*/
typedef ui_param (*ui_msg_handler_fn)(ui_handle_t *handle, int message, ui_param param1, ui_param param2); //control messages handler prototype

/**
*	Registers a control draw handler
*	ATTENTION: This procedure only deals with rendering the control!
*/
typedef void(*ui_draw_handler_fn)(ui_handle_t *); //control draw handler prototype

// UI element handle struct
typedef struct ui_handle_s {
	int id;
	int texid;
	long flags;
	bool enabled;
	ushort_t type;
	void *elemptr;
	ui_msg_handler_fn p_msgfunc;
	ui_cliprect_t clip;
	ui_draw_handler_fn draw;
	dynamic_memory_t child_controls;
} ui_handle_t;

struct ui_input {
	int mouse[2];
	int prevmouse[2];
	int window_width, window_height;
} input; extern struct ui_input input;

struct ui_data_s {
	dynamic_memory_t elems;
	dynamic_memory_t registred_elems;
} uidata; extern struct ui_data_s uidata;

#define UIEvent_WindowResize(width, height)\
	input.window_width = width;\
	input.window_height = height;

char *m_strdup(const char *src);
void UI_Init(int reserved_size, int reserved_newelements);

//Contains all parameters passed to ui_create function
typedef struct ui_create_data_s {
	int n_identifier;
	int id;
	const char *name;
	ui_handle_t *hparent;
	ui_param param1;
	ui_param param2;
	ui_param param3;
	ui_param param4;
	int x, y, width, height;
	long flags;
} ui_create_data_t;

/**
*	Creates a UI element
*/
ui_handle_t *ui_create(int n_identifier, int id, const char *name, ui_handle_t *hparent, ui_param uparam1, ui_param uparam2, ui_param uparam3, ui_param uparam4, int posx, int posy, int width, int height, long flags);

/**
*	Send message to UI element
*	
*	message - specific or standard element message
*	param1, param2 - specific or standard parameter
*	return - the value or pointer that the handler will return.
*/
ui_param ui_send(ui_handle_t *handle, int message, ui_param param1, ui_param param2);

/**
*	Get element flags
*	return - bit flags.
*/
long ui_getflags(ui_handle_t *handle);

/**
*	Set element new flags
*	return - old element flags.
*/
long ui_setflags(ui_handle_t *handle, long flags);

/**
*	Input messages
*/
// dispatched as soon as the mouse moved.
// param1 - (0 - 16 bits) = mouse X position, (16 - 32 bits) = mouse Y position
// param2 - unused 14.06.2021 14:08
#define UIMSG_MOUSEMOVE 0x2

// dispatched as soon as the mouse buttons clicked.
// param1 - (0 - 8 bits)	mouse button
// 			(8 - 16 bits)	button state
// 			(16 - 32 bits)	unused
// param2 - (0 - 16 bits)	mouse position X
// 			(16 - 32 bits)	mouse position Y.
#define UIMSG_MOUSECLICK 0x3

// dispatched as soon as the mouse wheel move up or down.
#define UIMSG_MOUSEWHEEL 0x4

// dispatched as soon as the recived char.
#define UIMSG_CHAR 0x5

// dispatched as soon as the keyboard button pressed.
// param1 - (0 - 16 bits)	button code
// 			(16 - 32 bits)	unused 14.06.2021 14:08
// param2 - unused  14.06.2021 14:08
#define UIMSG_KEYDOWN 0x6

// dispatched as soon as the keyboard button released.
// param1 - (0 - 16 bits)	button code
// 			(16 - 32 bits)	unused 14.06.2021 14:08
// param2 - unused  14.06.2021 14:08
#define UIMSG_KEYUP 0x7


/**
*	General messages
*/

// DISPATCHED AS SOON AS THE CONTROL IS CREATED.
// param1 - control unique id
// param2 - pointer to a structure that stores the data passed to ui_create function
// RETURN - ATTENTION!!! : ADDRESS ALLOCATED MEMORY FOR ELEMENT DATA!
#define UIMSG_CREATE 0x0

// DISPATCHED AS SOON AS THE CONTROL IS DESTROYED. 
#define UIMSG_DESTROY 0x1

// dispatch activate control state
#define UIMSG_SETACTIVE 0xA

// get activate control state
#define UIMSG_GETACTIVE 0xB

// set control font
#define UIMSG_SETFONT 0xC

// get control font
#define UIMSG_GETFONT 0xD

// set control text
#define UIMSG_SETTEXT 0xE

// get control text
#define UIMSG_GETTEXT 0xF

// set control rect
#define UIMSG_SETRECT 0x11

// get control rect
#define UIMSG_GETRECT 0x12

// set control clip plane
#define UIMSG_SETCLIP 0x13

// get control clip plane
#define UIMSG_GETCLIP 0x14

// set control texture
#define UIMSG_SETTEXTURE 0x15

// get control texture
#define UIMSG_GETTEXTURE 0x16

// set control texture coordinates
#define UIMSG_SETTEXCOORDS 0x17

// get control texture coordinates
#define UIMSG_GETTEXCOORDS 0x18

/**
*	Process unprocessed messages
*/
ui_param ui_default_handler(ui_handle_t *handle, int message, ui_param param1, ui_param param);

/**
*	Registering new gui element
*/
enum {
	UI_REGISTER_SUCCESS = 0,			  // State OK
	UI_REGISTER_TYPE_ALIVE,				  // UI Element type already registred
	UI_REGISTER_INVALID_HANDLER_FUNCTION, // UI handler set is NULL
	UI_UNREGISTER_SUCCESS = 0,			  // UI element unregistred succesfully
	UI_UNREGISTER_FAILURE				  // UI element unregistering failed. (Element type not found)
};

//Structure for registering new element
typedef struct ui_register_s {
	int n_identifier;
	unsigned int n_bytes_child;
	unsigned int n_bytes_reserve;
	ui_draw_handler_fn p_drawfunc;
	ui_msg_handler_fn p_msgfunc;
} ui_register_t;

/**
*	Register new element
*	
*	1. Fill the ui_register_t structure with zeros using memset.
*	2. Set information to structure ui_register_t.
*	3. Call ui_register_element function for register this element.
*	return - Error code. If error not found, UI_REGISTER_SUCCESS.
*/
int ui_register_element(ui_register_t *peleminf);

/**
*	Unregister UI element
*/
int ui_unregister_element(int element_type);

void UIEvent_MouseMove(int x, int y);
void UIEvent_MouseClick(int x, int y, char button);
void UIEvent_Render();


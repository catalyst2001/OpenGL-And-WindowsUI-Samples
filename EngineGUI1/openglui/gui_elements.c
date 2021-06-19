#include "gui_elements.h"

//  ^ y
//  |  (1)-----(2)
//  | 	|		|
//  | 	|		|
//  |  (4)-----(3)
//  +---------------> x
static ui_pointf_t texcoord_defaut[] = {
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f},
};

/**
*	Checkbox
*/
#define CHECKBUTTON_WIDTH 20
#define CHECKBUTTON_HEIGHT 20
typedef struct {
	bool b_touched;
	char *name;
	ui_rect_t rect;
	ui_color4_t edge_color;
	ui_color4_t rect_color;
	ui_color4_t rect_enabled_color;
	ui_color4_t rect_focus_color;
	ui_color4_t text_color;
	bool *b_value;
	//ui_button_action_fn button_callback;
} UIBUTTONCHECK;

/**
*	Radio button
*/
#define RADIOBUTTON_EDGE_RADIUS 10
#define RADIOBUTTON_SELECT_RADIUS 7
#define RADIOBUTTON_CIRCLE_STRIDE 10
typedef struct {
	char *name;
	ui_point_t pos;
	ui_rect_t rect;
	ui_color4_t radio_disabled_color;
	ui_color4_t radio_enabled_color;
	ui_color4_t text_color;
	bool *b_value;
	//ui_button_action_fn button_callback;
} UIRADIOBUTTON;

/**
*	Trackbar
*/
#define TRACKBAR_HORIZONTAL (1 << 0)
#define TRACKBAR_VERTICAL (1 << 1)
typedef int(*TRACKBARMOVEFN)(int id, ui_handle_t *this_elem, float *newval, float *prevval);
typedef struct { float min, max; } TRACKBARBOUNDS;
typedef struct {
	bool b_touched;
	bool b_pressed;
	ui_point_t pos;
	ui_rect_t rect;
	float minval;
	float maxval;
	float prevval;
	TRACKBARMOVEFN trackbarmove_callback;
} UITRACKBAR;

/**
*	Canvas
*/
#pragma region CANVAS
typedef struct ui_canvas_s {
	int id; //TODO: remove  this
	int flags;
	char *name;
	ui_rect_t rect;
	ui_color4_t edge_color;
	ui_color4_t rect_color;
	ui_color4_t text_color;
} ui_canvas_t;

ui_param msg_canvas(ui_handle_t *handle, int message, ui_param param1, ui_param param2)
{
	switch (message) {
	case UIMSG_CREATE: {
		ui_create_data_t *p_create_data = (ui_create_data_t *)param2;
		ui_canvas_t *pcanvas = (ui_canvas_t *)malloc(sizeof(ui_canvas_t));
		if (!pcanvas)
			return NULL;

		printf("\n0x%x address\n", p_create_data);
		printf("name: %s\n\n", p_create_data->name);

		memset(pcanvas, NULL, sizeof(ui_canvas_t));
		pcanvas->name = p_create_data->name; //TODO: CANVAS NAME strdup not used
		pcanvas->rect_color.r = 20;
		pcanvas->rect_color.g = 20;
		pcanvas->rect_color.b = 20;
		pcanvas->rect_color.a = 255;
		
		//left top
		pcanvas->rect.p1.x = p_create_data->x;
		pcanvas->rect.p1.y = p_create_data->y;

		//right top
		pcanvas->rect.p2.x = p_create_data->x + p_create_data->width;
		pcanvas->rect.p2.y = p_create_data->y;

		//right bottom
		pcanvas->rect.p3.x = pcanvas->rect.p2.x;
		pcanvas->rect.p3.y = p_create_data->y + p_create_data->height;

		//left bottom
		pcanvas->rect.p4.x = p_create_data->x;
		pcanvas->rect.p4.y = pcanvas->rect.p3.y;

		pcanvas->id = p_create_data->id;

		return pcanvas;
	}

	default:
		return ui_default_handler(handle, message, param1, param2);
	}
}

void draw_canvas(ui_handle_t *handle)
{
	ui_canvas_t *p_canvas = (ui_canvas_t *)handle->elemptr;
	glVertexPointer(2, GL_INT, 0, &p_canvas->rect);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoord_defaut);
	glColor4ubv((const unsigned char *)&p_canvas->rect_color);
	glDrawArrays(GL_QUADS, 0, 4);
	glColor4ub(128, 128, 128, 1);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}
#pragma endregion

/**
*	Button
*/
#pragma region BUTTON
ui_param msg_button(ui_handle_t *handle, int message, ui_param param1, ui_param param2)
{
	switch (message) {
	case UIMSG_CREATE: {
		ui_create_data_t *p_create_data = (ui_create_data_t *)param2;
		ui_button_t *pbutton = (ui_button_t *)malloc(sizeof(ui_button_t));
		if (!pbutton)
			return NULL;

		memset(pbutton, NULL, sizeof(ui_button_t));
		pbutton->name = p_create_data->name; //TODO: CANVAS NAME strdup not used
		pbutton->rect_color.r = 40;
		pbutton->rect_color.g = 40;
		pbutton->rect_color.b = 40;
		pbutton->rect_color.a = 255;

		pbutton->rect_focus_color.r = 80;
		pbutton->rect_focus_color.g = 80;
		pbutton->rect_focus_color.b = 80;
		pbutton->rect_focus_color.a = 255;

		//left top
		pbutton->rect.p1.x = p_create_data->x;
		pbutton->rect.p1.y = p_create_data->y;

		//right top
		pbutton->rect.p2.x = p_create_data->x + p_create_data->width;
		pbutton->rect.p2.y = p_create_data->y;

		//right bottom
		pbutton->rect.p3.x = pbutton->rect.p2.x;
		pbutton->rect.p3.y = p_create_data->y + p_create_data->height;

		//left bottom
		pbutton->rect.p4.x = p_create_data->x;
		pbutton->rect.p4.y = pbutton->rect.p3.y;

		//compute clip rect
		handle->clip.left = p_create_data->x;
		handle->clip.top = p_create_data->y;
		handle->clip.right = p_create_data->x + p_create_data->width;
		handle->clip.bottom = p_create_data->y + p_create_data->height;
		if (!p_create_data->flags) {
			handle->flags = BF_DEFAULT;
		}
		handle->id = p_create_data->id;
		pbutton->button_callback = (ui_button_action_fn)p_create_data->param1;
		return pbutton;
	}

	case UIMSG_MOUSEMOVE: {
		short xPos = param1 & 0xffff; //TODO: change bitwise operators to macros
		short yPos = (param1 >> 16) & 0xffff;
		ui_button_t *p_button = (ui_button_t *)handle->elemptr;
		static bool b_last_touch = false;
		p_button->b_touched = (bool)POINT_IN_RECT(handle->clip.left, handle->clip.top, handle->clip.right, handle->clip.bottom, input.mouse[0], input.mouse[1]);

		//TODO: button touch event not work!
		//if (p_button->b_touched && !b_last_touch && p_button->button_callback) {
		//	p_button->button_callback(handle->id, handle, UI_BUTTON_ACTION_TOUCH, NULL, NULL);
		//	b_last_touch = true;
		//}
		//if (!p_button->b_touched && b_last_touch) {
		//	b_last_touch = false;
		//}
		//printf("%s\n", p_button->b_touched ? "TRUE" : "FALSE");
		//printf("Mouse move event for BUTTON  x: %d  y: %d  %s ", xPos, yPos, p_button->b_touched ? "IN BUTTON!" : "");
		//printf("clip: left(%d)  top(%d)  right(%d)  bottom(%d)\n", handle->clip.left, handle->clip.top, handle->clip.right, handle->clip.bottom);
		break;
	}

	case UIMSG_MOUSECLICK: {
		char button = (char)param1 & 0xff;
		char state = (char)(param1 >> 8) & 0xff; //TODO: unused
		short x = param2 & 0xffff, y = (param2 >> 16) & 0xffff;
		ui_button_t *p_button = (ui_button_t *)handle->elemptr;
		if (p_button && p_button->button_callback && POINT_IN_RECT(handle->clip.left, handle->clip.top, handle->clip.right, handle->clip.bottom, x, y)) {
			p_button->button_callback(handle->id, handle, /*state*/UI_BUTTON_ACTION_CLICK, button, BUTTONDOWN);
		}
		//switch (button) {
		//case MLEFTBUTTON:

		//	break;

		//case MRIGHTBUTTON:

		//	break;

		//case MWHEELBUTTON:

		//	break;
		//}
		break;
	}

	default:
		return ui_default_handler(handle, message, param1, param2);
	}
}

void draw_button(ui_handle_t *handle)
{
	ui_button_t *p_button = (ui_button_t *)handle->elemptr;
	if (handle->flags & BF_DRAWBACKGROUND) {
		if ((handle->flags & BF_DRAWTOUCH) && p_button->b_touched)
		 glColor4ubv((const unsigned char *)&p_button->rect_focus_color);
		else glColor4ubv((const unsigned char *)&p_button->rect_color);

		if (handle->flags & BF_DRAWTEXTURE && handle->texid > 0) {
			//TODO: временно uv по дефолту
			//glBindTexture(GL_TEXTURE_2D, handle->texid);
			//glTexCoordPointer(2, GL_FLOAT, 0, &texcoord_defaut);
		}

	}

	glBindTexture(GL_TEXTURE_2D, handle->texid);
	glVertexPointer(2, GL_INT, 0, &p_button->rect);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoord_defaut);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (handle->flags & BF_DRAWEDGE) {
		glColor4ub(128, 128, 128, 1);
		glVertexPointer(2, GL_INT, 0, &p_button->rect);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}

	if (handle->flags & BF_DRAWTEXT) {
		//TODO: BUTTON DRAW. draw text there
	}
}
#pragma endregion

void ui_register_elements()
{
	/**
	*	UI Canvas
	*/
	ui_register_t register_element;
	register_element.n_identifier = UI_CANVAS;
	register_element.n_bytes_child = 1;
	register_element.n_bytes_reserve = 1;
	register_element.p_drawfunc = draw_canvas;
	register_element.p_msgfunc = msg_canvas;
	if (ui_register_element(&register_element) != UI_REGISTER_SUCCESS) {
		printf("UI_CANVAS not registred!\n");
	}

	/**
	*	UI Button
	*/
	register_element.n_identifier = UI_BUTTON;
	register_element.n_bytes_child = 1;
	register_element.n_bytes_reserve = 1;
	register_element.p_drawfunc = draw_button;
	register_element.p_msgfunc = msg_button;
	if (ui_register_element(&register_element) != UI_REGISTER_SUCCESS) {
		printf("UI_BUTTON not registred!\n");
	}
}

void ui_unregister_elements()
{
	ui_unregister_element(UI_CANVAS);
}

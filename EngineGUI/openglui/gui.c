#include "gui.h"

ui_color4_t canvas_edge_color = { 128, 140, 140, 255 };
ui_color4_t canvas_rect_color = { 50, 40, 40, 255 };
ui_color4_t canvas_text_color = { 255, 255, 255, 255 };
ui_color4_t button_edge_color = { 128, 140, 140, 255 };
ui_color4_t button_rect_color = { 120, 130, 130, 255 };
ui_color4_t button_rect_focus_color = { 180, 180, 180, 255 };
ui_color4_t button_text_color = { 255, 255, 255, 255 };

char *m_strdup(const char *src)
{
	if (src) {
		char *dst = (char *)calloc(strlen(src) + 1, sizeof(char));
		strcpy(dst, src);
		return dst;
	}
	return NULL;
}

void UI_Init(int reserved_size, int reserved_newelements)
{
	//init_circle(&radiobutton_edge, RADIOBUTTON_EDGE_RADIUS, RADIOBUTTON_CIRCLE_STRIDE, GL_POLYGON);
	//init_circle(&radiobutton_selection, RADIOBUTTON_SELECT_RADIUS, RADIOBUTTON_CIRCLE_STRIDE, GL_POLYGON);
	MEM_INIT(&uidata.elems, sizeof(ui_handle_t *), reserved_size);
	MEM_INIT(&uidata.registred_elems, sizeof(ui_register_t), reserved_newelements);
}

//static void DrawButton(ui_handle_t *elem)
//{
//	if (elem->flags & FL_BTNNORMAL) {
//		UIBUTTON *pButton = (UIBUTTON*)elem->elemptr;
//		if (pButton->b_touched)
//			glColor4ub(pButton->rect_focus_color.r, pButton->rect_focus_color.g, pButton->rect_focus_color.b, pButton->rect_focus_color.a);
//		else
//			glColor4ub(pButton->rect_color.r, pButton->rect_color.g, pButton->rect_color.b, pButton->rect_color.a);
//		draw_rect(GL_QUADS, pButton->rect.x, pButton->rect.y, pButton->rect.r, pButton->rect.t);
//		return;
//	}
//
//	if (elem->flags & FL_BTNCHECK) {
//		UIBUTTONCHECK *pBtnCheck = (UIBUTTONCHECK *)elem->elemptr;
//		if (*pBtnCheck->b_value) //Checked?
//			glColor4ub(button_rect_color.r, button_rect_color.g, button_rect_color.b, button_rect_color.a);
//		else
//			glColor4ub(button_rect_focus_color.r, button_rect_focus_color.g, button_rect_focus_color.b, button_rect_focus_color.a);
//		glRecti(pBtnCheck->rect.x, pBtnCheck->rect.y, pBtnCheck->rect.r, pBtnCheck->rect.t);
//		return;
//	}
//
//	if (elem->flags & FL_BTNRADIO) {
//		UIRADIOBUTTON *pRadioButton = (UIRADIOBUTTON *)elem->elemptr;
//		if (*pRadioButton->b_value) { //Checked?
//			glColor3ub(255, 255, 255);
//			draw_circle(&radiobutton_selection, pRadioButton->pos.x, pRadioButton->pos.y);
//		}
//		glColor4ub(button_rect_color.r, button_rect_color.g, button_rect_color.b, button_rect_color.a);
//		draw_circle(&radiobutton_edge, pRadioButton->pos.x, pRadioButton->pos.y);
//		return;
//	}
//}

ui_handle_t *ui_create(int n_identifier, int id, const char *name, ui_handle_t *hparent, ui_param uparam1, ui_param uparam2, ui_param uparam3, ui_param uparam4, int posx, int posy, int width, int height, long flags)
{
#pragma region LEGACY_CODE
//	switch (n_identifier) {
//
//	case UIELEM_CANVAS: {
//		UICANVAS *pCanvas = (UICANVAS*)malloc(sizeof(UICANVAS));
//		pCanvas->name = m_strdup(name);
//		pCanvas->rect.x = posx;
//		pCanvas->rect.y = posy;
//		pCanvas->rect.r = posx + width;
//		pCanvas->rect.t = posy + height;
//		pCanvas->rect.width = width;
//		pCanvas->rect.height = height;
//		pCanvas->edge_color = canvas_edge_color;
//		pCanvas->rect_color = canvas_rect_color;
//		pCanvas->text_color = canvas_text_color;
//		hRetElement->type = UIELEM_CANVAS;
//		hRetElement->elemptr = pCanvas;
//		hRetElement->draw = &DrawCanvas;
//		hRetElement->enabled = true;
//		MEM_INIT(&hRetElement->child_controls, sizeof(ui_handle_t*), 1);
//		MEM_PUSH(&uidata.elems, ui_handle_t *, hRetElement);
//		return hRetElement;
//	}
//
//	case UIELEM_BUTTON: {
//		//If flags is not set, set normal button flag :)
//		if (!flags)
//			flags = FL_BTNNORMAL;
//
//		if (flags & FL_BTNNORMAL) {
//			UIBUTTON *pButton = (UIBUTTON*)malloc(sizeof(UIBUTTON));
//			pButton->name = m_strdup(name);
//			pButton->rect.x = posx;
//			pButton->rect.y = posy;
//			pButton->rect.r = posx + width;
//			pButton->rect.t = posy + height;
//			pButton->rect.width = width;
//			pButton->rect.height = height;
//			pButton->button_click_callback = (BTNCLICKFN)uparam1;
//			pButton->b_touched = false;
//			pButton->rect_color = button_rect_color;
//			pButton->rect_focus_color = button_rect_focus_color;
//			hRetElement->elemptr = pButton;
//			hRetElement->flags = flags;
//		}
//		else if (flags & FL_BTNCHECK) {
//			UIBUTTONCHECK *pButtonCheck = (UIBUTTONCHECK *)malloc(sizeof(UIBUTTONCHECK));
//			pButtonCheck->name = m_strdup(name);
//			pButtonCheck->rect.x = posx;
//			pButtonCheck->rect.y = posy;
//			pButtonCheck->rect.r = posx + CHECKBUTTON_WIDTH;
//			pButtonCheck->rect.t = posy + CHECKBUTTON_HEIGHT;
//			pButtonCheck->rect.width = CHECKBUTTON_WIDTH;
//			pButtonCheck->rect.height = CHECKBUTTON_HEIGHT;
//			pButtonCheck->b_value = (bool *)uparam3;
//			pButtonCheck->button_click_callback = (BTNCLICKFN)uparam1;
//			hRetElement->elemptr = pButtonCheck;
//			hRetElement->flags = flags;
//		}
//		else if (flags & FL_BTNRADIO) {
//			UIRADIOBUTTON *pRadioButton = (UIRADIOBUTTON *)malloc(sizeof(UIRADIOBUTTON));
//			pRadioButton->name = m_strdup(name);
//			pRadioButton->pos.x = posx + RADIOBUTTON_EDGE_RADIUS;
//			pRadioButton->pos.y = posy + RADIOBUTTON_EDGE_RADIUS;
//			pRadioButton->rect.x = posx;
//			pRadioButton->rect.y = posy;
//			pRadioButton->rect.r = posy + (RADIOBUTTON_EDGE_RADIUS + RADIOBUTTON_EDGE_RADIUS);
//			pRadioButton->rect.t = posy + (RADIOBUTTON_EDGE_RADIUS + RADIOBUTTON_EDGE_RADIUS);
//			pRadioButton->rect.width = (RADIOBUTTON_EDGE_RADIUS + RADIOBUTTON_EDGE_RADIUS);
//			pRadioButton->rect.height = (RADIOBUTTON_EDGE_RADIUS + RADIOBUTTON_EDGE_RADIUS);
//			pRadioButton->b_value = (bool *)uparam3;
//			pRadioButton->button_click_callback = (BTNCLICKFN)uparam1;
//			hRetElement->elemptr = pRadioButton;
//			hRetElement->flags = flags;
//		}
//		hRetElement->id = (int)uparam2;
//		hRetElement->draw = &DrawButton;
//		hRetElement->type = UIELEM_BUTTON;
//		hRetElement->enabled = true;
//		if (hparent) {
//			if (hparent->type == UIELEM_CANVAS) {
//				printf("Button: child element added to %d\n", hparent->type);
//				//Prevent 'push' to NULL pointer
//				if (hparent->child_controls.ptr) {
//					MEM_PUSH(&hparent->child_controls, ui_handle_t *, hRetElement);
//				}
//			}
//			return hRetElement;
//		}
//
//		//If not parent, push element to root ui
//		MEM_PUSH(&uidata.elems, ui_handle_t *, hRetElement);
//		return hRetElement;
//	}
//
//	case UIELEM_TRACKBAR: {
//		UITRACKBAR *hTrackbar = (UITRACKBAR*)malloc(sizeof(UITRACKBAR));
//		memset(hTrackbar, NULL, sizeof(UITRACKBAR));
//		hTrackbar->trackbarmove_callback = (TRACKBARMOVEFN)uparam1;
//		hTrackbar->minval = ((TRACKBARBOUNDS*)uparam4)->min;
//		hTrackbar->maxval = ((TRACKBARBOUNDS*)uparam4)->max;
//		//hTrackbar->b_pressed = false;
//		//hTrackbar->b_touched = false;
//		hTrackbar->pos.x = posx;
//		hTrackbar->pos.y = posy;
//		//hTrackbar->prevval = 0.f;
//		hTrackbar->rect.x = posx;
//		hTrackbar->rect.y = posx;
//		hTrackbar->rect.r = posx + width;
//		hTrackbar->rect.t = posy + height;
//		hTrackbar->rect.width = width;
//		hTrackbar->rect.height = height;
//		hRetElement->id = (int)uparam2;
//		hRetElement->flags = (int)uparam3;
//		hRetElement->draw = &DrawTrackbar;
//		hRetElement->elemptr = hTrackbar;
//		hRetElement->type = UIELEM_TRACKBAR;
//		hRetElement->enabled = true;
//		if (hparent && hparent->type == UIELEM_CANVAS) {
//			if (hparent->child_controls.ptr) {
//				MEM_PUSH(&hparent->child_controls, ui_handle_t *, hRetElement);
//				return hRetElement;
//			}
//		}
//		//If not parent, push element to root ui
//		MEM_PUSH(&uidata.elems, ui_handle_t *, hRetElement);
//		return hRetElement;
//		break;
//	}
//	}
#pragma endregion
	for (int i = 0; i < MEM_SIZE(&uidata.registred_elems); i++) {
		ui_register_t *p_elem = &MEM_GETELEMENT(&uidata.registred_elems, ui_register_t, i);
		//printf("idx: %d | elem: %d\n", i, p_elem->n_identifier);

		if (p_elem->n_identifier == n_identifier) {
			ui_handle_t *hRetElement = (ui_handle_t *)malloc(sizeof(ui_handle_t));
			memset(hRetElement, 0, sizeof(ui_handle_t));
			hRetElement->draw = p_elem->p_drawfunc;
			hRetElement->p_msgfunc = p_elem->p_msgfunc;
			hRetElement->enabled = true;

			ui_create_data_t create_data;
			memset(&create_data, NULL, sizeof(ui_create_data_t));
			create_data.n_identifier = n_identifier;
			create_data.id = id;
			create_data.name = name;
			create_data.hparent;
			create_data.param1;
			create_data.param2;
			create_data.param3;
			create_data.param4;
			create_data.x = posx;
			create_data.y = posy;
			create_data.width = width;
			create_data.height = height;
			create_data.flags = flags;

			//compute clip rect
			hRetElement->clip.left = create_data.x;
			hRetElement->clip.top = create_data.y;
			hRetElement->clip.right = create_data.x + create_data.width;
			hRetElement->clip.bottom = create_data.y + create_data.height;
			hRetElement->elemptr = hRetElement->p_msgfunc(hRetElement, UIMSG_CREATE, (ui_param)id, (ui_param)&create_data);
			if (hparent) {
				if (!hparent->child_controls.ptr) {
					MEM_INIT(&hparent->child_controls, sizeof(ui_handle_t*), 1);
				}
				MEM_PUSH(&hparent->child_controls, ui_handle_t *, hRetElement);
				return hRetElement;
			}
			MEM_PUSH(&uidata.elems, ui_handle_t *, hRetElement);
			return hRetElement;
		}
	}
	printf("ui_create: element not registred!\n");
	return NULL;
}

ui_param ui_send(ui_handle_t *p_control, int message, ui_param param1, ui_param param2)
{
	return p_control->p_msgfunc(p_control, message, param1, param2);
}

//TODO: Unimplemented
ui_param ui_default_handler(ui_handle_t *handle, int message, ui_param param1, ui_param param)
{
	switch (message) {
	case UIMSG_MOUSEMOVE: break;
	case UIMSG_MOUSECLICK: break;
	case UIMSG_MOUSEWHEEL: break;
	case UIMSG_CHAR: break;
	case UIMSG_KEYDOWN: break;
	case UIMSG_KEYUP: break;

	case UIMSG_CREATE: return NULL;
	case UIMSG_DESTROY: break;
	case UIMSG_SETACTIVE: break;
	case UIMSG_GETACTIVE: break;
	case UIMSG_SETFONT: break;
	case UIMSG_GETFONT: break;
	case UIMSG_SETTEXT: break;
	case UIMSG_GETTEXT: break;
	case UIMSG_SETRECT: break;
	case UIMSG_GETRECT: break;
	case UIMSG_SETCLIP: break;
	case UIMSG_GETCLIP: break;
	case UIMSG_SETTEXTURE: break;
	case UIMSG_GETTEXTURE: break;
	case UIMSG_SETTEXCOORDS: break;
	case UIMSG_GETTEXCOORDS: break;
	}
	return NULL;
}

int ui_register_element(ui_register_t *peleminf)
{
	if(!peleminf->n_identifier)
		return UI_REGISTER_TYPE_ALIVE;

	for (int i = 0; i < MEM_SIZE(&uidata.registred_elems); i++)
		if (MEM_GETELEMENT(&uidata.registred_elems, ui_register_t, i).n_identifier == peleminf->n_identifier)
			return UI_REGISTER_TYPE_ALIVE;

	if (!peleminf->p_msgfunc)
		return UI_REGISTER_INVALID_HANDLER_FUNCTION;
	
	//find free spaces
	for (int i = 0; i < MEM_SIZE(&uidata.registred_elems); i++) {
		if (MEM_GETELEMENT(&uidata.registred_elems, ui_register_t, i).n_identifier == NULL) {
			MEM_SETELEMENT(&uidata.registred_elems, ui_register_t, i, *peleminf);
			return UI_REGISTER_SUCCESS;
		}
	}
	MEM_PUSH(&uidata.registred_elems, ui_register_t, *peleminf); //if free space not found, push to back
	return UI_REGISTER_SUCCESS;
}

int ui_unregister_element(int element_type)
{
	for (int i = 0; i < MEM_SIZE(&uidata.registred_elems); i++) {
		ui_register_t *ptr = &MEM_GETELEMENT(&uidata.registred_elems, ui_register_t, i);
		if (ptr->n_identifier == element_type) {
			memset(ptr, NULL, sizeof(ui_register_t)); //set space to NULL
			return UI_UNREGISTER_SUCCESS;
		}
	}
	return UI_UNREGISTER_FAILURE;
}

void UIEvent_MouseMove(int x, int y)
{
	input.mouse[0] = x;
	input.mouse[1] = y;
#pragma region LEGACY_CODE
	//for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
	//	ui_handle_t *hElem = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
	//	if (!hElem->enabled) {
	//		continue;
	//	}

	//	switch (hElem->type) {
	//	case UIELEM_CANVAS: {
	//		//UNDONE: проверить, переделать обработчик холста и всё нормально оптимизировать
	//		for (int i = 0; i < MEM_SIZE(&hElem->child_controls); i++) {
	//			ui_handle_t *hControl = MEM_GETELEMENT(&hElem->child_controls, ui_handle_t *, i);
	//			if (hControl && !hControl->enabled)
	//				continue;

	//			switch (hControl->type) {
	//			case UIELEM_BUTTON: {
	//				UIBUTTON *pButton = (UIBUTTON *)hControl->elemptr;
	//				if (POINT_IN_RECT(pButton->rect.x, pButton->rect.y, pButton->rect.r, pButton->rect.t, input.mouse[0], input.mouse[1])) {
	//					pButton->b_touched = true;
	//					return;
	//				}
	//				pButton->b_touched = false;
	//				continue;
	//			}

	//								//...
	//			}
	//		}
	//		continue;
	//	}

	//						//UNDONE: ОСТАВИТЬ КАК ЕСТЬ. РАБОТАЕТ НОРМАЛЬНО
	//	case UIELEM_BUTTON: {
	//		UIBUTTON *pButton = (UIBUTTON *)hElem->elemptr;
	//		if (POINT_IN_RECT(pButton->rect.x, pButton->rect.y, pButton->rect.r, pButton->rect.t, input.mouse[0], input.mouse[1])) {
	//			pButton->b_touched = true;
	//			return;
	//		}
	//		pButton->b_touched = false;
	//		continue;
	//	}
	//	}
	//}
#pragma endregion
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *p_element = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (p_element && p_element->enabled && p_element->p_msgfunc) {
			if (POINT_IN_RECT(p_element->clip.left, p_element->clip.top, p_element->clip.right, p_element->clip.bottom, x, y)) {
				p_element->p_msgfunc(p_element, UIMSG_MOUSEMOVE, (ui_param)((input.mouse[0] & 0xffff) | ((input.mouse[1] & 0xffff) << 16)), (ui_param)NULL);

				//TODO: handling child elements
				for (int iparent = 0; iparent < MEM_SIZE(&p_element->child_controls); iparent++) {
					ui_handle_t *p_child = MEM_GETELEMENT(&p_element->child_controls, ui_handle_t*, iparent);
					if (p_child && p_child->p_msgfunc) {
						p_child->p_msgfunc(p_child, UIMSG_MOUSEMOVE, (ui_param)((input.mouse[0] & 0xffff) | ((input.mouse[1] & 0xffff) << 16)), (ui_param)NULL);
					}
				}
			}
		}
	}
}

void UIEvent_MouseClick(int x, int y, char button)
{
#pragma region LEGACY_CODE
	//for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
	//	ui_handle_t *hElem = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
	//	if (!hElem->enabled)
	//		continue;

	//	if (button == MOUSELBUTTON) {
	//		switch (hElem->type) {
	//		case UIELEM_CANVAS: {
	//			UICANVAS *pCanvas = (UICANVAS *)hElem->elemptr;
	//			if (hElem->child_controls.ptr) {
	//				for (int j = 0; j < MEM_SIZE(&hElem->child_controls); j++) {
	//					ui_handle_t *pUIElem = MEM_GETELEMENT(&hElem->child_controls, ui_handle_t *, j);
	//					if (pUIElem->flags & FL_BTNNORMAL) {
	//						UIBUTTON *pBtn = (UIBUTTON *)pUIElem->elemptr;
	//						if (pBtn->button_click_callback && POINT_IN_RECT(pBtn->rect.x, pBtn->rect.y, pBtn->rect.r, pBtn->rect.t, x, y)) {
	//							pBtn->button_click_callback(hElem->id, pUIElem);
	//						}
	//					}

	//					if (pUIElem->flags & FL_BTNCHECK) {
	//						UIBUTTONCHECK *pBtnChk = (UIBUTTONCHECK *)pUIElem->elemptr;
	//						if (POINT_IN_RECT(pBtnChk->rect.x, pBtnChk->rect.y, pBtnChk->rect.r, pBtnChk->rect.t, x, y)) {
	//							if (pBtnChk->button_click_callback) {
	//								pBtnChk->button_click_callback(hElem->id, pUIElem);
	//							}

	//							if (pBtnChk->b_value) { //if address not NULL
	//								*pBtnChk->b_value = !(*pBtnChk->b_value); //invert value
	//							}
	//						}
	//					}

	//					if (pUIElem->flags & FL_BTNRADIO) {
	//						UIRADIOBUTTON *pBtnRadio = (UIRADIOBUTTON *)pUIElem->elemptr;
	//						if (POINT_IN_RECT(pBtnRadio->rect.x, pBtnRadio->rect.y, pBtnRadio->rect.r, pBtnRadio->rect.t, x, y)) {
	//							if (pBtnRadio->button_click_callback) {
	//								pBtnRadio->button_click_callback(hElem->id, pUIElem);
	//							}

	//							if (pBtnRadio->b_value) { //if address not NULL
	//								*pBtnRadio->b_value = !(*pBtnRadio->b_value); //invert value
	//							}
	//						}
	//					}
	//				}
	//			}

	//			break;
	//		}

	//		case UIELEM_BUTTON: {
	//			UIBUTTON *pButton = (UIBUTTON *)hElem->elemptr;
	//			if (pButton->button_click_callback && POINT_IN_RECT(pButton->rect.x, pButton->rect.y, pButton->rect.r, pButton->rect.t, x, y)) {
	//				pButton->button_click_callback(hElem->id, hElem);
	//			}
	//			break;
	//		}
	//		}
	//	}
	//}
#pragma endregion
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *p_element = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (p_element && p_element->enabled && p_element->p_msgfunc) {
			char state = true; //TODO: temporary
			p_element->p_msgfunc(p_element, UIMSG_MOUSECLICK, (ui_param)((button & 0xff) | ((state & 0xff) << 8)), (ui_param)((x & 0xffff) | ((y & 0xffff) << 16)));

			//TODO: handling child elements
			for (int iparent = 0; iparent < MEM_SIZE(&p_element->child_controls); iparent++) {
				ui_handle_t *p_child = MEM_GETELEMENT(&p_element->child_controls, ui_handle_t*, iparent);
				if (p_child && p_child->p_msgfunc) {
					p_child->p_msgfunc(p_child, UIMSG_MOUSECLICK, (ui_param)((button & 0xff) | ((state & 0xff) << 8)), (ui_param)((x & 0xffff) | ((y & 0xffff) << 16)));
				}
			}
		}
	}
}

void UIEvent_Render()
{
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *hElem = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (!hElem->enabled) //If ui element disabled
			continue;


		//TODO: PROBLEM MESTO
		for (int j = 0; j < MEM_SIZE(&hElem->child_controls); j++) {
			ui_handle_t *hChildElem = MEM_GETELEMENT(&hElem->child_controls, ui_handle_t *, j);
			//if (!hChildElem->enabled)
			//	continue;

			hChildElem->draw(hChildElem);
		}
		hElem->draw(hElem);
	}
}

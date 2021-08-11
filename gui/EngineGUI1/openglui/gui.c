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
	MEM_INIT(&uidata.elems, sizeof(ui_handle_t *), reserved_size);
	MEM_INIT(&uidata.registred_elems, sizeof(ui_register_t), reserved_newelements);
}

ui_handle_t *ui_create(int n_identifier, int id, const char *name, ui_handle_t *hparent, ui_param uparam1, ui_param uparam2, ui_param uparam3, ui_param uparam4, int posx, int posy, int width, int height, long flags)
{
	for (int i = 0; i < MEM_SIZE(&uidata.registred_elems); i++) {
		ui_register_t *p_elem = &MEM_GETELEMENT(&uidata.registred_elems, ui_register_t, i);
		if (p_elem->n_identifier == n_identifier) {
			ui_handle_t *p_handle = (ui_handle_t *)malloc(sizeof(ui_handle_t));
			memset(p_handle, 0, sizeof(ui_handle_t));
			p_handle->draw = p_elem->p_drawfunc;
			p_handle->p_msgfunc = p_elem->p_msgfunc;
			p_handle->enabled = true;

			ui_create_data_t create_data;
			memset(&create_data, NULL, sizeof(ui_create_data_t));
			create_data.n_identifier = n_identifier;
			create_data.id = id;
			create_data.name = name;
			create_data.hparent = hparent;
			create_data.param1 = uparam1;
			create_data.param2 = uparam2;
			create_data.param3 = uparam3;
			create_data.param4 = uparam4;
			create_data.x = posx;
			create_data.y = posy;
			create_data.width = width;
			create_data.height = height;
			create_data.flags = flags;

			//compute clip rect
			p_handle->clip.left = create_data.x;
			p_handle->clip.top = create_data.y;
			p_handle->clip.right = create_data.x + create_data.width;
			p_handle->clip.bottom = create_data.y + create_data.height;
			p_handle->elemptr = p_handle->p_msgfunc(p_handle, UIMSG_CREATE, (ui_param)id, (ui_param)&create_data);
			p_handle->flags = flags;
			if (hparent) {
				if (!hparent->child_controls.ptr) {
					MEM_INIT(&hparent->child_controls, sizeof(ui_handle_t*), 1);
				}
				MEM_PUSH(&hparent->child_controls, ui_handle_t *, p_handle);
				return p_handle;
			}
			MEM_PUSH(&uidata.elems, ui_handle_t *, p_handle);
			return p_handle;
		}
	}
	printf("ui_create: element not registred!\n");
	return NULL;
}

ui_param ui_send(ui_handle_t *p_control, int message, ui_param param1, ui_param param2)
{
	return p_control->p_msgfunc(p_control, message, param1, param2);
}

long ui_getflags(ui_handle_t *handle)
{
	return handle->flags;
}

long ui_setflags(ui_handle_t *handle, long flags)
{
	long fl_old = handle->flags;
	handle->flags = flags;
	return fl_old;
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

//TODO: онднгпемхе мю рн, врн еякх пндярбеммнярэ щкелемрнб асдер днярхцюрэ анкее ндмнцн пндхрекъ, напюанрйю днвепмху щкелемрнб ме асдер опнхяундхрэ. бнглнфмн пеьхр пейспяхбмюъ напюанрйю днвепмху щкелемрнб!
//TODO: бнглнфмне дсакхпнбюмхе  йндю, йнрнпши лнфмн нанаыхрэ. UIEvent_MouseMove(int x, int y), void UIEvent_MouseClick(int x, int y, char button)
void UIEvent_MouseMove(int x, int y)
{
	input.mouse[0] = x;
	input.mouse[1] = y;
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *p_element = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (p_element && p_element->enabled && p_element->p_msgfunc) {
			if (POINT_IN_RECT(p_element->clip.left, p_element->clip.top, p_element->clip.right, p_element->clip.bottom, x, y)) {
				p_element->p_msgfunc(p_element, UIMSG_MOUSEMOVE, (ui_param)((input.mouse[0] & 0xffff) | ((input.mouse[1] & 0xffff) << 16)), (ui_param)NULL);
				for (int iparent = 0; iparent < MEM_SIZE(&p_element->child_controls); iparent++) { //HACK: handling child elements
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
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *p_element = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (p_element && p_element->enabled && p_element->p_msgfunc) {
			char state = true; //TODO: temporary
			p_element->p_msgfunc(p_element, UIMSG_MOUSECLICK, (ui_param)((button & 0xff) | ((state & 0xff) << 8)), (ui_param)((x & 0xffff) | ((y & 0xffff) << 16)));
			if (POINT_IN_RECT(p_element->clip.left, p_element->clip.top, p_element->clip.right, p_element->clip.bottom, input.mouse[0], input.mouse[1])) { //TODO: handling child elements
				for (int iparent = 0; iparent < MEM_SIZE(&p_element->child_controls); iparent++) {
					ui_handle_t *p_child = MEM_GETELEMENT(&p_element->child_controls, ui_handle_t*, iparent);
					if (p_child && p_child->p_msgfunc) {
						p_child->p_msgfunc(p_child, UIMSG_MOUSECLICK, (ui_param)((button & 0xff) | ((state & 0xff) << 8)), (ui_param)((x & 0xffff) | ((y & 0xffff) << 16)));
					}
				}
			}
		}
	}
}

void UIEvent_Render()
{
	glDisable(GL_DEPTH_TEST);
	glPushAttrib(GL_CURRENT_BIT);
	for (int i = 0; i < MEM_SIZE(&uidata.elems); i++) {
		ui_handle_t *hElem = MEM_GETELEMENT(&uidata.elems, ui_handle_t *, i);
		if (!hElem->enabled) //If ui element disabled
			continue;

		hElem->draw(hElem); //draw parent
		for (int j = 0; j < MEM_SIZE(&hElem->child_controls); j++) { //TODO: PROBLEM MESTO
			ui_handle_t *hChildElem = MEM_GETELEMENT(&hElem->child_controls, ui_handle_t *, j);
			//if (!hChildElem->enabled)
			//	continue;
			
			hChildElem->draw(hChildElem); //draw child
		}
	}
	glPopAttrib();
	glEnable(GL_DEPTH_TEST);
}

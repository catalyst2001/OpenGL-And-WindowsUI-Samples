#include "mm.h"

#ifdef _DEBUG
#ifdef _MSC_VER
int __msg(const char *text)
{
	fprintf(stderr, "Assertion failed: %s", text);
	return MessageBoxA(HWND_DESKTOP, text, "Assertion failed!", MB_ICONERROR|MB_RETRYCANCEL);
}
#else
#define asrt_continue 0
#define asrt_cancel 0
int __msg(const char *text) { return 0; }
#endif

int __fastassertion(const char *format, ...)
{
	char buffer[1024];
	va_list arg;
	va_start(arg, format);
	vsprintf_s(buffer, format, arg);
	va_end(arg);
	return __msg(buffer);
}


#endif

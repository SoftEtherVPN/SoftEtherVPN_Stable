#include "Hook.h"

CEDAR *hookCedar = NULL;
void hookEvent(HOOK_EVENT event)
{
	hookLog(L"Hook Log receiving event: %d", event);
}
void hookLog(wchar_t *fmt, ...)
{

	if(!hookCedar)
		return;
	
	// Validate arguments
	if (fmt == NULL)
	{
		return;
	}

	UINT sz = sizeof(fmt)*2+16;
	wchar_t wc[sz];
	
	wcscpy(wc,L"\u2591\u2592\u2593IPB> ");
	wcscat(wc,fmt);
	
	wchar_t buf[MAX_SIZE * 2];
	va_list args;
	va_start(args, fmt);
	UniFormatArgs(buf, sizeof(buf), wc, args);
	WriteServerLog(hookCedar, buf);
	va_end(args);

}
void hookSetCedar(CEDAR* cedar)
{
	hookCedar = cedar;
}

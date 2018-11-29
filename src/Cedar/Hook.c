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

	wchar_t buf[MAX_SIZE * 2];
	va_list args;
	// Validate arguments
	if (fmt == NULL)
	{
		return;
	}

	va_start(args, fmt);
	UINT sz = sizeof(fmt)*2+16;
	wchar_t wc[sz];
	swprintf(wc,sz,L"\u2591\u2592\u2593IPB> %S",fmt); 
	UniFormatArgs(buf, sizeof(buf), wc, args);

	WriteServerLog(hookCedar, buf);
	va_end(args);

}
void hookSetCedar(CEDAR* cedar)
{
	hookCedar = cedar;
}

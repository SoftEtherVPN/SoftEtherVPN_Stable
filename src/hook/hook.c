#include "hook.h"
CEDAR *c = NULL;
void hookEvent(HOOK_EVENT event)
{
	hookLog(L"Hook Log receiving event: %d", event);
}
void hookLog(wchar_t *fmt, ...)
{
	if(!c)
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
	swprintf(wc,sz,L"\u2591\u2592\u2593IPB> %s",fmt); 
	UniFormatArgs(buf, sizeof(buf), wc, args);

	WriteServerLog(c, buf);
	va_end(args);

}
void setCedar(CEDAR* cedar)
{
	c = cedar;
}

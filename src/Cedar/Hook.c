#include "Hook.h"

CEDAR *hookCedar = NULL;
bool hookEvent(HOOK_EVENT event, LIST* params)
{
	switch(event)
	{
		case SESSION_BEGIN:
			hookLog("SESSION_START event=> username:%s session:%s clientip:%s",StrMapSearch(params,"username"), StrMapSearch(params,"session"), StrMapSearch(params,"clientip"));
		break;
		case SESSION_END:
			hookLog("SESSION_END event=> username:%s session:%s",StrMapSearch(params,"username"), StrMapSearch(params,"session"));
		break;
		case DHCP_DISPATCHED:
			hookLog("DHCP_DISPATCHED event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));
		break;
		case DHCP_UPDATE:
			hookLog("DHCP_UPDATE event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));
		break;
		case DHCP_RELEASED:
			hookLog("DHCP_RELEASED event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));
		break;
	}
	char* v = (char*)StrMapSearch(params,"username");
	hookLog("Hook Log receiving event: %d %s", event,v);
	return true;
}
void hookLog(char *fmt, ...)
{

	if(!hookCedar)
		return;
	
	// Validate arguments
	if (fmt == NULL)
	{
		return;
	}

	char buf[MAX_SIZE];
	va_list args;
	va_start(args, fmt);
	FormatArgs(buf,sizeof(buf),fmt,args);
	va_end(args);
	
	wchar_t wc[MAX_SIZE*2];
	
	swprintf(wc,sizeof(wc),L"\u2591\u2592\u2593IPB> %s", buf); 
	WriteServerLog(hookCedar, wc);
	
}
void hookSetCedar(CEDAR* cedar)
{
	hookCedar = cedar;
}

#ifndef HOOK_H
#define HOOK_H

#include "CedarPch.h"

typedef enum hook_event { SESSION_BEGIN, SESSION_END, DHCP_COMPLETE} HOOK_EVENT;
void hookEvent(HOOK_EVENT event);
void hookLog(wchar_t *fmt, ...);
void hookSetCedar(CEDAR* cedar);

#endif

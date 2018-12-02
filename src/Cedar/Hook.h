#ifndef HOOK_H
#define HOOK_H

#include "CedarPch.h"

typedef enum hook_event { SESSION_BEGIN, SESSION_END, DHCP_DISPATCHED, DHCP_RELEASED} HOOK_EVENT;
bool hookEvent(HOOK_EVENT event, LIST* params);
void hookLog(char *fmt, ...);
void hookSetCedar(CEDAR* cedar);

#endif

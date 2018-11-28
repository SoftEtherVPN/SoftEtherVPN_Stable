#ifndef HOOK_H
#define HOOK_H

#include <Cedar/Cedar.h>

enum HOOK_EVENT = { SESSION_BEGIN, SESSION_END, DHCP_COMPLETE};
void hookEvent(HOOK_EVENT event);
void hookLog(wchar_t *fmt, ...);
void setCedar(CEDAR* cedar);

#endif

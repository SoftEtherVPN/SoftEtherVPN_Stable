#include "Hook.h"

CEDAR *hookCedar = NULL;

typedef struct Record
{	//do not use pointers unless memory management is handled for pointer element
	char username[128];		
	char sessionName[64];	//unique session name to identify this record
	char tapIp[64];
	char tapMac[64];
} Record;
LIST* Records =NULL;

Record* searchBySessionName(char* name)
{
	WriteServerLog(hookCedar,L"sbsn0");
	if(Records == NULL)
		return NULL;
	WriteServerLog(hookCedar,L"sbsn1");
	for (int i = 0;i < LIST_NUM(Records);i++)
	{
		WriteServerLog(hookCedar,L"sbsn2");
		Record *r = LIST_DATA(Records, i);
		WriteServerLog(hookCedar,L"sbsn3");

		hookLog("%s %s %s %s",r->username, r->sessionName, r->tapIp, r->tapMac);
		
		if (StrCmp(r->sessionName, name) == 0)
		{
			WriteServerLog(hookCedar,L"sbsn4");
			return r;
		}
	}
	WriteServerLog(hookCedar,L"sbsn5");
	return NULL;
}

Record* searchByTapMac(char* mac)
{
	if(Records == NULL)
		return NULL;
	
	for (int i = 0;i < LIST_NUM(Records);i++)
	{
		Record *r = LIST_DATA(Records, i);

		if (StrCmpi(r->tapMac, mac) == 0)
		{
			return r;
		}
	}
	return NULL;
}
void addRecord(Record* r)
{
	hookLog("%s %s %s %s",r->username, r->sessionName, r->tapIp, r->tapMac);
	
	if(Records==NULL)
		Records = NewList(NULL);

	Record *tmp = searchBySessionName(r->sessionName);
	if(tmp == NULL)
	{
		Add(Records, Clone(r,sizeof(Record)));
	}
}

void removeRecord(Record* r)
{
	Record *tmp = searchBySessionName(r->sessionName);
	if(tmp != NULL)
	{
		Delete(Records, tmp);
		Free(tmp);
	}
}

void updateRecord(Record* r){
	Record* tmp = searchBySessionName(r->sessionName);
	if(tmp == NULL)
		addRecord(r);
	else
		Copy(tmp,r,sizeof(Record));
}

bool onSessionBegin(LIST* params)
{
	hookLog("SESSION_START event=> username:%s session:%s clientip:%s",StrMapSearch(params,"username"), StrMapSearch(params,"session"), StrMapSearch(params,"clientip"));

	//Record r = {StrMapSearch(params,"username"),StrMapSearch(params, "session"),"",""};
	Record r;
	StrCpy(r.username,sizeof(r.username),StrMapSearch(params,"username"));
	StrCpy(r.sessionName,sizeof(r.sessionName),StrMapSearch(params,"session"));
	addRecord(&r);
	return true;
}

bool onSessionEnd(LIST* params)
{
	hookLog("SESSION_END event=> username:%s session:%s",StrMapSearch(params,"username"), StrMapSearch(params,"session"));
	Record* r = searchBySessionName(StrMapSearch(params,"session"));
	if(r == NULL)
		return false;

	RunScript("dhcp_release.sh", "--tapip %s --tapmac %s", r->tapIp, r->tapMac);
	removeRecord(r);
	return true;
}

bool onDHCPDispatched(LIST* params)
{
	hookLog("DHCP_DISPATCHED event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));

	Record* r = searchBySessionName(StrMapSearch(params,"session"));
	if(r==NULL)
		return false;
		
	StrCpy(r->tapIp, sizeof(r->tapIp), StrMapSearch(params,"tap_ip"));
	StrCpy(r->tapMac, sizeof(r->tapMac), StrMapSearch(params,"tap_mac"));

	RunScript("dhcp_new.sh", "--tapip %s --tapmac %s", r->tapIp, r->tapMac);
	return true;
}

bool onDHCPUpdate(LIST* params)
{
	hookLog("DHCP_UPDATE event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));

	Record* r = searchByTapMac(StrMapSearch(params,"tap_mac"));
	if(r==NULL)//session information should exist before any update, meaning is should go though session_begin event at least once
		return false;

	//if renewed ip is same as the old one, do nothing
	if(StrCmp(r->tapIp,StrMapSearch(params,"tap_ip"))==0)
	{
		//do nothing
	}
	else{
		//release old ip table rules
		RunScript("dhcp_release.sh", "--tapip %s --tapmac %s", r->tapIp, r->tapMac);
		//add new ip table rules
		StrCpy(r->tapIp, sizeof(r->tapIp), StrMapSearch(params,"tap_ip"));

		RunScript("dhcp_new.sh", "--tapip %s --tapmac %s", r->tapIp, r->tapMac);
	}
	return true;
}

//event based interface for integrating any interaction from other parts of the program
bool hookEvent(HOOK_EVENT event, LIST* params)
{
	#ifdef OS_UNIX

	switch(event)
	{
		case SESSION_BEGIN:
			return onSessionBegin(params);
		break;
		case SESSION_END:
			return onSessionEnd(params);
		break;
		case DHCP_DISPATCHED:
			return onDHCPDispatched(params);
		break;
		case DHCP_UPDATE:
			return onDHCPUpdate(params);
		break;
		case DHCP_RELEASED:
			hookLog("DHCP_RELEASED event=> tap_ip:%s tap_mac:%s session:%s",StrMapSearch(params,"tap_ip"),StrMapSearch(params,"tap_mac"),StrMapSearch(params,"session"));
		break;
	}

	#endif
	return true;
}

//Helpful logging for IPburger
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

	//convert to wchar for server log
	wchar_t wc[MAX_SIZE*2];
	swprintf(wc,sizeof(wc),L"\u2591\u2592\u2593IPB> %s", buf); 
	WriteServerLog(hookCedar, wc);
	
}

//Needs to be called at least once in order to be able to write to log file
void hookSetCedar(CEDAR* cedar)
{
	hookCedar = cedar;
}

bool RunScript(const char* name, const char* fmt, ...)
{
	#ifdef OS_UNIX
		char exe_dir[MAX_PATH];
		GetExeDir(exe_dir, sizeof(exe_dir));
		char script_path[MAX_PATH*2];
		sprintf(script_path, "%s/hooks/%s",exe_dir,name);
		if(IsFileExists(script_path))
		{
			hookLog("Running hook script %s",name);
			
			char buf[MAX_SIZE];
			va_list args;
			va_start(args, fmt);
			FormatArgs(buf,sizeof(buf),fmt,args);
			va_end(args);

			sprintf(script_path, "%s %s",script_path,buf);

			TOKEN_LIST *t = UnixExec(script_path);
			if (t != NULL)
			{
				UINT i;
				UINT tmp_num = 0;
				for (i = 0;i < t->NumTokens;i++)
				{
					char *line = t->Token[i];
					if(StartWith(line,"SE_ERROR"))
					{
						return false;
					}
					else{
						hookLog("%s",line);
						/*
						wchar_t tmp[sizeof(line)+16];
						swprintf(tmp, sizeof(line)+16,L"\u2591\u2592\u2593IPB> %hs",line);
						WriteServerLog(c,tmp);
						*/
					}
				}
			}
		}
	#endif
	return true;
}

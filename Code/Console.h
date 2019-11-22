#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "imgui/imgui.h"
#include "mmgr/mmgr.h"

class Console : public Module
{
public:
	Console(Application* app, bool start_enabled = true);
	~Console();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	void ClearLog();
	void Draw(const char* title, bool* p_open);

	bool IsActive();
	void SwitchActive();

private:
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

public:
	bool p_open = true;

private:
	bool active = true;
};

#endif

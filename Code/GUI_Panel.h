#ifndef __GUIPanel_H__
#define __GUIPanel_H__

#include "Globals.h"
#include "SDL/include/SDL_scancode.h"

enum GUI_WINDOWS
{
	GUI_NONE = 0,
	GUI_TOPBAR,
	GUI_ABOUT,
	GUI_CONFIGURATION,
	GUI_CONSOLE,
	GUI_PROPERTIES,
	GUI_HIERACHY,
	GUI_QUADTREE,
	GUI_PLAYGAME,
	GUI_ASSETS
};

class GUI_Panel
{
public:
	GUI_Panel(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN, GUI_WINDOWS type = GUI_WINDOWS::GUI_NONE);
	~GUI_Panel();

	virtual void Draw() = 0;

	void SwitchActive();
	bool IsActive() const;
	SDL_Scancode GetShortCut() const;
	GUI_WINDOWS GetType() const;

protected:
	bool active = false;
	SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN;
	GUI_WINDOWS type = GUI_WINDOWS::GUI_NONE;
};

#endif
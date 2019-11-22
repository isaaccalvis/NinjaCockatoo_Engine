#ifndef __GUITopBar_H__
#define __GUITopBar_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_TopBar : public GUI_Panel
{
public:
	GUI_TopBar(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();

private:
	bool saveWindowActive = false;
	bool loadWindowActive = false;
};

#endif
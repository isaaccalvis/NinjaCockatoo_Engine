#ifndef __GUIAbout_H__
#define __GUIAbout_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_About : public GUI_Panel
{
public:
	GUI_About(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
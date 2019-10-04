#ifndef __GUIHireachy_H__
#define __GUIHireachy_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_Hireachy : public GUI_Panel
{
public:
	GUI_Hireachy(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
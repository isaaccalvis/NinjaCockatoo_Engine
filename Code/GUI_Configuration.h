#ifndef __GUIConfiguration_H__
#define __GUIConfiguration_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_Configuration : public GUI_Panel
{
public:
	GUI_Configuration(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
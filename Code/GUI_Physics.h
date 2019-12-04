#ifndef __GUIPhysics_H__
#define __GUIPhysics_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_Physics : public GUI_Panel
{
public:
	GUI_Physics(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
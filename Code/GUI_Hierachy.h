#ifndef __GUIHierachy_H__
#define __GUIHierachy_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_Hierachy : public GUI_Panel
{
public:
	GUI_Hierachy(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();

	void RecursiveTakeChilds(GameObject* parent) const;
};

#endif
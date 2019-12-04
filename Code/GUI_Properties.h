#ifndef __GUIProperties_H__
#define __GUIProperties_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"
#include "Component.h"

class GUI_Properties : public GUI_Panel
{
public:
	GUI_Properties(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();

	void GameComponentPopUp(Component* component) const;
};

#endif
#ifndef __GUIAssets_H__
#define __GUIAssets_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_Assets: public GUI_Panel
{
public:
	GUI_Assets(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
#ifndef __GUIPlayGame_H__
#define __GUIPlayGame_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_PlayGame : public GUI_Panel
{
public:
	GUI_PlayGame(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
#ifndef __GUIQuadTree_H__
#define __GUIQuadTree_H__

#include "SDL/include/SDL_scancode.h"
#include "GUI_Panel.h"

class GUI_QuadTree : public GUI_Panel
{
public:
	GUI_QuadTree(SDL_Scancode shortcut = SDL_SCANCODE_UNKNOWN);

	void Draw();
};

#endif
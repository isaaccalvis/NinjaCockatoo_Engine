#include "GUI_Panel.h"

GUI_Panel::GUI_Panel(SDL_Scancode shortcut, GUI_WINDOWS type)
{
	this->shortcut = shortcut;
	this->type = type;
}

GUI_Panel::~GUI_Panel()
{

}

void GUI_Panel::SwitchActive()
{
	active = !active;
}

bool GUI_Panel::IsActive() const
{
	return active;
}

SDL_Scancode GUI_Panel::GetShortCut() const
{
	return shortcut;
}

GUI_WINDOWS GUI_Panel::GetType() const
{
	return type;
}
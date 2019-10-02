#ifndef __Component_H__
#define __Component_H__

#include "GameObject.h"

class Component
{
public:
	Component();
	~Component();

	void Update(float dt);

	void SetEnable(bool set);
	bool GetEnabled();

public:
	COMPONENT_TYPE type;
	bool active = true;
	GameObject* parent;
};

#endif
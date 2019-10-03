#ifndef __Component_H__
#define __Component_H__

#include "GameObject.h"

class Component
{
public:
	Component(GameObject* parent, COMPONENT_TYPE type);
	~Component();

	virtual void Update(float dt) = 0;

	void SetEnable(bool set);
	bool GetEnabled();

public:
	COMPONENT_TYPE type;
	bool active = true;
	GameObject* parent;
};

#endif
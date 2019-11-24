#ifndef __C_Material_H__
#define __C_Material_H__

#include "Component.h"
#include "Texture.h"

class C_Material : public Component
{
public:
	C_Material(GameObject* parent);
	~C_Material();

	void Update(float dt);

	void OnSaveJson(JSON_Object* object);
	void OnLoadJson(JSON_Object* object);

	void SetTexture(Texture* tex);
	Texture* GetTexture() const;

public:
	uuid_unit material_resources_uuid = 0;

private:
	Texture* texture = nullptr;
};

#endif
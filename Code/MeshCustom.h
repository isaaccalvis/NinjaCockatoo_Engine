#ifndef __MeshCustom_H__
#define __MeshCustom_H__

#include "Mesh.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

class MeshCustom : public Mesh
{
public:
	MeshCustom();
	MeshCustom(const aiScene* scene, int num);

	void Render();
};

#endif
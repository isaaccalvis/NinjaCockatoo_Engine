#ifndef __QuadTree_H__
#define __QuadTree_H__

#include <list>
#include <vector>
#include "MathGeoLib-1.5/src/Geometry/AABB.h"
#include "GameObject.h"

class QT_Node
{
public:
	QT_Node(math::AABB& boundingBox);
	~QT_Node();

	bool HaveChildNode() const;

	void InsertGameObject(GameObject* go);
	void SubdivideNode();
	void RedistributeChilds();

public:
	math::AABB boundingBox;
	QT_Node* parent = nullptr;
	QT_Node* children[4] = { nullptr, nullptr, nullptr, nullptr };

	std::list<GameObject*> objects;
	unsigned int subidivision = 0;
};
// QuadTree is a fusion between QuadTree & Red, because QuadTree already exists at MathGeolib
class QuadTree_d
{
public:
	QuadTree_d();
	~QuadTree_d();

	void Render();

	void Create(math::AABB limits);
	void Clear();

	void Insert(GameObject* go);
	void Remove(GameObject* go);
	//void Interesct(std::vector<GameObject*>& objectColliding, );

public:
	QT_Node* root = nullptr;
	DebugCube* debugCube = nullptr;
};

#endif
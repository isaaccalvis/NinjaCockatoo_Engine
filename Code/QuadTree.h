#ifndef __QuadTree_H__
#define __QuadTree_H__

#include <list>
#include <vector>
#include "MathGeoLib-1.5/src/Geometry/AABB.h"
#include "MathGeoLib-1.5/src/Geometry/LineSegment.h"
#include "GameObject.h"

class QuadTree_d;

class QT_Node
{
public:
	QT_Node(math::AABB& boundingBox);
	~QT_Node();

	bool HaveChildNode() const;

	void InsertGameObject(GameObject* go);
	void SubdivideNode();
	void RedistributeChilds();

	void GetIntersectSections(math::LineSegment line, std::list<QT_Node*> &colectNodes);

public:
	math::AABB boundingBox;
	QT_Node* parent = nullptr;
	QT_Node* children[4] = { nullptr, nullptr, nullptr, nullptr };

	std::list<GameObject*> objects;
	unsigned int subidivision = 0;
	QuadTree_d* quadtree = nullptr;
};

// QuadTree is a fusion between QuadTree & Red, because QuadTree already exists at MathGeolib
class QuadTree_d
{
public:
	QuadTree_d();
	~QuadTree_d();

	void Render(QT_Node* node);

	void GenerateQuadTree();
	void Create(math::AABB limits, QuadTree_d* quadtree);
	void Clear();

	void Insert(GameObject* go);
	void Interesct(math::LineSegment line, std::list<GameObject*>& objectColliding);

public:
	QT_Node* root = nullptr;
	DebugCube* debugCube = nullptr;
	std::list<GameObject*> static_go_list;

	int maxDivisions = 10;
	int bucketSize = 1;

	bool useQuadTree = false;
	bool renderQuadTree = true;
};

#endif
#include "QuadTree.h"

QT_Node::QT_Node(math::AABB& boundingBox)
{
	this->boundingBox = boundingBox;
}

QT_Node::~QT_Node()
{
	if (HaveChildNode())
	{
		for (int i = 0; i < 4; i++)
		{
			if (children[i] != nullptr)
			{
				delete children[i];
			}
		}
	}
}

bool QT_Node::HaveChildNode() const
{
	if (children[0] == nullptr)
		return false;
	return true;
}

void QT_Node::InsertGameObject(GameObject* go)
{
	objects.push_back(go);
	if (!HaveChildNode() && objects.size() > quadtree->bucketSize)
	{
		if (subidivision < quadtree->maxDivisions)
		{
			SubdivideNode();
			RedistributeChilds();
		}
	}
	else if (HaveChildNode() && objects.size() > quadtree->bucketSize)
	{
		RedistributeChilds();
	}
}

void QT_Node::SubdivideNode()
{
	const math::float3 size = boundingBox.Size();
	const math::float3 center = boundingBox.CenterPoint();
	const math::float3 halfSize(size.x / 2.0f, size.y, size.z / 2.0f);
	const math::float3 quartSize(size.x / 4.0f, size.y, size.z / 4.0f);

	math::float3 quartCenter;
	math::AABB quarter;

	quartCenter = { center.x + quartSize.x, center.y, center.z - quartSize.z };
	quarter.SetFromCenterAndSize(quartCenter, halfSize);
	children[0] = new QT_Node(quarter);

	quartCenter = { center.x - quartSize.x, center.y, center.z - quartSize.z };
	quarter.SetFromCenterAndSize(quartCenter, halfSize);
	children[1] = new QT_Node(quarter);

	quartCenter = { center.x + quartSize.x, center.y, center.z + quartSize.z };
	quarter.SetFromCenterAndSize(quartCenter, halfSize);
	children[2] = new QT_Node(quarter);

	quartCenter = { center.x - quartSize.x, center.y, center.z + quartSize.z };
	quarter.SetFromCenterAndSize(quartCenter, halfSize);
	children[3] = new QT_Node(quarter);

	for (int i = 0; i < 4; i++)
	{
		children[i]->quadtree = quadtree;
	}
}

void QT_Node::RedistributeChilds()
{
	std::list<GameObject*>::iterator it = objects.begin();

	while (it != objects.end())
	{
		uint totalIntersections = 0u;
		uint lastIntersection = 0u;
		for (int i = 0; i < 4; i++)
		{
			if ((*it)->boundingBox.Intersects(children[i]->boundingBox))
			{
				totalIntersections++;
				lastIntersection = i;
			}
		}
		if (totalIntersections == 4)
		{
			it++;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if ((*it)->boundingBox.Intersects(children[i]->boundingBox))
				{
					children[i]->InsertGameObject((*it));
				}
			}
			it = objects.erase(it);
		}
	}
}

QuadTree_d::QuadTree_d()
{
	debugCube = new DebugCube();
}

QuadTree_d::~QuadTree_d()
{
	Clear();
}

void QuadTree_d::Render(QT_Node* node)
{
	math::float3 cubeToDraw[8];
	node->boundingBox.GetCornerPoints(cubeToDraw);
	debugCube->DirectRender(cubeToDraw, White);
	if (node->HaveChildNode())
	{
		for (int i = 0; i < 4; i++)
		{
			Render(node->children[i]);
		}
	}
}

void QuadTree_d::GenerateQuadTree()
{
	// Know size
	math::AABB generalBoundingBox;
	generalBoundingBox.SetNegativeInfinity();
	float3* corners = new float3[static_go_list.size() * 8];
	float3* pointer = corners;
	for (std::list<GameObject*>::iterator it = static_go_list.begin(); it != static_go_list.end(); it++)
	{
		math::float3 tmpCorners[8];
		(*it)->boundingBox.GetCornerPoints(tmpCorners);
		for (int i = 0; i < 8; i++)
		{
			*pointer = tmpCorners[i];
			pointer++;
		}
	}
	generalBoundingBox.SetFrom(corners, static_go_list.size() * 8);

	// Generate QuadTree
	Create(generalBoundingBox, this);

	// Add Game Object
	for (std::list<GameObject*>::iterator it = static_go_list.begin(); it != static_go_list.end(); it++)
	{
		Insert((*it));
	}
}

void QuadTree_d::Create(math::AABB limits, QuadTree_d* quadtree)
{
	Clear();
	root = new QT_Node(limits);
	root->quadtree = quadtree;
}

void QuadTree_d::Clear()
{
	if (root != nullptr)
	{
		delete root;
	}
}

void QuadTree_d::Insert(GameObject* go)
{
	if (go->boundingBox.Intersects(root->boundingBox))
	{
		root->InsertGameObject(go);
	}
}
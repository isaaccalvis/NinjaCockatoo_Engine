#include "QuadTree.h"

#define MAX_DIVISIONS 10
#define MARX_ELEMENTS_AT_DIVISION 3

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
	for (int i = 0; i < 4; i++)
	{
		if (children[i] != nullptr)
		{
			return true;
		}
	}
	return false;
}

void QT_Node::InsertGameObject(GameObject* go)
{
	objects.push_back(go);
}

void QT_Node::SubdivideNode()
{
	const math::float3 size = boundingBox.Size();
	const math::float3 center = boundingBox.CenterPoint();
	const math::float3 halfSize(size.x / 2.0f, size.y, size.z / 2.0f);
	const math::float3 quartSize(size.x / 4.0f, size.y, size.z / 2.0f);

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
}

QuadTree::QuadTree()
{

}

QuadTree::~QuadTree()
{
	Clear();
}

void QuadTree::Create(math::AABB limits)
{

}

void QuadTree::Clear()
{

}

void QuadTree::Insert(GameObject* go)
{

}

void QuadTree::Remove(GameObject* go)
{

}
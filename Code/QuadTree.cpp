#include "QuadTree.h"

QT_Node::QT_Node(math::AABB& boundingBox)
{

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

void QT_Node::CreateSubdivision()
{

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
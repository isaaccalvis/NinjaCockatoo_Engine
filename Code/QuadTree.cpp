#include "QuadTree.h"

#define MAX_DIVISIONS 10
#define MAX_ELEMENTS_AT_DIVISION 1

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
	if (HaveChildNode() && objects.size() > MAX_ELEMENTS_AT_DIVISION)
	{
		if (subidivision < MAX_DIVISIONS)
		{
			SubdivideNode();
			RedistributeChilds();
		}
	}
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
			parent->InsertGameObject((*it));
			parent->RedistributeChilds();
			it = objects.erase(it);
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

void QuadTree_d::Render()
{
	QT_Node* ptr = root;
	static math::float3 frustumToDraw[8];
	ptr->boundingBox.GetCornerPoints(frustumToDraw);
	debugCube->DirectRender(frustumToDraw, White);
}

void QuadTree_d::Create(math::AABB limits)
{
	Clear();
	root = new QT_Node(limits);
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

void QuadTree_d::Remove(GameObject* go)
{

}
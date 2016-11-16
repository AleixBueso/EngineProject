#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H
#include "Globals.h"
#include "GameObject.h"
#include <list>
#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;

class QuadTreeNode
{
public:
	QuadTreeNode();
	~QuadTreeNode();


private:
	math::AABB box;
	std::list<GameObject*> gameObjects;
	GameObject* parent;
	GameObject* childs[4];
};

class MyQuadTree
{
public:
	MyQuadTree();
	~MyQuadTree();

	QuadTreeNode* Create(AABB limits);
	void Clear();
	bool Insert(GameObject*);
	bool Remove(GameObject*);
	//bool Intersect(vector<GameObject*>&, PRIMITIVE);

private:
	QuadTreeNode* root = nullptr;
};

#endif // !QUADTREE_H




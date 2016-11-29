#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H
#include "Globals.h"
#include "GameObject.h"
#include <list>
#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject;
class MyQuadTree;

class QuadTreeNode
{
public:
	QuadTreeNode();
	QuadTreeNode(MyQuadTree* tree);
	~QuadTreeNode();

	void Update();

	bool AddGameObject(GameObject* game_object);
	void CollectIntersections(std::vector<GameObject*>& objects, AABB& primitive) const;

	QuadTreeNode* parent;
	QuadTreeNode* childs[4];
	math::AABB box;

	uint max_capacity = 1;
	MyQuadTree* tree_form;

private:
	std::list<GameObject*> gameObjects;

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
	void Update();

	std::list<QuadTreeNode*> all_nodes;
	void Add(GameObject* game_object);


private:
	QuadTreeNode* root = nullptr;
};

#endif // !QUADTREE_H




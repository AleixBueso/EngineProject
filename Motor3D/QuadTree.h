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

	void AddGameObject(GameObject* game_object);

	GameObject* parent;
	QuadTreeNode* childs[4];
	math::AABB box;

	uint max_capacity = 2;
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


private:
	QuadTreeNode* root = nullptr;
};

#endif // !QUADTREE_H




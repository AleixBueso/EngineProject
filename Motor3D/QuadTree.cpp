#include "QuadTree.h"
#include "Globals.h"
#include "Application.h"
#include "SDL\include\SDL_opengl.h"

//Node -----------------------------------------------------------------------------
QuadTreeNode::QuadTreeNode()
{

}

QuadTreeNode::QuadTreeNode(MyQuadTree* tree)
{
	tree_form = tree;

	for (uint i = 0; i < 4; i++)
	{
		childs[i] = nullptr;
	}
}

QuadTreeNode::~QuadTreeNode()
{
	for (uint i = 0; i < 4; i++)
	{
		childs[i] = nullptr;
	}

	tree_form = nullptr;
	parent = nullptr;
}

void QuadTreeNode::Update()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);

	vec corners[8];
	box.GetCornerPoints(corners),
	App->renderer3D->DrawBox(corners, Yellow);

	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool QuadTreeNode::AddGameObject(GameObject* game_object)
{
	bool ret = false;

	// If there's enough space
	if (gameObjects.size() <= max_capacity)
	{
		gameObjects.push_back(game_object);
		ret = true;
	}

	// If there isn't
	else
	{
		if (childs[0] == nullptr)
		{
			vec half = ((box.maxPoint - box.minPoint) / 2);

			// 1 --------------------------------
			childs[0] = new QuadTreeNode(tree_form);

			childs[0]->box.maxPoint.x = box.maxPoint.x;
			childs[0]->box.maxPoint.y = box.maxPoint.y;
			childs[0]->box.maxPoint.z = box.maxPoint.z;

			childs[0]->box.minPoint.x = box.minPoint.x + half.x;
			childs[0]->box.minPoint.y = box.minPoint.y;
			childs[0]->box.minPoint.z = box.minPoint.z + half.z;

			childs[0]->parent = this;
			tree_form->all_nodes.push_back(this->childs[0]);

			// 2 --------------------------------
			childs[1] = new QuadTreeNode(tree_form);

			childs[1]->box.maxPoint.x = box.maxPoint.x - half.x;
			childs[1]->box.maxPoint.y = box.maxPoint.y;
			childs[1]->box.maxPoint.z = box.maxPoint.z;

			childs[1]->box.minPoint.x = box.minPoint.x;
			childs[1]->box.minPoint.y = box.minPoint.y;
			childs[1]->box.minPoint.z = box.minPoint.z + half.z;

			childs[1]->parent = this;
			tree_form->all_nodes.push_back(this->childs[1]);

			// 3 --------------------------------
			childs[2] = new QuadTreeNode(tree_form);

			childs[2]->box.maxPoint.x = box.maxPoint.x;
			childs[2]->box.maxPoint.y = box.maxPoint.y;
			childs[2]->box.maxPoint.z = box.maxPoint.z - half.z;

			childs[2]->box.minPoint.x = box.minPoint.x + half.x;
			childs[2]->box.minPoint.y = box.minPoint.y;
			childs[2]->box.minPoint.z = box.minPoint.z;

			childs[2]->parent = this;
			tree_form->all_nodes.push_back(this->childs[2]);

			// 4 --------------------------------
			childs[3] = new QuadTreeNode(tree_form);

			childs[3]->box.maxPoint.x = box.maxPoint.x - half.x;
			childs[3]->box.maxPoint.y = box.maxPoint.y;
			childs[3]->box.maxPoint.z = box.maxPoint.z - half.z;

			childs[3]->box.minPoint.x = box.minPoint.x;
			childs[3]->box.minPoint.y = box.minPoint.y;
			childs[3]->box.minPoint.z = box.minPoint.z;

			childs[3]->parent = this;
			tree_form->all_nodes.push_back(this->childs[3]);

		}

		childs[0]->AddGameObject(game_object);
	}

	return ret;
}

void QuadTreeNode::CollectIntersections(std::vector<GameObject*>& objects, AABB& primitive) const
{
	if (primitive.Intersects(box))
	{

		for (std::list<GameObject*>::const_iterator it = App->gameobject_manager->all_gameobjects.begin(); it != App->gameobject_manager->all_gameobjects.end(); ++it)
		{
			if (primitive.Intersects((*it)->mesh->GetGlobalBox()))
				objects.push_back(*it);
		}

		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);
	}
}

//Tree -----------------------------------------------------------------------------
MyQuadTree::MyQuadTree()
{
}

MyQuadTree::~MyQuadTree()
{
	Clear();
}

void MyQuadTree::Clear()
{
	std::list<QuadTreeNode*>::iterator it = all_nodes.begin();
	while (it != all_nodes.end())
	{
		RELEASE((*it));
		it++;
	}

	all_nodes.clear();
}

void MyQuadTree::Update()
{
	std::list<QuadTreeNode*>::iterator it = all_nodes.begin();
	while (it != all_nodes.end())
	{
		(*it)->Update();
		it++;
	}
}

void MyQuadTree::Add(GameObject* game_object)
{
	//if(game_object->mesh)
		root->AddGameObject(game_object);
}

QuadTreeNode* MyQuadTree::Create(AABB limits)
{
	QuadTreeNode* new_node = new QuadTreeNode(this);

	root = new_node;
	root->parent = nullptr;
	root->box = limits;

	all_nodes.push_back(new_node);

	return new_node;
}
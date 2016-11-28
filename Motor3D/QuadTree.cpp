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

void QuadTreeNode::AddGameObject(GameObject* game_object)
{
	// If there's enough space
	if (gameObjects.size() <= max_capacity)
		gameObjects.push_back(game_object);

	// If there isn't
	else
	{
		for (uint i = 0; i < 4; i++)
		{
			if (childs[i] == nullptr)
			{
				childs[i] = new QuadTreeNode(tree_form);
				childs[i]->AddGameObject(game_object);
			}
		}
	}
}

//Tree -----------------------------------------------------------------------------
MyQuadTree::MyQuadTree()
{

}

MyQuadTree::~MyQuadTree()
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

QuadTreeNode* MyQuadTree::Create(AABB limits)
{
	QuadTreeNode* new_node = new QuadTreeNode(this);

	root = new_node;
	root->parent = nullptr;
	root->box = limits;

	all_nodes.push_back(new_node);

	return new_node;
}
#include "Node.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>

using namespace Inaba;

Node::Node()
{
	childs.clear();
}

Node::Node(Node* Node)
{

}

Node::~Node()
{
	while(!childs.empty())
	{
		Entity3D* child = childs.back();
		childs.pop_back();

		delete child;
		child = NULL;
	}
}

void Node::AddChild(Entity3D* child)
{
	childs.push_back(child);
}

void Node::RemoveChild(Entity3D* child)
{
	childs.remove(child);
}

void Node::UpdateTransformation()
{
	//Entity3D::UpdateTransformation();
	
	for(std::list<Entity3D*>::iterator iter = childs.begin(); iter != childs.end(); iter++)
	{
		(*iter)->UpdateTransformation();
	}
}

void Node::UpdateAABB()
{
	//std::vector<Entity3D*>::iterator iter;
	for (std::list<Entity3D*>::iterator it = childs.begin(); it != childs.end(); it++)
	//for(list<Entity3D*>::iterator iter = childs.begin(); iter != childs.end(); iter++)
	{
		(*it)->UpdateAABB();
	}
	UpdateNodeAABB();
}

void Node::UpdateNodeAABB() {

	std::vector<TextureCoordVertex> _vertex;

	for (std::list<Entity3D*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		_vertex.push_back((*it)->getAABB()->getMin());
		_vertex.push_back((*it)->getAABB()->getMax());
	}
	
	_AABB->setBounds(_vertex, _vertex.size());
}

void Node::Update(Timer& timer)
{
	
}

void Node::Draw(Renderer& r)
{
	drawAABB(r);
	for (std::list<Entity3D*>::iterator it = childs.begin(); it != childs.end(); it++) {
		(*it)->Draw(r);
	}
}

std::list<Entity3D*> Node::getChilds(){
	return childs;
}
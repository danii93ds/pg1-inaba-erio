#include "Scene.h"
#include "../Entity2D/Entity2D.h"
#include "../Entity3D/Entity3D.h"
#include "../Entity2D/Quad.h"
#include "../Entity2D/Sprite.h"
#include "../Entity2D/Animation.h"
#include "../Entity3D/Mesh.h"
#include "../Entity3D/Node.h"
#include "../Entity3D/BSPNode.h"
#include "../Timer/Timer.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"
#include "../Game.h"

using namespace Inaba;

Scene::Scene() :
BSP(NULL)
{

}

bool Scene::deInit()
{
	if(_entities.empty())
		return true;

	for(int i=0; i < _entities.size(); i ++)
	{
		delete _entities[i];
		_entities[i] = NULL;
	}	
	_entities.clear();

	return true;
}

bool Scene::Draw(Renderer &renderer,Timer &timer)
{
	if(_entities.empty() && _entities3D.empty())
		return false;
	
	std::vector<Entity2D*>::iterator iter;
	for(iter = _entities.begin(); iter != _entities.end(); iter++)
	{
		(*iter)->Update(timer);
		(*iter)->Draw(renderer);
	}	

	if (BSP != NULL)
	{
		BSP->Draw(&renderer, renderer.getCamera()->getPosition());
	}
	else
	{
		std::vector<Node*>::iterator iter2;
		for (iter2 = _entities3D.begin(); iter2 != _entities3D.end(); iter2++)
		{
			//		(*iter2)->UpdateTransformation();
			//		(*iter2)->UpdateAABB();

			//CheckDraw(renderer,*(*iter2));
			(*iter2)->Update(timer);
			(*iter2)->Draw(renderer);           // mas viejo
			//CheckDraw(renderer,*(*iter2));		// viejo 
		}
	}

	return true;
}

bool Scene::CheckDraw(Renderer& renderer,Entity3D& entity) {
	
	/*int r = renderer.getCamera()->checkCollisionAABB(&entity);

	if(r == Camera::INSIDE)
		entity.Draw(renderer);
	else if (r == Camera::INTERSECT){
        Inaba::Node* child = dynamic_cast<Inaba::Node*>(&entity);
        if(child)
		{ 
			std::list<Entity3D*> tmp = child->getChilds();
			for(std::list<Entity3D*>::iterator it = tmp.begin(); it != tmp.end(); it++) 
			{
				CheckDraw(renderer,*(*it));
			}
		}
		else{
			entity.Draw(renderer);                
		}
	}*/
	
	return true;
}

bool Scene::getEntity(Sprite **ent ,std::string name)
{
	if(_entities.empty())
		return false;

	for(int i=0; i < _entities.size(); i ++){
		if(_entities[i]->name() == name){
			*ent = (Sprite*)_entities[i];
			return true;
		}
	}
	return false;
}

bool Scene::getEntity(Quad **ent ,std::string name)
{
	if(_entities.empty())
		return false;

	for(int i=0; i < _entities.size(); i ++){
		if(_entities[i]->name() == name){
			*ent = (Quad*)_entities[i];
			return true;
		}
	}
	return false;
}

bool Scene::getEntity(Mesh** ent ,std::string name)
{
	if(_entities3D.empty())
		return false;

	for(int i=0; i < _entities3D.size(); i++){
		if(_entities3D[i]->name() == name){
			*ent = (Mesh*)_entities3D[i];
			return true;
		}
	}
}

void Scene::AddEntity(Entity2D* entity)
{
	_entities.push_back(entity);
}

void Scene::AddEntity(Node* entity){
	_entities3D.push_back(entity);
} 

void Scene::setName(std::string sceneName)
{
	_name = sceneName;
}

void Scene::AddNodeToBSP(Node* node)
{
	if (node->isPlane())
		AddBSPPlane(node);

	nodesToBSP.push_back(node);

	std::list<Node*> childs = node->getChilds();
	std::list<Node*>::iterator iter;
	for (iter = childs.begin(); iter != childs.end(); iter++)
	{
		AddNodeToBSP((*iter));
	}
}

void Scene::AddBSPPlane(Node* node)
{
	D3DXPLANE plane = node->GetPlane();
	D3DXVECTOR3 point(node->getWorldTransformation()._41, node->getWorldTransformation()._42, node->getWorldTransformation()._43);
	BSPNode* bspNode = new BSPNode(plane, point);
	BSPNodes.push_back(bspNode);
	bspNode->setName(node->name());
}

void Scene::RegisterInBSPtree(Node* node, bool isBSP, Renderer& renderer)
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	node->UpdateTransformation(&identity, &renderer);
	if (!isBSP)
		parentNodes.push_back(node);
	else
		AddNodeToBSP(node);
}

void Scene::ArrangeBSPTree()
{
	if (nodesToBSP.size() != 0){
		BSP = BSPNodes[0];
		for (int i = 1; i < BSPNodes.size(); i++){
			if (BSPNodes[i] != NULL)
				BSP->AddNode(BSPNodes[i]);
		}

		for (int i = 0; i < nodesToBSP.size(); i++){
			if (nodesToBSP[i]->getMeshes().size())
				BSP->AddChild(nodesToBSP[i]);
		}
	}
}
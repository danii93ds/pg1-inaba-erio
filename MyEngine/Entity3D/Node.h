#ifndef NODE_H
#define NODE_H

#include "Entity3D.h"
#include "Mesh.h"
#include "Bones.h"
#include "Animation3D.h"
#include <list>
#include <string>
#include "../myengine_api.h"

namespace Inaba
{	
	class Entity3D;
	class Timer;

	class MYENGINE_API Node : public Entity3D
	{
		public:
			Node();
			~Node();

			void AddChild(Node*);
			void RemoveChild(Node*);
			void AddMesh(Mesh*);
			void RemoveMesh(Mesh*);
			void AddAnimation(Animation3D*);
			void RemoveAnimation(Animation3D*);
			std::list<Node*> getChilds();
			std::list<Mesh*> getMeshes();
			std::list<Animation3D*> getAnimations();
			void DrawMeshes(Renderer&);
			void Draw(Renderer&);
			void Update(Timer&);
			void UpdateTransformation(Matrix transformation, Renderer *renderer);
			D3DXMATRIX getWorldTransformation();
			D3DXMATRIX getLocalTransformation();
			void SetFirstTransform(	float a1,float a2,float a3,float a4,
									float b1,float b2,float b3,float b4,
									float c1,float c2,float c3,float c4,
									float d1,float d2,float d3,float d4);
			void NodeDraw(Renderer* renderer);
			void setBone(Bones*);
			Bones* nodeBone();
			bool playAnimation(std::string);
			void setAnimation(Animation3D*);
			bool isPlane();
			void isPlane(bool);
			void calculateBB();
			D3DXPLANE GetPlane();
			D3DXVECTOR3* getBB();

		private:
			Matrix _worldTransformMatrix;
			Matrix _AssimpTransformMatrix;
			std::list<Node*> childs;
			std::list<Mesh*> meshes;
			std::list<Animation3D*> animations;
			void GetBoundings(D3DXVECTOR3* pOutMin, D3DXVECTOR3* pOutMax);
			Bones* _bone;
			Animation3D* _currentAnimation;
			int iKeyFrame;
			bool _isPlane;

			D3DXVECTOR3* _vBB;
			D3DXVECTOR3 _minBound;
			D3DXVECTOR3 _maxBound;

	};
}
#endif
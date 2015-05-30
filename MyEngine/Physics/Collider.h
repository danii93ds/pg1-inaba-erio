#ifndef COLLIDER_H
#define COLLIDER_H

#include "../myengine_api.h"
#include <iostream>

class hkpRigidBody;
class hkpShape;
class hkpBoxShape;
class hkpConvexVerticesShape;

namespace Inaba
{
	class Mesh;
	

	class MYENGINE_API Collider
	{
		public:
			Collider();
			virtual ~Collider();

			virtual void build(const Mesh* _mesh) = 0;
			void setPosition(float, float, float);

			virtual hkpShape* shape() = 0;

		protected:
			
			hkpRigidBody* rigidbody();
			hkpRigidBody *_rigidbody;
	};

	class MYENGINE_API BoxCollider : public Collider
	{
		public:
			BoxCollider();
			~BoxCollider();
			void build(const Mesh* _mesh);
		
		public:
			hkpShape* shape();

		private: 
			hkpBoxShape *_box;
	};

	class MYENGINE_API MeshCollider : public Collider
	{
		public:
			MeshCollider();
			void build(const Mesh* _mesh);

		public:
			hkpShape *shape();

	private:
			hkpConvexVerticesShape *_meshCollider;

	};

}

#endif


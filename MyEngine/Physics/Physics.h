#ifndef PHYSICS_H
#define PHYSICS_H

#include "../myengine_api.h"
#include "RigidBody.h"
#include <iostream>

class hkVisualDebugger;
class hkpPhysicsContext;
class hkpWorld;
class hkpRigidBody;
namespace Inaba
{	
	class MYENGINE_API Physics
	{
		private:
			static Physics *_instance;
			static hkpWorld *_world;
			hkVisualDebugger *_vdb;
			hkpPhysicsContext *_physicsContext;

			void InitPhysics();
			void DeInitPhysics();
		public:
			Physics();
			~Physics();
			static Physics* GetInstance();
			static hkpWorld* GetWorld();
			void stepSimulation(float deltaTime);
			static void addEntity(RigidBody* _rigidb);
	};
}
#endif
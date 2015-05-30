//dependencias hkBase.lib;hkVisualize.lib;hkInternal.lib;hkSerialize.lib;hkGeometryUtilities.lib;hkcdInternal.lib;hkcdCollide.lib;hkpCollide.lib;hkpConstraint.lib;hkpConstraintSolver.lib;hkpDynamics.lib;hkpInternal.lib;hkpUtilities.lib;

#include "Physics.h"

#include <xmmintrin.h>
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012\Collide\Shape\Convex\Sphere\hkpSphereShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

#include <Physics2012\Collide\Shape\Convex\ConvexVertices\hkpConvexVerticesShape.h>
#include <Common\Internal\ConvexHull\hkGeometryUtility.h>

using namespace Inaba;

Physics* Physics::_instance = NULL;
hkpWorld* Physics::_world = NULL;


Physics::Physics() :
_vdb(NULL),
_physicsContext(NULL)
{
	InitPhysics();
}

Physics::~Physics()
{
	DeInitPhysics();
}

Physics* Physics::GetInstance()
{
	if (!_instance)
	{
		_instance = new Physics();
	}
	return _instance;
}


hkpWorld* Physics::GetWorld()
{
	return _world;
}

static void HK_CALL errorReport(const char* msg, void *userContext)
{
	std::cerr << msg << std::endl;
}

void Physics::InitPhysics()
{
	hkMemoryRouter *memoryRouter = hkMemoryInitUtil::initChecking(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024 * 1024));
	hkBaseSystem::init(memoryRouter, errorReport);

	hkpWorldCinfo worldInfo;
	worldInfo.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);

	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
	worldInfo.setBroadPhaseWorldSize(1000.0f);

	_world = new hkpWorld(worldInfo);

	hkpAgentRegisterUtil::registerAllAgents(_world->getCollisionDispatcher());

	_physicsContext = new hkpPhysicsContext();
	hkpPhysicsContext::registerAllPhysicsProcesses();
	_physicsContext->addWorld(_world);
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(_physicsContext);
	_vdb = new hkVisualDebugger(contexts);
	_vdb->serve();
}

void Physics::DeInitPhysics()
{
	_vdb->removeReference();
	_physicsContext->removeReference();

	_world->removeReference();

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}

void Physics::stepSimulation(float deltaTime)
{
	/*_vdb->step();

	float fHavokStep = (deltaTime / 1000.0f);
	if (fHavokStep < 0.00000001f) {
		return;
	}

	if (fHavokStep > 4.0f) {
		fHavokStep = 3.9f;
	}

	_world->stepDeltaTime(fHavokStep);*/
}

void Physics::addEntity(RigidBody *_rigidb)
{
	_world->markForWrite();
	_world->addEntity(_rigidb->rigidbody());
	_world->unmarkForWrite();
}


#include <Common\Base\KeyCode.cxx>
//**************************************************************
// Saco Cositas que no me interesan (Al menos por ahora)
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_BEHAVIOUR
#undef HK_FEATURE_PRODUCT_MILSIM
#undef HK_FEATURE_PRODUCT_PHYSICS

#define HK_EXCLUDE_LIBRARY_hkpVehicle
#define HK_EXCLUDE_LIBRARY_hkCompat
#define HK_EXCLUDE_LIBRARY_hkSceneData
#define HK_EXCLUDE_LIBRARY_hkcdCollide

#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_hkpHeightField
#define HK_EXCLUDE_FEATURE_hkpAccurateInertiaTensorComputer
#define HK_EXCLUDE_FEATURE_hkpUtilities
#define HK_EXCLUDE_FEATURE_hkpVehicle
#define HK_EXCLUDE_FEATURE_hkpCompressedMeshShape
#define HK_EXCLUDE_FEATURE_hkpConvexPieceMeshShape
#define HK_EXCLUDE_FEATURE_hkpExtendedMeshShape
#define HK_EXCLUDE_FEATURE_hkpMeshShape
#define HK_EXCLUDE_FEATURE_hkpSimpleMeshShape
#define HK_EXCLUDE_FEATURE_hkpPoweredChainData
#define HK_EXCLUDE_FEATURE_hkMonitorStream
//Fin Cositas que no me interesan :)
#include <Common/Base/Config/hkProductFeatures.cxx>
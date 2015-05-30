#include "Collider.h"
#include "../Entity3D/Mesh.h"

#include "../VertexBuffer/indexbuffer.h"
#include "../VertexBuffer/vertexbuffer3D.h"

#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Common/Base/Types/Geometry/hkStridedVertices.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>

using namespace Inaba;

Collider::Collider() :
_rigidbody(NULL)
{
}

Collider::~Collider()
{
	_rigidbody->removeReference();
	_rigidbody = NULL;
}

void Collider::setPosition(float x, float y, float z)
{
	_rigidbody->setPosition(hkVector4(x, y, z));
}


BoxCollider::BoxCollider() : Collider(),
_box(NULL)
{
}

BoxCollider::~BoxCollider()
{

}

hkpShape *BoxCollider::shape()
{
	return _box;
}

void BoxCollider::build(const Mesh* pkMesh)
{
}

MeshCollider::MeshCollider() : Collider(),
_meshCollider(NULL)
{
}

void MeshCollider::build(const Mesh *_mesh)
{
	size_t iCount = _mesh->indexs().size();
	hkArray<hkVector4> hk_vertex(iCount);

	for (int i = 0; i < iCount; i++)
	{
		hk_vertex[i].set(-_mesh->vertexs()[_mesh->indexs()[i]].x,
						  _mesh->vertexs()[_mesh->indexs()[i]].y,
						  _mesh->vertexs()[_mesh->indexs()[i]].z);

	}

	hkStridedVertices StridedVertices(hk_vertex);
	_meshCollider = new hkpConvexVerticesShape(StridedVertices);
	hkpRigidBodyCinfo rbInfo;
	rbInfo.m_shape = _meshCollider;
	rbInfo.m_position = hkVector4(0.0f, 0.0f, 0.0f);
	rbInfo.m_motionType = hkpMotion::MOTION_FIXED;

	_rigidbody = new hkpRigidBody(rbInfo);
	_meshCollider->removeReference();
}

hkpShape* MeshCollider::shape(){
	return _meshCollider;
}
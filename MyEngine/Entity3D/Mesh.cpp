#include "Mesh.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>
using namespace Inaba;

int Mesh::DrawnMeshes = 0;

Mesh::Mesh(Renderer& pRenderer) : _renderer(pRenderer){
	_vertexBuffer3D = _renderer.createVertexBuffer3D(sizeof(Inaba::TextureCoordVertex), Inaba::TextureCoordVertexType);
    _indexBuffer = _renderer.createIndexBuffer();	
}

Mesh::~Mesh()
{
	if(_vertexBuffer3D){
		delete _vertexBuffer3D;
		_vertexBuffer3D = NULL;
	}
	if(_indexBuffer){
		delete _indexBuffer;
		_indexBuffer = NULL;
	}
}

void Mesh::setData(const TextureCoordVertex* Tex_Vertex, size_t vertexCount, Inaba::Primitive Prim, const unsigned short* pInt, size_t indexCount)
{
	_numVertex = vertexCount;
	_numIndex = indexCount;
	pPrimitive = Prim;

	_vVertex = new TextureCoordVertex[_numVertex];
	_vIndex = new USHORT[indexCount];

	for (int i = 0; i < _numVertex; i++)
	{
		_vVertex[i].x = Tex_Vertex[i].x;
		_vVertex[i].y = Tex_Vertex[i].y;
		_vVertex[i].z = Tex_Vertex[i].z;

		_vVertex[i].u = Tex_Vertex[i].u;
		_vVertex[i].v = Tex_Vertex[i].v;
		
		_vVertex[i].nx = Tex_Vertex[i].nx;
		_vVertex[i].ny = Tex_Vertex[i].ny;
		_vVertex[i].nz = Tex_Vertex[i].nz;
	}

	for (int i = 0; i < _numIndex; i++)
	{
		_vIndex[i] = pInt[i];
	}

	_vertexBuffer3D->setVertexData((void*) &_vVertex,_numVertex);
	_indexBuffer->setIndexData(_vIndex,_numIndex);
	//_vertexBuffer3D->setVertexData((void*) Tex_Vertex,vertexCount);
	//_indexBuffer->setIndexData(pInt,indexCount);
	
	
}

void Mesh::Draw(Renderer& renderer) 
{
	DrawnMeshes++;
	_vertexBuffer3D->bind();
	_indexBuffer->bind();
	renderer.setCurrentTexture(NoTexture);
	renderer.Draw(pPrimitive);
}

void Mesh::Update(Timer& timer) 
{
	Entity3D::Update(timer);
}

const TextureCoordVertex* Mesh::vertexs() const{
	return _vVertex;
}

const unsigned short* Mesh::indexs() const{
	return _vIndex;
}

std::vector<Bones*> Mesh::getBones()
{
	return _Bones;
}

void Mesh::insertBone(Bones* bone)
{
	this->_Bones.push_back(bone);
}

void Mesh::UpdateAABB()
{
	_AABB->setBounds(_vVertex,_numVertex);
}

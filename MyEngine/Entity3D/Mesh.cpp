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

	pPrimitive = Prim;

	_vVertex.resize(vertexCount);
	memcpy((void*)&_vVertex, Tex_Vertex, vertexCount * sizeof(TextureCoordVertex));

	_vIndex.resize(indexCount);
	memcpy(&(_vIndex.front()), pInt, indexCount * sizeof(unsigned short));

	_vertexBuffer3D->setVertexData((void*) Tex_Vertex,vertexCount);
	_vertexBuffer3D->setVertexData((void*) &_vVertex,vertexCount);
	_indexBuffer->setIndexData(pInt,indexCount);
	
	
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

const std::vector<TextureCoordVertex>& Mesh::vertexs() const{
	return _vVertex;
}

const std::vector<unsigned short> Mesh::indexs() const{
	return _vIndex;
}

void Mesh::UpdateAABB()
{
	_AABB->setBounds(_vVertex,_numVertex);
}

#include "Mesh.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>
using namespace Inaba;

int Mesh::DrawnMeshes = 0;

Mesh::Mesh(Renderer& pRenderer) : 
	_renderer(pRenderer),
	_vecHuesos(NULL),
	_vectorDraw(NULL)
{
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

void Mesh::setData( TextureCoordVertex* Tex_Vertex, size_t vertexCount, Inaba::Primitive Prim,  unsigned short* pInt, size_t indexCount)
{
	_numVertex = vertexCount;
	_numIndex = indexCount;
	pPrimitive = Prim;

	_vVertex = new TextureCoordVertex[_numVertex];
	_vIndex = new USHORT[indexCount];

	_vecHuesos = new D3DXVECTOR3[_numVertex];
	_vectorDraw = new D3DXVECTOR3[_numVertex];
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
	
	_vertexBuffer3D->setVertexData((void*) _vVertex,_numVertex);
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

const TextureCoordVertex* Mesh::vertexs() const
{
	return _vVertex;
}

const unsigned short* Mesh::indexs() const
{
	return _vIndex;
}

std::vector<BoneInfo*> Mesh::getBones()
{
	return _Bones;
}

void Mesh::insertBone(BoneInfo* bone)
{
	this->_Bones.push_back(bone);
}

void Mesh::UpdateAABB()
{
	_AABB->setBounds(_vVertex,_numVertex);
}

void Mesh::drawAnimation(Renderer& r)
{

	ZeroMemory((void*)_vectorDraw, sizeof(D3DXVECTOR3) * _numVertex);

	for (int iBone = 0; iBone < _Bones.size(); iBone++)
	{
		
		int sizeW = _Bones[iBone]->getWeights().size();
		for (int iWeight = 0; iWeight < sizeW; iWeight++)
		{
			D3DXVECTOR3 vecAux(0, 0, 0);
			D3DXMATRIX matrix = _Bones[iBone]->getBone()->getTransformationMatrix();
			D3DXMatrixMultiply(&matrix, &_Bones[iBone]->getOffsetMatrix(), &matrix);
			BoneInfo::Weight* weight = _Bones[iBone]->getWeights()[iWeight];
			int vecId = weight->VertexId;
			D3DXVec3TransformCoord(&vecAux, &_vecHuesos[vecId], &matrix);
			_vectorDraw[vecId] += vecAux * weight->_Weigth;	
		}
	}

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	r.setMatrix(World, &identity);
	//renderer->setMaterial(pk_Material);
		
	for (int i = 0; i < _numVertex; i++)
	{
		_vVertex[i].x = _vectorDraw[i].x;
		_vVertex[i].y = _vectorDraw[i].y;
		_vVertex[i].z = _vectorDraw[i].z;
	}
	//_renderer.setCurrentTexture(s_Texture);

	_vertexBuffer3D->setVertexData((void *)_vVertex, _numVertex);
	_vertexBuffer3D->bind();
	_indexBuffer->bind();
	r.Draw(pPrimitive);
}

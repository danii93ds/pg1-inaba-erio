#include "Mesh.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>
using namespace Inaba;

int Mesh::DrawnMeshes = 0;

Mesh::Mesh(Renderer& pRenderer) : 
	_renderer(pRenderer),
	_vecHuesos(NULL),
	_vectorDraw(NULL),
	_vBB(new D3DXVECTOR3[8])
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
	
	delete _vecHuesos;
	delete _vectorDraw;

}

void Mesh::setData( TextureCoordVertex* Tex_Vertex, size_t vertexCount, Inaba::Primitive Prim,  unsigned short* pInt, size_t indexCount)
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
	
	_vertexBuffer3D->setVertexData((void*) _vVertex,_numVertex);
	_indexBuffer->setIndexData(_vIndex,_numIndex);
	//_vertexBuffer3D->setVertexData((void*) Tex_Vertex,vertexCount);
	//_indexBuffer->setIndexData(pInt,indexCount);
	_vecHuesos = new D3DXVECTOR3[_numVertex];
	_vectorDraw = new D3DXVECTOR3[_numVertex];
	
	calculateBB();

	/*
	for (short i = 0; i < _numVertex; i++)
	{
		_vecHuesos[i].x = _vVertex[i].x;
		_vecHuesos[i].y = _vVertex[i].y;
		_vecHuesos[i].z = _vVertex[i].z;
	}
	*/
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

Texture Mesh::getTexture()
{
	return _texture;
}

void Mesh::setTexture(Texture texture)
{
	_texture = texture;
}

Material* Mesh::getMaterial()
{
	return _material;
}

void Mesh::setMaterial(Material* material)
{
	_material = material;
}

void Mesh::insertBone(BoneInfo* bone)
{
	this->_Bones.push_back(bone);
}

void Mesh::UpdateAABB()
{
	_AABB->setBounds(_vVertex,_numVertex);
}

void Mesh::getTransformedBox(D3DXMATRIX * pMatrizMundo, D3DXVECTOR3* pOut){
	for (int i = 0; i < 8; i++){
		D3DXVec3TransformCoord(&pOut[i], &_vBB[i], pMatrizMundo);
	}
}
//---------------------------------------------------------------
void Mesh::calculateBB(){
	D3DXVECTOR3 v_MaxBound;
	D3DXVECTOR3 v_MinBound;

	v_MaxBound.x = _vVertex[0].x;
	v_MaxBound.y = _vVertex[0].y;
	v_MaxBound.z = _vVertex[0].z;

	v_MinBound.x = _vVertex[0].x;
	v_MinBound.y = _vVertex[0].y;
	v_MinBound.z = _vVertex[0].z;

	for (int i = 1; i < _numVertex; i++){
		if (_vVertex[i].x > v_MaxBound.x)
			v_MaxBound.x = _vVertex[i].x;
		else if (_vVertex[i].x < v_MinBound.x)
			v_MinBound.x = _vVertex[i].x;

		if (_vVertex[i].y > v_MaxBound.y)
			v_MaxBound.y = _vVertex[i].y;
		else if (_vVertex[i].y < v_MinBound.y)
			v_MinBound.y = _vVertex[i].y;

		if (_vVertex[i].z > v_MaxBound.z)
			v_MaxBound.z = _vVertex[i].z;
		else if (_vVertex[i].z < v_MinBound.z)
			v_MinBound.z = _vVertex[i].z;
	}
	_vBB[0].x = v_MaxBound.x;
	_vBB[0].y = v_MaxBound.y;
	_vBB[0].z = v_MaxBound.z;
	
	_vBB[1].x = v_MaxBound.x;
	_vBB[1].y = v_MinBound.y;
	_vBB[1].z = v_MaxBound.z;
	
	_vBB[2].x = v_MinBound.x;
	_vBB[2].y = v_MinBound.y;
	_vBB[2].z = v_MaxBound.z;
	
	_vBB[3].x = v_MinBound.x;
	_vBB[3].y = v_MaxBound.y;
	_vBB[3].z = v_MaxBound.z;
	
	_vBB[4].x = v_MaxBound.x;
	_vBB[4].y = v_MaxBound.y;
	_vBB[4].z = v_MinBound.z;
	
	_vBB[5].x = v_MaxBound.x;
	_vBB[5].y = v_MinBound.y;
	_vBB[5].z = v_MinBound.z;
	
	_vBB[6].x = v_MinBound.x;
	_vBB[6].y = v_MinBound.y;
	_vBB[6].z = v_MinBound.z;
	
	_vBB[7].x = v_MinBound.x;
	_vBB[7].y = v_MaxBound.y;
	_vBB[7].z = v_MinBound.z;
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

D3DXPLANE Mesh::GetPlane(D3DXMATRIX * transform){
	D3DXVECTOR3 planepoints[3];
	for (int i = 0; i < 3; i++){
		D3DXVECTOR3 pos(_vVertex[i].x, _vVertex[i].y, _vVertex[i].z);
		D3DXVec3TransformCoord(&planepoints[i], &pos, transform);
	}
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane, &planepoints[0], &planepoints[1], &planepoints[2]);
	return plane;
}


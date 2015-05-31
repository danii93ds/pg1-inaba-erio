#include "Import3D.h"
#include "Scene.h"
#include <string>
#include "../Entity3D/Entity3D.h"
#include "../Entity3D/Mesh.h"
#include "../Entity3D/Node.h"
#include "../Renderer/Renderer.h"
#include <math.h>

using namespace Inaba;

Import3D* Import3D::_instance = NULL;

Import3D::Import3D()
	: _renderer(NULL)
{
}

Import3D* Import3D::GetInstance()
{
	if (!_instance)
	{
		_instance = new Import3D();
	}
	return _instance;
}

void Import3D::setRenderer(Renderer *renderer)
{
	_renderer = renderer;
}

bool Import3D::importMesh(aiMesh* myAiMeshes,Scene& scene, Mesh* mesh)
{
	UINT numVertices = 0;
	UINT numFaces = 0;
	UINT inx_faces = 0;

	TextureCoordVertex* vertices;
	USHORT* indices;

	

	numVertices += myAiMeshes->mNumVertices;
	numFaces += myAiMeshes->mNumFaces;
	vertices = new TextureCoordVertex[numVertices];
	indices = new USHORT[numFaces*3];


	for(int nVertex = 0; nVertex < myAiMeshes->mNumVertices; nVertex++)
	{
		vertices[nVertex].x = myAiMeshes->mVertices[nVertex].x;
		vertices[nVertex].y = myAiMeshes->mVertices[nVertex].y;
		vertices[nVertex].z = myAiMeshes->mVertices[nVertex].z;
		//vertices[inx_vertex].Nx = myAiMeshes[nMeshes]->mNormals[nVertex].x;
		//vertices[inx_vertex].Ny = myAiMeshes[nMeshes]->mNormals[nVertex].y;
		//vertices[inx_vertex].Nz = myAiMeshes[nMeshes]->mNormals[nVertex].z;
	}

	for(int nFaces = 0; nFaces < myAiMeshes->mNumFaces; nFaces++)
	{
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[0];
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[1];
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[2];
	}

	mesh->setData(vertices, myAiMeshes->mNumVertices,Inaba::TriangleList,indices,numFaces*3);
	mesh->setName(myAiMeshes->mName.C_Str());
	
	

	delete[] vertices;
	vertices = NULL;
	delete indices;

	return true;
}

bool Import3D::importScene(const std::string& fileName,Scene& scene)
{
	Assimp::Importer importer;
	
	const aiScene* objScene = importer.ReadFile( fileName.c_str(), 
		aiPrimitiveType_LINE|aiPrimitiveType_POINT |
		aiProcess_Triangulate |aiProcess_SortByPType
		|aiProcess_MakeLeftHanded);

	if( !objScene)
	{
		//mandar mensajes de window
		return false;
	}

	Node *rootNode = new Node();
	importNode(objScene->mRootNode, objScene, scene, *rootNode);
	rootNode->setName("rootNode");
	scene.AddEntity(rootNode);	

	return true;
}

bool Import3D::importNode(aiNode* myAiNode,const aiScene* myAiScene, Scene& scene, Node& pNode)
{	
	aiMatrix4x4 m = myAiNode->mTransformation.Transpose();
	pNode.SetFirstTransform(m.a1, m.a2, m.a3, m.a4,
							m.b1, m.b2, m.b3, m.b4,
							m.c1, m.c2, m.c3, m.c4,
							m.d1, m.d2, m.d3, m.d4);


	for(int nChild=0; nChild < myAiNode->mNumChildren; nChild++)
	{
		Node *childNode = new Node();
		pNode.AddChild(childNode);
		childNode->SetParent(&pNode);
		childNode->setName(myAiNode->mChildren[nChild]->mName.C_Str());

        importNode(myAiNode->mChildren[nChild], myAiScene, scene, *childNode);

		
	}
	
	for(int nMeshes=0; nMeshes < myAiNode->mNumMeshes ; nMeshes++)
	{
		Mesh *childMesh = new Mesh(*_renderer);
		importMesh(myAiScene->mMeshes[myAiNode->mMeshes[nMeshes]], scene, childMesh);

		pNode.AddMesh(childMesh);
		childMesh->SetParent(&pNode);
	}

	

	return true;

}

void Import3D::quaternionToEuler(float qX,float qY,float qZ,float qW,float& rotX,float& rotY,float& rotZ)
{
	double test = qX * qY + qZ * qW;
	if(test > 0.499)
	{
		rotX = 2.0 * atan2(qX,qW);
		rotY = AI_MATH_PI_F / 2;
		rotZ = 0;
		return;
	}
	if(test < -0.499)
	{
		rotX = - 2.0 * atan2(qX,qW);
		rotY = - AI_MATH_PI_F / 2;
		rotZ = 0;
		return;
	}
	float sqx = qX*qX;
    float sqy = qY*qY;
    float sqz = qZ*qZ;

	rotX = atan2(2*qY*qW-2*qX*qZ , 1 - 2*sqy - 2*sqz);
	rotY = asin(2*test);
	rotZ = atan2(2*qX*qW-2*qY*qZ , 1 - 2*sqx - 2*sqz);
}

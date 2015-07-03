#include "Import3D.h"
#include "Scene.h"
#include <string>
#include "../Entity3D/Entity3D.h"
#include "../Entity3D/Mesh.h"
#include "../Entity3D/Node.h"
#include "../Entity3D/Bones.h"
#include "../Entity3D/Animation3D.h"
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

bool Import3D::importScene(const std::string& fileName,Scene& scene, Node &rootNode)
{
	Assimp::Importer importer;
	
	const aiScene* objScene = importer.ReadFile( fileName.c_str(), 
		aiPrimitiveType_LINE|aiPrimitiveType_POINT |
		aiProcess_Triangulate |aiProcess_SortByPType
		|aiProcess_MakeLeftHanded | aiProcess_FlipUVs);

	if( !objScene)
	{
		//mandar mensajes de window
		return false;
	}

	importNode(objScene->mRootNode, objScene, scene, rootNode);
	rootNode.setName(objScene->mRootNode->mName.C_Str());
	scene.AddEntity(&rootNode);

	if (objScene->HasAnimations())
	{
		for (int nAnim = 0; nAnim < objScene->mNumAnimations; nAnim++)
		{
			// TODAS Y CADA UNA DE LAS ANIMACIONES
			Animation3D *animation = new Animation3D();
			importAnimation(objScene->mAnimations[nAnim],*objScene,*animation);
			rootNode.AddAnimation(animation);
		}
	}

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
		importMesh(myAiScene->mMeshes[myAiNode->mMeshes[nMeshes]], childMesh);

		pNode.AddMesh(childMesh);
		childMesh->SetParent(&pNode);
	}	

	return true;

}

bool Import3D::importMesh(aiMesh* myAiMeshes, Mesh* mesh)
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

		if(myAiMeshes->HasNormals())
		{
			vertices[nVertex].nx = myAiMeshes->mNormals[nVertex].x;
			vertices[nVertex].ny = myAiMeshes->mNormals[nVertex].y;
			vertices[nVertex].nz = myAiMeshes->mNormals[nVertex].z;
		}

		if (myAiMeshes->HasTextureCoords(0))
		{
			vertices[nVertex].u = myAiMeshes->mTextureCoords[0][nVertex].x;
			vertices[nVertex].v = myAiMeshes->mTextureCoords[0][nVertex].y;
		}
	}

	for(int nFaces = 0; nFaces < myAiMeshes->mNumFaces; nFaces++)
	{
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[0];
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[1];
		indices[inx_faces++] = myAiMeshes->mFaces[nFaces].mIndices[2];
	}
 
	if (myAiMeshes->HasBones())
	{

		for (int nBones = 0; nBones < myAiMeshes->mNumBones; nBones++)
		{
			Bones* meshBone = new Bones();
			meshBone->setName(myAiMeshes->mBones[nBones]->mName.C_Str());
			meshBone->setWeightCount(myAiMeshes->mBones[nBones]->mNumWeights);

			D3DMATRIX boneOffsetMatrix;

			boneOffsetMatrix._11 = myAiMeshes->mBones[nBones]->mOffsetMatrix.a1; boneOffsetMatrix._12 = myAiMeshes->mBones[nBones]->mOffsetMatrix.a2; boneOffsetMatrix._13 = myAiMeshes->mBones[nBones]->mOffsetMatrix.a3; boneOffsetMatrix._14 = myAiMeshes->mBones[nBones]->mOffsetMatrix.a4;
			boneOffsetMatrix._21 = myAiMeshes->mBones[nBones]->mOffsetMatrix.b1; boneOffsetMatrix._22 = myAiMeshes->mBones[nBones]->mOffsetMatrix.b2; boneOffsetMatrix._23 = myAiMeshes->mBones[nBones]->mOffsetMatrix.b3; boneOffsetMatrix._24 = myAiMeshes->mBones[nBones]->mOffsetMatrix.b4;
			boneOffsetMatrix._31 = myAiMeshes->mBones[nBones]->mOffsetMatrix.c1; boneOffsetMatrix._32 = myAiMeshes->mBones[nBones]->mOffsetMatrix.c2; boneOffsetMatrix._33 = myAiMeshes->mBones[nBones]->mOffsetMatrix.c3; boneOffsetMatrix._34 = myAiMeshes->mBones[nBones]->mOffsetMatrix.c4;
			boneOffsetMatrix._41 = myAiMeshes->mBones[nBones]->mOffsetMatrix.d1; boneOffsetMatrix._42 = myAiMeshes->mBones[nBones]->mOffsetMatrix.d2; boneOffsetMatrix._43 = myAiMeshes->mBones[nBones]->mOffsetMatrix.d3; boneOffsetMatrix._44 = myAiMeshes->mBones[nBones]->mOffsetMatrix.d4;

			meshBone->setOffsetMatrix(boneOffsetMatrix);
			
			for (int nWeights = 0; nWeights < myAiMeshes->mBones[nBones]->mNumWeights; nWeights++)
			{
				Bones::Weight *boneWeight = new Bones::Weight();

				boneWeight->VertexId = myAiMeshes->mBones[nBones]->mWeights[nWeights].mVertexId;
				boneWeight->_Weigth = myAiMeshes->mBones[nBones]->mWeights[nWeights].mWeight;

				meshBone->insertWeight(boneWeight);
			}
			

			mesh->insertBone(meshBone);
			
		}
	}

	mesh->setData(vertices, myAiMeshes->mNumVertices,Inaba::TriangleList,indices,numFaces*3);
	mesh->setName(myAiMeshes->mName.C_Str());

	//delete[] vertices;
	//vertices = NULL;
	//delete indices;

	return true;
}

bool Import3D::importAnimation(aiAnimation* myAiAnimation,const aiScene& myAiScene, Animation3D& pAnimation)
{
	pAnimation.setName(myAiAnimation->mName.C_Str());
	pAnimation.setDuration(myAiAnimation->mDuration);
	pAnimation.setTicksPerSecond(myAiAnimation->mTicksPerSecond);

	for (int nChannel = 0; nChannel < myAiAnimation->mNumChannels; nChannel++)
	{
		Animation3D::keyFrame *newKeyFrame = new Animation3D::keyFrame();
		newKeyFrame->nodeName = myAiAnimation->mChannels[nChannel]->mNodeName;
		newKeyFrame->nPositionKeys = myAiAnimation->mChannels[nChannel]->mNumPositionKeys;
		newKeyFrame->nRotationKeys = myAiAnimation->mChannels[nChannel]->mNumRotationKeys;
		newKeyFrame->nScalingKeys = myAiAnimation->mChannels[nChannel]->mNumScalingKeys;

		newKeyFrame->positionKeys = new aiVectorKey[newKeyFrame->nPositionKeys];
		for (int i = 0; i < newKeyFrame->nPositionKeys; i++)
		{
			newKeyFrame->positionKeys[i].mTime = myAiAnimation->mChannels[nChannel]->mPositionKeys[i].mTime;
			newKeyFrame->positionKeys[i].mValue = myAiAnimation->mChannels[nChannel]->mPositionKeys[i].mValue;
		}
		
		newKeyFrame->rotationKeys = new aiQuatKey[newKeyFrame->nRotationKeys];
		for (int i = 0; i < newKeyFrame->nRotationKeys; i++)
		{
			newKeyFrame->rotationKeys[i].mTime = myAiAnimation->mChannels[nChannel]->mRotationKeys[i].mTime;
			newKeyFrame->rotationKeys[i].mValue = myAiAnimation->mChannels[nChannel]->mRotationKeys[i].mValue;
		}
		
		newKeyFrame->scalingKeys = new aiVectorKey[newKeyFrame->nScalingKeys];
		for (int i = 0; i < newKeyFrame->nScalingKeys; i++)
		{
			newKeyFrame->scalingKeys[i].mTime = myAiAnimation->mChannels[nChannel]->mScalingKeys[i].mTime;
			newKeyFrame->scalingKeys[i].mValue = myAiAnimation->mChannels[nChannel]->mScalingKeys[i].mValue;
		}

		pAnimation.addKeyFrame(newKeyFrame);
	}

	// add anim node
	return true;
}

bool Import3D::importBone(aiBone* myAiBone)
{
	Bones *newBone = new Bones();

	newBone->setName(myAiBone->mName.C_Str());
	newBone->setWeightCount(myAiBone->mNumWeights);
	
	for (int iWeight = 0; iWeight < myAiBone->mNumWeights; iWeight++)
	{
		Bones::Weight *boneWeight = new Bones::Weight;
		boneWeight->VertexId = myAiBone->mWeights[iWeight].mVertexId;
		boneWeight->_Weigth = myAiBone->mWeights[iWeight].mWeight;
		newBone->insertWeight(boneWeight);
	}

	D3DXMATRIX boneOffsetMatrix;
	boneOffsetMatrix._11 = myAiBone->mOffsetMatrix.a1; boneOffsetMatrix._12 = myAiBone->mOffsetMatrix.a2; boneOffsetMatrix._13 = myAiBone->mOffsetMatrix.a3; boneOffsetMatrix._14 = myAiBone->mOffsetMatrix.a4;
	boneOffsetMatrix._21 = myAiBone->mOffsetMatrix.b1; boneOffsetMatrix._22 = myAiBone->mOffsetMatrix.b2; boneOffsetMatrix._23 = myAiBone->mOffsetMatrix.b3; boneOffsetMatrix._24 = myAiBone->mOffsetMatrix.b4;
	boneOffsetMatrix._31 = myAiBone->mOffsetMatrix.c1; boneOffsetMatrix._32 = myAiBone->mOffsetMatrix.c2; boneOffsetMatrix._33 = myAiBone->mOffsetMatrix.c3; boneOffsetMatrix._34 = myAiBone->mOffsetMatrix.c4;
	boneOffsetMatrix._41 = myAiBone->mOffsetMatrix.d1; boneOffsetMatrix._42 = myAiBone->mOffsetMatrix.d2; boneOffsetMatrix._43 = myAiBone->mOffsetMatrix.d3; boneOffsetMatrix._44 = myAiBone->mOffsetMatrix.d4;
	
	newBone->setOffsetMatrix(boneOffsetMatrix);
	
	
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

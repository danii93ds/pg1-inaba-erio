#ifndef IMPORT3D_H
#define IMPORT3D_H

#include "../myengine_api.h"
#include "../Entity3D/Bones.h"
#include "../Entity3D/BoneInfo.h"
#include <map>
#include <string>
#include <fstream>
#include "../Assimp32/include/Importer.hpp"
#include "../Assimp32/include/types.h"
#include "../Assimp32/include/scene.h"
#include "../Assimp32/include/postprocess.h"
#include <assert.h>

namespace Inaba
{	
	class Scene;
	class Mesh;
	class Renderer;
	class Node;
	class Animation3D;

	class MYENGINE_API Import3D
	{
		private:
			static Import3D *_instance;
			Renderer *_renderer;
			Import3D();
			Import3D(Import3D &const);
			Import3D& operator= (Import3D const&);

		public:
			static Import3D* GetInstance();
			std::map<std::string, Bones*> _boneMap;
			void setRenderer(Renderer*);
			bool importScene(const std::string&,Scene&,Node&);
			bool importNode(aiNode*,const aiScene*,Scene&,Node&);
			bool importMesh(aiMesh*, Mesh*);
			bool importAnimation(aiAnimation*,const aiScene&, Animation3D&);
			bool importBone(aiBone*, Mesh*); 
			void addBoneNode(Node*);
			void quaternionToEuler(float qX,float qY,float qZ,float qW,float& rotX,float& rotY,float& rotZ);
	};
}
#endif

#ifndef IMPORT3D_H
#define IMPORT3D_H

#include "../myengine_api.h"
#include <string>
#include <fstream>
#include "../Assimp32/include/Importer.hpp"
#include "../Assimp32/include/scene.h"
#include "../Assimp32/include/postprocess.h"

namespace Inaba
{	
	class Scene;
	class Mesh;
	class Renderer;
	class Node;

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
			void setRenderer(Renderer*);
			bool importMesh(aiMesh*,Scene&, Mesh*);
			bool importNode(aiNode*,const aiScene*,Scene&,Node&);
			bool importScene(const std::string&,Scene&);
			void quaternionToEuler(float qX,float qY,float qZ,float qW,float& rotX,float& rotY,float& rotZ);
	};
}
#endif

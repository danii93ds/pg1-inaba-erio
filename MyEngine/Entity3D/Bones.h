#ifndef BONES_H
#define BONES_H

#include <string>
#include <vector>
#include <d3dx9.h>
#include "../myengine_api.h"

namespace Inaba 
{
	class MYENGINE_API Bones 
	{

		public:

			Bones(std::string);
			~Bones();

			std::string name();
			void setName(std::string);
			D3DXMATRIX getTransformationMatrix();
			void setTransformationMatrix(D3DXMATRIX);

		private:
			std::string _name;
			D3DXMATRIX _BoneTransformationMatrix;

	};
}

#endif
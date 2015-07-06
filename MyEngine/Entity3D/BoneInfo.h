#ifndef BONEINFO_H
#define BONEINFO_H

#include <string>
#include <vector>
#include "Bones.h"
#include <d3dx9.h>
#include "../myengine_api.h"

namespace Inaba 
{
	class MYENGINE_API BoneInfo
	{

		public:

			struct Weight
			{
				UINT VertexId;
				float _Weigth;
			};
			

			BoneInfo();
			~BoneInfo();

			UINT numWeights();
			void setWeightCount(UINT);
			std::vector<Weight*> getWeights();
			void insertWeight(Weight*);
			D3DXMATRIX getOffsetMatrix();
			void setOffsetMatrix(D3DXMATRIX);
			Bones* getBone();
			void setNodeBone(Bones*);

		private:
			UINT _nWeights;
			std::vector<Weight*> _Weights;
			D3DMATRIX _offsetMatrix;
			Bones* _bone;

	};
}

#endif
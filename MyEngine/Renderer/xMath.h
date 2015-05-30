#ifndef MATH_H
#define MATH_H

#include "../myengine_api.h"
#include <math.h>
#include <limits>
#include <cstddef>
#include <iostream>
#include "../Assimp32/include/Importer.hpp"
#include "../Assimp32/include/scene.h"
#include "../Assimp32/include/postprocess.h"

struct D3DXMATRIX;

namespace Inaba
{
	typedef D3DXMATRIX* Matrix;

	class MYENGINE_API MATHX
	{
		public:
			MATHX();
			~MATHX();
			static void eulerAnglesToQuaternion(float fRotX, float fRotY, float fRotZ,
				float& orQX, float& orQY, float& orQZ, float& orQW);
			static void quaternionToEulerAngles(float qX, float qY, float qZ, float qW,
				float& orfRotX, float& orfRotY, float& orfRotZ);
	};
}

#endif
#include "xMath.h"
using namespace Inaba;

MATHX::MATHX(){
	// NOTHING TO DO
}

MATHX::~MATHX(){
	// NOTHING TO DO. AGAIN!
}
void MATHX::eulerAnglesToQuaternion(float fRotX, float fRotY, float fRotZ,
	float& orQX, float& orQY, float& orQZ, float& orQW){
	double c1 = cos(fRotX / 2);
	double s1 = sin(fRotX / 2);

	double c2 = cos(fRotY / 2);
	double s2 = sin(fRotY / 2);

	double c3 = cos(fRotZ / 2);
	double s3 = sin(fRotZ / 2);

	double c1c2 = c1*c2;
	double s1s2 = s1*s2;

	orQW = c1c2*c3 - s1s2*s3;
	orQX = c1c2*s3 + s1s2*c3;
	orQY = s1*c2*c3 + c1*s2*s3;
	orQZ = c1*s2*c3 - s1*c2*s3;
}

void MATHX::quaternionToEulerAngles(float qX, float qY, float qZ, float qW,
	float& orfRotX, float& orfRotY, float& orfRotZ){
	double test = qX * qY + qZ * qW;
	if (test > 0.499f){
		// singularity at north pole
		orfRotX = 2.0f * atan2(qX, qW);
		orfRotY = AI_MATH_PI_F / 2.0f;
		orfRotZ = 0.0f;
		return;
	}

	if (test < -0.499f){
		// singularity at south pole
		orfRotX = -2.0f * atan2(qX, qW);
		orfRotY = -AI_MATH_PI_F / 2.0f;
		orfRotZ = 0.0f;
		return;
	}

	float sqx = qX * qX;
	float sqy = qY * qY;
	float sqz = qZ * qZ;

	orfRotX = atan2(2.0f * qY * qW - 2.0f * qX * qZ,
		1.0f - 2.0f * sqy - 2.0f * sqz);

	orfRotY = static_cast<float>(asin(2.0f * test));

	orfRotZ = atan2(2.0f * qX * qW - 2.0f * qY * qZ,
		1.0f - 2.0f * sqx - 2.0f * sqz);
}
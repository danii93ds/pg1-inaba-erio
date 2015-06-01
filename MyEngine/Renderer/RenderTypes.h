#ifndef RENDERTYPES_H
#define RENDERTYPES_H

#include <Windows.h>
#include "../myengine_api.h"
#include <d3d9.h>

namespace Inaba
{
	struct MYENGINE_API ColorVertex
	{
		float x, y, z;
		DWORD color;
	};
	extern const DWORD ColorVertexType;

	struct MYENGINE_API TextureCoordVertex
	{
		float x, y, z;
		float u, v;
	};
	
	extern const int TextureCoordVertexType;

	enum Primitive
	{
		TriangleList = 0,
		TriangleStrip,
		PointList,
		LineList,
		LineStrip,
		TriangleFan,
		PrimitiveCount
	};

	enum MatrixType
	{
		View = 0,
		Projection,
		World,
		MatrixTypeCount
	};

	enum FONTALIGNMENT { FA_LEFT, FA_CENTER, FA_RIGHT, FA_TOPRIGHT, FA_TOPLEFT, FA_BOTTOMRIGHT, FA_BOTTOMLEFT }; 

	typedef IDirect3DTexture9 *Texture;
	static const Texture NoTexture = NULL;

	#define Inaba_COLOR_ARGB(a,r,g,b)  ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
	#define Inaba_COLOR_RGB(r,g,b) Inaba_COLOR_ARGB(0xff,r,g,b)
}
#endif
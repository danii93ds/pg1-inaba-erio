
//---------------------------------------------------------------------------
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
//---------------------------------------------------------------------------
#include "../myengine_api.h"
#include "../Renderer/RenderTypes.h"
//---------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>
//---------------------------------------------------------------------------
namespace Inaba
{
//---------------------------------------------------------------------------
class Renderer;
//---------------------------------------------------------------------------
class MYENGINE_API IndexBuffer
{
public:
	IndexBuffer (Renderer& rkRenderer, IDirect3DDevice9* pkDevice);
	~IndexBuffer ();

	void bind ();
	void setIndexData (const unsigned short* pausIndices, size_t uiIndexCount);
	
	size_t indexCount () const;

private:
	size_t m_uiIndexCount;

	D3DPRIMITIVETYPE m_ePrimitiveType;
	LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	IDirect3DDevice9* m_pkDevice;
	Renderer& m_rkRenderer;
};
//---------------------------------------------------------------------------
#include "indexbuffer.inl"
//---------------------------------------------------------------------------
} // end namespace
//---------------------------------------------------------------------------
#endif  // PG2_INDEXBUFFER_H
//---------------------------------------------------------------------------

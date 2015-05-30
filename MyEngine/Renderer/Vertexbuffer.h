
//---------------------------------------------------------------------------
#ifndef Inaba_VERTEXBUFFER_H
#define Inaba_VERTEXBUFFER_H
//---------------------------------------------------------------------------
#include "../myengine_api.h"
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
class MYENGINE_API VertexBuffer
{
public:
	VertexBuffer (IDirect3DDevice9* pkDevice, size_t uiVertexSize, unsigned int uiFVF);
	~VertexBuffer ();

	bool create ();
	void bind ();
	void flush ();
	void draw (const void* pvVertices, D3DPRIMITIVETYPE ePrimitiveType, unsigned int uiVertexCount);
	void release ();

private:

	// Tamaño de Vertex Buffer
	static const unsigned int SIZE = 400;
	static const unsigned int FLUSH_COUNT = 200;

	unsigned int m_uiBase;
	unsigned int m_uiVertexToLock;
	unsigned int m_uiVertexSize;
	unsigned int m_uiFVF;

	D3DPRIMITIVETYPE m_ePrimitiveType;
	LPDIRECT3DVERTEXBUFFER9 m_pkVertexBuffer;
	IDirect3DDevice9 * m_pkDevice;
};
//---------------------------------------------------------------------------
} // end namespace
//---------------------------------------------------------------------------
#endif  // Inaba_VERTEXBUFFER_H
//---------------------------------------------------------------------------

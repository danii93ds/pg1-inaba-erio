#include "Quad.h"
#include "../Renderer/Renderer.h"
#include <d3dx9.h>

using namespace Inaba;

Quad::Quad():
Entity2D(),
_vertex(new ColorVertex[4])
{
	static const float SIZE = 0.5f;
	_vertex[0].x = -SIZE;	_vertex[0].y = SIZE;	_vertex[0].z = 0.0f;
	_vertex[1].x = SIZE;	_vertex[1].y = SIZE;	_vertex[1].z = 0.0f;
	_vertex[2].x = -SIZE;	_vertex[2].y = -SIZE;	_vertex[2].z = 0.0f;
	_vertex[3].x = SIZE;	_vertex[3].y = -SIZE;	_vertex[3].z = 0.0f;

	_vertex[0].color = D3DCOLOR_XRGB(255,0,0);	
	_vertex[1].color = D3DCOLOR_XRGB(255,0,0);	
	_vertex[2].color = D3DCOLOR_XRGB(255,0,0);
	_vertex[3].color = D3DCOLOR_XRGB(255,0,0);

	UpdateLocalTransformation();
}

Quad::~Quad()
{
	delete[] _vertex;
	_vertex = NULL;
}

void Quad::Draw(Renderer& renderer) const
{
	renderer.setCurrentTexture(NoTexture);
	renderer.setMatrix(World,_transformationMatrix);
	renderer.Draw(_vertex,Inaba::TriangleStrip, 4);
}

void Quad::setColor(DWORD c, int v){
 if(v >= 0 && v < 4)
  _vertex[v].color = c;
}
void Quad::setColor(DWORD c){
 for(int i = 0; i < 4; i++)
  _vertex[i].color = c;
}
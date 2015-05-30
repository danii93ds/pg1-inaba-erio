#ifndef RENDERER_H
#define RENDERER_H

#include "Vertexbuffer.h"
#include "../VertexBuffer/vertexbuffer3D.h"
#include "../VertexBuffer/indexbuffer.h"
#include "RenderTypes.h"
#include "xMath.h"
#include "../myengine_api.h"
#include <windows.h>
#include <d3d9.h>
#include <iostream>
#include <vector>
#include "../Font.h"
#include "../Camera/Camera.h"

namespace Inaba
{
	class MYENGINE_API Renderer
	{
	private:
		IDirect3D9 *_d3d;
		IDirect3DDevice9 *_d3ddev;
		Inaba::VertexBuffer *_vertexbuffer;
		Inaba::VertexBuffer3D *_vertexBuffer3D;
		Inaba::IndexBuffer *_indexBuffer;
		Inaba::VertexBuffer *_textureCoordVertexbuffer;
		std::vector<Texture> _vectorTextures;
		Font *_font;
		Camera *_camera;
	public:
		Renderer();
		~Renderer();
		bool Init(HWND hwnd);
		void deInit();
		void BeginFrame();
		void EndFrame();
		void Draw(ColorVertex*, Inaba::Primitive, size_t);
		void Draw(TextureCoordVertex*, Inaba::Primitive, size_t);
		void Draw(Inaba::Primitive primitive); 
		void setMatrix(MatrixType, const Matrix&);
		const Texture LoadTexture(const std::string& FileName, int KeyCode);
		void setCurrentTexture(const Texture&);
		void setCurrentVertexBuffer(VertexBuffer3D* vertexBuffer3D);
		void setCurrentIndexBuffer(IndexBuffer* indexBuffer);
		VertexBuffer3D * createVertexBuffer3D(size_t vSize, unsigned int FVF);
		IndexBuffer * createIndexBuffer();
		Font * getFont();
		Camera * getCamera();
		void UpdateCamera();

	};

}
#endif
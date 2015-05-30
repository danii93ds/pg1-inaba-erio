#ifndef MESH_H
#define MESH_H

#include "Entity3D.h"
#include "../Renderer/xMath.h"
#include "../Renderer/RenderTypes.h"
#include "../VertexBuffer/indexbuffer.h"
#include "../VertexBuffer/vertexbuffer3D.h"
#include "../myengine_api.h"
#include <vector>

namespace Inaba
{
	class Renderer;
	class Timer;
	
	class MYENGINE_API Mesh : public Entity3D
	{
		public:
			Mesh(Renderer & p_Renderer);
			~Mesh();

			void setData(const TextureCoordVertex* Tex_Vertex, size_t vertexCount, Inaba::Primitive Prim, const unsigned short* pInt, size_t indexCount);
            void Draw(Renderer& r);
			void Update(Timer&);
			void UpdateAABB();

			const std::vector<TextureCoordVertex>& vertexs() const;
			const std::vector<unsigned short> indexs() const;

			static int DrawnMeshes;

		private:
			TextureCoordVertex* _vertex;
			IndexBuffer* _indexBuffer;
			VertexBuffer3D* _vertexBuffer3D;
			Primitive pPrimitive;
			Renderer& _renderer;
			std::vector<TextureCoordVertex> _vVertex;
			std::vector<unsigned short> _vIndex;
			UINT _numVertex;
	};
}
#endif
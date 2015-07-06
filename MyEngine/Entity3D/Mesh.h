#ifndef MESH_H
#define MESH_H

#include "Entity3D.h"
#include "../Entity3D/BoneInfo.h"
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

			void setData( TextureCoordVertex* Tex_Vertex, size_t vertexCount, Inaba::Primitive Prim,  unsigned short* pInt, size_t indexCount);
            void Draw(Renderer& r);
			void Update(Timer&);
			void UpdateAABB();

			const TextureCoordVertex* vertexs() const;
			const unsigned short* indexs() const;
			std::vector<BoneInfo*> getBones();
			void insertBone(BoneInfo*);
			void drawAnimation(Renderer&);

			static int DrawnMeshes;

		private:
			IndexBuffer* _indexBuffer;
			VertexBuffer3D* _vertexBuffer3D;
			Primitive pPrimitive;
			Renderer& _renderer;
			TextureCoordVertex* _vVertex;
			unsigned short* _vIndex;
			
			D3DXVECTOR3* _vecHuesos;
			D3DXVECTOR3* _vectorDraw;


			std::vector<BoneInfo*> _Bones;

			UINT _numVertex;
			UINT _numIndex;
	};
}
#endif
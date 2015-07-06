#ifndef MESH_H
#define MESH_H

#include "Entity3D.h"
#include "../Entity3D/BoneInfo.h"
#include "../Renderer/xMath.h"
#include "../Renderer/RenderTypes.h"
#include "../Renderer/Material.h"
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

			Texture getTexture();
			void setTexture(Texture texture);
			Material* getMaterial();
			void setMaterial(Material* material);
			D3DXPLANE GetPlane(D3DXMATRIX*);
			void calculateBB();
			void getTransformedBox(D3DXMATRIX * pMatrizMundo, D3DXVECTOR3 * pOut);

			static int DrawnMeshes;

		private:
			IndexBuffer* _indexBuffer;
			VertexBuffer3D* _vertexBuffer3D;
			Primitive pPrimitive;
			Renderer& _renderer;
			Texture _texture;
			Material *_material;

			TextureCoordVertex* _vVertex;
			unsigned short* _vIndex;
			UINT _numVertex;
			UINT _numIndex;
			
			D3DXVECTOR3* _vecHuesos;
			D3DXVECTOR3* _vectorDraw;
			D3DXVECTOR3* _vBB;
			std::vector<BoneInfo*> _Bones;
	};
}
#endif
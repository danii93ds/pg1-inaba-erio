#ifndef QUAD_H
#define QUAD_H

#include "Entity2D.h"
#include "../Renderer/xMath.h"
#include "../Renderer/RenderTypes.h"
#include "../myengine_api.h"

namespace Inaba
{
	class Renderer;
	class Timer;
	class MYENGINE_API Quad : public Entity2D
	{
		public:
			Quad();
			~Quad();

			void Draw(Renderer&) const;
			void setColor(DWORD c, int v);
			void setColor(DWORD c);
			void Update(Timer&){}
		private:
			ColorVertex* _vertex;
	};
}

#endif
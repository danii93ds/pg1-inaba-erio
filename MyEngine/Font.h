#ifndef FONT_H
#define FONT_H

#include "myengine_api.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Renderer\RenderTypes.h"
#include <string>

namespace Inaba
{
	class MYENGINE_API Font
	{
		public:
			Font();
			~Font();
			bool Init( LPDIRECT3DDEVICE9 pDevice, char* faceName, int size, bool bold = false, bool italic = false );
			bool DeInit();
			void Print( const char *text, int xPosition, int yPosition, DWORD color,
						LPD3DXSPRITE sprite = NULL,int textBoxWidth = 0, int textBoxHeight = 0, FONTALIGNMENT alignment = FA_LEFT ); 

		private:
			ID3DXFont *_font;
	};
	 
}

#endif
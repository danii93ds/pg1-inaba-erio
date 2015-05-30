#include "Font.h"
using namespace Inaba;

Font::Font():
_font(NULL)
{
}

Font::~Font()
{

}

bool Font::Init(LPDIRECT3DDEVICE9 pDevice, char* faceName, int size, bool bold, bool italic )
{
	if(_font != NULL)
	{
		return false;
	}

	HRESULT hr = 0; 
	hr = D3DXCreateFont( pDevice,						//D3D Device
						size,							//Font height
						0,								//Font width
						bold ? 800 : 0,					//Font Weight
						1,								//MipLevels
						italic,							//Italic
						DEFAULT_CHARSET,				//CharSet
						OUT_DEFAULT_PRECIS,				//OutputPrecision
						DEFAULT_QUALITY,				//Quality
						DEFAULT_PITCH | FF_DONTCARE,	//PitchAndFamily
						faceName,						//pFacename,
						&_font );						//ppFont
	if ( FAILED( hr ) )
	{
		return false; 
	} 
	return true; 
}

bool Font::DeInit()
{
	if (_font != NULL){
		_font->Release();
		_font = NULL;
		return true;
	}
	return false;
}


void Font::Print( const char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite,
				  int textBoxWidth, int textBoxHeight, FONTALIGNMENT alignment ) 
{ 
	if ( !_font ) 
	{ 
		return; 
	} 
	DWORD format = DT_EXPANDTABS; 
	if ( textBoxWidth == 0 ) 
	{ 
		format |= DT_NOCLIP; 
	} 
	else 
	{ 
		format |= DT_WORDBREAK;
		switch ( alignment )
		{ 
		case FA_LEFT:
			format |= DT_LEFT; 
			break; 
		case FA_CENTER: 
			format |= DT_CENTER; 
			break; 
		case FA_RIGHT: 
			format |= DT_RIGHT; 
			break; 
		case FA_TOPRIGHT: 
			format |= DT_RIGHT | DT_TOP; 
			break; 
		case FA_BOTTOMRIGHT: 
			format |= DT_RIGHT | DT_BOTTOM; 
			break; 
		case FA_TOPLEFT: 
			format |= DT_LEFT | DT_TOP; 
			break; 
		case FA_BOTTOMLEFT: 
			format |= DT_LEFT | DT_BOTTOM; 
			break; 
		} 
		if ( textBoxHeight == 0 ) 
		{ 
			textBoxHeight = 2000;
		} 
	}
	RECT rect = { xPosition, yPosition, xPosition + textBoxWidth, yPosition + textBoxHeight }; 
	_font->DrawText( sprite, text, -1, &rect, format, color );
}
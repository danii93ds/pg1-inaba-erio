#include "Renderer.h"

using namespace Inaba;
 
D3DPRIMITIVETYPE primitivesMapping[Inaba::PrimitiveCount] =
{
	D3DPT_TRIANGLELIST, 
	D3DPT_TRIANGLESTRIP,
	D3DPT_POINTLIST,
	D3DPT_LINELIST,     
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLEFAN
};

D3DTRANSFORMSTATETYPE MatrixTypeMapping[MatrixTypeCount] =
{
	D3DTS_VIEW,
	D3DTS_PROJECTION,
	D3DTS_WORLD
};

Renderer::Renderer():
_d3d(NULL),
_d3ddev(NULL),
_vertexbuffer(NULL),
_textureCoordVertexbuffer(NULL)
{
	wireFrameMode = false;
}

Renderer::~Renderer()
{
	_d3d->Release();
	_d3d = NULL;
	
	_d3ddev->Release();
	_d3ddev = NULL;
	
	delete _vertexbuffer;
	_vertexbuffer = NULL;

	delete _textureCoordVertexbuffer;
	_textureCoordVertexbuffer = NULL;

	for(std::vector<Texture>::iterator it = _vectorTextures.begin(); it!= _vectorTextures.end(); it++)
	{
		(*it)->Release();
		(*it) = NULL;
	}
	_vectorTextures.clear();
}

bool Renderer::Init(HWND hWnd)
{
	_d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE; // program windowed, not fullscreen

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames

    d3dpp.hDeviceWindow = hWnd; // set the window to be used by Direct3D

	d3dpp.EnableAutoDepthStencil = true;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and information from the d3dpp
    if(_d3d->CreateDevice(	D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL, // Hardware Abstraction Layer is used to indicate that Direct3D 
										    //should be using hardware to process graphics
							hWnd, // handle window
							D3DCREATE_SOFTWARE_VERTEXPROCESSING, // indicates that all 3D calculations should be done with
							&d3dpp,
							&_d3ddev) 
	!= D3D_OK)
	{		
		return false;
	}

	_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE); // setea el uso de luz o no. 

	_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
	_d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE); //activa alpha
	_d3ddev->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	_d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//_d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	_d3ddev->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	//INIT CAMERA
	_camera = new Camera();
	_d3ddev->SetTransform(D3DTS_VIEW, _camera->getViewMatrix()); 
	_d3ddev->SetTransform(D3DTS_PROJECTION, _camera->getProjectionMatrix());	
	

	/* ORTHOGRAPHIC
	
	D3DVIEWPORT9 viewport;
	_d3ddev->GetViewport(&viewport);
	float viewportWidth = static_cast<float>(viewport.Width);
	float viewportHeight = static_cast<float>(viewport.Height);
	D3DXMatrixOrthoLH(&projectionMatrix,viewportWidth,viewportHeight, -1.0f, 1.0f); // left handed
	*/

	_vertexbuffer = new Inaba::VertexBuffer(_d3ddev,sizeof(Inaba::ColorVertex),Inaba::ColorVertexType);
	_textureCoordVertexbuffer = new VertexBuffer(_d3ddev,sizeof(Inaba::TextureCoordVertex),Inaba::TextureCoordVertexType);


	//FONT
	_font = new Font();
	_font->Init(_d3ddev, "Arial", 22);
	return true;
}

void Renderer::deInit()
{
	for(std::vector<Texture>::iterator it = _vectorTextures.begin(); it!= _vectorTextures.end(); it++)
	{
		(*it)->Release();
		(*it) = NULL;
	}
	_vectorTextures.clear();
}

void Renderer::BeginFrame()
{
	// Limpia el buffer	
	_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
    
	//tells Direct3D you are ready to start rendering. 
	//to tell Direct3D that you are in control of the memory. 
	//locking, where the buffer in the video RAM is 'locked'
	_d3ddev->BeginScene(); 

}

void Renderer::EndFrame()
{
	if(!_vertexbuffer)
		_vertexbuffer->flush();

	_d3ddev->EndScene(); // unlocks
    _d3ddev->Present(NULL, NULL, NULL, NULL);
}

void Renderer::Draw(ColorVertex* vertex,Inaba::Primitive primitive,size_t vertexCount)
{
	_vertexbuffer->bind();
	_vertexbuffer->draw(vertex,primitivesMapping[primitive], vertexCount);
}

void Renderer::Draw(TextureCoordVertex* vertex,Inaba::Primitive primitive,size_t vertexCount)
{
	_textureCoordVertexbuffer->bind();
	_textureCoordVertexbuffer->draw(vertex,primitivesMapping[primitive], vertexCount);
}

void Renderer::Draw(Inaba::Primitive primitive)
{
	_d3ddev->DrawIndexedPrimitive(primitivesMapping[primitive], 0, 0, _vertexBuffer3D->vertexCount(), 0, _indexBuffer->indexCount() / 3);
}

void Renderer::setMatrix(MatrixType matrixType, const Matrix& matrix)
{
	_d3ddev->SetTransform(MatrixTypeMapping[matrixType], matrix);
}

void Renderer::setWireFrameMode(bool wireFrame){
	if (wireFrame == true){
		_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else{
		_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	wireFrameMode = wireFrame;
}

bool Renderer::getWireFrameMode()
{
	return wireFrameMode;
}

const Texture Renderer::LoadTexture(const std::string& FileName, int KeyCode)
{
	IDirect3DTexture9 *texture = NULL;

	HRESULT hr = D3DXCreateTextureFromFileEx(
												_d3ddev,
												FileName.c_str(),
												0,0,0,0,
												D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
												D3DX_FILTER_NONE, D3DX_FILTER_NONE,
												KeyCode,
												NULL,
												NULL,
												&texture
												);

	if (hr != D3D_OK)
	{
		return NoTexture;
	}
	else
	{
		_vectorTextures.push_back(texture);
		return texture;
	}

}

void Renderer::setCurrentTexture(const Texture& texture)
{
	_d3ddev->SetTexture(0,texture);
}

void Renderer::setLight(D3DLIGHT9 * light, unsigned long in)
{
	_d3ddev->SetLight(in, light);
}

void Renderer::enableLight(bool enabled, unsigned long in)
{
	_d3ddev->LightEnable(in, enabled);
}

void Renderer::setCurrentVertexBuffer(VertexBuffer3D* vertexBuffer3D)
{
	_vertexBuffer3D = vertexBuffer3D;
}

void Renderer::setCurrentIndexBuffer(IndexBuffer* indexBuffer)
{
	_indexBuffer = indexBuffer;
}

VertexBuffer3D * Renderer::createVertexBuffer3D(size_t vSize, unsigned int FVF)
{
	return new VertexBuffer3D(*this,_d3ddev,vSize,FVF);
}

IndexBuffer * Renderer::createIndexBuffer()
{
	return new IndexBuffer(*this,_d3ddev);
}

Font* Renderer::getFont()
{
	return _font;
}

Camera* Renderer::getCamera()
{
	return _camera;
}

void Renderer::UpdateCamera()
{
	_camera->Update();
	_camera->BuildViewFrustum();
	_d3ddev->SetTransform(D3DTS_VIEW, _camera->getViewMatrix());
	_d3ddev->SetTransform(D3DTS_PROJECTION, _camera->getProjectionMatrix());

}
#include "Sprite.h"
#include "../Renderer/Renderer.h"
#include "Animation.h"
#include "../Timer/timer.h"
#include <d3dx9.h>

using namespace Inaba;

Sprite::Sprite():
Entity2D(),
_texture(NoTexture),
_vertex(new TextureCoordVertex[4]),
_animation(new Animation())
{
	static const float SIZE = 0.5f;
	_vertex[0].x = -SIZE;	_vertex[0].y = SIZE;	_vertex[0].z = 0.0f;
	_vertex[1].x = SIZE;	_vertex[1].y = SIZE;	_vertex[1].z = 0.0f;
	_vertex[2].x = -SIZE;	_vertex[2].y = -SIZE;	_vertex[2].z = 0.0f;
	_vertex[3].x = SIZE;	_vertex[3].y = -SIZE;	_vertex[3].z = 0.0f;

	_vertex[0].u = 0.0;		_vertex[0].v = 0.0;
	_vertex[1].u = 1.0;		_vertex[1].v = 0.0;
	_vertex[2].u = 0.0;		_vertex[2].v = 1.0;
	_vertex[3].u = 1.0;		_vertex[3].v = 1.0;

	//UpdateLocalTransformation();
}

Sprite::~Sprite()
{
	delete[] _vertex;
	_vertex = NULL;

	delete _animation;
	_animation = NULL;

	_animations.clear();
}

void Sprite::setTexture(Texture& texture)
{
	_texture = texture;
}

void Sprite::setTextureCoords  (float fU1, float fV1,
								float fU2, float fV2,
								float fU3, float fV3,
								float fU4, float fV4)
{
	_vertex[0].u = fU1;		_vertex[0].v = fV1;
	_vertex[1].u = fU2;		_vertex[1].v = fV2;
	_vertex[2].u = fU3;		_vertex[2].v = fV3;
	_vertex[3].u = fU4;		_vertex[3].v = fV4;
}

void Sprite::Draw(Renderer& renderer) const
{
	renderer.setCurrentTexture(_texture);
	renderer.setMatrix(World,_transformationMatrix);
	renderer.Draw(_vertex,Inaba::TriangleStrip, 4);
}

void Sprite::setAnimation(Animation* pkAnimation)
{
	_animation = pkAnimation;
}

void Sprite::setAnimation(std::string nameAnimation)
{
	if(_animations.empty())
		return;
	
	std::vector<Animation>::iterator iter;
	for(iter = _animations.begin(); iter != _animations.end(); iter++)
	{		
		if(iter->name() == nameAnimation)
		{
			_animation = iter._Ptr;
			return;
		}
	}
}

void Sprite::Update(Timer& rkTimer)
{
	if(!_animation)
	{
		return;
	}
	_animation->Update(rkTimer);

	unsigned int uiCurrentFrame = _animation->currentFrame();

	if(uiCurrentFrame != _previousFrame)
	{
		const Animation::Frame& frame = _animation->frames()[uiCurrentFrame];
		setTextureCoords(frame.u1, frame.v1,frame.u2, frame.v2,frame.u3, frame.v3,frame.u4, frame.v4);
	}
}

void Sprite::AddAnimation(Animation *animation)
{
	_animation = animation;
	_animations.push_back(*animation);
}

void Sprite::AddAnimation(std::vector<Animation> animation)
{
	_animations = animation;

	if(!_animations.empty())
		_animation = &_animations[0];
}

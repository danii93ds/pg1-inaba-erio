#include "Animation.h"
#include "../Timer/timer.h"

using namespace Inaba;
Animation::Animation() :
_currentFrame(0),
_currentTime(0),
_length(1.0f)
{
}

Animation::~Animation()
{
	_frames.clear();
}

unsigned int Animation::currentFrame() const{
 return _currentFrame;
}

void Animation::addFrame(float fTextureWidth, float fTextureHeight, float fFramePosX,  float fFramePosY,float fFrameWidth,   float fFrameHeight){
Frame frame;
frame.u1 = (fFramePosX / fTextureWidth);
frame.v1 = (fFramePosY / fTextureHeight);

frame.u2 = ( (fFramePosX + fFrameWidth) / fTextureWidth);
frame.v2 = (fFramePosY / fTextureHeight);

frame.u3 = (fFramePosX / fTextureWidth);
frame.v3 = ( (fFramePosY + fFrameHeight) / fTextureHeight);

frame.u4 = ( (fFramePosX + fFrameWidth) / fTextureWidth);
frame.v4 = ( (fFramePosY + fFrameHeight)/ fTextureHeight);

_frames.push_back(frame);
}

void Animation::resetFrames(){
 _currentFrame = 0;
}

void Animation::Update(Timer& rkTimer){
 _currentTime += rkTimer.timeBetweenFrames();
 while(_currentTime > _length){
  _currentTime-= _length;
 }
 _currentFrame = static_cast<unsigned int>((_currentTime / _length) * _frames.size());
}

void Animation::setLength(float Length){
 _length = Length;
}

std::string Animation::name() const
{
	return _name;
}

void Animation::setName(std::string name)
{
	_name = name;
}
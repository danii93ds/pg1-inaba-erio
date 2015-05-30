#ifndef ANIMATION_H
#define ANIMATION_H

#include "../myEngine_API.h"
#include <vector>
#include <string>

namespace Inaba
{
	class Timer;
	class MYENGINE_API Animation

	{
		public:
			Animation();
			~Animation();
			struct Frame{
				float u1; float v1;
				float u2; float v2;
				float u3; float v3;
				float u4; float v4;
			};
			unsigned int _currentFrame;
			unsigned int currentFrame() const;
		
			const std::vector<Frame> &frames () const { return _frames; }
		
			void addFrame (	float fTextureWidth, float fTextureHeight, 
							float fFramePosX, float fFramePosY,
							float fFrameWidth, float fFrameHeight
						  );

		private:
			std::vector<Frame> _frames;
		public:
			void setLength(float fLength);
			void Update(Timer&);
			void resetFrames();
			std::string name() const;
			void setName(std::string);
		private:
			float _currentTime;
			float _length;
			std::string _name;
	};
}

#endif
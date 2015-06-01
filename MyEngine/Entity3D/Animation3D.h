#ifndef ANIMATION3D_H
#define ANIMATION3D_H

#include <string>
#include <vector>
#include <d3dx9.h>
#include "../Assimp32/include/Importer.hpp"
#include "../Assimp32/include/types.h"
#include "../Assimp32/include/scene.h"
#include "../Assimp32/include/postprocess.h"
#include "../myengine_api.h"

namespace Inaba
{	

	class Timer;

	class MYENGINE_API Animation3D
	{		

		public:
			Animation3D();
			~Animation3D();

			enum State{
				STOP,
				PLAY,
				PAUSE
			};

			struct keyFrame {
				aiString nodeName;
				unsigned int nRotationKeys;
				unsigned int nScalingKeys;
				unsigned int nPositionKeys;
				aiQuatKey *rotationKeys;
				aiVectorKey *scalingKeys;
				aiVectorKey *positionKeys;
			};


			std::string name();
			double duration();
			double ticksPerSecond();
			void setDuration(double);
			void setTicksPerSecond(double);
			void setName(std::string);
			void addKeyFrame(keyFrame*);
			State getState();
			void Update(Timer&);

			void Play();
			void Stop();

			Matrix getTransformationMatrix(unsigned int);
			D3DXVECTOR3 getInterpolatingVec(aiVectorKey*, int, double);
			D3DXQUATERNION getInterpolatingQuat(aiQuatKey*, int, double);
			
		private:
			std::string _name;
			double _duration;
			double _ticksPerSecond;
			double _cFrameTime;
			State _state;

			std::vector<keyFrame*> keyFrames;
	};
}
#endif
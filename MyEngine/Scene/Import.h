#ifndef IMPORT_H
#define IMPORT_H

#include "../myengine_api.h"
#include <string>
#include "../xml/tinyxml2.h"
#include <vector>
namespace Inaba
{	
	class Scene;
	class Quad;
	class Sprite;
	class Animation;
	class Renderer;

	class MYENGINE_API Import
	{
		private:
			static Import *_instance;
			Renderer *_renderer;
			Import();
			Import(Import &const);
			Import& operator= (Import const&);

		public:
			static Import* GetInstance();
			void setRenderer(Renderer*);
			bool importScene(Scene&);
			
		private:
			void importSprite(Scene&,tinyxml2::XMLElement*);
			void importQuad(Scene&,tinyxml2::XMLElement*);
			void importAnimation(std::vector<Animation> **animations,tinyxml2::XMLElement*);
	};
}


#endif

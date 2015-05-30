#include "Import.h"
#include "Scene.h"
#include "../Entity2D/Quad.h"
#include "../Entity2D/Sprite.h"
#include "../Entity2D/Animation.h"
#include "../Entity2D/Entity2D.h"
#include "../Renderer/Renderer.h"
using namespace Inaba;

Import* Import::_instance = NULL;

Import::Import()
: _renderer(NULL)
{
}

Import* Import::GetInstance()
{
	if (!_instance)
	{
		_instance = new Import();
	}
	return _instance;
}

void Import::setRenderer(Renderer *renderer)
{
	_renderer = renderer;
}

bool Import::importScene(Scene &scene)
{
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile(scene.fileName.c_str());
	if(xmlDoc.Error())
		return false;

	tinyxml2::XMLElement *root = xmlDoc.FirstChildElement("SCENE");

	importSprite(scene,root);
	importQuad(scene,root);
}

void Import::importSprite(Scene &scene,tinyxml2::XMLElement* root)
{
	tinyxml2::XMLElement *sprite = root->FirstChildElement("SPRITE");
	tinyxml2::XMLElement *instance = root->FirstChildElement("INSTANCE");
	scene.setName(root->Attribute("name"));

	while(sprite!= NULL)
	{	
		std::string nameSprite = sprite->Attribute("name");
		std::string texturePath = sprite->Attribute("texture");
		int r = sprite->IntAttribute("r");
		int g = sprite->IntAttribute("g");
		int b = sprite->IntAttribute("b");

		Inaba::Texture texture = _renderer->LoadTexture(texturePath,Inaba_COLOR_RGB(r,g,b));

		//CREAR LISTA ANIM
		std::vector<Animation> *list_animations = new std::vector<Animation>();

		importAnimation(&list_animations,sprite->FirstChildElement("ANIMATION"));
		
		instance = root->FirstChildElement("INSTANCE");
		while(instance != NULL)
		{
			//CREAR SPRITE 
			Sprite *ent_sprite = new Sprite();

			if(instance->Attribute("sprite") == nameSprite)
			{
				std::string name = instance->Attribute("name");
				float posX = instance->FloatAttribute("posX");
				float posY = instance->FloatAttribute("posY");
				float rotation = instance->FloatAttribute("rotation");
				float scaleX = instance->FloatAttribute("scaleX");
				float scaleY = instance->FloatAttribute("scaleY");
				
				//GRABAMOS SPRITE			
				ent_sprite->setPos(posX,posY);
				ent_sprite->setName(name);
				ent_sprite->setRotation(rotation);
				ent_sprite->setScale(scaleX,scaleY);
				ent_sprite->setTexture(texture);

			}
			//CARGO ANIMACIONES
			ent_sprite->AddAnimation(*list_animations);

			//PUSH_BACK A LISTA ENTITY2D
			scene.AddEntity(ent_sprite);

			instance = instance->NextSiblingElement("INSTANCE");
		}
		sprite = sprite->NextSiblingElement("SPRITE");
	}
}

void Import::importQuad(Scene &scene,tinyxml2::XMLElement* root)
{
	tinyxml2::XMLElement *quad = root->FirstChildElement("QUAD");

	while(quad != NULL)
	{
		//CREO QUAD
		Quad *ent_quad = new Quad();

		std::string name = quad->Attribute("name");
		float posX = quad->FloatAttribute("posX");
		float posY = quad->FloatAttribute("posY");
		float rotation = quad->FloatAttribute("rotation");
		float scaleX = quad->FloatAttribute("scaleX");
		float scaleY = quad->FloatAttribute("scaleY");
		int r = quad->IntAttribute("r");
		int g = quad->IntAttribute("g");
		int b = quad->IntAttribute("b");

		ent_quad->setName(name);
		ent_quad->setPos(posX,posY);
		ent_quad->setRotation(rotation);
		ent_quad->setScale(scaleX,scaleY);
		ent_quad->setColor(Inaba_COLOR_RGB(r,g,b));

		//PUSH_BACK A LISTA ENTITY2D
		scene.AddEntity(ent_quad);

		quad = quad->NextSiblingElement("QUAD");
	}

}

void Import::importAnimation (std::vector<Animation> **list_animations,tinyxml2::XMLElement* animations)
{
	while(animations != NULL)
	{
		//CREATE ANIMATION AUX
		Animation *anim = new Animation();

		std::string name = animations->Attribute("name");
		float lenght = animations->FloatAttribute("lenght");
		float t_width = animations->FloatAttribute("width");
		float t_height = animations->FloatAttribute("height");

		anim->setName(name);
		anim->setLength(lenght);

		tinyxml2::XMLElement *frame = animations->FirstChildElement("FRAME");
		while(frame != NULL)
		{			
			float posX = frame->FloatAttribute("posX");
			float posY = frame->FloatAttribute("posY");
			float width = frame->FloatAttribute("width");
			float height = frame->FloatAttribute("height");

			//ADDFRAME (t_width, t_height, posX, posY, width, height);
			anim->addFrame(t_width, t_height, posX, posY, width, height);

			frame = frame->NextSiblingElement("FRAME");
		}
		// PUSH_BACK ANIMATION
		(*list_animations)->push_back(*anim);

		animations = animations->NextSiblingElement("ANIMATION");
	}
}



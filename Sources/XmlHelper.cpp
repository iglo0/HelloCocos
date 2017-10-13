#include "XmlHelper.h"
#include "pugixml.hpp"

#include "AnimSprites.h"	// para tratar con la clase

AnimSprites *XmlHelper::loadAnimation(Node *parentNode, const char *animSetName){
	char *currAnimName, *framePath;
	float frameWait, spriteScale;
	bool animLoop;
	AnimSprites *tmpAnimSprites;

	auto directorInstance = Director::getInstance();
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename_);


	if(!result){
		// error! salir o algo
		//CCLOG("Error parseando xml %s", filename);
		return nullptr;
	}

	// cuando cargo un "animSet" la idea es que sea completo e independiente, así que creo un nuevo objeto en vez de intentar añadir a otro.
	// en resumen, que new AnimSprites(...)
	tmpAnimSprites = new AnimSprites(parentNode);
	AnimSprites::frame *tmpFrame;
	AnimSprites::animation *tmpAnimation;

	// Search for the first matching entry with the given hint attribute

	// Juer siempre estoy igual :D ==> Error de compilador, no puedo sumar dos punteros...
	//std::string searchStr = "definitions/animset/" + animSetName;	// "xxx" es const char *
	std::string searchStr = "definitions/animset/" + std::string(animSetName);
	// conversion a string y de vuelta a const char *
	pugi::xpath_node xpathNode = doc.select_single_node(searchStr.c_str());

	if(xpathNode){
		pugi::xml_node selectedNode = xpathNode.node();

		for(pugi::xml_node xmlAnim = selectedNode.child("anim"); xmlAnim; xmlAnim = xmlAnim.next_sibling("anim")){

			// cada anim de animSet tiene un nombre para la animación, atributos como si es loop o no y la lista de frames
			currAnimName = (char *)xmlAnim.attribute("name").value();
			animLoop = xmlAnim.attribute("loop").as_bool();

			// voy creando una estructura en memoria para el juego
			tmpAnimation = new AnimSprites::animation(animLoop);

			// busco los frames que hay y los asigno
			for(pugi::xml_node xmlFrame = xmlAnim.child("frame"); xmlFrame; xmlFrame = xmlFrame.next_sibling("frame")){

				// ruta al archivo del sprite
				framePath = (char *)xmlFrame.attribute("path").value();
				frameWait = xmlFrame.attribute("wait").as_float();
				spriteScale = xmlFrame.attribute("size").as_float();

				tmpFrame = new AnimSprites::frame(parentNode, framePath, frameWait, spriteScale);
				// addFrame añade el frame a la animación si existe en el set (por nombre), o crea una nueva si no.
				tmpAnimation->addFrame(tmpFrame);
			}

			tmpAnimSprites->addAnimation(currAnimName, tmpAnimation);

		}

	} else{
		// definicion no encontrada
		CCLOG("[XML]Definicion para: %s no encontrada", searchStr.c_str());
	}

	return tmpAnimSprites;
}



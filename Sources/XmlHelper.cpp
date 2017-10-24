#include "XmlHelper.h"
#include "pugixml.hpp"
#include "GameActor.h"	// TODO: no se si con esto cojo los gameactor y sus hijos?
#include "Game.h"
#include "AnimSprites.h"	// para tratar con la clase

AnimSprites *XmlHelper::loadAnimation(Node *parentNode, const char *animSetName, GameActor *gameActor){
	char *currAnimName, *framePath;
	float frameWait, spriteScale, ttl;
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
	tmpAnimSprites = new AnimSprites(gameActor);
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
			// si encuentra el atributo ttl lo aplica, si no no.
			if(!xmlAnim.attribute("ttl").empty()){
				ttl = xmlAnim.attribute("ttl").as_float();
				if(gameActor){
					gameActor->setTTL(ttl);
				}
			}

			// voy creando una estructura en memoria para el juego
			tmpAnimation = new AnimSprites::animation(animLoop);

			// busco los frames que hay y los asigno
			for(pugi::xml_node xmlFrame = xmlAnim.child("frame"); xmlFrame; xmlFrame = xmlFrame.next_sibling("frame")){

				// ruta al archivo del sprite
				framePath = (char *)xmlFrame.attribute("path").value();
				frameWait = xmlFrame.attribute("wait").as_float();
				spriteScale = xmlFrame.attribute("size").as_float();

				tmpFrame = new AnimSprites::frame(parentNode, framePath, frameWait, spriteScale);
				tmpFrame->sprite_->setUserData(gameActor);
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


void XmlHelper::assignPhysicsToAnimation(AnimSprites *anim, GameActor *gA, int tipoColision, int colisionaCon){
	AnimSprites::animation *tmpAnim;
	AnimSprites::frame *tmpFrame;

	if(anim){
		for(auto it = anim->animations_.cbegin(); it != anim->animations_.cend(); ++it){
			tmpAnim = (AnimSprites::animation *)(*it).second;

			for(auto f = tmpAnim->animationFrames_.cbegin(); f != tmpAnim->animationFrames_.cend(); ++f){
				tmpFrame = (AnimSprites::frame *)(*f);
				tmpFrame->sprite_->setUserData(gA);

				Game::anadeFisica(tmpFrame->sprite_, tipoColision, colisionaCon,"psch", tmpFrame->sprite_->getName().c_str());

				// HACK: PRUEBA! A ver is se enlentece aquí
				// respuesta: Sí.
				// TODO: tengo que darle una vuelta a ver por qué se terminan creando los sprites de las animaciones con la física activada
				//tmpFrame->sprite_->setVisible(false);
				//auto pb = tmpFrame->sprite_->getPhysicsBody();
				//if(pb){
				//	pb->setEnabled(false);
				//}
			}
		}
	}
}
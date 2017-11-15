#include "XmlHelper.h"
#include "pugixml.hpp"
#include "GameActor.h"	// TODO: no se si con esto cojo los gameactor y sus hijos?
#include "Game.h"
#include "AnimSprites.h"	// para tratar con la clase
#include "Bullet.h"
#include "Movimiento.h"
#include "Enemy.h"
#include "Pool.h"

AnimSprites *XmlHelper::loadAnimation(Node *parentNode, const char *animSetName, GameActor *gameActor){
	char *currAnimName, *framePath;
	float frameWait, spriteScale, ttl;
	bool animLoop, animFixed;
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
		//tmpAnimSprites->setIsFixedImage(selectedNode.attribute("fixedImage").as_bool());

		for(pugi::xml_node xmlAnim = selectedNode.child("anim"); xmlAnim; xmlAnim = xmlAnim.next_sibling("anim")){

			// cada anim de animSet tiene un nombre para la animación, atributos como si es loop o no y la lista de frames
			currAnimName = (char *)xmlAnim.attribute("name").value();
			animLoop = xmlAnim.attribute("loop").as_bool();
			animFixed = xmlAnim.attribute("fixedImage").as_bool();

			// si encuentra el atributo ttl lo aplica, si no no.
			if(!xmlAnim.attribute("ttl").empty()){
				ttl = xmlAnim.attribute("ttl").as_float();
				if(gameActor){
					gameActor->setTTL(ttl);
				}
			}

			// voy creando una estructura en memoria para el juego
			tmpAnimation = new AnimSprites::animation(animLoop, animFixed);

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

Bullet *XmlHelper::loadBullet(Node *parentNode, const char *xmlBulletDef, const char *bulletName){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename_);
	Bullet *tmp = nullptr;
	AnimSprites *bulletAnim;

	if(!result){
		// error! salir o algo
		CCLOG("Error parseando xml %s", xmlFilename_);
		return nullptr;
	}

	// Si no le he puesto un nombre a la bala, le casca el de su definición
	if(std::strcmp(bulletName, "")==0){
		bulletName = xmlBulletDef;
	}

	// buscar y carga los parámetros de la bala
	std::string searchStr = std::string("definitions/bullets/") + std::string(xmlBulletDef);
	// conversion a string y de vuelta a const char *
	pugi::xpath_node xpathNode = doc.select_single_node(searchStr.c_str());

	if(xpathNode){
		pugi::xml_node selectedNode = xpathNode.node();

		const char *animSetName = (char *)selectedNode.attribute("animSet").value();
		float dmg = selectedNode.attribute("dmg").as_float();
		//const char *movement = (char *)selectedNode.attribute("movement").value();
		int movement = Bullet::devuelveTipoPorNombre(selectedNode.attribute("movement").value());
		float speed = selectedNode.attribute("speed").as_float();
		int tipoColision = selectedNode.attribute("tipoColision").as_int();
		int colisionoCon = selectedNode.attribute("colisionoCon").as_int();
		float ttl = selectedNode.attribute("ttl").as_float();

		// busca y carga la animación (o el sprite)
		bulletAnim = loadAnimation(parentNode, animSetName);

		//tmp = new Bullet(parentNode, bulletName, "", "", "", speed, dmg, tipoColision, colisionoCon);
		tmp = new Bullet(parentNode);
		tmp->setTTL(ttl);

		assignPhysicsToAnimation(bulletAnim, tmp, tipoColision, colisionoCon);
		
		tmp->animSprites_ = bulletAnim;
		tmp->setType((Bullet::bulletTypes)movement);

		switch(movement){
		case Bullet::bulletTypes::tipoBossHoming:
			tmp->movimiento_ = new MueveHoming();
			break;
		case Bullet::bulletTypes::tipoEnemyDirigido:
			tmp->movimiento_ = new MueveDireccion(speed);
			break;
		case Bullet::bulletTypes::tipoEnemyNormal:
			tmp->movimiento_ = new MueveVcal(speed);
			break;
		case Bullet::bulletTypes::tipoPlayer:
			tmp->movimiento_ = new MueveVcal(speed);
			break;
		default:
			CCLOG("Tipo de movimiento desconocido en XmlHelper: %d", movement);
			break;
		}
	}

	return tmp;
}

Enemy *XmlHelper::loadEnemy(Node *parentNode, const char *xmlEnemyDef){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename_);
	Enemy *tmp = nullptr;
	AnimSprites *tmpAnim;

	if(!result){
		// error! salir o algo
		CCLOG("Error parseando xml %s", xmlFilename_);
		return nullptr;
	}

	// buscar y carga los parámetros de la bala
	std::string searchStr = std::string("definitions/enemies/") + std::string(xmlEnemyDef);
	// conversion a string y de vuelta a const char *
	pugi::xpath_node xpathNode = doc.select_single_node(searchStr.c_str());

	if(xpathNode){
		pugi::xml_node selectedNode = xpathNode.node();

		//<honesto speed="100.0" hp="1.0" points="100" animSetName="enemyHonesto" />
		const char *animSetName = (char *)selectedNode.attribute("animSetName").value();
		float hp = selectedNode.attribute("hp").as_float();
		int movement = Bullet::devuelveTipoPorNombre(selectedNode.attribute("movement").value());
		float speed = selectedNode.attribute("speed").as_float();
		int tipoColision = selectedNode.attribute("tipoColision").as_int();
		int colisionoCon = selectedNode.attribute("colisionoCon").as_int();
		const char *poolName = (char *)selectedNode.attribute("poolBalas").value();
		int points = selectedNode.attribute("points").as_int();

		// busca y carga la animación (o el sprite)
		tmpAnim = loadAnimation(parentNode, animSetName);

		//initEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipoNormal);
		tmp = new Enemy();
		tmp->animSprites_ = tmpAnim;

		assignPhysicsToAnimation(tmpAnim, tmp, tipoColision, colisionoCon);
		tmp->poolMisBalas_ = Pool::getBulletPoolByName(poolName);

		tmp->setHP(hp);
		tmp->setPoints(points);
		tmp->gameActorSpeed_ = speed;

		//tmp->movimiento_ = 

		//movimiento_ = nullptr;

		//tmp = new Bullet(parentNode);
		//tmp->setTTL(ttl);

		//assignPhysicsToAnimation(tmpAnim, tmp, tipoColision, colisionoCon);

		//tmp->animSprites_ = tmpAnim;
		//tmp->setType((Bullet::bulletTypes)movement);

		//switch(movement){
		//case Bullet::bulletTypes::tipoBossHoming:
		//	tmp->movimiento_ = new MueveHoming();
		//	break;
		//case Bullet::bulletTypes::tipoEnemyDirigido:
		//	tmp->movimiento_ = new MueveDireccion(speed);
		//	break;
		//case Bullet::bulletTypes::tipoEnemyNormal:
		//	tmp->movimiento_ = new MueveVcal(speed);
		//	break;
		//case Bullet::bulletTypes::tipoPlayer:
		//	tmp->movimiento_ = new MueveVcal(speed);
		//	break;
		//default:
		//	CCLOG("Tipo de movimiento desconocido en XmlHelper: %d", movement);
		//	break;
		//}
	}

	return tmp;
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

				//Game::anadeFisica(tmpFrame->sprite_, tipoColision, colisionaCon, "psch", tmpFrame->sprite_->getName().c_str());
				Game::anadeFisica(tmpFrame->sprite_, tipoColision, colisionaCon, tmpFrame->sprite_->getName().c_str(), tmpFrame->sprite_->getName().c_str());
			}
		}
	}
}
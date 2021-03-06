#include "XmlHelper.h"
#include "pugixml.hpp"
#include "GameActor.h"	// TODO: no se si con esto cojo los gameactor y sus hijos?
#include "Game.h"
#include "AnimSprites.h"	// para tratar con la clase
#include "Bullet.h"
#include "Movimiento.h"
#include "Enemy.h"
#include "Pool.h"
#include "SpaceInvaders.h"
#include "HiScores.h"

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

	// cuando cargo un "animSet" la idea es que sea completo e independiente, as� que creo un nuevo objeto en vez de intentar a�adir a otro.
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

			// cada anim de animSet tiene un nombre para la animaci�n, atributos como si es loop o no y la lista de frames
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
				// addFrame a�ade el frame a la animaci�n si existe en el set (por nombre), o crea una nueva si no.
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

	// Si no le he puesto un nombre a la bala, le casca el de su definici�n
	if(std::strcmp(bulletName, "")==0){
		bulletName = xmlBulletDef;
	}

	// buscar y carga los par�metros de la bala
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

		// busca y carga la animaci�n (o el sprite)
		bulletAnim = loadAnimation(parentNode, animSetName);

		//tmp = new Bullet(parentNode, bulletName, "", "", "", speed, dmg, tipoColision, colisionoCon);
		tmp = new Bullet(parentNode);
		// TODO: por qu� esto falta s�lo en las balas homing y en el resto no dan problemas?
		tmp->gameActorSpeed_ = speed;
		tmp->setTTL(ttl);

		assignPhysicsToAnimation(bulletAnim, tmp, tipoColision, colisionoCon);
		
		tmp->animSprites_ = bulletAnim;
		tmp->setType((Bullet::bulletTypes)movement);

		switch(movement){
		case Bullet::bulletTypes::tipoBossHoming:
			tmp->movimiento_ = new MueveHoming(speed);
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

	// buscar y carga los par�metros de la bala
	std::string searchStr = std::string("definitions/enemies/") + std::string(xmlEnemyDef);
	// conversion a string y de vuelta a const char *
	pugi::xpath_node xpathNode = doc.select_single_node(searchStr.c_str());

	if(xpathNode){
		pugi::xml_node selectedNode = xpathNode.node();

		const char *animSetName = (char *)selectedNode.attribute("animSetName").value();
		float hp = selectedNode.attribute("hp").as_float();
		float speed = selectedNode.attribute("speed").as_float();
		int tipoColision = selectedNode.attribute("tipoColision").as_int();
		int colisionoCon = selectedNode.attribute("colisionoCon").as_int();
		const char *poolName = (char *)selectedNode.attribute("poolBalas").value();
		int points = selectedNode.attribute("points").as_int();
		const char *sonidoDispara = (char *)selectedNode.attribute("sonidoDispara").value();
		const char *sonidoMuerte = (char *)selectedNode.attribute("sonidoMuerte").value();
		int tipoEnemigo = selectedNode.attribute("tipoEnemigo").as_int();

		// busca y carga la animaci�n (o el sprite)
		tmpAnim = loadAnimation(parentNode, animSetName);

		//initEnemy(nodo, gameInstance->enemy_t1_path_sprite.c_str(), "", gameInstance->enemy_t1_initial_size, gameInstance->enemy_t1_initial_rotation, gameInstance->enemy_generic_hp, gameInstance->enemy_generic_points, &Pool::currentBulletsTipoNormal);
		switch(tipoEnemigo){
		case 1:
			tmp = new Enemy(Enemy::tiposEnemigo::tipo1);
			break;
		case 2:
			tmp = new Enemy(Enemy::tiposEnemigo::tipo2);
			break;
		default:
			CCLOG("Creando tipo de enemigo desconocido: %d", tipoEnemigo);
			tmp = new Enemy();
			break;
		}

		tmp->animSprites_ = tmpAnim;

		assignPhysicsToAnimation(tmpAnim, tmp, tipoColision, colisionoCon);
		tmp->poolMisBalas_ = Pool::getBulletPoolByName(poolName);

		tmp->setHP(hp);
		tmp->setPoints(points);
		tmp->gameActorSpeed_ = speed;
		tmp->setSonidoDispara(sonidoDispara);
		tmp->setSonidoMuerte(sonidoMuerte);

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

std::unordered_map<int, SpaceInvaders *> XmlHelper::loadInvaderLevels(Node *parentNode, const char *xmlDefName){
	std::vector<Enemy::tiposEnemigo> tiposEnemigo;
	std::unordered_map<int, SpaceInvaders *> levels;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilename_);
	SpaceInvaders *tmp;
	//AnimSprites *tmpAnim;

	if(!result){
		// error! salir o algo
		CCLOG("Error parseando xml %s", xmlFilename_);
		return levels;
	}

	// buscar y carga los par�metros de la bala
	std::string searchStr = std::string("definitions/oleadas/") + std::string(xmlDefName);
	// conversion a string y de vuelta a const char *
	pugi::xpath_node xpathNode = doc.select_single_node(searchStr.c_str());

	// si esto ha ido bien, estoy delante de varias "oleada"s
	if(xpathNode){
		pugi::xml_node selectedNode = xpathNode.node();

		int level, tamaX, tamaY, margX, margY/*, probDisparoAleat*/;
		float comprX, comprY/*, velHtal, velVcal, vcalMoveAmount*/;

		while(selectedNode){

			// datos generales de cada oleada
			// level es informativo, realmente los cargo y los sacar� en el orden que vienen
			level = selectedNode.attribute("level").as_int();
			tamaX = selectedNode.attribute("tamaX").as_int();
			tamaY = selectedNode.attribute("tamaY").as_int();
			comprX = selectedNode.attribute("comprX").as_float();
			comprY = selectedNode.attribute("comprY").as_float();
			margX = selectedNode.attribute("margX").as_int();
			margY = selectedNode.attribute("margY").as_int();

			// Ign Nuevo! Con estas variables tengo todo lo que necesito
			tmp = new SpaceInvaders(tamaX, tamaY, comprX, comprY, margX, margY);

			tmp->velMovHtal_ = selectedNode.attribute("velHtal").as_float();
			tmp->velMovVcal_ = selectedNode.attribute("velVcal").as_float();
			tmp->vcalMoveAmount_ = selectedNode.attribute("vcalMoveAmount").as_float();
			tmp->probDisparoAleat_ = selectedNode.attribute("probDisparoAleat").as_float();

			// busca a los integrantes de la oleada
			auto xmlIntegrantes = selectedNode.child("integrantes");
			if(xmlIntegrantes){
				for(auto integrante = xmlIntegrantes.first_child(); integrante; integrante = integrante.next_sibling()){

					if(!std::strcmp(integrante.name(), "birojo")) {
						tmp->tipos_.push_back(Enemy::tiposEnemigo::tipo2);
					} else if(!std::strcmp(integrante.name(), "honesto")) {
						tmp->tipos_.push_back(Enemy::tiposEnemigo::tipo1);
					} else{
						CCLOG("tipo desconocio %s", integrante.name());
					}
				}
			}

			levels[level] = tmp;

			selectedNode = selectedNode.next_sibling();
		}
	}

	return levels;
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

void XmlHelper::saveHiScores(const char *fileName, HiScores *hiScores){
	// Generate new XML document within memory
	pugi::xml_document doc;

	// Generate XML declaration
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	// A valid XML doc must contain a single root node of any name
	auto root = doc.append_child("Records");


	// recorre la tabla de records y va creando hijos

	pugi::xml_node nodeChild;
	HiScores::record *rTmp;
	for(auto r = hiScores->tablaRecords.begin(); r != hiScores->tablaRecords.end(); ++r){
		rTmp = &r->second;

		// Append some child elements below root
		nodeChild = root.append_child("record");
		nodeChild.append_attribute("nombre") = rTmp->name.c_str();
		nodeChild.append_attribute("puntos") = rTmp->puntos;
		nodeChild.append_attribute("nivel") = rTmp->nivelAlcanzado.c_str();
	}


	
	// Save XML tree to file.
	// Remark: second optional param is indent string to be used;
	// default indentation is tab character.
	bool saveSucceeded = doc.save_file(fileName, PUGIXML_TEXT("  "));
	assert(saveSucceeded);


}

void XmlHelper::loadHiScores(const char *fileName, HiScores *hiScores){
	// Create empty XML document within memory
	pugi::xml_document doc;
	// Load XML file into memory
	// Remark: to fully read declaration entries you have to specify
	// "pugi::parse_declaration"
	pugi::xml_parse_result result = doc.load_file(fileName,	pugi::parse_default | pugi::parse_declaration);
	if(!result){
		CCLOG("Error parseando xml %s", fileName);
		std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
		return;
	}

	// Obtengo el primer hijo de tipo "record" (todos deber�an serlo)
	pugi::xml_node selectedNode = doc.document_element().child("record");

	while(selectedNode){
		//CCLOG(selectedNode.attribute("nombre").as_string());
		hiScores->insertHiScore(selectedNode.attribute("nombre").as_string(), selectedNode.attribute("nivel").as_string(), selectedNode.attribute("puntos").as_int());

		selectedNode = selectedNode.next_sibling();
	}
	//// Read attribute value
	//pugi::xml_attribute attr;
	//if(attr = selectedNode.attribute("intVal")) // attribute really exists
	//{
	//	// Read value as string
	//	std::cout << "read as string: intVal=" << attr.value() << std::endl;
	//	// Read value as int
	//	int intVal = attr.as_int();
	//	std::cout << "read as int   : intVal=" << intVal << std::endl;
	//	// for other types use as_double, as_bool, as_uint, ...
	//}


}
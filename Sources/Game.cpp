#include "Game.h"
#include "PhysicsShapeCache.h"
#include "HiScores.h"

Game::Game(){
	ellapsedTime = 0;
	//estadoActual = estadosJuego::menus;

	// TODO: me tengo que asegurar que esto se inicializa hacia el principio
	// TODO: esto no es multiplataforma, aparentemente android/ios no leen el fichero desde resources
	loadConfig("test.xml");
	vidas = vidas_iniciales;

	//hiScore = initial_hi_score;
	hiScores_ = new HiScores();

	//hiScores_->cargaTablaRecords();
	hiScoreCached = hiScores_->devuelveHiScoreTablaRecords();
}

Game *Game::getInstance(){
	// thread safe. Así es como me aseguro que se inicializa exactamente una vez, aun con concurrencia
	// el modo tradicional (una variable miembro privada) podría fallar por lo anterior.
	static Game *instance = new Game;
	return instance;
}

Sprite *Game::creaSprite(const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale){
	Sprite *tmp;

	// OJO: Sprite::createWithSpriteFrameName(ruta) -> CASE SENSITIVE! 
	tmp = Sprite::createWithSpriteFrameName(ruta);

	if(!tmp){
		CCLOG("Game::creaSprite '%s'=SIN DEFINIR", ruta);
		return nullptr;// o return sprite pal caso
	}

	tmp->setScale(initialScale);

	tmp->setName(name);

	anadeFisica(tmp, tipoColision, colisionaCon, name, ruta);

	//TODO: Ops¿?
	//tmp->setUserData(this);
	// y su tipo
	tmp->setTag(tipoColision);

	//desactiva();

	// hecho
	//nodo->addChild(tmp);

	return tmp;
}

void Game::anadeFisica(Sprite *sprite, int tipoColision, int colisionaCon, const char *name, const char *colliderName){
	PhysicsBody *fisicaSprite;
	PhysicsShapeCache *physicsShapeCache;

	if(!sprite){
		CCLOG("Error anadiendo fisica al sprite: sprite sin definir");
		return;
	}

	// TODO: PhysicsMaterial OJO
	// Density=0.1f, Restitution=1.0f, Friction=0
	// No tengo ni idea, copiado de un ejemplo sencillo para solo colisiones y que funciona

	// TODO: parece que no es posible convertir autoPolygons en PhysicsBody::createPolygon. LA recomendación es hacerlos a mano (p.ej. con PhysicsEditor -trial 7 dias-)
	// el autopolygon está optimizado para render, no para física
	// los poligonos de física soportan formas menos complejas que los sprites autopolygon
	// empiezo por crear una caja alrededor del sprite

	// =v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v
	// NUEVO: Uso PhysicsEditor con PhysicsShapeCache ligeramente modificado

	// Si encuentro shapecache con el mismo nombre de este sprite, lo uso. Y si no uso el estándar: PhysicsBody::createBox(..)
	// Ojo: physicsBody es undefined ahora, pasará a ser "null" si SetBodyOnSprite no lo consigue
	physicsShapeCache = PhysicsShapeCache::getInstance();
	fisicaSprite = physicsShapeCache->ignSetBodyOnSprite(colliderName, sprite);
	if(!fisicaSprite){
		// physicsBody == null -> sheeit, ponerle una caja
		fisicaSprite = PhysicsBody::createBox(Size(sprite->getContentSize().width, sprite->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	}
	// =^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^


	// set the category, collision and contact test bit masks
	// tipo del objeto
	fisicaSprite->setCategoryBitmask(tipoColision);
	// qué tipos de objeto generan eventos de colisión con este?
	fisicaSprite->setContactTestBitmask(colisionaCon);

	// estableciendo la física como dynamic, el motor no aplicará fuerzas a este objeto
	// Lo que significa que es controlado por el programador (como iskinetic en Unity)
	fisicaSprite->setDynamic(true);

	// que objetos deberian afectar a este en las colisiones
	// (no afectaría por que es dinamico, está puesto por completar)
	fisicaSprite->setCollisionBitmask((int)Game::CategoriaColision::None);

	// TODO: Prueba a capón
	fisicaSprite->setEnabled(false);

	sprite->setPhysicsBody(fisicaSprite);

	sprite->setName(name);

	// hecho

}

void Game::sumaPuntos(int p){
	// suma los puntos dados
	// actualiza el marcador
	// actualiza el hiscore, si procede

	// ---------------------------------------------------------------------------------------------
	// actualiza los puntos
	// ---------------------------------------------------------------------------------------------
	puntos += p;

	// ---------------------------------------------------------------------------------------------
	// actualiza el marcador
	// ---------------------------------------------------------------------------------------------
	// lbl->setString() espera un string
	// para formatear el string utilizo un stringstream, y luego lo convierto a string para el lbl

	std::stringstream ss;
	
	//The most common IO manipulators that control padding are:

	//std::setw(width) sets the width of the field.
	//std::setfill(fillchar) sets the fill character.
	//std::setiosflags(align) sets the alignment, where align is ios::left or ios::right
	
	ss << std::setw(6) << std::setfill('0') << puntos;// << std::endl;
	lblPuntos->setString(ss.str());

	// ---------------------------------------------------------------------------------------------
	// TODO: Comprobar hiscore
	// ---------------------------------------------------------------------------------------------
	if(puntos > hiScoreCached){
		// TODO: animacion, fanfarria o similar (una sola vez)

		// ya tenía los puntos calculados como string :)
		lblHiScore->setString(ss.str());

	}

}

void Game::inicializaGUI(){
	std::stringstream ss;
	// puntos
	ss << std::setw(6) << std::setfill('0') << 0;
	lblPuntos->setString(ss.str());

	// hiscore
	ss.str(std::string()); // is technically more efficient, because you avoid invoking the std::string constructor that takes const char*
	ss << std::setw(6) << std::setfill('0') << hiScoreCached;
	lblHiScore->setString(ss.str());

	// vidas
	actualizaVidas();

}

void Game::actualizaVidas(){
	std::stringstream ss;

	ss << std::setw(2) << std::setfill('0') << vidas;
	lblVidas->setString(ss.str());
}


void Game::loadConfig(const char *filename){

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	pugi::xml_node xml_default_values = doc.child("default_values");
	
	player_path_sprite = xml_default_values.child_value("player_path_sprite");
	player_initial_speed = atof(xml_default_values.child_value("player_initial_speed"));
	player_sonido_dispara = xml_default_values.child_value("player_sonido_dispara");
	player_sonido_muerte = xml_default_values.child_value("player_sonido_muerte");

	game_sonido_invaders_loop = xml_default_values.child_value("game_sonido_invaders_loop");
	game_sonido_casa_impacto = xml_default_values.child_value("game_sonido_casa_impacto");
	game_music_menus = xml_default_values.child_value("game_music_menus");
	game_music_level = xml_default_values.child_value("game_music_level");

	bullet_default_dmg = atof(xml_default_values.child_value("bullet_default_dmg"));

	enemy_generic_speed = atof(xml_default_values.child_value("enemy_generic_speed"));
	enemy_boss_generic_hp = atof(xml_default_values.child_value("enemy_boss_generic_hp"));
	enemy_boss_path_sprite = xml_default_values.child_value("enemy_boss_path_sprite");
	enemy_boss_initial_size = atof(xml_default_values.child_value("enemy_boss_initial_size"));
	enemy_boss_initial_rotation = atof(xml_default_values.child_value("enemy_boss_initial_rotation"));
	enemy_boss_points = atoi(xml_default_values.child_value("enemy_boss_points"));

	duracion_estado_intronivel = atof(xml_default_values.child_value("duracion_estado_intronivel"));
	duracion_estado_finnivel = atof(xml_default_values.child_value("duracion_estado_finnivel"));
	duracion_estado_muerte = atof(xml_default_values.child_value("duracion_estado_muerte"));
	duracion_estado_gameover = atof(xml_default_values.child_value("duracion_estado_gameover"));

	//initial_hi_score = atoi(xml_default_values.child_value("initial_hi_score"));
	vidas_iniciales = atoi(xml_default_values.child_value("vidas_iniciales"));

	// casitas
	sprite_casa_bloque = xml_default_values.child_value("sprite_casa_bloque");
	sprite_casa_bloque_roto_01 = xml_default_values.child_value("sprite_casa_bloque_roto_01");
	sprite_casa_bloque_roto_02 = xml_default_values.child_value("sprite_casa_bloque_roto_02");
	sprite_casa_bloque_roto_03 = xml_default_values.child_value("sprite_casa_bloque_roto_03");
	sprite_casa_bloque_roto_04 = xml_default_values.child_value("sprite_casa_bloque_roto_04");
	//sprite_casa_bloque_roto_05 = xml_default_values.child_value("sprite_casa_bloque_roto_05");
	//sprite_casa_bloque_roto_06 = xml_default_values.child_value("sprite_casa_bloque_roto_06");
	//sprite_casa_bloque_roto_07 = xml_default_values.child_value("sprite_casa_bloque_roto_07");
	//sprite_casa_bloque_roto_08 = xml_default_values.child_value("sprite_casa_bloque_roto_08");
	//sprite_casa_bloque_roto_09 = xml_default_values.child_value("sprite_casa_bloque_roto_09");
	//sprite_casa_bloque_roto_10 = xml_default_values.child_value("sprite_casa_bloque_roto_10");
	//sprite_casa_bloque_roto_11 = xml_default_values.child_value("sprite_casa_bloque_roto_11");
	//sprite_casa_bloque_roto_12 = xml_default_values.child_value("sprite_casa_bloque_roto_12");
	//sprite_casa_bloque_roto_13 = xml_default_values.child_value("sprite_casa_bloque_roto_13");
	//sprite_casa_bloque_roto_14 = xml_default_values.child_value("sprite_casa_bloque_roto_14");
	//sprite_casa_bloque_roto_15 = xml_default_values.child_value("sprite_casa_bloque_roto_15");
	sprite_casa_bloque_roto_dch = xml_default_values.child_value("sprite_casa_bloque_roto_dch");
	sprite_casa_bloque_roto_izq = xml_default_values.child_value("sprite_casa_bloque_roto_izq");
	sprite_casa_esquina_dch = xml_default_values.child_value("sprite_casa_esquina_dch");
	sprite_casa_esquina_izq = xml_default_values.child_value("sprite_casa_esquina_izq");

}

//Game::~Game(){
//}

const char *Game::devuelveBloqueRotoAleatorio(){
	int num = RandomHelper::random_int<int>(1, 15);

	switch(num){
	case 1:
		return sprite_casa_bloque_roto_01.c_str();
		//break;
	case 2:
		return sprite_casa_bloque_roto_02.c_str();
		break;
	case 3:
		return sprite_casa_bloque_roto_03.c_str();
		break;
	case 4:
		return sprite_casa_bloque_roto_04.c_str();
		break;
	//case 5:
	//	return sprite_casa_bloque_roto_05.c_str();
	//	break;
	//case 6:
	//	return sprite_casa_bloque_roto_06.c_str();
	//	break;
	//case 7:
	//	return sprite_casa_bloque_roto_07.c_str();
	//	break;
	//case 8:
	//	return sprite_casa_bloque_roto_08.c_str();
	//	break;
	//case 9:
	//	return sprite_casa_bloque_roto_09.c_str();
	//	break;
	//case 10:
	//	return sprite_casa_bloque_roto_10.c_str();
	//	break;
	//case 11:
	//	return sprite_casa_bloque_roto_11.c_str();
	//	break;
	//case 12:
	//	return sprite_casa_bloque_roto_12.c_str();
	//	break;
	//case 13:
	//	return sprite_casa_bloque_roto_13.c_str();
	//	break;
	//case 14:
	//	return sprite_casa_bloque_roto_14.c_str();
	//	break;
	//case 15:
	//	return sprite_casa_bloque_roto_15.c_str();
	//	break;
	default:
		CCLOG("Fallo devolviendo bloque roto aleatorio!");
		return sprite_casa_bloque_roto_01.c_str();
		break;
	}
}


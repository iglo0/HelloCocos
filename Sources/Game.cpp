#include "Game.h"

#include "PhysicsShapeCache.h"

Game::Game(){
	ellapsedTime = 0;
	estadoActual = estadosJuego::menus;

	// TODO: me tengo que asegurar que esto se inicializa hacia el principio
	// TODO: esto no es multiplataforma, aparentemente android/ios no leen el fichero desde resources
	loadConfig("test.xml");
	vidas = vidas_iniciales;
	hiScore = initial_hi_score;
}

Game *Game::getInstance(){
	// thread safe. Así es como me aseguro que se inicializa exactamente una vez, aun con concurrencia
	// el modo tradicional (una variable miembro privada) podría fallar por lo anterior.
	static Game *instance = new Game;
	return instance;
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
	if(puntos > hiScore){
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
	// clear stringstream
	//ss.str("");
	ss.str(std::string()); // is technically more efficient, because you avoid invoking the std::string constructor that takes const char*
	ss << std::setw(6) << std::setfill('0') << hiScore;
	lblHiScore->setString(ss.str());

	// vidas
	actualizaVidas();
	// clear stringstream
	//ss.str(std::string());
	//ss << std::setw(2) << std::setfill('0') << vidas;
	//lblVidas->setString(ss.str());

}

void Game::actualizaVidas(){
	std::stringstream ss;

	ss << std::setw(2) << std::setfill('0') << vidas;
	lblVidas->setString(ss.str());
}


void Game::loadConfig(const char *filename){

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	//std::cout << "Load result: " << result.description() << ", whatever: " << doc.child("book").attribute("category").value() << std::endl;
	//std::cout << doc.child("person").child_value("firstname") << std::endl;
	//std::cout << doc.child("person").child_value("lastname") << std::endl;

	// C++11
	//for(pugi::xml_node person : doc.children("person")){
	//	//std::cout << person.child_value("firstname") << " " << person.child_value("lastname") << std::endl;
	//	CCLOG("%s", person.child_value("firstname"));
	//}

	//config_properties.insert({ CONFIG_PLAYER_INITIAL_SPEED, xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED) });
	//config_properties.insert({ CONFIG_PLAYER_PATH_SPRITE, xml_default_values.child_value(CONFIG_PLAYER_PATH_SPRITE) });

	//auto iter = config_properties.find(CONFIG_PLAYER_INITIAL_SPEED);

	//if(iter != config_properties.end()){
	//	// ojo find devuelve un iterator
	//	CCLOG("%s", iter->second);
	//}

	//iter = config_properties.find(CONFIG_PLAYER_PATH_SPRITE);
	//if(iter != config_properties.end()){
	//	CCLOG("%s", iter->second);
	//}


	pugi::xml_node xml_default_values = doc.child("default_values");
	
	// TODO: yee-haw! sin comprobaciones
	player_path_sprite = xml_default_values.child_value(CONFIG_PLAYER_PATH_SPRITE);
	player_initial_speed = atof(xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED));
	
	bullet_player_path_sprite1 = xml_default_values.child_value(CONFIG_BULLET_PLAYER_PATH_SPRITE1);
	bullet_enemy_path_sprite2 = xml_default_values.child_value(CONFIG_BULLET_ENEMY_PATH_SPRITE2);
	bullet_path_sound_fire = xml_default_values.child_value(CONFIG_BULLET_PATH_SOUND_FIRE);
	bullet_path_sound_impact = xml_default_values.child_value(CONFIG_BULLET_PATH_SOUND_IMPACT);
	bullet_default_scale = atof(xml_default_values.child_value(CONFIG_BULLET_DEFAULT_SCALE));
	bullet_default_boss_scale = atof(xml_default_values.child_value(CONFIG_BULLET_DEFAULT_BOSS_SCALE));
	bullet_default_dmg = atof(xml_default_values.child_value(CONFIG_BULLET_DEFAULT_DMG));
	bullet_default_speed = atof(xml_default_values.child_value(CONFIG_BULLET_DEFAULT_SPEED));

	enemy_generic_speed = atof(xml_default_values.child_value(CONFIG_ENEMY_GENERIC_SPEED));
	enemy_generic_hp = atof(xml_default_values.child_value(CONFIG_ENEMY_GENERIC_HP));
	enemy_generic_points = atoi(xml_default_values.child_value(CONFIG_ENEMY_GENERIC_POINTS));
	enemy_t1_path_sprite = xml_default_values.child_value(CONFIG_ENEMY_T1_PATH_SPRITE);
	enemy_t1_initial_size = atof(xml_default_values.child_value(CONFIG_ENEMY_T1_INITIAL_SIZE));
	enemy_t1_initial_rotation = atof(xml_default_values.child_value(CONFIG_ENEMY_T1_INITIAL_ROTATION));
	enemy_t2_path_sprite = xml_default_values.child_value(CONFIG_ENEMY_T2_PATH_SPRITE);
	enemy_t2_initial_size = atof(xml_default_values.child_value(CONFIG_ENEMY_T2_INITIAL_SIZE));
	enemy_t2_initial_rotation = atof(xml_default_values.child_value(CONFIG_ENEMY_T2_INITIAL_ROTATION));
	enemy_boss_generic_hp = atof(xml_default_values.child_value(CONFIG_ENEMY_BOSS_GENERIC_HP));
	enemy_boss_path_sprite = xml_default_values.child_value(CONFIG_ENEMY_BOSS_PATH_SPRITE);
	enemy_boss_initial_size = atof(xml_default_values.child_value(CONFIG_ENEMY_BOSS_INITIAL_SIZE));
	enemy_boss_initial_rotation = atof(xml_default_values.child_value(CONFIG_ENEMY_BOSS_INITIAL_ROTATION));
	enemy_boss_points = atoi(xml_default_values.child_value(CONFIG_ENEMY_BOSS_POINTS));
	enemy_path_sound_die = xml_default_values.child_value(CONFIG_ENEMY_PATH_SOUND_DIE);

	duracion_estado_intronivel = atof(xml_default_values.child_value(CONFIG_DURACION_ESTADO_INTRONIVEL));
	duracion_estado_finnivel = atof(xml_default_values.child_value(CONFIG_DURACION_ESTADO_FINNIVEL));
	duracion_estado_muerte = atof(xml_default_values.child_value(CONFIG_DURACION_ESTADO_MUERTE));
	duracion_estado_gameover = atof(xml_default_values.child_value(CONFIG_DURACION_ESTADO_GAMEOVER));

	initial_hi_score = atoi(xml_default_values.child_value(CONFIG_INITIAL_HI_SCORE));
	vidas_iniciales = atoi(xml_default_values.child_value(CONFIG_VIDAS_INICIALES));

}

//Game::~Game(){
//}

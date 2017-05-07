#include "Testz.h"

Testz::Testz(){}

Testz::~Testz(){}

void Testz::xmlTest(const char *filename){

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

	// 
	//std::unordered_map<const char *, const char *> default_values;

	pugi::xml_node xml_default_values = doc.child("default_values");

	//CCLOG("%s", default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED));
	//CCLOG("%s", default_values.child_value(CONFIG_PLAYER_PATH_SPRITE));
	//CCLOG("%s", default_values.child_value("a"));
	//CCLOG("%s", default_values.child_value("b"));

	//defvals.insert(std::make_pair(CONFIG_PLAYER_INITIAL_SPEED, default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED)));
	//defvals.insert(std::make_pair(CONFIG_PLAYER_PATH_SPRITE, default_values.child_value(CONFIG_PLAYER_PATH_SPRITE)));
	// TODO: es equivalente {key,value} a std::make_pair(key,value) ??
	// make_pair -> infiere los tipos sin necesidad de especificárselos
	// parece que {key,value} es C++11
	Game *gameInstance = Game::getInstance();

	gameInstance->config_properties.insert({ CONFIG_PLAYER_INITIAL_SPEED, xml_default_values.child_value(CONFIG_PLAYER_INITIAL_SPEED) });
	gameInstance->config_properties.insert({ CONFIG_PLAYER_PATH_SPRITE, xml_default_values.child_value(CONFIG_PLAYER_PATH_SPRITE) });

	auto iter = gameInstance->config_properties.find(CONFIG_PLAYER_INITIAL_SPEED);
	
	if(iter != gameInstance->config_properties.end()){
		// ojo find devuelve un iterator
		CCLOG("%s", iter->second);
	}

	iter = gameInstance->config_properties.find(CONFIG_PLAYER_PATH_SPRITE);
	if(iter != gameInstance->config_properties.end()){
		CCLOG("%s", iter->second);
	}

}
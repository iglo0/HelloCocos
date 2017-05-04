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
	for(pugi::xml_node person : doc.children("person")){
		//std::cout << person.child_value("firstname") << " " << person.child_value("lastname") << std::endl;
		CCLOG("%s", person.child_value("firstname"));
	}

}
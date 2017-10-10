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
	//tmpAnimSprites = new AnimSprites(Vec2::ZERO);
	tmpAnimSprites = new AnimSprites(nullptr);
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


//void XmlHelper::loadAnimationsTestz(const char *filename, Node *parentNode){
//	/*
//	* xml_node is the handle to document node; it can point to any node in the document, including document itself.
//
//	* There is a special value of xml_node type, known as null node or empty node. It does not correspond to any node in any document, and thus resembles null pointer. 
//	However, all operations are defined on empty nodes; generally the operations don’t do anything and return empty nodes/attributes or empty strings as their result. 
//	This is useful [...] you can get the grandparent of a node like so: node.parent().parent(); 
//	if a node is a null node or it does not have a parent, the first parent() call returns null node; the second parent() call then also returns null node, so you don’t have to check for errors twice.
//
//	* You can test if a handle is null via implicit boolean cast: if (node) { …? } or if (!node) { …? }.
//	
//	* xml_attribute is the handle to an XML attribute; it has the same semantics as xml_node
//	
//	* You can get node or attribute name via name() accessor, and value via value() accessor. Note that both functions never return null pointers
//	they either return a string with the relevant content, or an empty string if name/value is absent or if the handle is null.
//
//	Also there are two notable things for reading values:
//
//    * It is common to store data as text contents of some node - i.e. <node><description>This is a node< / description>< / node>.
//	In this case, <description> node does not have a value, but instead has a child of type node_pcdata with value "This is a node".
//	pugixml provides child_value() and text() helper functions to parse such data.
//
//	In many cases attribute values have types that are not strings - i.e.an attribute may always contain values that should be treated as integers, 
//	despite the fact that they are represented as strings in XML.pugixml provides several accessors that convert attribute value to some other type.
//	*/
//	
//	auto directorInstance = Director::getInstance();
//
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_file(filename);
//
//	if(!result){
//		// error! salir o algo
//		//CCLOG("Error parseando xml %s", filename);
//		return;
//	}
//
//	char *animSetName, *animName, *framePath;
//	float frameWait, spriteScale;
//	bool animLoop;
//
//	//animaciones_ = new AnimSprites(Vec2(directorInstance->getWinSize().width / 2.0f, directorInstance->getWinSize().height / 2.0f));
//	// TODO: estoy inicializando la posicion de las animaciones en la carga del Xml cuando no tengo ni idea de dónde ponerlas. Tengo que dearle otra vuelta
//	animacionesTestz_ = new AnimSprites(Vec2::ZERO);
//	AnimSprites::frame *tmpFrame;
//	AnimSprites::animation *tmpAnimation;
//
//	// al nivel raíz del xml tengo default_values que uso para la configuración y al mismo nivel varios "animset" que definen un set de animaciones
//	for(pugi::xml_node xmlAnimSet = doc.child("animset"); xmlAnimSet; xmlAnimSet = xmlAnimSet.next_sibling("animset")){
//		// encontrado un <animset>. Dentro debería haber 0..n <anim>s y 0..m <frame>s
//
//		// animSet tiene name y nada más
//		animSetName = (char *)xmlAnimSet.attribute("name").value();
//
//		// dentro debería haber varios anim
//		for(pugi::xml_node xmlAnim = xmlAnimSet.child("anim"); xmlAnim; xmlAnim = xmlAnim.next_sibling("anim")){
//
//			// cada anim de animSet tiene un nombre para la animación, atributos como si es loop o no y la lista de frames
//			animName = (char *)xmlAnim.attribute("name").value();
//			animLoop = xmlAnim.attribute("loop").as_bool();
//
//			// voy creando una estructura en memoria para el juego
//			tmpAnimation = new AnimSprites::animation(animLoop);
//
//			// busco los frames que hay y los asigno
//			for(pugi::xml_node xmlFrame = xmlAnim.child("frame"); xmlFrame; xmlFrame = xmlFrame.next_sibling("frame")) {
//
//				// ruta al archivo del sprite
//				framePath = (char *)xmlFrame.attribute("path").value();
//				frameWait = xmlFrame.attribute("wait").as_float();
//				spriteScale = xmlFrame.attribute("size").as_float();
//
//				tmpFrame = new AnimSprites::frame(parentNode, framePath, frameWait, spriteScale);
//				// addFrame añade el frame a la animación si existe en el set (por nombre), o crea una nueva si no.
//				tmpAnimation->addFrame(tmpFrame);
//			}
//		}
//
//		animacionesTestz_->addAnimation(animName, tmpAnimation);
//	}
//}


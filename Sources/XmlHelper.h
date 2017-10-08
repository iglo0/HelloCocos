#pragma once 


#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

USING_NS_CC;

// TODO: para que sepa qué devolver? 
class AnimSprites;

class XmlHelper{
public:
	XmlHelper(){};
	~XmlHelper(){};

	AnimSprites *loadAnimation(Node *parentNode, const char *animName);

	// TODO: tests, borrar
	//void loadAnimationsTestz(const char *filename, Node *parentNode);

	//AnimSprites *animacionesTestz_;

protected:
	// TODO: moverlo a otro sitio
	const char *xmlFilename_ = "test.xml";
};

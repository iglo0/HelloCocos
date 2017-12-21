#pragma once 

#include <vector>
#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

USING_NS_CC;

// TODO: para que sepa qué devolver? 
class AnimSprites;
class GameActor;
class Bullet;
class Enemy;
class SpaceInvaders;

class XmlHelper{
public:
	XmlHelper(){};
	~XmlHelper(){};

	AnimSprites *loadAnimation(Node *parentNode, const char *animName, GameActor *gameActor = nullptr);
	Bullet *loadBullet(Node *parentNode, const char *xmlBulletDef, const char *bulletName="");
	Enemy *loadEnemy(Node *parentNode, const char *xmlEnemyDef);
	// las oleadas no vienen definidas una a una sino todo un bloque
	//SpaceInvaders *loadInvaders(Node *parentNode, const char *xmlDef);
	std::vector<SpaceInvaders *> loadInvaderLevels(Node *parentNode, const char *xmlDefName);

	void assignPhysicsToAnimation(AnimSprites *anim, GameActor *gA, int tipoColision, int colisionaCon);

	// TODO: tests, borrar
	//void loadAnimationsTestz(const char *filename, Node *parentNode);

	//AnimSprites *animacionesTestz_;

protected:
	// TODO: moverlo a otro sitio
	const char *xmlFilename_ = "test.xml";
};

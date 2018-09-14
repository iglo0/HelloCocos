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
class HiScoresScene;
class HiScores;

class XmlHelper{
public:
	XmlHelper(){};
	~XmlHelper(){};

	AnimSprites *loadAnimation(Node *parentNode, const char *animName, GameActor *gameActor = nullptr);
	Bullet *loadBullet(Node *parentNode, const char *xmlBulletDef, const char *bulletName="");
	Enemy *loadEnemy(Node *parentNode, const char *xmlEnemyDef);
	// las oleadas no vienen definidas una a una sino todo un bloque
	//std::vector<SpaceInvaders *> loadInvaderLevels(Node *parentNode, const char *xmlDefName);
	std::unordered_map<int, SpaceInvaders *> loadInvaderLevels(Node *parentNode, const char *xmlDefName);

	void assignPhysicsToAnimation(AnimSprites *anim, GameActor *gA, int tipoColision, int colisionaCon);

	// TODO: tests, borrar
	//void loadAnimationsTestz(const char *filename, Node *parentNode);
	void loadHiScores(const char *filename, HiScores *scores);
	void saveHiScores(const char *filename, HiScores *scores);
	//AnimSprites *animacionesTestz_;

protected:
	// TODO: moverlo a otro sitio
	const char *xmlFilename_ = "test.xml";
	const char *xmlRecords_ = "records.xml";
};

#pragma once

#include "cocos2d.h"
#include "GameActor.h"
#include "Bullet.h"
#include "Pool.h"

// --------------------------
//#include "Weapon.h"
// --------------------------

// TODO: No puedo incluir InputComponent en Player y Player en InputComponent... soluciones?
//#include "InputComponent.h"

// TODO: No se si tiene sentido "colgar" InputComponent de Player o dejarlo como clase externa y controlarlo desde el juego

USING_NS_CC;

// Solución1: Forward declaration? 
class InputComponent;

class Player : public GameActor{
public:
	Player(Node *nodo, float playerSpeed);
	~Player();

	void update(float deltaT);
	virtual void impacto(float dmg) override;
	bool disparar;

	InputComponent *inputComponent;

	//Weapon *currentWeapon;
	std::vector<Bullet *> *poolMisBalas;

private:
	//const char *pathSprite = PLAYER_PATH_SPRITE;
	float playerInitialScale = 0.5f;
	float playerInitialRotation = 0;
	int playerInitialZOrder = 0;
};


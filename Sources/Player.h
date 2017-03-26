#pragma once

#include "cocos2d.h"
#include "GameActor.h"
// --------------------------
#include "Testz.h"
// --------------------------

// TODO: No puedo incluir InputComponent en Player y Player en InputComponent... soluciones?
//#include "InputComponent.h"

// TODO: No se si tiene sentido "colgar" InputComponent de Player o dejarlo como clase externa y controlarlo desde el juego

USING_NS_CC;

// Solución1: Forward declaration? 
class InputComponent;

class Player : public GameActor{
public:
	Player(Node *nodo);
	~Player();

	Sprite *setSprite(Node *nodo);
	void dispara();
	void update(float deltaT);

	bool disparar;

	InputComponent *inputComponent;

	Weapon *currentWeapon;

private:
	const char *pathSprite = "spaceshipspr.png";
	float playerInitialScale = 0.5f;
	float playerInitialRotation = 0;
	int playerInitialZOrder = 0;
	//float playerSpeed = 100.0f;
	float movementSpeed = 300.0f;
	
};


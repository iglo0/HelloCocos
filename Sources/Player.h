#pragma once

//#include "cocos2d.h"
#include "GameActor.h"
//#include "Bullet.h"
//#include "Pool.h"

// --------------------------
//#include "Weapon.h"
// --------------------------

// TODO: No puedo incluir InputComponent en Player y Player en InputComponent... soluciones?
//#include "InputComponent.h"

// TODO: No se si tiene sentido "colgar" InputComponent de Player o dejarlo como clase externa y controlarlo desde el juego

//USING_NS_CC;

// Solución1: Forward declaration? 
class InputComponent;
class Bullet;

class Player : public GameActor{
public:
	Player(Node *nodo, float playerSpeed);
	~Player();

	void update(float deltaT);
	virtual void impacto(float dmg) override;
	void killPlayer();
	void activatePlayerInInitialPos();

	bool disparar;
	InputComponent *inputComponent;
	// En Pool::xxx están todos los pools, aquí guardo el pool concreto que dispara el prota ahora. Supongo que así es más fácil cambiar de arma más adelante
	std::vector<Bullet *> *poolMisBalas;

	// TODO: una prueba para tener situado al player en todo momento (por ejemplo para dispararle con mala uva)
	static Vec2 getCurrentPlayerPosition();

private:
	//const char *pathSprite = PLAYER_PATH_SPRITE;
	float playerInitialScale = 0.5f;
	float playerInitialRotation = 0;
	int playerInitialZOrder = 0;

	static Vec2 playerPosition;
};


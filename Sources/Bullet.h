#pragma once

#include <vector>	// vector ya incluido a traves de GameActor <- Game
#include "GameActor.h"

//#include "cocos2d.h"
//#include "SimpleAudioEngine.h"
//#include "AudioEngine.h"
//#include "Game.h"

//USING_NS_CC;

class Bullet : public GameActor{
public:
	// name for debug purposes
	Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialSize = 1.0f);
	~Bullet();

	// Bala tiene un mueve especial que solo usa la velocidad (de GameActor) para ir arriba o abajo
	void mueve () override;	
	// con override dejo claro al compilador que quiero implementar el m�todo mueve de la clase base. No es 
	// estrictamente necesario, pero as� se que comprueba que las firmas sean id�nticas y que en general 
	// haga lo que creo que estoy haciendo.

	// m�todo est�tico que crea un "pool" de Bullets, mientras el resto de m�todos tratan con una instancia concreta
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char * name, const char * pathSprite, const char * pathSonidoDisparo, const char * pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale = 1.0f);
	
	void impacto(float dmg) override;

	float bulletDmg;

private:
	//bool createBullet(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon);

	//float bulletSpeed; // para eso, uso Base::gameActorSpeed
};


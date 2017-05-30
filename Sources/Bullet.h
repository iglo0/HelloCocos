#pragma once

#include "GameActor.h"
#include <vector>	// vector ya incluido a traves de GameActor <- Game

class Bullet : public GameActor{
public:
	enum tiposBala {tipoPlayer, tipoEnemy, tipoBoss};

	// name for debug purposes
	Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialSize = 1.0f);
	~Bullet();

	// Bala tiene un mueve especial que solo usa la velocidad (de GameActor) para ir arriba o abajo
	void mueve () override;	
	// con override dejo claro al compilador que quiero implementar el método mueve de la clase base. No es 
	// estrictamente necesario, pero así se que comprueba que las firmas sean idénticas y que en general 
	// haga lo que creo que estoy haciendo.

	// métodos estáticos que crean instancias
	static Bullet *creaBala(Node *nodo, tiposBala tipoBala, const char *bulletName);
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale = 1.0f);
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, tiposBala tipoBala);

	void impacto(float dmg) override;

	float bulletDmg;

private:
	//bool createBullet(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon);

	//float bulletSpeed; // para esto, uso Base::gameActorSpeed
};


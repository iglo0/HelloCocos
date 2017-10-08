#pragma once

//#include "cocos2d.h"
#include "GameActor.h"
#include <vector>

// para poder disparar
// Declaración adelantada. No necesito usar "Bullet", solo saber que usaré punteros a ese tipo, por tanto basta con declararla aquí.
// OJO: No necesito usar "Bullet", solo saber que usaré punteros a ese tipo, por tanto basta con declararla aquí.
//#include "Bullet.h"
class Bullet;

//USING_NS_CC;

class Enemy : public GameActor{
public:
	enum tiposEnemigo{ tipo1, tipo2, tipoBoss };
	// es menos engorroso así. Puedo usar funcionMovimiento en vez de void(...:*...)(...)... etc
	typedef void(Enemy::*funcionControlEnemigo)(float);

	// OJO: Es mejor evitar el constructor y destructor (usarlos solo para inicializar variables por ejemplo) y la inicialización sacarla fuera. Por ejemplo:
	//Enemy();
	//bool Init(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp);
	//bool Init(Node *nodo, enum tiposEnemigo);
	//void End();
	Enemy(tiposEnemigo tipo);
	~Enemy();

	//Enemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, Movimiento *muevemueve = nullptr);
	//Enemy(Node *nodo, enum tiposEnemigo);

	void initEnemy(Node *nodo);
	void impacto(float) override;
	void update(float deltaT) override;

	// ---------------------------------------
	// TEST!
	// ---------------------------------------
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	// todo: sobrecargarlo desde GameActor???
	void dispara();
	//void disparaN(int n);

	// ---------------------------------------------------------------------------------------------------------
	// funciones de control
	// TODO: ¿convertir en clases? Pros: cada clase contiene sus métodos y variables || Cons: ???
	void funControlFireAtInterval(float interval);
	void funControlFireRandom(float wat);
	// TODO: Parámetros de control
	funcionControlEnemigo funcionControlActual_;
	float funcionControlTiempoDisparo_;
	int funcionControlProbDisparoAleatoria_;	// 1..x. Dispara en 1, cuanto más alta menos probabilidad por frame.
	// ---------------------------------------------------------------------------------------------------------

	// a ve, parámetros de las balas que voy a disparar...
	// TODO: Acabo de sacar la clase Weapon y ya veo que me va a hacer falta de nuevo :D

	// TODO: ¿con qué dispara un enemigo?
	// esto apuntará a un pool en algún otro sitio
	std::vector<Bullet *> *poolMisBalas_;
	// TODO: ¿Y para un enemigo con más tipos de ataques?


	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// ---------------------------------------
	// ---------------------------------------

private:
	float tIniDisparo_;
	int pointsOnDeath_;
	// TODO: guardo mi tipo por si lo quiero para algo luego
	tiposEnemigo tipoEnemigo_;
	//void createEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points = Game::getInstance()->enemy_generic_points);
	//void createEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas=nullptr);
	void initEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas = nullptr);
	//void liberaVectorBalas(std::vector<Bullet *> bs);
	//std::vector<Bullet *> reservaVectorBalas(int n);
};

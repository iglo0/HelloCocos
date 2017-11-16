#pragma once

//#include "cocos2d.h"
#include "GameActor.h"
#include <vector>

// para poder disparar
// Declaraci�n adelantada. No necesito usar "Bullet", solo saber que usar� punteros a ese tipo, por tanto basta con declararla aqu�.
// OJO: No necesito usar "Bullet", solo saber que usar� punteros a ese tipo, por tanto basta con declararla aqu�.
class Bullet;

//USING_NS_CC;

class Enemy : public GameActor{
public:
	enum tiposEnemigo{ tipo1, tipo2, tipoBoss };
	// es menos engorroso as�. Puedo usar funcionMovimiento en vez de void(...:*...)(...)... etc
	typedef void(Enemy::*funcionControlEnemigo)(float);

	// OJO: Es mejor evitar el constructor y destructor (usarlos solo para inicializar variables por ejemplo) y la inicializaci�n sacarla fuera. Por ejemplo:
	Enemy();
	Enemy(tiposEnemigo tipo);
	~Enemy();

	void initEnemy(Node *nodo);
	static Enemy *createEnemy(Node *nodo, const char *enemyDefName);
	void impacto(float) override;
	void update(float deltaT) override;
	void setPoints(int);
	void setSonidoDispara(const char *);
	void setSonidoMuerte(const char *);

	// todo: sobrecargarlo desde GameActor???
	void dispara();
	//void disparaN(int n);

	// ---------------------------------------------------------------------------------------------------------
	// funciones de control
	// TODO: �convertir en clases? Pros: cada clase contiene sus m�todos y variables || Cons: ???
	void funControlFireAtInterval(float interval);
	void funControlFireRandom(float wat);
	// TODO: Par�metros de control
	funcionControlEnemigo funcionControlActual_;
	float funcionControlTiempoDisparo_;
	int funcionControlProbDisparoAleatoria_;	// 1..x. Dispara en 1, cuanto m�s alta menos probabilidad por frame.
	// ---------------------------------------------------------------------------------------------------------

	// �con qu� dispara un enemigo? esto apuntar� a un pool en otro sitio
	std::vector<Bullet *> *poolMisBalas_;
	// TODO: �Y para un enemigo con m�s tipos de ataques?


private:
	float tIniDisparo_;
	int pointsOnDeath_;	// ojo, tengo points tambi�n en GameActor... �redundante? -> YES, corregido
	// TODO: guardo mi tipo por si lo quiero para algo luego
	tiposEnemigo tipoEnemigo_;
	//void createEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points = Game::getInstance()->enemy_generic_points);
	//void createEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas=nullptr);
	void initEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas = nullptr);
	//void liberaVectorBalas(std::vector<Bullet *> bs);
	//std::vector<Bullet *> reservaVectorBalas(int n);
	std::string sonidoDispara_;
	std::string sonidoMuerte_;
};

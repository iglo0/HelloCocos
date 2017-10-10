#pragma once

#include "GameActor.h"
#include <vector>	// vector ya incluido a traves de GameActor <- Game

class AnimSprites;

class Bullet : public GameActor{
public:
	enum bulletTypes {tipoPlayer, tipoEnemyNormal, tipoEnemyDirigido, tipoBossHoming};

	// name for debug purposes
	Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialSize = 1.0f);
	~Bullet();

	// métodos estáticos que crean instancias
	static Bullet *creaBala(Node *nodo, bulletTypes tipoBala, const char *bulletName);
	//static Bullet *creaBalaAnimada(Node *nodo, bulletTypes tipoBala, const char *bulletName, const char *animSetName);
	//static Bullet *creaBala(Node *nodo, bulletTypes tipoBala, const char *bulletName, punteroAFuncionMovimiento pFuncionMovimiento, Movimiento *claseMovimiento);
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon, float initialScale = 1.0f);
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, bulletTypes tipoBala);

	// con override dejo claro al compilador que quiero implementar el método mueve de la clase base. No es 
	// estrictamente necesario, pero así se que comprueba que las firmas sean idénticas y que en general 
	// haga lo que creo que estoy haciendo.
	void activa(Vec2 posIni) override;
	void activa(float x, float y) override;
	void impacto(float dmg) override;
	void update(float deltaT) override;

	float bulletDmg;

private:

	// TODO: Probando con balas dirigidas
	float ttl_;
	bulletTypes bulletType_;
};


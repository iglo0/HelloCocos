#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
//#include "Pool.h"

#include "Game.h"
#include "GameActor.h"
#include <vector>

USING_NS_CC;

class Bullet : public GameActor{
public:
	// name for debug purposes
	Bullet(Node *nodo, const char *name, const char *pathSprite, const char *pathSonidoDisparo, const char *pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon);
	~Bullet();

	// Bala tiene un mueve especial que solo usa la velocidad (de GameActor) para ir arriba o abajo
	void mueve () override;	
	// con override dejo claro al compilador que quiero implementar el método mueve de la clase base. No es 
	// estrictamente necesario, pero así se que comprueba que las firmas sean idénticas y que en general 
	// haga lo que creo que estoy haciendo.

	// método estático que crea un "pool" de Bullets, mientras el resto de métodos tratan con una instancia concreta
	static void createBulletPool(Node *nodo, std::vector<Bullet *> &pool, int poolSize, const char * name, const char * pathSprite, const char * pathSonidoDisparo, const char * pathSonidoImpacto, float speed, float dmg, int tipoColision, int colisionoCon);
	
	void impacto(float dmg) override;

	float bulletDmg;

private:
	//bool createBullet(Node *nodo, const char *ruta, const char *name, int tipoColision, int colisionaCon);

	//float bulletSpeed; // para eso, uso Base::gameActorSpeed
};

class BalaOLD {
public:
	BalaOLD(const char *pathSprite);	// bala sin fisica
	BalaOLD(const char *name, const char *pathSprite, int tipoColision, int colisionoCon, float dmg=1.0f);	// bala con física
	~BalaOLD();
	float getDanyoBala();

	enum tiposBala{ balaTipo1, balaTipo2};
	enum sonidosBala { disparo, impacto};

	// mueve la bala segun su velocidad 
	void update();

	bool setSpriteConFisica(const char *name, const char *ruta, int tipoColision, int colisionaCon);

	// carga sprite para este tipo de bala
	bool setSprite(const char *ruta);
	Sprite *getSprite();

	// incializa la variable y precarga el sonido
	void setSonido(sonidosBala tipo, const char *ruta);

	void activar(Vec2 posInicial);
	void desActivar();

	void reproduceSonido(sonidosBala);

	// la velocidad de esta vala
	void setVelocidad(float);


	bool isActive();

private:


	Sprite *sprite;
	//std::string rutaSprite;
	//const char *rutaSprite;
	const char *rutaSonidoDisparo, *rutaSonidoImpacto;
	float velocidadAbs;
	//cocos2d::Size visibleSize;
	tiposBala tipoBala;
	float danyoBala;

	bool active = true;	// si esta bala está activa o está a la espera de ser utilizada

};


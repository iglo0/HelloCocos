#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "..\proj.win32\Bala.h"
#include <vector>

USING_NS_CC;

class Level1 : public cocos2d::Layer
{
public:
	~Level1();

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	//void menuCloseCallback(cocos2d::Ref* pSender);
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// keyboard listeners
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

	// el gameloop
	void update(float delta) override;

	// otras funciones

	//bool mueveBala(Sprite *bala, float cuanto);	// mueve una bala
	void mueveBalas(std::vector<Bala *> pool);	// procesa pools de balas

	//bool mueveBalaEnemiga(Sprite *bala, float cuanto);
	//void mueveBalasEnemigas();

	void mueveProta();
	void creaNaveProta();
	void creaEnemigos();
	void mueveEnemigos(float cuanto);
	void mueveEnemigo(Sprite *enemigo, float cuanto);
	void enemigoDispara(Sprite *enemigo);

	void creaPoolBalas(std::vector<Bala *> *pool, int cant, std::string pathSpriteBala, const char *pathSonidoBala, float size);

private:
	Sprite *protaSprite,*balaSprite;
	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	bool dispara;
	bool sale;	// salir de la pantalla
	float protaSpeed = 500.0f, balaSpeed = 1000.0f, balaEnemigaSpeed = -750.0f, enemigoSpeed = 100.0f;
	float deltaT;
	float tiempoTranscurrido;

	Size visibleSize;
	Vec2 origin;

	// hiyaaa
	// primera version: a lo bruto
	// TODO: podría hacer una estructura donde cada elemento tenga el sprite, la funcion que lo controla, etc?
	// o mejor, encapsularlo en una clase
	//Vector<Sprite *> balas;
	Vector<Sprite *> balasEnemigas;
	Vector<Sprite *> enemigos;

	// pruebas audio
	CocosDenshion::SimpleAudioEngine *audio;

	// pruebas
	std::vector<Bala *> poolBalas;	// mis disparos
	std::vector<Bala *> poolBalasGordas;	// disparos del jefe
	std::vector<Bala *> poolBalasEnemigas;	// disparos de los aliens normales
};

#endif // __LEVEL1_SCENE_H__

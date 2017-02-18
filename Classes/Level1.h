#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

class Level1 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// keyboard listeners
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

	// el gameloop
	void update(float delta) override;

	// otras funciones

	// TODO: no me gusta mucho pasarles el delta cada frame ¿?
	bool mueveBala(Sprite *bala, float cuanto);	// mueve una bala
	void mueveBalas(float cuanto);				// procesa el array de balas

	bool mueveBalaEnemiga(Sprite *bala, float cuanto);
	void mueveBalasEnemigas(float cuanto);

	void creaEnemigos();
	void mueveEnemigos(float cuanto);
	void mueveEnemigo(Sprite *enemigo, float cuanto);
	void enemigoDispara(Sprite *enemigo);


private:
	Sprite *protaSprite,*balaSprite;
	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	bool dispara;
	bool sale;	// salir de la pantalla
	float protaSpeed = 500.0f, balaSpeed = 1000.0f, balaEnemigaSpeed = 500.0f, enemigoSpeed = 100.0f;
	float deltaT;
	float tiempoTranscurrido;

	Size visibleSize;
	Vec2 origin;

	// hiyaaa
	// primera version: a lo bruto
	// TODO: podría hacer una estructura donde cada elemento tenga el sprite, la funcion que lo controla, etc?
	// o mejor, encapsularlo en una clase
	Vector<Sprite *> balas;
	Vector<Sprite *> balasEnemigas;
	Vector<Sprite *> enemigos;

	// pruebas audio
	CocosDenshion::SimpleAudioEngine *audio;
};

#endif // __HELLOWORLD_SCENE_H__

/* ************************************************************************
Howto display sprite bounding boxes:
Open ccConfig.h file in cocos2d folder of your project and change

#define CC_SPRITE_DEBUG_DRAW 0
line to
#define CC_SPRITE_DEBUG_DRAW 1
*************************************************************************/

#pragma once

#include "cocos2d.h"
#include <vector>

USING_NS_CC;// using namespace cocos2d

class PolyspriteTest : public cocos2d::Layer{
public:

#pragma region funciones de inicializacion y auxiliares de cocos
	static cocos2d::Scene *createScene();

	virtual bool init();

	// preparo el metodo update
	void update(float deltaT);

	// a selector callback
	// OJO!
	// Según cuantos parámetros tenga, se llama con CC_CALLBACK_0, CC_CALLBACK_1, CC_CALLBACK_2... 
	void MenuVuelveCallback(cocos2d::Ref *pSender);
	//void MenuJugarCallback(cocos2d::Ref *pSender);
	//void MenuKeyTestCallback(cocos2d::Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(PolyspriteTest);
#pragma endregion

	// mis cosas
	void megustaElMueveMueve(Sprite *, Vec2 &cant);

	// physicscontact test?
	bool onContactBegin(PhysicsContact &contactoConTacto);

	//Sprite *sprite1, *sprite2;
	//Vec2 sprite1Accel, sprite2Accel;

	struct reboton{
		Sprite *sprite;
		Vec2 accel;
	};

	std::vector<struct reboton> rebotones;
};


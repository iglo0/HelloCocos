#pragma once 

/* Animaciones que creo que necesitaré...
Aliens		-> ciclo de animación fijo
			-> [opcional] Anim al disparar
Balas		-> ciclo fijo
Ovni		-> ciclo fijo
			-> [opcional] Anim al disparar
Explosiones	-> ciclo fijo (con fin)
Nave		-> ciclo fijo?
Casas		-> nada

El sistema de animaciones necesitaría:
- ciclos repetitivos
- ciclos con fin (explosión)
- ciclos con fin que salten a otros ciclos

*/

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

USING_NS_CC;

#pragma region AnimSprites

class AnimSprites{
public:
	AnimSprites(Node *parent);
	~AnimSprites();

	struct frame{
		frame(Sprite *sprite, float delaySeconds, bool isLastFrame, std::string nextAnimName) : sprite_(sprite), delaySeconds_(delaySeconds), isLastFrame_(isLastFrame), nextAnimationName_(nextAnimName){}

		Sprite *sprite_;
		float delaySeconds_;
		bool isLastFrame_;	// indica que la animación acaba en este frame, y saltará a la siguiente o acabará
		std::string nextAnimationName_;
	};


	//void init(std::string animName, std::vector<frame *> frames);
	void addFrame(std::string animName, frame *f);
	void playStart(std::string animName);
	void update(float deltaT);

	void setPosition(Vec2 pos){ parent_->setPosition(pos); }
	Vec2 getPosition(){ return parent_->getPosition(); }

private:

	// TODO: la idea es animations_["attack"] = frames para "attack"
	std::unordered_map<std::string, std::vector<frame *>> animations_;
	Node *parent_;

	int currFrame_, lastFrame_;
	float currFrameTIni_, currFrameTEnd_;
	std::string currAnimName_;
	std::vector<frame *> *currAnimation_;

	void playFrame(int numFrame);
	void hideFrame(frame *f);
	void showFrame(frame *f);

};

#pragma endregion

#pragma region AnimTest

class AnimTest : public cocos2d::Layer
{
public:
	// funciones miembro públicas
	// --------------------------------------------------------------

	~AnimTest();
	
	#pragma region fluff
    static cocos2d::Scene* createScene();
    virtual bool init();

	// keyboard input listeners
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // a selector callback
	void menuVuelveCallback(cocos2d::Ref* pSender);
	void vuelveAlMenu();

	// implement the "static create()" method manually
	CREATE_FUNC(AnimTest);
	#pragma endregion
	
	// el gameloop
	void update(float delta) override;

	void mueveNodo();

private:
	void carga(const char *);
	void miInit();
	//void mueveNodo();

	bool mueveArr_, mueveAbj_, mueveIzq_, mueveDch_;

	//Node *nodo_;
	//std::vector<AnimSprites *> frames_;
	//size_t currFrame_;
	//size_t numFrames_;

	AnimSprites *animaciones_;
};

#pragma endregion
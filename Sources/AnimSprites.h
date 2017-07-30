#pragma once 

/* Animaciones que creo que necesitar�...
Aliens		-> ciclo de animaci�n fijo
			-> [opcional] Anim al disparar
Balas		-> ciclo fijo
Ovni		-> ciclo fijo
			-> [opcional] Anim al disparar
Explosiones	-> ciclo fijo (con fin)
Nave		-> ciclo fijo?
Casas		-> nada

El sistema de animaciones necesitar�a:
- ciclos repetitivos
- ciclos con fin (explosi�n)
- ciclos con fin que salten a otros ciclos

*/

#include "cocos2d.h"	// necesario para declarar CREATE_FUNC y crear la escena en general

USING_NS_CC;

class AnimSprites{
public:
	AnimSprites(Node *parent);
	~AnimSprites();

	struct frame{
		frame(Sprite *sprite, float delaySeconds, bool isLastFrame, std::string nextAnimName) : sprite_(sprite), delaySeconds_(delaySeconds), isLastFrame_(isLastFrame), nextAnimationName_(nextAnimName){}

		Sprite *sprite_;
		float delaySeconds_;
		bool isLastFrame_;	// indica que la animaci�n acaba en este frame, y saltar� a la siguiente o acabar�
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

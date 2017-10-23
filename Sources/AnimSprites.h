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

class GameActor;

USING_NS_CC;

// Clase que contiene todas las animaciones de un solo objeto
// Normalmente contendrá n sprites pero solo uno será visible
class AnimSprites{
public:
	//AnimSprites(Vec2 iniPos = Vec2::ZERO);
	// OJO: AnimSprites pasa a controlar el sprite del GameActor que lo contiene
	AnimSprites(GameActor *parent);
	~AnimSprites();

	struct frame{
		frame(Node *nodo, const char *spritePath, float displaySeconds, float spriteScale=1.0f);

		Sprite *sprite_;
		float displaySeconds_;
	};

	struct animation{
		animation(bool loop) : loop_(loop){}
		~animation();

		void addFrame(frame *f);

		bool loop_;
		std::vector<frame *> animationFrames_;
	};

	void addAnimation(std::string animName, animation *a);

	void playStart(std::string animName, Vec2 pos, bool randomStart=false);
	void update(float deltaT);
	//void linkTo(Sprite *);

	void setPosition(Vec2 pos);
	Vec2 getPosition();

	void desactiva();

	std::unordered_map<std::string, animation *> animations_;


private:

	void startAnimation(animation *a, Vec2 pos, bool randomStart=false);
	void playNextFrame();

	// TODO: la idea es animations_["attack"] = frames para "attack"
	animation *currentAnimation_;

	//Node *parent_;	// no puedo acceder desde frame o animation a parent_ ¿?
	frame *currentFrame_;// , *lastFrame_;
	int currentFrameNum_, lastFrameNum_;
	float currFrameTIni_, currFrameTEnd_;
	//void playFrame(int numFrame);
	void hideFrame(frame *f);
	void showFrame(frame *f, Vec2 donde);

	bool estaActivo_;

	//Vec2 position_;
	//Sprite *parentSprite_;
	GameActor *parent_;
};

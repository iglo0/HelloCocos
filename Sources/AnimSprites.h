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

class GameActor;

USING_NS_CC;

// Clase que contiene todas las animaciones de un solo objeto
// Normalmente contendr� n sprites pero solo uno ser� visible
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
		animation(bool loop, bool fixedImage);// : loop_(loop){}
		~animation();

		void addFrame(frame *f);

		bool loop_, fixedImage_;	// fixedImage_ = sustituye a un sprite, la imagen no cambia una vez mostrada
		std::vector<frame *> animationFrames_;
	};

	std::unordered_map<std::string, animation *> animations_;

	void addAnimation(std::string animName, animation *a);
	void playStart(std::string animName, Vec2 pos, bool randomStart=false);
	void update(float deltaT);
	void setPosition(Vec2 pos);
	Vec2 getPosition();
	void desactiva();

private:

	void startAnimation(animation *a, Vec2 pos, bool randomStart=false);
	void playNextFrame();
	void hideFrame(frame *f);
	void showFrame(frame *f, Vec2 donde);

	// TODO: la idea es animations_["attack"] = frames para "attack"
	animation *currentAnimation_;
	frame *currentFrame_;// , *lastFrame_;
	int currentFrameNum_, lastFrameNum_;
	float currFrameTIni_, currFrameTEnd_;
	bool estaActivo_;
	GameActor *parent_;
};

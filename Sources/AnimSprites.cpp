#include "AnimSprites.h"
//#include "GameActor.h"
#include "Game.h"

// C++11 -> std::this_thread::sleep_for(std::chrono::milliseconds(x));
//#include <chrono>
//#include <thread>

AnimSprites::AnimSprites(Node *parent) {
	currentAnimation_ = nullptr;
	currentFrame_ = nullptr;
	estaActivo_ = false;
}

AnimSprites::~AnimSprites(){
	// TODO: destruir:
	//std::unordered_map<std::string, animation *> animations_;
	animations_.erase(animations_.begin(), animations_.end());
}

AnimSprites::animation::~animation(){
	// TODO: destruir los animation frames
	for(auto af = animationFrames_.cbegin(); af != animationFrames_.cend();++af){
		auto *tmp = *af;
		if(tmp->sprite_){
			tmp->sprite_->removeFromParent();
		}
	}
	animationFrames_.erase(animationFrames_.begin(), animationFrames_.end());
}

void AnimSprites::animation::addFrame(frame * f){
	animationFrames_.push_back(f);
}

AnimSprites::frame::frame(Node *parent, const char *spritePath, float displaySeconds, float spriteScale){
	// TODO: no har�a falta un physicsbody?
	displaySeconds_ = displaySeconds;
	sprite_ = Sprite::createWithSpriteFrameName(spritePath);
	sprite_->setScale(spriteScale);
	sprite_->setVisible(false);
	// TODO: temp para almacenar el fichero de la textura y comprobar que las animaciones van en orden
	sprite_->setName(std::string(spritePath));
	
	//parent_->getSprite()->getParent()->addChild(sprite_);
	parent->addChild(sprite_);

	// a�ade f�sica
	// TODO: la f�sica est� unida a la clase padre de este componente, voy a dejarlo pasar de momento y centrarme en animaciones
	/* lo que viene faltando:

	Game::anadeFisica(sprite_, tipoColision, colisionaCon, spritePath, ruta);

	//TODO: ya tengo para recuperar mis datos :)
	sprite_->setUserData(this);
	// y su tipo
	sprite_->setTag(tipoColision);

	// lo creo invisible y sin colisiones activas
	desactiva();

	*/
}


void AnimSprites::addAnimation(std::string animName, animation *a){
	// TODO: comprobar si existe ya una y borrarla?

	// si existe la rellena y si no, la crea
	animations_[animName] = a;
}

Vec2 AnimSprites::getPosition(){
	if(currentFrame_){
		return currentFrame_->sprite_->getPosition();
	} else{
		CCLOG("opalee");
		return Vec2::ZERO;
	}
}

void AnimSprites::setPosition(Vec2 pos){
	currentFrame_->sprite_->setPosition(pos);
}

void AnimSprites::hideFrame(frame *f){
	PhysicsBody *p;
	if(!f){
		return;
	}
	if(!f->sprite_){
		return;
	}

	f->sprite_->setVisible(false);
	p = f->sprite_->getPhysicsBody();
	if(p){
		p->setEnabled(false);
	}
}

void AnimSprites::showFrame(frame *f){
	PhysicsBody *p;

	CCLOG("Showing frame %s", f->sprite_->getName().c_str());

	currentFrame_ = f;
	currFrameTIni_ = Game::getInstance()->ellapsedTime;
	currFrameTEnd_ = currFrameTIni_ + f->displaySeconds_;

	//f->sprite_->setPosition(position_);
	f->sprite_->setVisible(true);
	p = f->sprite_->getPhysicsBody();
	if(p){
		p->setEnabled(true);
	}

}

void AnimSprites::playStart(std::string animName, bool randomStart){
	startAnimation(animations_[animName], randomStart);
}

void AnimSprites::playNextFrame(){
	// TODO: Hacer m�s caso al loop
	if(currentFrameNum_ < lastFrameNum_){
		++currentFrameNum_;
		hideFrame(currentFrame_);
	} else if(currentAnimation_->loop_){
		hideFrame(currentFrame_);
		currentFrameNum_ = 0;
	} else{
		// finalizar la animacion
		// currFrameNum_ = currFrameNum_;
	}
	
	showFrame(currentAnimation_->animationFrames_[currentFrameNum_]);
	
	//CCLOG("frame [%d] <%s>", currentFrameNum_, currentFrame_->sprite_->getName().c_str());
}

void AnimSprites::startAnimation(animation *a, bool randomStart){
	currentAnimation_ = a;
	estaActivo_ = true;

	if(currentFrame_){
		hideFrame(currentFrame_);
	}

	lastFrameNum_ = a->animationFrames_.size() - 1;

	if(randomStart){
		currentFrameNum_ = RandomHelper::random_int<int>(0, lastFrameNum_);
	} else{
		currentFrameNum_ = 0;
	}

	showFrame(a->animationFrames_[currentFrameNum_]);

}

void AnimSprites::update(float deltaT){
	if(!estaActivo_){
		return;
	}

	if(Game::getInstance()->ellapsedTime >= currFrameTEnd_){
		// cambio de frame
		playNextFrame();
	}

}

void AnimSprites::desactiva(){
	estaActivo_ = false;
	hideFrame(currentFrame_);
}
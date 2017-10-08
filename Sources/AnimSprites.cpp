#include "AnimSprites.h"
#include "Game.h"

// C++11 -> std::this_thread::sleep_for(std::chrono::milliseconds(x));
//#include <chrono>
//#include <thread>

AnimSprites::AnimSprites(Vec2 posIni) : position_(posIni) {
	currentAnimation_ = nullptr;
	currentFrame_ = nullptr;
}

AnimSprites::~AnimSprites(){}

AnimSprites::frame::frame(Node *parent, const char *spritePath, float displaySeconds, float spriteScale){
	displaySeconds_ = displaySeconds;
	sprite_ = Sprite::createWithSpriteFrameName(spritePath);
	sprite_->setScale(spriteScale);
	sprite_->setVisible(false);
	parent->addChild(sprite_);
}

void AnimSprites::animation::addFrame(frame * f){
	animationFrames_.push_back(f);
}

void AnimSprites::addAnimation(std::string animName, animation *a){
	// TODO: comprobar si existe ya una y borrarla?

	// si existe la rellena y si no, la crea
	animations_[animName] = a;
}

Vec2 AnimSprites::getPosition(){
	return position_;
}

void AnimSprites::setPosition(Vec2 pos){
	position_ = pos;
}

void AnimSprites::hideFrame(frame *f){
	PhysicsBody *p;

	f->sprite_->setVisible(false);
	p = f->sprite_->getPhysicsBody();
	if(p){
		p->setEnabled(false);
	}
}

void AnimSprites::showFrame(frame *f){
	PhysicsBody *p;

	currentFrame_ = f;
	currFrameTIni_ = Game::getInstance()->ellapsedTime;
	currFrameTEnd_ = currFrameTIni_ + f->displaySeconds_;

	f->sprite_->setPosition(position_);
	f->sprite_->setVisible(true);
	p = f->sprite_->getPhysicsBody();
	if(p){
		p->setEnabled(true);
	}
}

void AnimSprites::playStart(std::string animName){
	initAnimation(animations_[animName]);
}

void AnimSprites::playNextFrame(){
	// TODO: Hacer más caso al loop
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
}

void AnimSprites::initAnimation(animation *a){
	currentAnimation_ = a;

	if(currentFrame_){
		hideFrame(currentFrame_);
	}

	currentFrameNum_ = 0;
	lastFrameNum_ = a->animationFrames_.size()-1;
	//currentFrame_ = a->animationFrames_[currentFrameNum_];
	//lastFrame_ = currentFrame_;

	showFrame(a->animationFrames_[currentFrameNum_]);

}

void AnimSprites::update(float deltaT){

	if(Game::getInstance()->ellapsedTime >= currFrameTEnd_){
		// cambio de frame
		playNextFrame();
	}

	if(currentFrame_){
		currentFrame_->sprite_->setPosition(position_);
	}

}


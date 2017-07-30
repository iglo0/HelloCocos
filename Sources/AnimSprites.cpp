#include "AnimSprites.h"
#include "Game.h"

// C++11 -> std::this_thread::sleep_for(std::chrono::milliseconds(x));
//#include <chrono>
//#include <thread>

AnimSprites::AnimSprites(Node *parent): parent_(parent){}

AnimSprites::~AnimSprites(){}

void AnimSprites::addFrame(std::string animName, AnimSprites::frame *f){

	// me aseguro que está desactivado
	hideFrame(f);
	//showFrame(f);

	// si existe key la añade, si no la crea. MOOOLA!
	animations_[animName].push_back(f);

	parent_->addChild(f->sprite_);
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

	f->sprite_->setVisible(true);
	p = f->sprite_->getPhysicsBody();
	if(p){
		p->setEnabled(true);
	}
}

void AnimSprites::update(float deltaT){
	//std::vector<frame *> currAnim = animations_[currAnimName_];
	frame *f;

	if(Game::getInstance()->ellapsedTime >= currFrameTEnd_){
		f = (*currAnimation_)[currFrame_];

		// oculto este frame
		hideFrame(f);
		if(!f->isLastFrame_){
			// sigo con el loop
			// lastframe empieza en 0 también
			if(++currFrame_ > lastFrame_){
				currFrame_ = 0;
			}

			playFrame(currFrame_);
		}

	}
}

void AnimSprites::playStart(std::string animName){
	//std::vector<frame *> tmp = animations_[animName];
	currAnimation_ = &animations_[animName];

	if(currAnimation_->size() > 0){
		lastFrame_ = currAnimation_->size() - 1;
	} else{
		lastFrame_ = 0;
	}
	currAnimName_ = animName;

	playFrame(0);	// asigna currFrame_
}

void AnimSprites::playFrame(int numFrame){
	frame *f;

	currFrame_ = numFrame;

	f = (*currAnimation_)[currFrame_];

	currFrameTIni_ = Game::getInstance()->ellapsedTime;
	currFrameTEnd_ = currFrameTIni_ + f->delaySeconds_;

	showFrame(f);
}

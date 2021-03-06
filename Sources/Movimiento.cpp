#include "Movimiento.h"
#include "Game.h"

#include <math.h>
#include "SpaceInvaders.h"


#pragma region Base

Movimiento::Movimiento(){}
Movimiento::~Movimiento(){}

#pragma endregion

#pragma region MueveVcal

MueveVcal::MueveVcal(float speed) : _speed(speed){
	_direction = Vec2(0, speed);
}
MueveVcal::~MueveVcal(){};

Vec2 MueveVcal::mueve(Vec2 posActual, float deltaT){
	//float deltaT = Director::getInstance()->getDeltaTime();

	Vec2 tmp = _direction * deltaT;

	return posActual + tmp;
}

#pragma endregion

#pragma region MueveDireccion

MueveDireccion::MueveDireccion(float speed): _speed(speed) {}
MueveDireccion::~MueveDireccion(){}

void MueveDireccion::init(Vec2 ori, Vec2 dest){
	_direction = ori - dest;
	// que no varie la velocidad seg�n la distancia
	_direction.normalize();
}

Vec2 MueveDireccion::mueve(Vec2 posActual, float deltaT){
	//float deltaT = Director::getInstance()->getDeltaTime();
	Vec2 deltaXY;
	Vec2 tmp;

	//deltaXY = (_direction - _origin) * deltaT * _speed;
	deltaXY = _direction * deltaT * _speed;
	tmp = posActual + deltaXY;
	
	return tmp;
}

#pragma endregion

#pragma region MueveHoming

MueveHoming::MueveHoming(float spd) : speed_(spd) {}
MueveHoming::~MueveHoming(){}

void MueveHoming::init(float spd, Sprite *target){
	speed_ = spd;
	//_amplitude = amp;
	//_origin = ori;
	//_direction = dir;
	//_posInicial = posIni;
	target_ = target;

}

Vec2 MueveHoming::mueve(Vec2 posActual, float deltaT){
	//float deltaT = Director::getInstance()->getDeltaTime();
	//Vec2 deltaXY;
	Vec2 tmp;

	deltaXY = posActual - target_->getPosition();
	deltaXY.normalize();	// para que no dependa de la distancia entre el enemigo y el jugador
	deltaXY *= deltaT * speed_;

	tmp = posActual + deltaXY;

	return tmp;
}

#pragma endregion

#pragma region MueveSeno

MueveSeno::MueveSeno(){ 
	amplitude_ = 0; 
	posInicial_ = Vec2::ZERO;
}

MueveSeno::~MueveSeno(){}

void MueveSeno::init(float amplitude, Vec2 posInicial, float speedFactor){
	amplitude_ = amplitude;
	posInicial_ = posInicial;
	speedFactor_ = speedFactor;
}

Vec2 MueveSeno::mueve(Vec2 posActual, float deltaT){
	float eTime = Game::getInstance()->ellapsedTime;
	// eTime * speedFactor modifica la velocidad de movimiento, amplitude_ lo amplio que es.
	float posX = sin(eTime * speedFactor_) * amplitude_;

	posActual.x = posInicial_.x + posX;

	return posActual;
}


#pragma endregion

#pragma region MueveSpaceInvader

MueveSpaceInvader::MueveSpaceInvader(){}
MueveSpaceInvader::~MueveSpaceInvader(){}

void MueveSpaceInvader::init(
	float spaceInvaderMovement_speedX, float spaceInvaderMovement_speedY,
	float spaceInvaderMovement_xMin, float spaceInvaderMovement_xMax, 
	float spaceInvaderMovement_vcalMoveAmount, float spaceInvaderMovement_vcalMoveCurrTarget){

	spaceInvaderMovement_speedX_ = spaceInvaderMovement_speedX;
	spaceInvaderMovement_speedY_ = spaceInvaderMovement_speedY;
	spaceInvaderMovement_xMin_ = spaceInvaderMovement_xMin;
	spaceInvaderMovement_xMax_ = spaceInvaderMovement_xMax;
	spaceInvaderMovement_vcalMoveAmount_ = spaceInvaderMovement_vcalMoveAmount;
	spaceInvaderMovement_vcalMoveCurrTarget_ = spaceInvaderMovement_vcalMoveCurrTarget;
}

Vec2 MueveSpaceInvader::mueve(Vec2 posIni, float deltaT){
	//float deltaT = Director::getInstance()->getDeltaTime();

	if(SpaceInvaders::spaceInvaderMovement_goingDown_) {
		// MOVIMIENTO VERTICAL
		posIni.y -= spaceInvaderMovement_speedY_ * deltaT;

		if(posIni.y <= spaceInvaderMovement_vcalMoveCurrTarget_){
			SpaceInvaders::spaceInvaderMovement_goingDown_ = false;
			SpaceInvaders::spaceInvaderMovement_goingRight_ = !SpaceInvaders::spaceInvaderMovement_goingRight_;
		}
	} else{
		// MOVIMIENTO LATERAL
		if(SpaceInvaders::spaceInvaderMovement_goingRight_){
			posIni.x += spaceInvaderMovement_speedX_ * deltaT;

			if(posIni.x >= spaceInvaderMovement_xMax_){
				SpaceInvaders::spaceInvaderMovement_goingDown_ = true;
				spaceInvaderMovement_vcalMoveCurrTarget_ = posIni.y - spaceInvaderMovement_vcalMoveAmount_;
			}
		} else{
			posIni.x -= spaceInvaderMovement_speedX_ * deltaT;

			if(posIni.x <= spaceInvaderMovement_xMin_){
				SpaceInvaders::spaceInvaderMovement_goingDown_ = true;
				spaceInvaderMovement_vcalMoveCurrTarget_ = posIni.y - spaceInvaderMovement_vcalMoveAmount_;
			}
		}
	}

	return posIni;
}

#pragma endregion
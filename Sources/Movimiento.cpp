#include "Movimiento.h"

Movimiento::Movimiento(){}
Movimiento::~Movimiento(){}

void Movimiento::init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite * target){
	_speed = spd;
	_amplitude = amp;
	_origin = ori;
	_direction = dir;
	_posInicial = posIni;
	_target = target;
}


MueveDireccion::MueveDireccion(){}
MueveDireccion::~MueveDireccion(){}

Vec2 MueveDireccion::mueve(Vec2 posIni){
	float deltaT = Director::getInstance()->getDeltaTime();
	Vec2 deltaXY;
	Vec2 tmp;

	deltaXY = (_direction - _origin) * deltaT * _speed;
	tmp = posIni + deltaXY;
	
	return tmp;
}


MueveHoming::MueveHoming(){}
MueveHoming::~MueveHoming(){}

void MueveHoming::init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite * target){
	_speed = spd;
	_amplitude = amp;
	_origin = ori;
	_direction = dir;
	_posInicial = posIni;
	_target = target;

}

Vec2 MueveHoming::mueve(Vec2 posIni){
	float deltaT = Director::getInstance()->getDeltaTime();
	//Vec2 deltaXY;
	Vec2 tmp;

	deltaXY = posIni - _target->getPosition();
	deltaXY.normalize();	// para que no dependa de la distancia entre el enemigo y el jugador
	deltaXY *= deltaT * _speed / 3.0f;	// TODO: parametrizar la reducción de velocidad

	tmp = posIni + deltaXY;


	return tmp;
}



//void GameActor::mueveSpaceInvader(){
//float deltaT = Director::getInstance()->getDeltaTime();
//Vec2 curPos = sprite->getPosition();


////if(spaceInvaderMovement_goingDown){
//if(SpaceInvaders::spaceInvaderMovement_goingDown) {
//	// MOVIMIENTO VERTICAL
//	curPos.y -= spaceInvaderMovement_speedY * deltaT;

//	if(curPos.y <= spaceInvaderMovement_vcalMoveCurrTarget){
//		//spaceInvaderMovement_goingDown = false;
//		//spaceInvaderMovement_goingRight = !spaceInvaderMovement_goingRight;
//		SpaceInvaders::spaceInvaderMovement_goingDown = false;
//		SpaceInvaders::spaceInvaderMovement_goingRight = !SpaceInvaders::spaceInvaderMovement_goingRight;
//	}
//} else{
//	// MOVIMIENTO LATERAL
//	//if(spaceInvaderMovement_goingRight){
//	if(SpaceInvaders::spaceInvaderMovement_goingRight){
//		curPos.x += spaceInvaderMovement_speedX * deltaT;

//		if(curPos.x >= spaceInvaderMovement_xMax){
//			////spaceInvaderMovement_goingRight = false;
//			SpaceInvaders::spaceInvaderMovement_goingDown = true;
//			//spaceInvaderMovement_goingDown = true;
//			spaceInvaderMovement_vcalMoveCurrTarget = curPos.y - spaceInvaderMovement_vcalMoveAmount;
//			// no hago "clipping" del movimiento porque me parece que se me van a apuchurrar los invaders a los lados
//		}
//	} else{
//		curPos.x -= spaceInvaderMovement_speedX * deltaT;

//		if(curPos.x <= spaceInvaderMovement_xMin){
//			////spaceInvaderMovement_goingRight = true;
//			SpaceInvaders::spaceInvaderMovement_goingDown = true;
//			//spaceInvaderMovement_goingDown = true;
//			spaceInvaderMovement_vcalMoveCurrTarget = curPos.y - spaceInvaderMovement_vcalMoveAmount;
//			// no hago "clipping" del movimiento porque me parece que se me van a apuchurrar los invaders a los lados
//		}
//	}
//}

//sprite->setPosition(curPos);

//}

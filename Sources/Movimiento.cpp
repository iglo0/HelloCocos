#include "Movimiento.h"
#include "Game.h"

#pragma region Base

Movimiento::Movimiento(){}
Movimiento::~Movimiento(){}

//void Movimiento::init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite * target){
//	_speed = spd;
//	_amplitude = amp;
//	_origin = ori;
//	_direction = dir;
//	_posInicial = posIni;
//	_target = target;
//}

#pragma endregion

#pragma region MueveVcal

MueveVcal::MueveVcal(float speed) : _speed(speed){
	_direction = Vec2(0, speed);
}
MueveVcal::~MueveVcal(){};

Vec2 MueveVcal::mueve(Vec2 posActual){
	float deltaT = Director::getInstance()->getDeltaTime();

	Vec2 tmp = _direction * deltaT;

	return posActual + tmp;
}

#pragma endregion

#pragma region MueveDireccion

MueveDireccion::MueveDireccion(float speed): _speed(speed) {}
MueveDireccion::~MueveDireccion(){}

void MueveDireccion::init(Vec2 ori, Vec2 dest){
	_direction = ori - dest;
	// que no varie la velocidad según la distancia
	_direction.normalize();
}

Vec2 MueveDireccion::mueve(Vec2 posActual){
	float deltaT = Director::getInstance()->getDeltaTime();
	Vec2 deltaXY;
	Vec2 tmp;

	//deltaXY = (_direction - _origin) * deltaT * _speed;
	deltaXY = _direction * deltaT * _speed;
	tmp = posActual + deltaXY;
	
	return tmp;
}

#pragma endregion

#pragma region MueveHoming

MueveHoming::MueveHoming(){}
MueveHoming::~MueveHoming(){}

void MueveHoming::init(float spd, Sprite *target){
	_speed = spd;
	//_amplitude = amp;
	//_origin = ori;
	//_direction = dir;
	//_posInicial = posIni;
	_target = target;

}

Vec2 MueveHoming::mueve(Vec2 posActual){
	float deltaT = Director::getInstance()->getDeltaTime();
	//Vec2 deltaXY;
	Vec2 tmp;

	deltaXY = posActual - _target->getPosition();
	deltaXY.normalize();	// para que no dependa de la distancia entre el enemigo y el jugador
	deltaXY *= deltaT * _speed;

	tmp = posActual + deltaXY;

	return tmp;
}

#pragma endregion

#pragma region MueveSeno

MueveSeno::MueveSeno(){}
MueveSeno::~MueveSeno(){}

Vec2 MueveSeno::mueve(Vec2 posActual){
	float eTime = Game::getInstance()->ellapsedTime;

	
	return posActual;
}


#pragma endregion

#pragma region MueveSpaceInvader
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
#pragma endregion
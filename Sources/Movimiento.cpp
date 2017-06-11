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
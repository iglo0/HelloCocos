#pragma once

#include "cocos2d.h"	// para Vec2

USING_NS_CC;

class GameActor;

class Movimiento{
public:
	Movimiento();
	~Movimiento();

	virtual void init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite *target);
	virtual Vec2 mueve(Vec2 posIni)=0;

protected:

	float _speed;
	float _amplitude;		// si quiero darle un movimiento oscilante
	Vec2 _origin;			// para aplicar una direcci�n 
	Vec2 _direction;		// la t�pica bala podr�a ser ((0,0) -> (0,1)) * speed
	Vec2 _posInicial;		// en caso de ser una funci�n dependiente del tiempo (movimiento sinusoidal p.ej.), no se mueve en base a su pos actual sino a la inicial * tiempo
							// (aunque supongo que tambi�n podr�a hacerse relativo)
	Sprite *_target;		// movimiento hacia un sprite
};

class MueveDireccion : public Movimiento{
public:
	MueveDireccion();
	~MueveDireccion();

	//void init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite *buscar) override;
	Vec2 mueve(Vec2 posIni) override;
};

class MueveHoming : public Movimiento{
public:
	MueveHoming();
	~MueveHoming();

	void init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite *target) override;
	Vec2 mueve(Vec2 posIni) override;

	bool homing = false;	// si persigue al target o solo sale en su direcci�n inicial

private:
	Vec2 deltaXY;
};
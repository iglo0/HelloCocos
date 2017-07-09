#pragma once

#include "cocos2d.h"	// para Vec2

USING_NS_CC;

class GameActor;

class Movimiento{
public:
	Movimiento();
	~Movimiento();

	//void init(float spd, float amp, Vec2 ori, Vec2 dir, Vec2 posIni, Sprite *target);
	
	virtual Vec2 mueve(Vec2 posIni)=0;

protected:

	//float _speed;
	//float _amplitude;		// si quiero darle un movimiento oscilante
	//Vec2 _origin;			// para aplicar una dirección 
	//Vec2 _direction;		// la típica bala podría ser ((0,0) -> (0,1)) * speed
	//Vec2 _posInicial;		// en caso de ser una función dependiente del tiempo (movimiento sinusoidal p.ej.), no se mueve en base a su pos actual sino a la inicial * tiempo
	//						// (aunque supongo que también podría hacerse relativo)
	//Sprite *_target;		// movimiento hacia un sprite
};

class MueveVcal : public Movimiento{
public:

	MueveVcal(float speed);
	~MueveVcal();

	//void init();

	Vec2 mueve(Vec2 posActual) override;

private:
	float _speed;
	Vec2 _direction;
};

class MueveDireccion : public Movimiento{
public:
	MueveDireccion(float speed);
	~MueveDireccion();

	void init(Vec2 origin, Vec2 target);
	Vec2 mueve(Vec2 posActual) override;

private:
	float _speed;
	Vec2 _direction;
};

class MueveHoming : public Movimiento{
public:
	MueveHoming();
	~MueveHoming();

	void init(float spd, Sprite *target);
	Vec2 mueve(Vec2 posActual) override;

private:
	Vec2 deltaXY;
	float _speed;
	Sprite *_target;		// movimiento hacia un sprite

};

class MueveSeno : public Movimiento{
public:
	MueveSeno();
	~MueveSeno();

	void init(float amplitude, Vec2 posInicial, float speedFactor);
	Vec2 mueve(Vec2 posActual) override;

private:
	float amplitude_;
	Vec2 posInicial_;
	float speedFactor_;
};

class MueveSpaceInvader : public Movimiento{
	// movimiento tipo "space invader"
	// *movimiento lateral -> esquina -> bajar -> pal otro lado -> bajar -> repetir
	// cada nave se controla a sí misma, necesita:
	// - dirección inicial
	// - vel htal/vel vcal
	// - limites x

public:
	MueveSpaceInvader();
	~MueveSpaceInvader();

	void init(float speedX, float speedY, float xMin, float xMax, float vcalMoveAmount, float vcalMoveCurrTarget);
	Vec2 mueve(Vec2 posActual) override;


private:

	float spaceInvaderMovement_speedX_;				// cómo de rápido se mueve lateralmente
	float spaceInvaderMovement_speedY_;				// cómo de rápido baja
	float spaceInvaderMovement_xMin_;				// dónde hace tope
	float spaceInvaderMovement_xMax_;				// dónde hace tope
	float spaceInvaderMovement_vcalMoveAmount_;		// cuánto baja cuando baja
	float spaceInvaderMovement_vcalMoveCurrTarget_;	// cómo sé cuándo deja de bajar
};


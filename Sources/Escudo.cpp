#include "Escudo.h"


Escudo::Escudo(Node *parent, float vida, float regen) : vidaEscudoInicial(vida), regenRate(regen){
	vidaEscudoActual = vida;

	Sprite *tmp = cargaSprite(parent, 0, rutaImagen);

}

Escudo::~Escudo(){
}

float Escudo::impacto(float dmg){
	float dmgPasa;

	if(dmg >= vidaEscudoActual){
		dmgPasa = dmg - vidaEscudoActual;

		// escudo destruido!
		// por ejemplo...
		vidaEscudoActual = 0;
		escudoDestruido = true;
		escudoDestruidoTIni = Game::getInstance()->ellapsedTime;
	} else{
		dmgPasa = 0;
	}

	vidaEscudoActual -= dmg;

	//float porcen = vidaActual / vidaInicial;
	
	//// fades in the sprite in 1 seconds
	//auto fadeOut = FadeOut::create(1.0f);
	//sprite->runAction(fadeOut);


	/* efectos
	// efecto de daño
	float porcen = puntosDeGolpeActuales / puntosDeGolpeIniciales;

	// Actions
	// http://www.cocos2d-x.org/wiki/Actions
	// By -> relative, To -> absoulte

	// Tints a node to the specified RGB values
	// GLubyte = unsigned char
	GLubyte val = 255 * porcen;
	if(val < 128)
	val = 128;
	auto tintTo = TintTo::create(0.5f, val, val, val);
	sprite->runAction(tintTo);

	// Sequences

	// create a few actions.
	auto tintIda = TintTo::create(0.05f, Color3B(255, 0, 0));
	auto tintVuelta = TintTo::create(0.05f, Color3B(255, 255, 255));

	float escalaOri = sprite->getScale();
	auto escalaIda = ScaleTo::create(0.05f, escalaOri * 1.1);
	auto escalaVuelta = ScaleTo::create(0.05f, escalaOri);

	auto callbackTint = CallFunc::create([](){
	log("Tintorro!");
	});

	auto callbackScale = CallFunc::create([](){
	log ("Scaled");
	});

	// create a sequence with the actions and callbacks
	// TODO: mirar usos de los callbacks
	auto seq = Sequence::create(escalaIda, callbackScale, tintIda, tintVuelta, nullptr);

	// run it
	sprite->runAction(seq);

	*/

	return dmgPasa;
}

Sprite *Escudo::getSprite(){
	return sprite;
}

void Escudo::update(float deltaT){

	if(escudoDestruido){
		if(Game::getInstance()->ellapsedTime - escudoDestruidoTIni >= esperaInicioRegenTrasDestruccion){
			escudoDestruido = false;
		}
	} else{
		if(vidaEscudoActual < vidaEscudoInicial){
			vidaEscudoActual += regenRate * deltaT;
		}

		// dibuja el escudo con tanto alpha como vida tenga
		auto fadeTo = FadeTo::create(0, 255 * vidaEscudoActual / vidaEscudoInicial);

		// TODO: Ojo!! cada runAction empieza en el estado inicial así que si no espero a que terminen, es como si no hiciera nada
		if(sprite->getNumberOfRunningActions()<1)
			sprite->runAction(fadeTo);

	}

}


#pragma once

#include "cocos2d.h"

USING_NS_CC;

#pragma region CONSTANTES

// Player
#define PLAYER_INITIAL_SPEED 300.0f
#define PLAYER_PATH_SPRITE "spaceshipspr.png"
// Bullets
#define BULLET_PATH_SPRITE1 "bullet_2_blue.png"
#define BULLET_PATH_SPRITE2 "bullet_orange0000.png"
#define BULLET_PATH_SOUND_FIRE "sonidos/shoot.wav"
#define BULLET_PATH_SOUND_IMPACT "sonidos/fastinvader1.wav"

#define BULLET_DEFAULT_DMG 1.0f
#define BULLET_DEFAULT_SPEED 500.0f
// Enemies
#define ENEMY_GENERIC_SPEED 100.0f
#define ENEMY_GENERIC_HP 1.0f

#define ENEMY_T1_PATH_SPRITE "boss1.png"
#define ENEMY_T1_INITIAL_SIZE 0.5f
#define ENEMY_T1_INITIAL_ROTATION 0

#define ENEMY_T2_PATH_SPRITE "Spaceship15.png"
#define ENEMY_T2_INITIAL_SIZE 0.5f
#define ENEMY_T2_INITIAL_ROTATION 0

#define ENEMY_BOSS_GENERIC_HP 20.0f
#define ENEMY_BOSS_PATH_SPRITE "aliensprite2.png"
#define ENEMY_BOSS_INITIAL_SIZE 0.5f
#define ENEMY_BOSS_INITIAL_ROTATION 0

#define ENEMY_PATH_SOUND_DIE "sonidos/invaderkilled.wav"


#pragma endregion

// declaracion adelantada
class Bullet;

// Mi clase Singleton (o es la idea) donde guardar el estado general del juego
class Game{
private:
	// entiendo que los mantengo privados para que nadie lo pueda llamar desde fuera
	// así me aseguro que no se crea otra instancia
	Game();
	// nadie debería poder destruir esta clase
	//~Game();

	// para nota, declarar y no definir los operadores de copia y asignacion
	//Game(Game const&);              // Don't Implement.
	//void operator=(Game const&);	// Don't implement

public:

	// hay quien recomienda no usar punteros sino referencias, para asegurarse de que no se puede liberar esta memoria. Si lo he entendido bien...
	//static Game& getInstance(){ static Game instance; return instance; }
	
	// devuelve la instancia, y se asegura de inicializarlo solo una vez aun con concurrencia (recomendado desde c++11)
	static Game *getInstance();
	// prueba fisica
	static void anadeFisica(Sprite *sprite, int tipoColision, int colisionaCon, const char *name = "[NONAME]");


	// HACK: tengo que aprender a gestionar el tiempo mejor
	// la idea es que desde la escena que sea, vaya añadiendo el deltaT en cada Update, para saber así el tiempo transcurrido desde el inicio de la escena, al menos
	float ellapsedTime;

	enum estadosJuego{ menus, introNivel, jugando, finNivel, finHorda, muerte };
	
	estadosJuego estadoActual;


	// TODO: ES ESTE UN BUEN SITIO PARA ESTO???
	// Quiero? que las categorías estén accesibles en todas partes
	// --------------------------------------------------------------
	// Físicas (o sea, Colisiones)
	// --------------------------------------------------------------
	
	// una forma cómoda de definir la categoría de colisiones de cada objeto
	// no sé si lo he entendido bien, la forma que tiene Cocos de gestionar las categorías es activando bits de un integer de 32
	// definiéndolas así, me aseguro que cada una solo active un bit
	enum class CategoriaColision{
		// The category on Cocos2d-x is simply a single 32-bit integer; this syntax sets specific bits in the integer to
		// represent different categories, giving you 32 possible categories max. Here you set the first bit to indicate
		// a monster, the next bit over to represent a projectile, and so on.
		None = 0,
		Jugador = (1 << 0),			// 1
		Bala = (1 << 1),			// 2
		Enemigo = (1 << 2),			// 4
		BalaEnemigo = (1 << 3),		// 8
		//EnemigosYBalas = CategoriaColision::Enemigo | CategoriaColision::BalaEnemigo,
		All = CategoriaColision::Jugador | CategoriaColision::Bala | CategoriaColision::Enemigo | CategoriaColision::BalaEnemigo
	};



};

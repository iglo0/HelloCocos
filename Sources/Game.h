#pragma once

#include "cocos2d.h"
#include <vector>
#include <iostream>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
#include <iomanip>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
#include "pugixml.hpp"
#include <unordered_map>	// TODO: hace falta?? sin el include también funciona

USING_NS_CC;

// TODO: Sacarlo a un fichero
#pragma region CONSTANTES

// --------------------------------------------------------------------
// definicion de objetos
// --------------------------------------------------------------------
// prueba con los xml, esto es intellisense para pobres :P
// indica como se llaman los nodos en el xml de configuración
#define CONFIG_PLAYER_INITIAL_SPEED			"player_initial_speed"
#define CONFIG_PLAYER_PATH_SPRITE			"player_path_sprite"

#define CONFIG_BULLET_PATH_SPRITE1			"bullet_path_sprite1"
#define CONFIG_BULLET_PATH_SPRITE2			"bullet_path_sprite2"
#define CONFIG_BULLET_PATH_SOUND_FIRE		"bullet_path_sound_fire"
#define CONFIG_BULLET_PATH_SOUND_IMPACT		"bullet_path_sound_impact"
#define CONFIG_BULLET_DEFAULT_SCALE			"bullet_default_scale"
#define CONFIG_BULLET_DEFAULT_BOSS_SCALE	"bullet_default_boss_scale"
#define CONFIG_BULLET_DEFAULT_DMG			"bullet_default_dmg"
#define CONFIG_BULLET_DEFAULT_SPEED			"bullet_default_speed"

#define CONFIG_ENEMY_GENERIC_SPEED			"enemy_generic_speed"
#define CONFIG_ENEMY_GENERIC_HP				"enemy_generic_hp"
#define CONFIG_ENEMY_GENERIC_POINTS			"enemy_generic_points"
#define CONFIG_ENEMY_T1_PATH_SPRITE			"enemy_t1_path_sprite"
#define CONFIG_ENEMY_T1_INITIAL_SIZE		"enemy_t1_initial_size"
#define CONFIG_ENEMY_T1_INITIAL_ROTATION	"enemy_t1_initial_rotation"
#define CONFIG_ENEMY_T2_PATH_SPRITE			"enemy_t2_path_sprite"
#define CONFIG_ENEMY_T2_INITIAL_SIZE		"enemy_t2_initial_size"
#define CONFIG_ENEMY_T2_INITIAL_ROTATION	"enemy_t2_initial_rotation"
#define CONFIG_ENEMY_BOSS_GENERIC_HP		"enemy_boss_generic_hp"
#define CONFIG_ENEMY_BOSS_PATH_SPRITE		"enemy_boss_path_sprite"
#define CONFIG_ENEMY_BOSS_INITIAL_SIZE		"enemy_boss_initial_size"
#define CONFIG_ENEMY_BOSS_INITIAL_ROTATION	"enemy_boss_initial_rotation"
#define CONFIG_ENEMY_BOSS_POINTS			"enemy_boss_points"
#define CONFIG_ENEMY_PATH_SOUND_DIE			"enemy_path_sound_die"

#define CONFIG_DURACION_ESTADO_INTRONIVEL	"duracion_estado_intronivel"
#define CONFIG_DURACION_ESTADO_FINNIVEL		"duracion_estado_finnivel"
#define CONFIG_DURACION_ESTADO_MUERTE		"duracion_estado_muerte"
#define CONFIG_DURACION_ESTADO_GAMEOVER		"duracion_estado_gameover"

#define CONFIG_INITIAL_HI_SCORE				"initial_hi_score"
#define CONFIG_VIDAS_INICIALES				"vidas_iniciales"

// --------------------------------------------------------------------
// Player
//#define PLAYER_INITIAL_SPEED 300.0f
//#define PLAYER_PATH_SPRITE "spaceshipspr.png"
// Bullets
//#define BULLET_PATH_SPRITE1 "bullet_2_blue.png"
//#define BULLET_PATH_SPRITE2 "bullet_orange0000.png"
//#define BULLET_PATH_SOUND_FIRE "sonidos/shoot.wav"
//#define BULLET_PATH_SOUND_IMPACT "sonidos/fastinvader1.wav"
//#define BULLET_DEFAULT_SCALE 1.0f
//#define BULLET_DEFAULT_BOSS_SCALE 3.0f
//
//#define BULLET_DEFAULT_DMG 1.0f
//#define BULLET_DEFAULT_SPEED 500.0f
//// Enemies
//#define ENEMY_GENERIC_SPEED 100.0f
//#define ENEMY_GENERIC_HP 1.0f
//#define ENEMY_GENERIC_POINTS 100
//
//#define ENEMY_T1_PATH_SPRITE "aliensprite2.png"
//#define ENEMY_T1_INITIAL_SIZE 0.30f
//#define ENEMY_T1_INITIAL_ROTATION 0
//
//#define ENEMY_T2_PATH_SPRITE "Spaceship15.png"
//#define ENEMY_T2_INITIAL_SIZE 0.35f
//#define ENEMY_T2_INITIAL_ROTATION 0
//
//#define ENEMY_BOSS_GENERIC_HP 20.0f
//#define ENEMY_BOSS_PATH_SPRITE "boss1.png"
//#define ENEMY_BOSS_INITIAL_SIZE 1.0f
//#define ENEMY_BOSS_INITIAL_ROTATION 0
//
//#define ENEMY_BOSS_POINTS 500
//
//#define ENEMY_PATH_SOUND_DIE "sonidos/invaderkilled.wav"

//// --------------------------------------------------------------------
//// estados
//// --------------------------------------------------------------------
//#define duracion_estado_intronivel 2.0f
//#define duracion_estado_finnivel 2.0f
//#define duracion_estado_muerte 2.0f
//#define duracion_estado_gameover 5.0f
//
//// --------------------------------------------------------------------
//// varios
//// --------------------------------------------------------------------
//#define initial_hi_score 5000	// TODO: guardar la tabla de records
//#define vidas_iniciales 3
//
#pragma endregion

// declaraciones adelantadas
// TODO: Juer, no uso Enemy en Game y si quito esta declaracion peta todo o.O
class Enemy;


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
	//static void updateAll(float deltaT); // updates all gameActors in da pools. Ummm Game no conoce cómo está definido Enemy (declaración adelantada), y no puede acceder a sus miembros.

	// TODO: esto de mezclar funciones estaticas con instanciadas... 
	// actualiza los puntos y el marcador correspondiente
	void sumaPuntos(int p);

	// --------------------------------------------------------------
	// Puntuaciones
	// --------------------------------------------------------------
	int puntos = 0;
	int hiScore;
	int vidas;

	// --------------------------------------------------------------
	// GUI [aquí están los controles que van variando: puntos, etc]
	// --------------------------------------------------------------
	Label *lblMensajes;
	Label *lblPuntos;
	Label *lblHiScore;
	Label *lblVidas;

	void inicializaGUI();
	void actualizaVidas();

	// HACK: tengo que aprender a gestionar el tiempo mejor
	// la idea es que desde la escena que sea, vaya añadiendo el deltaT en cada Update, para saber así el tiempo transcurrido desde el inicio de la escena, al menos
	float ellapsedTime;

	enum estadosJuego{ menus, introNivel, jugando, finNivel, finHorda, muerte, gameOver };
	
	estadosJuego estadoActual;


	// --------------------------------------------------------------
	// Configuracion
	// --------------------------------------------------------------
	// TODO: Aparentemente copiar el .xml en resources no es multiplataforma y cascará en android/ios
	void loadConfig(const char *filename);
	
	// variables donde almacenar la configuracion

	// player
	float player_initial_speed;
	std::string player_path_sprite;
	// bullets
	std::string bullet_path_sprite1;
	std::string bullet_path_sprite2;
	std::string bullet_path_sound_fire;
	std::string bullet_path_sound_impact;
	float bullet_default_scale;
	float bullet_default_boss_scale;
	float bullet_default_dmg;
	float bullet_default_speed;
	// enemies
	float enemy_generic_speed;
	float enemy_generic_hp;
	int enemy_generic_points;
	std::string enemy_t1_path_sprite;
	float enemy_t1_initial_size;
	float enemy_t1_initial_rotation;
	std::string enemy_t2_path_sprite;
	float enemy_t2_initial_size;
	float enemy_t2_initial_rotation;
	float enemy_boss_generic_hp;
	std::string enemy_boss_path_sprite;
	float enemy_boss_initial_size;
	float enemy_boss_initial_rotation;
	int enemy_boss_points;
	std::string enemy_path_sound_die;
	// estados
	float duracion_estado_intronivel;
	float duracion_estado_finnivel;
	float duracion_estado_muerte;
	float duracion_estado_gameover;
	// varios
	int initial_hi_score;
	int vidas_iniciales;

	// --------------------------------------------------------------

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

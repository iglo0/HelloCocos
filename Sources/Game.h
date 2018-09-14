#pragma once

#include "cocos2d.h"
//#include <vector>
#include <iostream>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
#include <iomanip>	// para usar std::cout << ... << std::setfill('0') << 4 ... (leading zeros)
#include "pugixml.hpp"

USING_NS_CC;

class HiScores;

#pragma region CONSTANTES de configuraci�n

#define BULLET_HOMING_TTL					6.5

// TODO: como nota, los archivos JSON son como los XML pero m�s f�ciles de leer por un humano
// --------------------------------------------------------------------
// definicion de objetos
// --------------------------------------------------------------------
// prueba con los xml, esto es intellisense para pobres :P
// indica como se llaman los nodos en el xml de configuraci�n
// el problema es que por cada nodo ten�a un #define y una variable, he decidido quitar los defines y cargar las variables con nombres fijos en el c�digo. As� quito bastante morralla....
//#define XML_EXPLOSION_ANIM	"explosion"
//#define XML_IMPACT_ANIM	"impacto"

#pragma endregion

// Mi clase Singleton (o es la idea) donde guardar el estado general del juego
class Game{
private:
	// entiendo que los mantengo privados para que nadie lo pueda llamar desde fuera
	// as� me aseguro que no se crea otra instancia
	Game();
	// nadie deber�a poder destruir esta clase
	//~Game();

	// para nota, declarar y no definir los operadores de copia y asignacion
	//Game(Game const&);              // Don't Implement.
	//void operator=(Game const&);	// Don't implement

public:
	// hay quien recomienda no usar punteros sino referencias, para asegurarse de que no se puede liberar esta memoria. Si lo he entendido bien...
	//static Game& getInstance(){ static Game instance; return instance; }
	
	// devuelve la instancia, y se asegura de inicializarlo solo una vez aun con concurrencia (recomendado desde c++11)
	static Game *getInstance();

	// para que todas mis creaciones de sprite sean iguales
	// HACK: le falta asignarle spriteData para que funcionen las colisiones. OJETE!
	static Sprite *creaSprite(const char *ruta, const char *name, int tipoColision, int colisionaCon, float initialScale);

	// anade fisica (desactivada!) a un sprite.
	// name = el nombre del sprite (no lo uso mas que para debug y para mi)
	// colliderName = OJO! este s�, es el nombre en la cach� de f�sicas = nombre del sprite con extensi�n y todo
	static void anadeFisica(Sprite *sprite, int tipoColision, int colisionaCon, const char *name = "[NONAME]", const char *colliderName="");
	//static void updateAll(float deltaT); // updates all gameActors in da pools. Ummm Game no conoce c�mo est� definido Enemy (declaraci�n adelantada), y no puede acceder a sus miembros.

	// TODO: esto de mezclar funciones estaticas con instanciadas... 
	// actualiza los puntos y el marcador correspondiente
	void sumaPuntos(int p);

	// Esto se va a ejecutar solo durante la carga, as� que no me importa si no es optimo
	const char *devuelveBloqueRotoAleatorio();

	int vidas;

	#pragma region puntuacion
	// --------------------------------------------------------------
	// Puntuaciones
	// --------------------------------------------------------------
	int puntos = 0;
	int hiScoreCached;
	HiScores *hiScores_;

	/* TODO HiScores movidos a... HiScores 
	// map containers do not allow for duplicate key values, as� que para la tabla de records usar�...
	typedef struct {
		std::string name;
		std::string nivelAlcanzado;
		int puntos;	// redundante puesto que tambi�n es la clave del multimap, pero a la hora de leer los records grabados me obligar�a a tener una variable para los puntos y una para el resto de datos
	} record;
	// creo el typedef para hacer m�s f�cil la sintaxis de insercion
	// Voy a usar un multimap inverso, as� las puntuaciones m�s altas salen por defecto las primeras
	// simplemente tengo que a�adir un tercer par�metro a la definici�n, incluyendo el comparador por defecto:
	// std::less<int> -> de menor a mayor
	// std::greater<int> -> de mayor a menor
	typedef std::multimap<int, record, std::greater<int>> tTablaDeRecords;
	//typedef std::multimap<int, record> tTablaDeRecords;
	tTablaDeRecords tablaRecords;

	void insertHiScore(std::string nombre, std::string nivelAlcancado, int puntos);
	record getHiScore();	// solo obtiene un valor para el hiScore que se muestra en la pantalla durante la partida
	void cargaTablaRecords();
	void guardaTablaRecords();
	int devuelveHiScoreTablaRecords();
	*/

	#pragma endregion
	

	// --------------------------------------------------------------
	// GUI [aqu� est�n los controles que van variando: puntos, etc]
	// (se inicializan en Level)
	// --------------------------------------------------------------
	Label *lblMensajes;
	Label *lblPuntos;
	Label *lblHiScore;
	Label *lblHiScoreName;
	Label *lblVidas;
	Label *lblDebug;

	void inicializaGUI();
	void actualizaVidas();

	// HACK: tengo que aprender a gestionar el tiempo mejor (lol el mio sobre todo)
	// la idea es que desde la escena que sea, vaya a�adiendo el deltaT en cada Update, para saber as� el tiempo transcurrido desde el inicio de la escena, al menos
	float ellapsedTime;

	// --------------------------------------------------------------
	// Configuracion
	// --------------------------------------------------------------
	// TODO: Aparentemente copiar el .xml en resources no es multiplataforma y cascar� en android/ios
	void loadConfig(const char *filename);

	#pragma region variables de configuracion

	// variables donde almacenar la configuracion

	// player
	float player_initial_speed;
	std::string player_path_sprite;
	std::string player_sonido_dispara;
	std::string player_sonido_muerte;

	std::string game_sonido_invaders_loop;
	std::string game_sonido_casa_impacto;
	std::string game_music_menus;
	std::string game_music_level;

	// bullets
	float bullet_default_dmg;

	// enemies
	float enemy_generic_speed;

	float enemy_boss_generic_hp;
	std::string enemy_boss_path_sprite;
	float enemy_boss_initial_size;
	float enemy_boss_initial_rotation;
	int enemy_boss_points;

	// estados
	float duracion_estado_intronivel;
	float duracion_estado_finnivel;
	float duracion_estado_muerte;
	float duracion_estado_gameover;

	// varios
	//int initial_hi_score;
	int vidas_iniciales;

	// casitas
	std::string sprite_casa_bloque;
	std::string sprite_casa_bloque_roto_01;
	std::string sprite_casa_bloque_roto_02;
	std::string sprite_casa_bloque_roto_03;
	std::string sprite_casa_bloque_roto_04;
	//std::string sprite_casa_bloque_roto_05;
	//std::string sprite_casa_bloque_roto_06;
	//std::string sprite_casa_bloque_roto_07;
	//std::string sprite_casa_bloque_roto_08;
	//std::string sprite_casa_bloque_roto_09;
	//std::string sprite_casa_bloque_roto_10;
	//std::string sprite_casa_bloque_roto_11;
	//std::string sprite_casa_bloque_roto_12;
	//std::string sprite_casa_bloque_roto_13;
	//std::string sprite_casa_bloque_roto_14;
	//std::string sprite_casa_bloque_roto_15;
	std::string sprite_casa_bloque_roto_dch;
	std::string sprite_casa_bloque_roto_izq;
	std::string sprite_casa_esquina_dch;
	std::string sprite_casa_esquina_izq;
	#pragma endregion

	// --------------------------------------------------------------

	// --------------------------------------------------------------
	// F�sicas (o sea, Colisiones)
	// --------------------------------------------------------------
	// ES ESTE UN BUEN SITIO PARA ESTO???
	// Quiero? que las categor�as est�n accesibles en todas partes

	// una forma c�moda de definir la categor�a de colisiones de cada objeto
	// no s� si lo he entendido bien, la forma que tiene Cocos de gestionar las categor�as es activando bits de un integer de 32
	// defini�ndolas as�, me aseguro que cada una solo active un bit
	enum class CategoriaColision{
		// The category on Cocos2d-x is simply a single 32-bit integer; this syntax sets specific bits in the integer to
		// represent different categories, giving you 32 possible categories max. Here you set the first bit to indicate
		// a monster, the next bit over to represent a projectile, and so on.
		None = 0,
		Jugador = (1 << 0),			// 1
		BalaJugador = (1 << 1),		// 2
		Enemigo = (1 << 2),			// 4
		BalaEnemigo = (1 << 3),		// 8
		Destructible = (1 << 4),	// 16
		//EnemigosYBalas = CategoriaColision::Enemigo | CategoriaColision::BalaEnemigo,
		All = CategoriaColision::Jugador | CategoriaColision::BalaJugador | CategoriaColision::Enemigo | CategoriaColision::BalaEnemigo | CategoriaColision::Destructible
	};

};

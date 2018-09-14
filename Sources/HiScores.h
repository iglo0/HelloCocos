#pragma once

#include <string>
#include <map>			
#include <functional>	//std::greater

class HiScores {
public:
	HiScores();
	~HiScores();

	// Variables públicas
	// map containers do not allow for duplicate key values, así que para la tabla de records usaré...
	typedef struct{
		std::string name;
		std::string nivelAlcanzado;
		int puntos;	// redundante puesto que también es la clave del multimap, pero a la hora de leer los records grabados me obligaría a tener una variable para los puntos y una para el resto de datos
	} record;

	// creo el typedef para hacer más fácil la sintaxis de insercion
	// Voy a usar un multimap inverso, así las puntuaciones más altas salen por defecto las primeras
	// simplemente tengo que añadir un tercer parámetro a la definición, incluyendo el comparador por defecto:
	// std::less<int> -> de menor a mayor
	// std::greater<int> -> de mayor a menor
	typedef std::multimap<int, record, std::greater<int>> tTablaDeRecords;


	// Funciones públicas
	void insertHiScore(std::string nombre, std::string nivelAlcanzado, int puntos);
	int devuelveHiScoreTablaRecords();
	void guardaTablaRecords();

	//typedef std::multimap<int, record> tTablaDeRecords;
	tTablaDeRecords tablaRecords;

private:

	void cargaTablaRecords();
	record getHiScore();	// solo obtiene un valor para el hiScore que se muestra en la pantalla durante la partida
};
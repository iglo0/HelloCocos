#pragma once

#include <string>
#include <map>			
#include <functional>	//std::greater

class HiScores {
public:
	HiScores();
	~HiScores();

	// Variables p�blicas
	// map containers do not allow for duplicate key values, as� que para la tabla de records usar�...
	typedef struct{
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


	// Funciones p�blicas
	void insertHiScore(std::string nombre, std::string nivelAlcanzado, int puntos);
	int devuelveHiScoreTablaRecords();
	void guardaTablaRecords();

	//typedef std::multimap<int, record> tTablaDeRecords;
	tTablaDeRecords tablaRecords;

private:

	void cargaTablaRecords();
	record getHiScore();	// solo obtiene un valor para el hiScore que se muestra en la pantalla durante la partida
};
#include "HiScores.h"
#include "XmlHelper.h"

HiScores::HiScores(){
	cargaTablaRecords();
}

HiScores::~HiScores(){

}

void HiScores::guardaTablaRecords(){
	XmlHelper *xh = new XmlHelper();

	xh->saveHiScores("records.xml", this);
}

void HiScores::cargaTablaRecords(){

	XmlHelper *xh = new XmlHelper();

	xh->loadHiScores("records.xml", this);
}

int HiScores::devuelveHiScoreTablaRecords(){
	record rTmp = getHiScore();
	
	return rTmp.puntos;

}

void HiScores::insertHiScore(std::string nombre, std::string nivelAlcanzado, int puntos){
	record rTmp;

	rTmp.name = nombre;
	rTmp.nivelAlcanzado = nivelAlcanzado;
	rTmp.puntos = puntos;

	tablaRecords.insert(std::pair<int, record>(puntos, rTmp));

}

HiScores::record HiScores::getHiScore(){
	record rTmp;

	rTmp.name = "";
	rTmp.nivelAlcanzado = "";
	rTmp.puntos = 666;

	auto primer = tablaRecords.cbegin();
	//hiScoreCached = primer->first;
	rTmp = primer->second;

	return rTmp;

}
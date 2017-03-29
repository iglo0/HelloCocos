#pragma once

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class Base{
public:
	static void hazBaseArr(Base arrBases[]){};
	static void hazBaseVector(std::vector<Base>){};
};

class Derived : public Base{
public:
	static void hazDerivedArr(Derived arrDerived[]){};
	static void hazDerivedVector(std::vector<Derived>){};
};

class Sheeit{

public:

	Base arrBases[3];
	Derived arrDeriveds[3];
	std::vector<Base> vecBases;
	std::vector<Derived> vecDeriveds;

	int arrInts[3];

	void flipa(){
		Base::hazBaseArr(arrBases);
		Base::hazBaseArr(arrDeriveds);
		Derived::hazBaseArr(arrBases);
		Derived::hazBaseArr(arrDeriveds);
		//Derived::hazDerivedArr(arrBases);	// error, esto no
		Derived::hazDerivedArr(arrDeriveds);

		Base::hazBaseVector(vecBases);				// ok
		//Base::hazBaseVector(vecDeriveds);			// no ok

		//Derived::hazDerivedVector(vecBases);		// ok
		Derived::hazDerivedVector(vecDeriveds);		// no ok

	}
};
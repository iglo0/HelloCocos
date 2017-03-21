#pragma once

#include "cocos2d.h"

USING_NS_CC;

// holy shit, really?
// http://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

template <typename T>
class Testz{
public:
	void setVar(T x){
		var = x;
	}

	T getVar(){
		return var;
	}

private:
	T var;
};




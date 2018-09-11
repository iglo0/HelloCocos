#pragma once

#include <cocos2d.h>
#include <string>

class InputText {
public:
	InputText();
	~InputText();

	cocos2d::EventKeyboard::KeyCode keyCode;

	void pressedKey(const cocos2d::EventKeyboard::KeyCode);

	std::string devuelveLeido();

private:
	std::string cadena;


	const unsigned char keyCodeToChar(const cocos2d::EventKeyboard::KeyCode);
};
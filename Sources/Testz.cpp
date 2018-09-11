#include "Testz.h"

InputText::InputText(){
	cadena = "";
}

InputText::~InputText(){

}

void InputText::pressedKey(cocos2d::EventKeyboard::KeyCode kc){

	if(kc == cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE || kc == cocos2d::EventKeyboard::KeyCode::KEY_DELETE){
		// borra
		if(cadena.length() > 1){
			cadena.erase(cadena.length() - 1);
		}
	} else{
		cadena += keyCodeToChar(kc);
	}
}

std::string InputText::devuelveLeido(){
	return cadena;
}

// convierte keycodes de cocos2d en simples char (en mayúscula me vale)
const unsigned char InputText::keyCodeToChar(const cocos2d::EventKeyboard::KeyCode kc){
	// KeyCode es un enum y no parece estar relacionado con su código ascii así que...

	if(kc == cocos2d::EventKeyboard::KeyCode::KEY_SPACE){
		return ' ';
	}

	if(kc >= cocos2d::EventKeyboard::KeyCode::KEY_0 && kc <= cocos2d::EventKeyboard::KeyCode::KEY_9){
		unsigned char s = (unsigned char)kc - (unsigned char)cocos2d::EventKeyboard::KeyCode::KEY_0 + '0';

		return s;
	}


	if(kc >= cocos2d::EventKeyboard::KeyCode::KEY_A && kc <= cocos2d::EventKeyboard::KeyCode::KEY_Z){
		unsigned char s = (unsigned char)kc - (unsigned char)cocos2d::EventKeyboard::KeyCode::KEY_A + 'A';

		return s;
	}

	return '?';
}
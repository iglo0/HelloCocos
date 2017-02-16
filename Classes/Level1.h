#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class Level1 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuVuelveCallback(cocos2d::Ref* pSender);

	// keyboard listeners
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);


    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

	// el gameloop
	void update(float delta) override;

private:
	Sprite *prota;
	bool mueveIzq, mueveDch, mueveArr, mueveAbj;
	float protaSpeed = 500.0f;
};

#endif // __HELLOWORLD_SCENE_H__

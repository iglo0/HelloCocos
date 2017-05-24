#include "AppDelegate.h"

#include "Menus.h"
#include "PhysicsShapeCache.h"


USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size miResolutionSize = cocos2d::Size(1422, 800);	// 16:9 aprox, la que se ve bien en *mi* monitor :>
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        //glview = GLViewImpl::createWithRect("HelloCocos", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
		// Ign
		glview = GLViewImpl::createWithRect("HelloCocos", cocos2d::Rect(0, 0, miResolutionSize.width, miResolutionSize.height));
#else
        glview = GLViewImpl::create("HelloCocos");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    //glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	// Ign
	glview->setDesignResolutionSize(miResolutionSize.width, miResolutionSize.height, ResolutionPolicy::NO_BORDER);

	auto frameSize = glview->getFrameSize();

	// Ign FFFUUU que te quedes con la resolucion que he dicho
	/*
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height) {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    } else if (frameSize.height > smallResolutionSize.height) { // if the frame's height is larger than the height of small size.   
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    } else {  // if the frame's height is smaller than the height of medium size.      
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	*/

    register_all_packages();


	// =v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v=v
	// Indico donde están los archivos .plist de texturas y colisiones
	// haciéndolo aquí me aseguro que se cargan una sola vez

	// texturesheet plist:
	// OJO!!!: en la carpeta debug.win32 hay una copia de los archivos. Por eso se veía todo perfecto aunque borrara el .plist haciendo pruebas >.<
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("spritesheet.plist");

	// collision & physics plist (nada definido aparte de los polígonos y si hay algo es basura):
	PhysicsShapeCache *physicsShapeCache;
	physicsShapeCache = PhysicsShapeCache::getInstance();
	// OJO!!!: hacer esto una sola vez
	// en este fichero estarán colecciones de polígonos que luego usará la parte de colisiones
	physicsShapeCache->addShapesWithFile("physicscollision.plist");
	// Por si quisiera vaciar physicsShapeCache:
	//PhysicsShapeCache::getInstance()->removeAllShapes();
	// =^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^=^

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = Menus::CreateScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

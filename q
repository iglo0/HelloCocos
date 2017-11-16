[1mdiff --git a/README.md b/README.md[m
[1mindex c04bc59..d98f894 100644[m
[1m--- a/README.md[m
[1m+++ b/README.md[m
[36m@@ -47,7 +47,11 @@[m [mEn desarrollo [Estimación][Finalizado][m
 [m
 * Implementar la carga de enemigos mediante xml (comportamiento, animacion... todo en ficheros)[m
 ```diff[m
[31m-- cargando enemigos...[m
[32m+[m[32m+ carga básica de enemigos[m
[32m+[m[32m+ definición de disparos[m
[32m+[m[32m+ definición de sonido[m
[32m+[m[32m+ definición de animaciones[m
[32m+[m[32m- definición de comportamiento[m
 ```[m
 [m
 * Definición de niveles en archivos de texto[m
[36m@@ -55,10 +59,20 @@[m [mEn desarrollo [Estimación][Finalizado][m
 - sin empezar[m
 ```[m
 [m
[32m+[m[32m* Reintroduciendo el sonido[m
[32m+[m[32m```diff[m
[32m+[m[32m+ sonidos de los disparos enemigos[m
[32m+[m[32m+ sonidos de las explosiones enemigas[m
[32m+[m[32m- sonidos de los disparos propios[m
[32m+[m[32m- sonidos de la explosion del prota[m
[32m+[m[32m- música de fondo[m
[32m+[m
[32m+[m[32m```[m
[32m+[m
 BUGS:[m
 ```diff[m
 - El ovni no dispara ¿?[m
[31m-- Los enemigos no aparecen --> tengo una jodienda en displayseconds[m
[32m+[m[32m+ Los enemigos no aparecen --> ya no tengo una jodienda con displayseconds[m
 ```[m
 [m
 ### PLANIFICADO (lo siguiente que pienso hacer)[m
[36m@@ -71,9 +85,6 @@[m [mESTRUCTURA DEL JUEGO[m
 - detectar la destrucción de las oleadas de enemigos[m
 - avance de oleadas cada vez más dificiles[m
 [m
[31m-EFECTOS[m
[31m-- Sonido[m
[31m-	- reintroducir sonido[m
 - Música[m
 	- en los menús[m
 	- durante el juego[m
[1mdiff --git a/Resources/test.xml b/Resources/test.xml[m
[1mindex 1b5a38b..c76d874 100644[m
[1m--- a/Resources/test.xml[m
[1m+++ b/Resources/test.xml[m
[36m@@ -2,7 +2,7 @@[m
 <!-- ************************************************************************ -->[m
 <!-- ************************************************************************ -->[m
 <!-- ************************************************************************ -->[m
[31m-<!-- actual -->[m
[32m+[m[32m<!-- a desaparecer -->[m[41m[m
 <!-- ************************************************************************ -->[m
 <!-- ************************************************************************ -->[m
 <!-- ************************************************************************ -->[m
[36m@@ -101,7 +101,8 @@[m
 -->[m
 [m
 	<enemies>[m
[31m-		<honesto speed="100.0" hp="1.0" points="100" animSetName="enemyHonesto" tipoColision="4" colisionoCon="19" poolBalas="currentBulletsTipoNormal" />[m
[32m+[m		[32m<honesto speed="100.0" hp="1.0" points="100" animSetName="enemyHonesto" tipoColision="4" colisionoCon="19" poolBalas="currentBulletsTipoNormal" sonidoMuerte="sonidos/invaderkilled.wav" sonidoDispara="sonidos/shoot.wav"/>[m[41m[m
[32m+[m		[32m<birojo speed="200.0" hp="1.0" points="100" animSetName="enemyBirojo" tipoColision="4" colisionoCon="19" poolBalas="currentBulletsTipo2" sonidoMuerte="sonidos/invaderkilled.wav" sonidoDispara="sonidos/shoot.wav"/>[m[41m[m
 	</enemies>[m
 [m
 	<bullets>[m
[36m@@ -122,9 +123,18 @@[m
 <!-- ************************************************************************ -->[m
 <!-- ************************************************************************ -->[m
 	<animset>[m
[32m+[m[41m	[m
[32m+[m[41m	[m
 		<!-- me ha resultado más cómodo buscar por nodos <xxx> en vez de atributos <anim name="xxx">... -->[m
 		<!-- OJO!! de momento que exista siempre "default", que la cargo a cañón -->[m
 		<!-- en anim -> fixedImage="1" -> sprite fijo -->[m
[32m+[m[41m	[m
[32m+[m		[32m<!--[m[41m [m
[32m+[m		[32m************************************************************************[m[41m[m
[32m+[m		[32mbalas[m[41m[m
[32m+[m		[32m************************************************************************[m[41m [m
[32m+[m		[32m-->[m[41m[m
[32m+[m[41m[m
 		<balaPlayer>[m
 			<anim name="default" loop="1" fixedImage="0">[m
 				<frame path="bullet_2_blue.png" wait="0.05" size="1.0" />[m
[36m@@ -155,8 +165,15 @@[m
 				<frame path="bullet_purple0001.png" wait="0.1" size="1.0" />[m
 			</anim>[m
 		</balaDirigida>[m
[32m+[m[41m[m
[32m+[m		[32m<!--[m[41m [m
[32m+[m		[32m************************************************************************[m[41m[m
[32m+[m		[32mefectos[m[41m[m
[32m+[m		[32m************************************************************************[m[41m [m
[32m+[m		[32m-->[m[41m[m
[32m+[m[41m		[m
 		<explosion>[m
[31m-			<anim name="default" loop="0" ttl="1">[m
[32m+[m		[32m<anim name="default" loop="0" ttl="1">[m[41m[m
 				<frame path="explosion0.png" wait="0.05" size="0.6" />[m
 				<frame path="explosion1.png" wait="0.05" size="0.9" />[m
 				<frame path="explosion2.png" wait="0.05" size="0.6" />[m
[36m@@ -171,11 +188,25 @@[m
 				<frame path="impacto2.png" wait="0.05" size="1.0" />[m
 			</anim>[m
 		</impacto>[m
[32m+[m[41m		[m
[32m+[m		[32m<!--[m[41m [m
[32m+[m		[32m************************************************************************[m[41m[m
[32m+[m		[32menemigos[m[41m[m
[32m+[m		[32m************************************************************************[m[41m [m
[32m+[m		[32m-->[m[41m[m
[32m+[m[41m		[m
 		<enemyHonesto>[m
[31m-			<anim name="default" loop="0">[m
[31m-				<frame path="aliensprite2.png" size="0.30" />[m
[32m+[m			[32m<anim name="default" loop="1" fixedImage="0">[m[41m[m
[32m+[m				[32m<frame path="aliensprite1.png" wait="0.3" size="0.30" />[m[41m[m
[32m+[m				[32m<frame path="aliensprite2.png" wait="0.3" size="0.30" />[m[41m[m
 			</anim>[m
 		</enemyHonesto>[m
[32m+[m		[32m<enemyBirojo>[m[41m[m
[32m+[m			[32m<anim name="default" loop="1" fixedImage="0">[m[41m[m
[32m+[m				[32m<frame path="Spaceship01.png" wait="0.7" size="0.35" />[m[41m[m
[32m+[m				[32m<frame path="Spaceship15.png" wait="0.7" size="0.35" />[m[41m[m
[32m+[m			[32m</anim>[m[41m[m
[32m+[m		[32m</enemyBirojo>[m[41m[m
 	</animset>[m
 [m
 </definitions>[m
\ No newline at end of file[m
[1mdiff --git a/Sources/AnimSprites.cpp b/Sources/AnimSprites.cpp[m
[1mindex c879e4c..6e21afc 100644[m
[1m--- a/Sources/AnimSprites.cpp[m
[1m+++ b/Sources/AnimSprites.cpp[m
[36m@@ -39,7 +39,6 @@[m [mvoid AnimSprites::animation::addFrame(frame * f){[m
 AnimSprites::frame::frame(Node *parent, const char *spritePath, float displaySeconds, float spriteScale){[m
 	// TODO: no har�a falta un physicsbody?[m
 	displaySeconds_ = displaySeconds;[m
[31m-	displaySeconds_ = 30.0f;[m
 	sprite_ = Sprite::createWithSpriteFrameName(spritePath);[m
 	sprite_->setScale(spriteScale);[m
 	sprite_->setVisible(false);[m
[1mdiff --git a/Sources/Enemy.cpp b/Sources/Enemy.cpp[m
[1mindex 88ae376..9b5f397 100644[m
[1m--- a/Sources/Enemy.cpp[m
[1m+++ b/Sources/Enemy.cpp[m
[36m@@ -10,6 +10,7 @@[m
 #include "Movimiento.h"[m
 #include "AnimSprites.h"[m
 #include "XmlHelper.h"[m
[32m+[m[32m#include "SimpleAudioEngine.h"[m[41m[m
 [m
 Enemy::Enemy(){[m
 	GameActor::GameActor();[m
[36m@@ -18,6 +19,8 @@[m [mEnemy::Enemy(){[m
 	//tipoEnemigo_ = -1;[m
 	pointsOnDeath_ = 0;[m
 	poolMisBalas_ = nullptr;[m
[32m+[m	[32msonidoDispara_ = "";[m[41m[m
[32m+[m	[32msonidoMuerte_ = "";[m[41m[m
 }[m
 [m
 Enemy::Enemy(tiposEnemigo tipo){[m
[36m@@ -28,12 +31,12 @@[m [mEnemy::Enemy(tiposEnemigo tipo){[m
 	tipoEnemigo_ = tipo;[m
 	pointsOnDeath_ = 0;[m
 	poolMisBalas_ = nullptr;[m
[32m+[m	[32msonidoDispara_ = "";[m[41m[m
[32m+[m	[32msonidoMuerte_ = "";[m[41m[m
 }[m
 [m
 Enemy::~Enemy(){}[m
 [m
[31m-[m
[31m-[m
 void Enemy::initEnemy(Node *nodo){[m
 	Game *gameInstance = Game::getInstance();[m
 	//funcionMovimientoActual = nullptr;[m
[36m@@ -121,6 +124,11 @@[m [mvoid Enemy::impacto(float dmg){[m
 			SpaceInvaders::porcenInvadersVivos = (float)SpaceInvaders::numInvadersVivos / (float)SpaceInvaders::numInvadersInicial;[m
 		}[m
 [m
[32m+[m		[32mif(sonidoMuerte_ != ""){[m[41m[m
[32m+[m			[32mauto audio = CocosDenshion::SimpleAudioEngine::getInstance();[m[41m[m
[32m+[m			[32maudio->playEffect(sonidoMuerte_.c_str());[m[41m[m
[32m+[m		[32m}[m[41m[m
[32m+[m[41m[m
 	} else{[m
 		// no muere[m
 [m
[36m@@ -189,6 +197,11 @@[m [mvoid Enemy::dispara(){[m
 	if(poolMisBalas_){[m
 		//CCLOG("Enemigo dispara!");[m
 [m
[32m+[m		[32mif(sonidoDispara_!=""){[m[41m[m
[32m+[m			[32mauto audio = CocosDenshion::SimpleAudioEngine::getInstance();[m[41m[m
[32m+[m			[32maudio->playEffect(sonidoDispara_.c_str());[m[41m[m
[32m+[m		[32m}[m[41m[m
[32m+[m[41m[m
 		if(animSprites_){[m
 			Pool::activa(*poolMisBalas_, animSprites_->getPosition());[m
 		}[m
[36m@@ -209,4 +222,16 @@[m [mvoid Enemy::dispara(){[m
 [m
 void Enemy::setPoints(int points){[m
 	pointsOnDeath_ = points;[m
[31m-}[m
\ No newline at end of file[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mvoid Enemy::setSonidoDispara(const char *pathSonido){[m[41m[m
[32m+[m	[32msonidoDispara_ = std::string(pathSonido);[m[41m[m
[32m+[m	[32mauto audio = CocosDenshion::SimpleAudioEngine::getInstance();[m[41m[m
[32m+[m	[32maudio->preloadEffect(pathSonido);[m[41m[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mvoid Enemy::setSonidoMuerte(const char *pathSonido){[m[41m[m
[32m+[m	[32msonidoMuerte_ = std::string(pathSonido);[m[41m[m
[32m+[m	[32mauto audio = CocosDenshion::SimpleAudioEngine::getInstance();[m[41m[m
[32m+[m	[32maudio->preloadEffect(pathSonido);[m[41m[m
[32m+[m[32m}[m[41m[m
[1mdiff --git a/Sources/Enemy.h b/Sources/Enemy.h[m
[1mindex c00c615..203f8e2 100644[m
[1m--- a/Sources/Enemy.h[m
[1m+++ b/Sources/Enemy.h[m
[36m@@ -27,6 +27,8 @@[m [mpublic:[m
 	void impacto(float) override;[m
 	void update(float deltaT) override;[m
 	void setPoints(int);[m
[32m+[m	[32mvoid setSonidoDispara(const char *);[m[41m[m
[32m+[m	[32mvoid setSonidoMuerte(const char *);[m[41m[m
 [m
 	// todo: sobrecargarlo desde GameActor???[m
 	void dispara();[m
[36m@@ -58,4 +60,6 @@[m [mprivate:[m
 	void initEnemy(Node *nodo, const char *pathSprite, const char *rutaSonidoMuerte, float scale, float rotation, float hp, int points, std::vector<Bullet *> *poolMisBalas = nullptr);[m
 	//void liberaVectorBalas(std::vector<Bullet *> bs);[m
 	//std::vector<Bullet *> reservaVectorBalas(int n);[m
[32m+[m	[32mstd::string sonidoDispara_;[m[41m[m
[32m+[m	[32mstd::string sonidoMuerte_;[m[41m[m
 };[m
[1mdiff --git a/Sources/GameActor.cpp b/Sources/GameActor.cpp[m
[1mindex 0e36cdc..060eab8 100644[m
[1m--- a/Sources/GameActor.cpp[m
[1m+++ b/Sources/GameActor.cpp[m
[36m@@ -230,15 +230,9 @@[m [mvoid GameActor::createAnimationPool(Node *nodo, std::vector<GameActor*>& pool, i[m
 }[m
 [m
 void GameActor::setTTL(float ttl){[m
[31m-[m
 	ttl_ = ttl;[m
[31m-[m
 }[m
 [m
[31m-//void GameActor::setPoints(int points){[m
[31m-//	gameActorPoints_ = points;[m
[31m-//}[m
[31m-//[m
 void GameActor::setHP(float hp){[m
 	gameActorHP_ = hp;[m
 	gameActorHPInicial_ = hp;[m
[1mdiff --git a/Sources/XmlHelper.cpp b/Sources/XmlHelper.cpp[m
[1mindex 86c9985..7e0e23b 100644[m
[1m--- a/Sources/XmlHelper.cpp[m
[1m+++ b/Sources/XmlHelper.cpp[m
[36m@@ -185,6 +185,8 @@[m [mEnemy *XmlHelper::loadEnemy(Node *parentNode, const char *xmlEnemyDef){[m
 		int colisionoCon = selectedNode.attribute("colisionoCon").as_int();[m
 		const char *poolName = (char *)selectedNode.attribute("poolBalas").value();[m
 		int points = selectedNode.attribute("points").as_int();[m
[32m+[m		[32mconst char *sonidoDispara = (char *)selectedNode.attribute("sonidoDispara").value();[m[41m[m
[32m+[m		[32mconst char *sonidoMuerte = (char *)selectedNode.attribute("sonidoMuerte").value();[m[41m[m
 [m
 		// busca y carga la animaci�n (o el sprite)[m
 		tmpAnim = loadAnimation(parentNode, animSetName);[m
[36m@@ -199,6 +201,8 @@[m [mEnemy *XmlHelper::loadEnemy(Node *parentNode, const char *xmlEnemyDef){[m
 		tmp->setHP(hp);[m
 		tmp->setPoints(points);[m
 		tmp->gameActorSpeed_ = speed;[m
[32m+[m		[32mtmp->setSonidoDispara(sonidoDispara);[m[41m[m
[32m+[m		[32mtmp->setSonidoMuerte(sonidoMuerte);[m[41m[m
 [m
 		//tmp->movimiento_ = [m
 [m

![nice screenshot!](/Resources/HelloCocos.jpg)
<!--![status](https://dl2.pushbulletusercontent.com/JXwF9psx3Dcsh9vST2ElAgx3TNk398EX/IMG_1969.PNG)-->
<!--img src="http://www.cocos2d-x.org/attachments/801/cocos2dx_portrait.png" width=100-->

<!-- colores
```diff
+ esto sale en verde
-  y esto en rojo
```

tabla col | col
-- | --
fila | fila
-->

# cocos2d-x
## aprendiendo c++ con cocos

Git user attention
-------------------

* Cómo clonar la repo con Git

        $ git clone https://github.com/iglo0/HelloCocos.git

---

### RESUMEN (GENERAL) DEL ESTADO ACTUAL

- Salen enemigos en formato "Space Invaders" que disparan aleatoriamente
- Hay un ovni gordo que dispara también
- Control de una nave con sus disparos
- Menú inicial y pruebas
- Inicio del juego
- Game over

### ÚLTIMO HECHO

- salen las típicas "casitas" destructibles tras las que te puedes proteger
- configuración con archivos en vez de #defines
- rediseño de código (segunda de 'n' :)
- comenzada planificacion de tareas (este readme)
- limpieza general de #includes del proyecto
- Atlas de sprites en una sola textura (.plist y spritesheet.png generado con TexturePacker)
- colisiones precisas funcionando (faltan por definir sprites aun)

----
# TRABAJO EN CURSO
En desarrollo [Estimación]
```diff

ESTRUCTURA DEL JUEGO
+ creada otra máquina de estados para el juego, intentando implementarla con el patrón "Estado"
- reimplementando las funciones de movimiento (balas mayormente)

GRÁFICOS
+ Colisiones precisas. Definidos el resto de sprites

LÓGICA DEL JUEGO
[en pausa]Añadir comportamientos a los disparos: preparando la estructura para hacer esto
+ tiros en la dirección general del prota [03/06/17] [hecho hacia el 01/07]
+ disparo de proyectiles que sigan trayectorias (seno/coseno) [04/06/17] [hecho hacia el 01/07]
- disparo de múltiples proyectiles en abanico (bosses?) [07/06/17]

```


### PLANIFICADO (lo siguiente que pienso hacer)

ESTRUCTURA DEL JUEGO
- detectar la destrucción de las oleadas de enemigos
- avance de oleadas cada vez más dificiles

EFECTOS
- Sonido
	- reintroducir el sonido


### POR PLANIFICAR (tareas pendientes que ir planificando)
LÓGICA DEL JUEGO
- Añadir comportamientos a las naves
	- enemigos que salen y entran en la formación (por ejemplo: Galaxian/Galaga)
		- que sigan splines? LOL ni siquiera he empezado por y=x
	- que los enemigos disparen más cuantos menos queden
GRÁFICOS
- Independencia de resolución

### IDEAS (a hacer ~~cuando las ranas críen pelo~~ más adelante)

En ningún orden particular:

LÓGICA DEL JUEGO
- Añadir las casitas (las típicas del Space Invaders que protegen al prota)
	- que sean destructibles
		- que se vayan destruyendo ahí donde sean disparadas
- Añadir power-ups
	- nuevos tipos de disparo para el jugador
		- tiro a tiro
		- metralleta
		- láser
		- que gasten energía o munición
			- gestión de la energía para armas, escudos, motores... si gastas mucho de uno que afecte al resto
			- gui de esto
	- escudos
		- puntos de golpe fijos
		- energía recargable
	- power-ups usables por los enemigos (no que los cojan sino poder crear enemigos con ellos)

EFECTOS
- Sonido
	- buscar una alternativa que permita variar pitch y otros parámetros en windows (el motor sonoro por defecto no lo hace)
	- ojo a los parones al reproducir sonidos
- Música
	- en los menús
	- durante el juego
	- para los bosses
	- de game over
	- de records
- Usar Acciones (de cocos) para darle brillo al juego
	- "terremotos"
	- rotación y zoomeado de sprites
	- movimientos con aceleración y frenado (no bruscos)
- Parpadeo de las naves (periodo de invulnerabilidad)

GRÁFICOS
- Animaciones
	- Animaciones sencillas (en loop) de los objetos
	- Poder definir animaciones en loop y one-off

ESTRUCTURA DEL JUEGO
- jefes cada "x" oleadas

GENERAL
- Tabla de records
	- pantalla para verla, o reutilizar los menús... o montarla encima de la del juego tras el game over...
	- poder definir "n" entradas
	- Almacenamiento de tabla de records
- Definición de niveles en archivos de texto
	- cantidad y comportamiento de los enemigos por nivel
	- o...
	- definición de niveles a manija, con enemigos que salgan en puntos fijos o al de tiempo
- Separar el manejo de datos, lógica y memoria de la clase de Pool (ir viendo Pool_manager)	

### ~~BUGS~~ OUTSTANDING FEATURES
- a veces el jugador puede perder dos vidas juntas. ¿Es por que le dan dos balas? ¿o no? no lo tengo claro
- los mensajes que deberían salir centrados no lo hacen (calculo la posición solo al principio y no lo actualizo cuando cambio el mensaje)
- las colisiones precisas están mal gestionadas: cada sprite tiene varios polígonos y es posible colisionar con varios a la vez. No es problema salvo para la nave del jugador, que puede perder las vidas de 3 en 3 :-D
- la gestión de movimiento de los space invaders tiene un problema, si destruyo al que uso de guía se va el movimiento a tomar por saco.

### DUDAS TÉCNICAS

- ¡ninguna!

![nice screenshot!](/Resources/HelloCocos.jpg)
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

```diff
+ limpieza general de #includes del proyecto
+ comenzada planificacion de tareas (este readme)
```
- configuración con archivos en vez de #defines
- rediseño de código (primera de 'n' :)

----
# TRABAJO EN CURSO
En desarrollo | Estimacion
-- | --
??? | ???
---

### PLANIFICADO (lo siguiente que pienso hacer)
???

### POR PLANIFICAR (tareas pendientes que ir planificando)
???

### IDEAS (a hacer ~~cuando las ranas críen pelo~~ más adelante)

En ningún orden particular:

LÓGICA DEL JUEGO
- Añadir comportamientos a las naves
	- enemigos que salen y entran en la formación (por ejemplo: Galaxian/Galaga)
		- que sigan splines? LOL ni siquiera he empezado por y=x
	- que los enemigos disparen más cuantos menos queden
- Añadir comportamientos a los disparos
	- tiros en la dirección general del prota (ecuación lineal)
	- disparo de proyectiles que sigan trayectorias (seno/coseno)
        - disparo de múltiples proyectiles en abanico (bosses?)
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
	- reintroducir el sonido
	- ojo a los parones al reproducir sonidos
	- que funcione más o menos solo (los objetos que reproduzcan los sonidos correspondientes cuando les toque)
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
- Animaciones
	- Poder definir animaciones en loop y one-off

GRÁFICOS
- Colisiones precisas
	- trazar las formas poligonales de cada sprite relevante
- Atlas de sprites en una sola textura
- Animaciones
	- Animaciones sencillas (en loop) de los objetos
	- Poder definir animaciones en loop y one-off
- Independencia de resolución
        - fijar un ratio de pantalla

ESTRUCTURA DEL JUEGO
- detectar la destrucción de las oleadas de enemigos
- avance de oleadas cada vez más dificiles
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
	

### ~~BUGS~~ OUTSTANDING FEATURES

- el comportamiento de los "space invaders" no se ajusta a las dimensiones del grupo (debieran desplazarse por toda la pantalla y ajustarse cuando van cayendo)
- los mensajes que deberían salir centrados no lo hacen (calculo la posición solo al principio y no lo actualizo cuando cambio el mensaje)


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
+ colisiones precisas funcionando (faltan por definir sprites aun)
- De algún modo ahora los sprites con colisiones precisas trabajan con coordenadas distintas, ahora el 0,0 no está en el centro. Lo tengo que mirar.
```
- configuración con archivos en vez de #defines
- rediseño de código (primera de 'n' :)
- comenzada planificacion de tareas (este readme)
- limpieza general de #includes del proyecto
- Atlas de sprites en una sola textura (.plist y spritesheet.png generado con TexturePacker)

----
# TRABAJO EN CURSO
En desarrollo | Estimacion
-- | --
Colisiones precisas - trazar las formas poligonales de cada sprite relevante | ~~19/05/2017~~ 24/05
Revisar el bug de la posición de los sprites | 26/05
---

### PLANIFICADO (lo siguiente que pienso hacer)



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
	- reintroducir el sonido
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
- Separar el manejo de datos, lógica y memoria de la clase de Pool (ir viendo Pool_manager)	

### ~~BUGS~~ OUTSTANDING FEATURES

- el comportamiento de los "space invaders" no se ajusta a las dimensiones del grupo (debieran desplazarse por toda la pantalla y ajustarse cuando van cayendo)
- los mensajes que deberían salir centrados no lo hacen (calculo la posición solo al principio y no lo actualizo cuando cambio el mensaje)

### DUDAS TÉCNICAS

- ???

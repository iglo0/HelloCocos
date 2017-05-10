![nice screenshot!](/Resources/HelloCocos.jpg)
<!--img src="http://www.cocos2d-x.org/attachments/801/cocos2dx_portrait.png" width=100-->

<!-- colores
```diff
+ esto sale en verde
-  y esto en rojo
```
-->

# cocos2d-x
## aprendiendo c++ con cocos

Git user attention
-----------------------

* Cómo clonar la repo con Git

        $ git clone https://github.com/iglo0/HelloCocos.git

---

### RESUMEN DEL ESTADO ACTUAL

- Salen enemigos en formato "Space Invaders" que disparan aleatoriamente
- Hay un ovni gordo que dispara también
- Control de una nave que dispara proyectiles sencillos
- Menú inicial y pruebas
- Inicio del juego
- Game over

### ÚLTIMO HECHO

```diff
+ configuración con archivos en vez de #defines
```

----
# TRABAJO EN CURSO
En desarrollo | Estimacion
----------------- | -------------
planificación del desarrollo | ~11/05/17
actualizando readme :P | ~11/05/17

---

### PLANIFICADO

- 

### POR PLANIFICAR

EFECTOS
- Sonido
        - buscar una alternativa que permita variar pitch y otros parámetros en windows (el motor sonoro por defecto no lo hace)
- Música
- Animaciones
	- Poder definir animaciones en loop y one-off

GRÁFICOS
- Colisiones precisas
	- trazar las formas poligonales de cada sprite relevante
- Atlas de sprites en una sola textura
- Independencia de resolución
        - fijar un ratio de pantalla

COMPORTAMIENTO
- Añadir comportamientos a las naves
- Añadir comportamientos a los disparos
        - disparo de múltiples proyectiles en abanico (enemigos)
	- disparo de proyectiles en todas direcciones (no solo rectos como hasta ahora)
	- disparo de proyectiles que sigan trayectorias (seno/coseno)

ESTRUCTURA DEL JUEGO
- detectar la destrucción de las oleadas de enemigos
- avance de oleadas cada vez más dificiles
- jefes cada "x" oleadas

GENERAL
- Tabla de records
- Almacenamiento de tabla de records
- Definición de niveles en archivos de texto

### IDEAS

- Añadir power-ups
	- que usen los enemigos

EFECTOS
- Usar Acciones (de cocos) para darle brillo al juego
	- "terremotos"
	- rotación y zoomeado de sprites
	- movimientos con aceleración y frenado (no bruscos)
- Parpadeo de las naves (periodo de invulnerabilidad)
	
	
	

### BUGS

- el comportamiento de los "space invaders" no se ajusta a las dimensiones del grupo (debieran desplazarse por toda la pantalla y ajustarse cuando van cayendo)
- los mensajes que deban salir en el centro, que lo hagan

---


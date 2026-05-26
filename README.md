# Proyecto Final — Escena inspirada en Minecraft

Escena interactiva en OpenGL que recrea un ambiente estilo Minecraft con personajes animados, ciclo de día/noche, cámara libre y múltiples técnicas de animación (keyframes, máquina de estados y esqueleto FBX).

---

## Requisitos previos

Antes de clonar y abrir el proyecto se debe instalar **Visual Studio Community**: [https://visualstudio.microsoft.com/vs/community/](https://visualstudio.microsoft.com/vs/community/)

Durante la instalación, selecciona el componente **"Desarrollo para escritorio con C++"**.

---

## Clonar el repositorio

```bash
git clone https://github.com/not-markosh/proyecto-computacion-grafica-teoria.git
```

---

## Opciones para ejecutar

### Opción 1 — Visual Studio Community

1. Abre la carpeta del repositorio clonado.
2. Haz doble clic en el archivo **`.slnx`** para abrirlo en Visual Studio Community.
3. Selecciona la configuración **Release** o **Debug** con **x86** y presiona el botón de play para compilar y ejecutar.

### Opción 2 — Ejecutable precompilado

Descarga directamente el ejecutable: [`EjecutableProyectoFinal.exe`](https://github.com/not-markosh/proyecto-computacion-grafica-teoria/blob/main/EjecutableProyectoFinal.exe)

- Al ejecutarlo, **las librerías necesarias se instalarán automáticamente**.
- Para desinstalarlo limpiamente, en la misma carpeta encontrarás un **Uninstaller** que elimina todos los archivos generados.

---

## Controles

### Cámara
| Tecla | Acción |
|---|---|
| `W` / `↑` | Mover hacia adelante |
| `S` / `↓` | Mover hacia atrás |
| `A` / `←` | Mover a la izquierda |
| `D` / `→` | Mover a la derecha |
| Mouse | Rotar la cámara |
| `ESC` | Cerrar la ventana |

### Escena
| Tecla | Acción |
|---|---|
| `N` | Activar / pausar ciclo día-noche |
| `,` / `.` | Retroceder / avanzar el sol manualmente |
| `SPACE` | Encender / apagar luz puntual |

### Animaciones de personajes
| Tecla | Acción |
|---|---|
| `B` | Animación de **Steve** (caminar) |
| `V` | Animación de **Alex** (idle) |
| `C` | Animación de **Zombie** (pushups) |
| `X` | Animación de **Ghast** (flotando) |
| `G` | Animación de **Cangrejo** (saludo) |
| `Q` | Animación de **Pato** (caminar) |
| `Z` | Animación de **Zorro** (sentar) |
| `P` + `L`  | Animación de **Gato** (saltar) |

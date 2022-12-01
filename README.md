Malleable-Kvazaar
=======
Una versión ligeramente modificada de Kvazaar para soportar maleabilidad.
El proyecto original está [aquí](https://github.com/ultravideo/kvazaar).


# Descripción
Esta implementación de Kvazaar divide la secuencia de entrada en multiples bloques formados por `NUM_FRAMES_PER_BATCH` frames, imprimiendo por salida estandar los FPS obtenidos para cada bloque.

Al inicio de cada bloque, el usuario debe indicar el número de hilos a utilizar para codificarlo, mediante la entrada estándar en la forma `nThs:<nHilos>` (ver formato en los ejemplos).

Al finalizar la ejecución, la aplicación imprime el token `END` seguido de los FPS globales obtenidos durante toda la ejeción (`Overall_FPS`). Estos FPS contemplan el tiempo de interacción con el usuario, siendo una medida relativa del tiempo de espera de la aplcación en comparación con los FPS obtenidos en cada bloque procesado.


Notas:
1) El número de frames que forma cada bloque puede modificarse mediante variables de entorno antes del inicio de la ejeción (`export NUM_FRAMES_PER_BATCH=<nFrames>`)
2) La aplicación lee los cambios de hilos por la entrada estándar (stdin), e imprime los FPS obtenidos por la salida estándar (stdout). De forma adicional, información extra del proceso de codificación se muestra por la salida de error (stderr)
3) Si el último bloque no está completo, los FPS obtenidos en ese bloque no se muestran para evitar medidas que puedan llevar a errores.

## Instalación
Una versión completa del proceso de compilación e instalación se puede encontrar en el repo original (incluido instrucciones para windows). [ENLACE](https://github.com/ultravideo/kvazaar)

En Linux, la forma más rápida de compilar e instalarlo es mediante autotools:
```
cd kvazaar_folder
autoreconf -fiv
./configure --prefix=<rutaInstalación>
make -j <nCores>
make install
```

# Ejemplo de ejecución
```
$NUM_FRAMES_PER_BATCH=100 taskset -c 0-19 ./kvazaar --input QuarterBackSneak1_1280x720_30p.yuv --output /dev/null --preset=ultrafast --qp=22 --owf=0 --threads=20 2>/dev/null
nThs:3
FPS:19.459319
nThs:20
FPS:31.498376
nThs:2
FPS:13.935963
nThs:8
END
Overall_FPS:9.908793
```

# Acknowledgements
This work was partially supported by the EU (FEDER) and Spanish MINECO (TIN2015-65277-R, RTI2018-093684-B-I00), Spanish MECD (GA. No. FPU15/02050), CM (S2018/TCS-4423), and the Complutense University under grant GA. No. PR65/19-22445

Conversor de AFN a AFD usando un archivo XML
============================================

Este proyecto es un conversor de Autómatas Finitos No Deterministas (AFN) a Autómatas Finitos Deterministas (AFD) utilizando un archivo XML como entrada. El objetivo principal es proporcionar una herramienta que facilite la conversión de AFN a AFD de una manera sencilla y eficiente.

Prerrequisitos
--------------

Antes de utilizar esta herramienta, asegúrate de cumplir con los siguientes requisitos:

### Para usuarios de Windows

1.  **Scoop**: Si aún no tienes Scoop instalado, puedes obtenerlo desde [scoop.sh](https://scoop.sh/#/).

2.  **WinFlexBison**: Instala WinFlexBison usando Scoop ejecutando el siguiente comando:

    bash```
    scoop install main/winflexbison
    ```
3. **Makefile**: Instala Makefile utilizando Scoop con el siguiente comando:

    bash```
    scoop install main/make
    ```

Cómo ejecutar la herramienta
----------------------------

Una vez que hayas cumplido con los prerrequisitos, sigue estos pasos para ejecutar la herramienta:

1.  Abre una terminal en la ubicación del proyecto.

2.  Ejecuta el siguiente comando para generar la librería que te permitirá trabajar con los archivos desde otro archivo `.c`:

    o `.cpp`

3.  `make`

Además, encontrarás dos archivos llamados `test.c` y `test.cpp` en el proyecto. Estos archivos contienen ejemplos de cómo llamar a las funciones de la librería para realizar la conversión de AFN a AFD.

Contribuciones
--------------

¡Las contribuciones son bienvenidas! Si deseas mejorar esta herramienta o agregar nuevas características, no dudes en crear una solicitud de extracción (pull request) en el repositorio.

Licencia
--------

Este proyecto está bajo la Licencia MIT. Consulta el archivo `LICENSE` para obtener más detalles.

* * * * *

¡Esperamos que esta herramienta te sea útil para convertir Autómatas Finitos No Deterministas (AFN) a Autómatas Finitos Deterministas (AFD) utilizando archivos XML como entrada! Si tienes alguna pregunta o necesitas ayuda adicional, no dudes en contactarnos.
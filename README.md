# Sistema de Alarma con Raspberry Pi

# Descripción del proyecto
Este proyecto consiste en el desarrollo de un sistema de alarma utilizando una Raspberry Pi y programación en Python. 
El sistema detecta movimiento mediante un sensor PIR y activa diferentes avisos visuales y acústicos mediante LEDs y un zumbador. 
Además, incluye un sistema de contraseña para desactivar la alarma y una llamada automática mediante Asterisk en caso de fallo.

# Funcionamiento
- Detección de movimiento mediante sensor PIR.
- LED amarillo como aviso inicial.
- LED rojo como estado de alarma.
- Introducción de contraseña para desactivar el sistema.
- Activación automática de aviso si la contraseña es incorrecta.

# Modificaciones realizadas
Durante el desarrollo del proyecto se han implementado las siguientes mejoras:

- Mejora del patrón acústico del buzzer mediante pitidos cortos.
- Implementación de bloqueo tras varios intentos fallidos.
- Validación de contraseña para asegurar un formato correcto.
- Mejora en la organización del código y control del sensor PIR.

# Tecnologías utilizadas
- Raspberry Pi
- Python
- Librería RPi.GPIO
- GitHub Desktop
- Control de versiones con Git





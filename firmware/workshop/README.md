# workshop

Firmware final utilizado en el Taller IoT para controlar los brazos robóticos.

## Características

- Control de 6 servos (BASE, BASE_JOINT, ELBOW, WRIST_YAW, WRIST_PITCH, CLAW).
- Control de un LED y una tira RGB.
- Conexión a WiFi y MQTT para recibir comandos.

## Configuración

1. Configura las credenciales de WiFi y MQTT en el archivo `include/secrets.h`.
2. Compila y sube el firmware al ESP32 utilizando PlatformIO.

## Uso

- El ESP32 se conecta al broker MQTT y suscribe a los tópicos definidos en `secrets.h`.
- Envía comandos MQTT para controlar los servos y los LEDs.

----------

<p align="middle">    
    <img src="https://ieeeitba.org.ar/assets/image/general-icons/itba.png" align="center" alt="ITBA Logo" width="75px" />
    <a href="https://ieeeitba.org.ar/"><img src="https://ieeeitba.org.ar/assets/image/general-icons/ieee-itba.png" align="center" alt="IEEE ITBA Logo" width="100px" /></a>
    <img src="https://ieeeitba.org.ar/assets/image/general-icons/ieee.png" align="center" alt="IEEE Logo" width="75px" />
</p>

<p align="middle">    
    <a href="https://github.com/IEEESBITBA"><img src="https://cdn-icons-png.flaticon.com/512/25/25231.png" align="center" alt="GitHub Repository" width="20px" /></a>
    <a href="https://www.linkedin.com/company/ieee-itba/"><img src="https://content.linkedin.com/content/dam/me/business/en-us/amp/brand-site/v2/bg/LI-Bug.svg.original.svg" align="center" alt="LinkedIn Organization" width="20px" /></a>
    <a href="https://www.instagram.com/ieee.itba/"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/e7/Instagram_logo_2016.svg/768px-Instagram_logo_2016.svg.png" align="center" alt="Instagram Profile" width="20px" /></a>
</p>
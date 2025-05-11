# Documentación

Este directorio contiene la documentación del proyecto.

- [Documentación](#documentación)
  - [Configuración del entorno de red](#configuración-del-entorno-de-red)
    - [Configuración del router](#configuración-del-router)
    - [Acceso al router](#acceso-al-router)
    - [Asignación de IP fija al host](#asignación-de-ip-fija-al-host)
  - [Configuración de servicios](#configuración-de-servicios)
    - [Broker MQTT (Mosquitto)](#broker-mqtt-mosquitto)
      - [Configuración del Broker MQTT](#configuración-del-broker-mqtt)
      - [Ejecución del Broker MQTT](#ejecución-del-broker-mqtt)
      - [Verificación de conexiones](#verificación-de-conexiones)
      - [Uso en el proyecto](#uso-en-el-proyecto)
      - [Observaciones para Windows](#observaciones-para-windows)
    - [Node-RED](#node-red)
      - [Ejecución con el script `run_nodes.sh`](#ejecución-con-el-script-run_nodessh)
      - [Ejecución manual de Node-RED](#ejecución-manual-de-node-red)
      - [Acceso a las instancias](#acceso-a-las-instancias)

## Configuración del entorno de red

### Configuración del router

- **WiFi SSID**: `IEEE_ITBA_IOT`
- **WiFi PASS**: `ieee2024`
- **Gateway IP**: `192.168.0.1`
- **Usuario**: `admin`
- **Contraseña**: `admin`

### Acceso al router

Para realizar configuraciones en el router, como asignar IPs fijas o cambiar las credenciales de WiFi, sigue estos pasos:

1. Conéctate a la red WiFi del router (SSID y PASS).
2. Abre un navegador web y accede a la interfaz de configuración del router ingresando la dirección IP del gateway en la barra de direcciones.
3. Ingresa las credenciales de administrador (usuario y contraseña).

### Asignación de IP fija al host

Es importante que la computadora host tenga una IP fija para garantizar la comunicación con los ESP32 y las instancias de Node-RED. Configura el router para asignar la IP fija `192.168.0.100` a la computadora host utilizando su dirección MAC (`XX:XX:XX:XX:XX:XX`).

> `192.168.0.100` es la asignada como IP del host en todos los brazos a través del `#define __MQTT_SERVER_IP`; si se la quiera cambiar, debe cambiarse dicho `#define` en el firmware del proyecto `workshop`.


## Configuración de servicios

### Broker MQTT (Mosquitto)

Mosquitto es un broker MQTT que permite la comunicación entre dispositivos IoT mediante el protocolo MQTT. En este proyecto, se utiliza para gestionar la comunicación entre los brazos robóticos y las interfaces de control. 

#### Configuración del Broker MQTT

El archivo de configuración `mosquitto.conf` debe estar correctamente configurado para permitir conexiones externas y garantizar la comunicación entre los dispositivos. Asegúrate de que el archivo contenga las siguientes líneas:

```bash
listener 1883 0.0.0.0
allow_anonymous true
```

#### Ejecución del Broker MQTT

Para iniciar el broker MQTT, utiliza el siguiente comando desde el directorio donde se encuentra el archivo de configuración:

```bash
mosquitto -c mosquitto.conf
```

#### Verificación de conexiones

Una vez iniciado Mosquitto, puedes verificar que está funcionando correctamente utilizando herramientas como `mosquitto_sub` y `mosquitto_pub` para suscribirte y publicar mensajes en un tema de prueba. Por ejemplo:

1. En una terminal, suscríbete a un tema de prueba:
    ```bash
    mosquitto_sub -h 192.168.0.100 -t test/topic
    ```

2. En otra terminal, publica un mensaje en el mismo tema:
    ```bash
    mosquitto_pub -h 192.168.0.100 -t test/topic -m "Hola, MQTT"
    ```

Si todo está configurado correctamente, deberías ver el mensaje publicado en la terminal suscrita.

#### Uso en el proyecto

En este proyecto, Mosquitto se utiliza para transmitir comandos y datos entre los brazos robóticos y las interfaces de control desarrolladas en Node-RED. *Asegúrate de que el broker esté en ejecución antes de iniciar las instancias de Node-RED para garantizar una comunicación fluida.*

#### Observaciones para Windows

En Windows, el servicio de Mosquitto suele ejecutarse automáticamente. Verifica que el archivo de configuración en `C:/Program Files/mosquitto/mosquitto.conf` tenga la configuración mencionada anteriormente. Si realizas cambios, reinicia el servicio de Mosquitto.

### Node-RED

Node-RED se utiliza para controlar los brazos robóticos mediante interfaces gráficas y flujos de datos. Puedes ejecutar las instancias de Node-RED utilizando el script `run_nodes.sh` o manualmente.

#### Ejecución con el script `run_nodes.sh`

El script `run_nodes.sh` ubicado en `host/scripts/` permite levantar automáticamente 6 instancias de Node-RED, una para cada brazo robótico, en los siguientes puertos:

- **Brazo 1**: Puerto `1871`
- **Brazo 2**: Puerto `1872`
- **Brazo 3**: Puerto `1873`
- **Brazo 4**: Puerto `1874`
- **Brazo 5**: Puerto `1875`
- **Brazo 6**: Puerto `1876`

Para ejecutar el script, utiliza el siguiente comando desde el directorio `host/scripts/`:

```bash
./run_nodes.sh
```

El script creará un directorio de usuario para cada instancia (`node-red-<puerto>/`) y ejecutará Node-RED en el puerto correspondiente.

#### Ejecución manual de Node-RED

Si prefieres ejecutar las instancias manualmente, sigue estos pasos:

1. Crea un directorio de usuario para cada instancia de Node-RED. Por ejemplo, para el puerto 1871:

    ```bash
    mkdir -p node-red-1871
    ```

2. Ejecuta Node-RED especificando el puerto y el directorio de usuario. Por ejemplo, para el puerto 1871:
   
    ```bash
    node-red -p 1871 -u node-red-1871/
    ```

3. Repite estos pasos para cada puerto (1871 a 1876), cambiando el número de puerto y el nombre del directorio.

#### Acceso a las instancias

Puedes acceder a las interfaces de Node-RED desde cualquier navegador utilizando la IP del host y el puerto correspondiente:

* Editor de Node-RED:
  * Brazo 1: `http://192.168.0.100:1871`
  * Brazo 2: `http://192.168.0.100:1872`
  * Brazo 3: `http://192.168.0.100:1873`
  * Brazo 4: `http://192.168.0.100:1874`
  * Brazo 5: `http://192.168.0.100:1875`
  * Brazo 6: `http://192.168.0.100:1876`
* Editor de Node-RED:
  * Brazo 1: `http://192.168.0.100:1871/ui`
  * Brazo 2: `http://192.168.0.100:1872/ui`
  * Brazo 3: `http://192.168.0.100:1873/ui`
  * Brazo 4: `http://192.168.0.100:1874/ui`
  * Brazo 5: `http://192.168.0.100:1875/ui`
  * Brazo 6: `http://192.168.0.100:1876/ui`
  
Si necesitas detener una instancia de Node-RED, simplemente cierra la terminal correspondiente o utiliza el comando kill para finalizar el proceso.

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
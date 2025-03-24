#!/bin/bash

# Ruta al archivo de configuración de Mosquitto
CONFIG_FILE="/etc/mosquitto/mosquitto.conf"

# Verificar si el archivo de configuración existe
if [ -f "$CONFIG_FILE" ]; then
    echo "Iniciando Mosquitto en una nueva ventana de terminal con el archivo de configuración: $CONFIG_FILE"
    
    # Abrir una nueva terminal y ejecutar Mosquitto
    xfce4-terminal --hold --command="bash -c 'mosquitto -c $CONFIG_FILE; exec bash'"
else
    echo "Archivo de configuración no encontrado: $CONFIG_FILE"
    exit 1
fi


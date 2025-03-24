#!/bin/bash

# Array con los puertos e instancias de Node-RED
ports=(1871 1872 1873 1874 1875 1876)

# Ejecutar cada instancia de Node-RED en una nueva terminal
for port in "${ports[@]}"; do
    # Crear el directorio de usuario si no existe
    mkdir -p "node-red-$port"

    # Abrir una nueva consola y ejecutar Node-RED
    xfce4-terminal --hold --command="bash -c 'node-red -p $port -u node-red-$port/; exec bash'"
    
    # Mostrar mensaje indicando que la instancia está en ejecución
    echo "Node-RED se está ejecutando en el puerto $port con el directorio node-red-$port/"
done

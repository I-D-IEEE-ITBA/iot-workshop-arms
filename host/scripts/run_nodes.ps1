# Array con los puertos e instancias de Node-RED
$ports = @(1871, 1872, 1873, 1874, 1875, 1876)

# Ejecutar cada instancia de Node-RED en una nueva terminal
foreach ($port in $ports) {
    # Crear el directorio de usuario si no existe
    New-Item -ItemType Directory -Path "node-red-$port" -Force

    # Abrir una nueva consola y ejecutar Node-RED
    Start-Process powershell.exe -ArgumentList "-NoExit", "node-red -p $port -u node-red-$port/"
    
    # Mostrar mensaje indicando que la instancia está en ejecución
    Write-Host "Node-RED se está ejecutando en el puerto $port con el directorio node-red-$port/"
}
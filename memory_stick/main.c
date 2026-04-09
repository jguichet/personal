#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>

int main(void) {
    t_log* logger = log_create("memory_stick.log", "MEMORY_STICK", 1, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando Memory Stick...");

    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints)); //limpieza de memoria
    hints.ai_family = AF_INET;   //ponemos AF_INET para que sea IPv4
    hints.ai_socktype = SOCK_STREAM; //SOCK_STREAM porque queremos conexion TCP(flujo de datos confiable)
    // 1. Preparamos la dirección
int err = getaddrinfo("127.0.0.1", "8000", &hints, &servinfo);
if (err != 0) {
    log_error(logger, "Error al preparar la direccion de red");
    return EXIT_FAILURE; // Cortamos la ejecución
}
/* * ¿Por qué usar getaddrinfo?
Lo usamos para traducir nuestra IP (en formato string "127.0.0.1") y el puerto 
a las estructuras binarias y dinámicas que requiere el sistema operativo. 
Además, nos ahorra armar las estructuras a mano,filtrando 
explícitamente para usar el protocolo IPv4 (AF_INET).
 */
// 2. Creamos el socket
int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
if (socket_cliente == -1) {
    log_error(logger, "Error al crear el socket");
    freeaddrinfo(servinfo); // Limpiamos la basura antes de salir
    return EXIT_FAILURE;
}

// 3. Intentamos conectar
int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
if (conexion == -1) {
    log_error(logger, "Error al conectar con el servidor");
    freeaddrinfo(servinfo);
    close(socket_cliente);
    return EXIT_FAILURE;
} else {
    log_info(logger, "Conexion exitosa con el servidor!");
}

// Si llegó hasta acá, todo salió perfecto
freeaddrinfo(servinfo); 
    close(socket_cliente); //cierra el canal de comunicacion
    log_destroy(logger);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>

int main(void) {
    t_log* logger = log_create("io.log", "IO", 1, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando modulo IO...");

    
    //Preparamos las estructuras para conectarnos al Scheduler 
       //(Sabemos que esta seteado en el Puerto 8001)
  
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
   // 1. Preparamos la dirección
int err = getaddrinfo("127.0.0.1", "8000", &hints, &servinfo);
if (err != 0) {
    log_error(logger, "Error al preparar la direccion de red");
    return EXIT_FAILURE; // Cortamos la ejecución
}

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
// Limpiamos la memoria y cortamos
    freeaddrinfo(servinfo); 
    close(socket_cliente);
    log_destroy(logger);

    return 0;
}
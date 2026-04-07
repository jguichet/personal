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
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("127.0.0.1", "8000", &hints, &servinfo);

    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
    
    if (conexion != -1) {
        log_info(logger, "## Conectado al Kernel Memory exitosamente!");
    } else {
        log_error(logger, "Error al conectar con Kernel Memory.");
    }

    freeaddrinfo(servinfo); 
    close(socket_cliente);
    log_destroy(logger);
    return 0;
}
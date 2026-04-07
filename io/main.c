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

    /* =========================================================================
       PASO 1: Preparamos las estructuras para conectarnos al Scheduler 
       (Sabemos que atiende en el Puerto 8001)
       ========================================================================= */
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("127.0.0.1", "8001", &hints, &servinfo);

    /* =========================================================================
       PASO 2: Creamos el socket (nuestro telefono)
       ========================================================================= */
    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    /* =========================================================================
       PASO 3: Intentamos la conexion (llamar)
       ========================================================================= */
    int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
    
    if (conexion != -1) {
        log_info(logger, "## Conectado al Kernel Scheduler exitosamente!");
    } else {
        log_error(logger, "Error al intentar conectar con el Scheduler.");
    }

    // Limpiamos la memoria y cortamos
    freeaddrinfo(servinfo); 
    close(socket_cliente);
    log_destroy(logger);

    return 0;
}
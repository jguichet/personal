#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // Para las funciones de sockets
#include <netdb.h>      // Para las estructuras de red (addrinfo)
#include <unistd.h>     // Para cerrar los sockets
#include <commons/log.h>

int main(void) {
    // Iniciamos el log
    t_log* logger = log_create("kernel_memory.log", "KERNEL_MEMORY", 1, LOG_LEVEL_INFO);

    // Preparar las estructuras de red.Le decimos al sistema que queremos usar internet (IP) y TCP (SOCK_STREAM)
       
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Pedimos que nos configure todo para usar el puerto "8000"
    getaddrinfo(NULL, "8000", &hints, &servinfo);

    /* =========================================================================
       PASO 2: Crear el Socket (nuestro teléfono)
       ========================================================================= */
    int socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    /* =========================================================================
       PASO 3: Bind (Enchufar el teléfono a la pared / puerto)
       Asocia nuestro socket al puerto 8000 de nuestra computadora.
       ========================================================================= */
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

    /* =========================================================================
       PASO 4: Listen (Poner el teléfono a sonar)
       ========================================================================= */
    listen(socket_servidor, SOMAXCONN); // SOMAXCONN es el limite de conexiones a la vez
    
    log_info(logger, "Servidor Kernel Memory escuchando conexiones en el puerto 8000...");
    
    // Ya usamos servinfo para configurar, lo liberamos de la memoria
    freeaddrinfo(servinfo); 

    /* =========================================================================
       PASO 5: Accept (Atender la llamada)
       IMPORTANTE: Al llegar a esta linea, el programa se PAUSA infinitamente
       hasta que alguien se conecte.
       ========================================================================= */
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    
    // Si pasamos la linea de arriba, significa que alguien se conecto!
    log_info(logger, "## Conectado un modulo cliente! (Se desbloqueo el accept)");

    // Terminamos y limpiamos
    log_destroy(logger);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>

int main(void) {
    t_log* logger = log_create("kernel_scheduler.log", "SCHEDULER", 1, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando Kernel Scheduler...");

    
       /*Actuar como CLIENTE
       Llamamos al Kernel Memory, que sabemos que esta seteado en el puerto 8000*/
   
 struct addrinfo hints_serv, *servinfo_serv;
    memset(&hints_serv, 0, sizeof(hints_serv));
    hints_serv.ai_family = AF_INET;
    hints_serv.ai_socktype = SOCK_STREAM;
    hints_serv.ai_flags = AI_PASSIVE;

    // 1. Validación de getaddrinfo (Puerto 8001 y variables _serv)
    if (getaddrinfo(NULL, "8001", &hints_serv, &servinfo_serv) != 0) {
        log_error(logger, "Error en getaddrinfo del servidor");
        return EXIT_FAILURE;
    }

    // 2. Validación de socket
    int socket_servidor = socket(servinfo_serv->ai_family, servinfo_serv->ai_socktype, servinfo_serv->ai_protocol);
    if (socket_servidor == -1) {
        log_error(logger, "No se pudo crear el socket de escucha");
        freeaddrinfo(servinfo_serv);
        return EXIT_FAILURE;
    }

    int activado = 1;
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &activado, sizeof(activado));

    // 3. Validación de bind (asociar puerto)
    if (bind(socket_servidor, servinfo_serv->ai_addr, servinfo_serv->ai_addrlen) == -1) {
        log_error(logger, "Error al hacer bind. ¿El puerto 8001 ya está en uso?");
        freeaddrinfo(servinfo_serv);
        close(socket_servidor);
        return EXIT_FAILURE;
    }

    // 4. Validación de listen (ponerse a escuchar)
    if (listen(socket_servidor, SOMAXCONN) == -1) {
        log_error(logger, "Error al poner el socket en modo listen");
        freeaddrinfo(servinfo_serv);
        close(socket_servidor);
        return EXIT_FAILURE;
    }

    // Ya usamos servinfo_serv para configurar, lo liberamos
    freeaddrinfo(servinfo_serv);

   
    // 5. Accept (Atender la llamada de la CPU)
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    if (socket_cliente == -1) {
        log_error(logger, "Error al aceptar una conexion entrante");
    } else {
        log_info(logger, "Se aceptó un nuevo cliente (CPU) correctamente");
    }

    
    // Limpieza Final
     close(socket_memoria);  // Cerramos la conexión que hicimos como cliente
    close(socket_servidor); // Cerramos nuestro local
    close(socket_cliente);  // Cerramos la conexión con el cliente
    log_destroy(logger);

    return 0;
}
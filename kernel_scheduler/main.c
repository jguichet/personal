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
       Llamamos al Kernel Memory, que sabemos que escucha en el puerto 8000*/
   
    struct addrinfo hints_mem, *servinfo_mem;
    memset(&hints_mem, 0, sizeof(hints_mem));
    hints_mem.ai_family = AF_UNSPEC;
    hints_mem.ai_socktype = SOCK_STREAM;
    getaddrinfo("127.0.0.1", "8000", &hints_mem, &servinfo_mem);

    int socket_memoria = socket(servinfo_mem->ai_family, servinfo_mem->ai_socktype, servinfo_mem->ai_protocol);
    connect(socket_memoria, servinfo_mem->ai_addr, servinfo_mem->ai_addrlen);
    
    // Log para saber que se conecto correctamente
    log_info(logger, "## Conectado a Kernel Memory"); 
    freeaddrinfo(servinfo_mem);

    
       /*Actuar como SERVIDOR
       Ahora abrimos nuestro propio puerto (8001) para escuchar a la CPU*/
       
    struct addrinfo hints_serv, *servinfo_serv;
    memset(&hints_serv, 0, sizeof(hints_serv));
    hints_serv.ai_family = AF_UNSPEC;
    hints_serv.ai_socktype = SOCK_STREAM;
    hints_serv.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "8001", &hints_serv, &servinfo_serv);

    int socket_servidor = socket(servinfo_serv->ai_family, servinfo_serv->ai_socktype, servinfo_serv->ai_protocol);
    bind(socket_servidor, servinfo_serv->ai_addr, servinfo_serv->ai_addrlen);
    listen(socket_servidor, SOMAXCONN);
    
    log_info(logger, "Scheduler escuchando peticiones en el puerto 8001...");
    freeaddrinfo(servinfo_serv);

    
       //Aceptar la "llamada" de la CPU
        
    int socket_cpu = accept(socket_servidor, NULL, NULL);
    
    // Log para saber que se conecto correctamente
    log_info(logger, "## CPU 1 Conectada");

    // Limpieza 
    close(socket_memoria);
    close(socket_servidor);
    close(socket_cpu);
    log_destroy(logger);
    
    return 0;
}
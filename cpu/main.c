#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // Para las funciones de sockets
#include <netdb.h>      // Para las estructuras de red
#include <unistd.h>     // Para cerrar los sockets
#include <commons/log.h>
#include <commons/config.h>

int main(void) {
    t_log* logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando modulo CPU. Intentando conectar al Kernel Memory...");

    // Preparar las estructuras de red (Cliente) 
    // Queremos conectarnos a nuestra propia maquina ("127.0.0.1" o "localhost") en el puerto "8000" (donde esta escuchando el Kernel Memory).
  
    struct addrinfo hints, *servinfo; //servinfo es un puntero que guarda la conexion(IP,puerto,protocolo,direccion)
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("127.0.0.1", "8001", &hints, &servinfo);

    //Crear el Socket("nuestro telefono")
      
    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

     //Connect ("poner el numero y llamar")
     
    int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
    
    // Verificamos si nos "atendieron"
    if (conexion != -1) {
        log_info(logger, "¡Conexion EXITOSA al Kernel Scheduler!");
    } else {
        log_error(logger, "Error al intentar conectar.");
    }

    // Ya usamos servinfo, lo liberamos
    freeaddrinfo(servinfo); 
    
    // "Cortamos la llamada"
    close(socket_cliente);
    log_destroy(logger);

    return 0;
}
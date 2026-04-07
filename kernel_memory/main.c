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

    // Preparar las estructuras de red
    //Le decimos al sistema que queremos usar internet (IP) y TCP (SOCK_STREAM)
    //addrinfo hints es un struct que define los filtos de nuestra busqueda(filtros de red)
    //addrinfo *servinfo es un struct donde getaddrinfo devolvera la informacion de red procesada
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //ai_family es para tipo de protocolo
    hints.ai_socktype = SOCK_STREAM; //ai_socktype define el tipo de comunicacion TPC O UDP
    hints.ai_flags = AI_PASSIVE; /* ai_flags son indicadores adicionales para la funcion getaddrinfo, 
    por ejemplo passive le dice que va a usar esa direccion para armar un server(bind)*/

    // Pedimos que nos configure todo para usar el puerto "8000"
    getaddrinfo(NULL, "8000", &hints, &servinfo);

    
      //Crear el Socket 
      
    int socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

   
       //Bind ("Enchufar el teléfono a la pared / puerto")
       //Asocia nuestro socket al puerto 8000 de nuestra computadora.
     
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

   
      //Listen ("Poner el teléfono a sonar")
      
    listen(socket_servidor, SOMAXCONN); // SOMAXCONN es el limite de conexiones a la vez,permite que varios modulos esperen su "turno"para ser atendidos por el accept()
    
    log_info(logger, "Servidor Kernel Memory escuchando conexiones en el puerto 8000...");
    
    // Ya usamos servinfo para configurar, lo liberamos de la memoria
    freeaddrinfo(servinfo); 

    
       //Accept (Atender la llamada)
       //IMPORTANTE: Al llegar a esta linea, el programa se PAUSA infinitamente hasta que alguien se conecte.
       
    int socket_cliente = accept(socket_servidor, NULL, NULL); // accept crea un NUEVO socket específico para hablar con ese cliente.
    
    // Si pasamos la linea de arriba, significa que alguien se conecto
    log_info(logger, "## Conectado un modulo cliente! (Se desbloqueo el accept)");

    // Terminamos y limpiamos
    log_destroy(logger);
    return 0;
}
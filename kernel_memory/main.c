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
    hints.ai_family = AF_INET; //ai_family es para tipo de protocolo
    hints.ai_socktype = SOCK_STREAM; //ai_socktype define el tipo de comunicacion TPC O UDP
    hints.ai_flags = AI_PASSIVE; /* ai_flags son indicadores adicionales para la funcion getaddrinfo, 
    por ejemplo passive le dice que va a usar esa direccion para armar un server(bind)*/

    // Pedimos que nos configure todo para usar el puerto "8000"
    // Validación de getaddrinfo
if (getaddrinfo(NULL, "8000", &hints, &servinfo) != 0) {
    log_error(logger, "Error en getaddrinfo del servidor");
    return EXIT_FAILURE;
}

// Validación de socket
int socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
if (socket_servidor == -1) {
    log_error(logger, "No se pudo crear el socket de escucha");
    freeaddrinfo(servinfo);
    return EXIT_FAILURE;
}
/* Configuracion del puerto (setsockopt)
-SO_REUSEPORT evita el error "Address already in use"
-Nos permite reiniciar el servidor rápidamente sin que Linux nos bloquee el puerto por unos minutos.
*/
   int activado = 1;
setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &activado, sizeof(activado));
    */   
/*Bind asocia nuestro socket al puerto 8000 de nuestra computadora.
-El if atrapa el error si otra aplicación ya está usando ese mismo puerto*/
if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    log_error(logger, "Error al hacer bind.El puerto 8000 ya está en uso");
    freeaddrinfo(servinfo);
    close(socket_servidor);
    return EXIT_FAILURE;
}

/*Validación de listen (ponerse a escuchar)
-Marcamos el socket como "pasivo".el SO sabe que debe 
encolar a los clientes que intenten conectarse (hasta el límite SOMAXCONN).
*/
if (listen(socket_servidor, SOMAXCONN) == -1) {
    log_error(logger, "Error al poner el socket en modo listen");
    freeaddrinfo(servinfo);
    close(socket_servidor);
    return EXIT_FAILURE;
}
    
    // Ya usamos servinfo para configurar, lo liberamos de la memoria
    freeaddrinfo(servinfo); 

    
/*Accept (Atender la llamada)
-El programa se "pausa" hasta que alguien haga un connect().
-Cuando alguien conecta, accept() crea y devuelve un NUEVO socket (socket_cliente)
exclusivo para hablar exclusivamente con ese cliente.
*/
       
   int socket_cliente = accept(socket_servidor, NULL, NULL);
if (socket_cliente == -1) {
    log_error(logger, "Error al aceptar una conexion entrante");
    // No hacemos return porque el servidor debe seguir andando para otros clientes
} else {
    log_info(logger, "Se aceptó un nuevo cliente correctamente");
}

    // Terminamos y limpiamos
    log_destroy(logger);
    return 0;
}
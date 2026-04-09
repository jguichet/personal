#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // Para las funciones de sockets
#include <netdb.h>      // Para las estructuras de red
#include <unistd.h>     // Para cerrar los sockets
#include <commons/log.h>
#include <commons/config.h>

int main(void) {
    t_log* logger = log_create("cpu.log"/*archivo que se crea*/, "CPU"/*etiqueta antes del mensaje*/, 1/*true(booleano),Le indica a la biblioteca que, 
        además de escribir en el archivo de texto, también imprima el mensaje en la salida estándar de la consola (stdout)*/
        , LOG_LEVEL_INFO/*nivel minimo de informacion a mostrar*/); //puntero apuntando a una estructura de tipo t_log
    log_info(logger, "Iniciando modulo CPU. Intentando conectar al Kernel Memory...");

    // Preparar las estructuras de red (Cliente) 
    // Queremos conectarnos a nuestra propia maquina ("127.0.0.1" o "localhost") en el puerto "8000" (donde esta escuchando el Kernel Memory).
  
    struct addrinfo hints, *servinfo; //servinfo es un puntero que guarda la conexion(IP,puerto,protocolo,direccion)
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /*Usamos el operador & (pasaje por referencia) por dos motivos:
   &hints: Pasamos la dirección de la estructura para que la función la lea sin tener que hacer una copia pesada en memoria,sino pasaria todo el struct completo y gastaria memoria innecesariamente.
  &servinfo: Pasamos la dirección de nuestro puntero (puntero a puntero) para que la función pueda modificarlo y hacerlo apuntar a la nueva lista de resultados que va a reservar.
 */
/* 1. Preparamos la dirección 
-(getaddrinfo):Traduce la IP y el Puerto a las estructuras binarias que usa el SO.
-Usamos AF_INET para forzar IPv4.
-El 'if' verifica que la IP sea válida antes de intentar hacer nada */
int err = getaddrinfo("127.0.0.1", "8000", &hints, &servinfo);
if (err != 0) {
    log_error(logger, "Error al preparar la direccion de red");
    return EXIT_FAILURE; // Cortamos la ejecución
}

/* 2. Creamos el socket
-Le pedimos al Sistema Operativo que nos cree un "File Descriptor" (fd).
-El 'if' nos protege por si el SO se quedó sin memoria o sin fds disponibles.
*/
int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
if (socket_cliente == -1) {
    log_error(logger, "Error al crear el socket");
    freeaddrinfo(servinfo); // Limpiamos la basura antes de salir
    return EXIT_FAILURE;
}

/*3. Intentamos conectar
-Intentamos conectarnos al Kernel mediante la red (127.0.0.1).
-El 'if' falla (devuelve -1) generalmente si el Servidor está apagado o 
si nos equivocamos de puerto.
*/

int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
if (conexion == -1) {
    log_error(logger, "Error al conectar con el servidor");
    freeaddrinfo(servinfo);
    close(socket_cliente);//cerramos el canal del lado del cliente
    return EXIT_FAILURE;
} else {
    log_info(logger, "Conexion exitosa con el servidor!");
}

// Si llegó hasta acá, todo salió perfecto
 // Ya usamos servinfo, lo liberamos
    freeaddrinfo(servinfo); 
 // "Cortamos la llamada"
    close(socket_cliente);
    log_destroy(logger);

    return 0;
}
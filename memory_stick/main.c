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
    memset(&hints, 0, sizeof(hints)); //limpieza de memoria
    hints.ai_family = AF_UNSPEC;   //ponemos AF_UNSPEC ya que no importa si es IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; //SOCK_STREAM porque queremos conexion TCP(flujo de datos confiable)
    getaddrinfo("127.0.0.1", "8000", &hints, &servinfo); // Traduce la IP y puerto (texto) a un formato que la placa de red entienda y lo guarda en servinfo

    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    // Crea el socket ("el teléfono") usando la configuración guardada en servinfo
    int conexion = connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen);
    // Intenta establecer la conexión con el servidor (tocar el timbre en el puerto 8000)

    
    if (conexion != -1) {
        log_info(logger, "## Conectado al Kernel Memory exitosamente!");
    } else {
        log_error(logger, "Error al conectar con Kernel Memory.");
    }

    freeaddrinfo(servinfo); 
    close(socket_cliente); //cierra el canal de comunicacion
    log_destroy(logger);
    return 0;
}
//¿porque usar getaddrinfo?
/*"Lo usamos para que el código sea independiente del protocolo. 
No nos importa si es IPv4 o IPv6, la función se encarga de llenar la estructura servinfo con lo que necesite el sistema operativo".*/
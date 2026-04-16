#include <stdlib.h>
#include "utils/sockets.h"
#include "utils/protocolo.h"

void* manejar_cliente_memoria(void* void_args) {
    t_args_cliente* args = (t_args_cliente*) void_args;
    int socket_cliente = args->socket_cliente;
    t_log* logger = args->logger;

    free(args); 

    // 1. Recibimos el código de operación (el primer int del paquete)
    int codigo_operacion = recibir_operacion(socket_cliente);

    switch (codigo_operacion) {
        case HANDSHAKE_MS_MEMORIA: {
            // 2. Como sabemos que el Memory Stick manda su tamaño justo después del Handshake, lo leemos
            int tam_memoria_stick;
            recv(socket_cliente, &tam_memoria_stick, sizeof(int), MSG_WAITALL);
            
            // Log obligatorio y demostración de que llegó la data
            log_info(logger, "## Memory Stick Conectado - FD: %d", socket_cliente);
            log_info(logger, "El Memory Stick nos informo que tiene un tamaño de: %d bytes", tam_memoria_stick);
            
            // Acá el Kernel Memory debería guardar este socket y este tamaño en alguna lista global
            break;
        }
        case -1:
            log_error(logger, "Un cliente se desconectó.");
            break;
        default:
            log_warning(logger, "Operación desconocida: %d", codigo_operacion);
            break;
    }
    
    return NULL;
}
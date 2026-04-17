#include <utils/protocolo.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "utils/sockets.h"
void* manejar_cliente_memoria(void* arg) {
    t_args_cliente* args = (t_args_cliente*)arg;
    int socket_cliente = args->socket_cliente;
    t_log* logger = args->logger;
    free(args); 

    // AGREGAMOS EL BUCLE INFINITO
    while (1) { 
        int codigo_operacion = recibir_operacion(socket_cliente);

        // Si recibir_operacion devuelve -1, el cliente se desconectó
        if (codigo_operacion == -1) {
            log_warning(logger, "Un cliente se desconectó del Kernel Memory.");
            break; // Rompemos el while para terminar el hilo
        }

        switch (codigo_operacion) {
            case HANDSHAKE_CPU_MEMORIA:
                log_info(logger, "¡Se conectó una CPU al Kernel Memory!");
                break;
            case HANDSHAKE_MS_MEMORIA:
                log_info(logger, "¡Se conectó un Memory Stick!");
                int tam_memoria;
                recv(socket_cliente, &tam_memoria, sizeof(int), MSG_WAITALL);
                log_info(logger, "El Memory Stick tiene un tamaño de: %d bytes", tam_memoria);
                break;
            case HANDSHAKE_SWAP_MEMORIA:
                log_info(logger, "¡Se conectó el módulo Swap al Kernel Memory!");
                break;
            default:
                log_warning(logger, "Operación desconocida en Kernel Memory. Código: %d", codigo_operacion);
                break;
        }
    }
    return NULL;
}




int main(int argc, char* argv[]) {
    // 1. Validamos que nos pasen el archivo por parámetro
    if (argc < 2) {
        printf("Uso: ./bin/kernel_memory [Archivo Config]\n");
        return EXIT_FAILURE;
    }

    t_log *logger = log_create("kernel_memory.log", "KERNEL_MEMORY", true, LOG_LEVEL_INFO);
    t_config *config = config_create(argv[1]);

    if (config == NULL) {
        log_error(logger, "No se encontro el archivo de configuracion");
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    // 2. Leemos el puerto desde el archivo
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    // 3. Levantamos el server pasándole NULL a la IP para que escuche en todas las interfaces locales
    int servidor = iniciar_servidor(NULL, puerto_escucha, logger);

    // 4. Usamos el nuevo cerebro de memoria
    atender_clientes_multihilo(servidor, logger, manejar_cliente_memoria);

    config_destroy(config);
    return 0;
}
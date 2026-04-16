#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "utils/sockets.h"

// Avisamos que esta función existe en otro archivo
extern void* manejar_cliente_memoria(void* void_args);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: ./bin/kernel_memory [Archivo Config]\n");
        return EXIT_FAILURE;
    }

    t_log* logger = log_create("kernel_memory.log", "KERNEL_MEMORY", 1, LOG_LEVEL_INFO);
    t_config* config = config_create(argv[1]);

    if (config == NULL) {
        log_error(logger, "No se encontro el archivo de configuracion");
        return EXIT_FAILURE;
    }

    char* puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    // Levantamos el servidor del Kernel Memory
    int socket_servidor = iniciar_servidor(NULL, puerto_escucha, logger);
    if (socket_servidor == -1) {
        log_destroy(logger);
        config_destroy(config);
        return EXIT_FAILURE;
    }

    // Bloqueamos el hilo principal para atender a los que se conecten (como el Memory Stick)
    atender_clientes_multihilo(socket_servidor, logger, manejar_cliente_memoria);

    log_destroy(logger);
    config_destroy(config);
    return EXIT_SUCCESS;
}
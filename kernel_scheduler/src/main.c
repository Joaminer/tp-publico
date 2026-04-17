
#include <utils/protocolo.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "utils/sockets.h"
void* manejar_cliente_scheduler(void* arg) {
    t_args_cliente* args = (t_args_cliente*)arg;
    int socket_cliente = args->socket_cliente;
    t_log* logger = args->logger;
    free(args);

    // AGREGAMOS EL BUCLE INFINITO
    while (1) {
        int codigo_operacion = recibir_operacion(socket_cliente);

        if (codigo_operacion == -1) {
            log_warning(logger, "Un cliente se desconectó del Scheduler.");
            break; // Rompemos el while para terminar el hilo
        }

        switch (codigo_operacion) {
            case HANDSHAKE_CPU_KERNEL:
                log_info(logger, "¡Se conectó una CPU al Kernel Scheduler!");
                int id_cpu_recibido;
                if (recv(socket_cliente, &id_cpu_recibido, sizeof(int), MSG_WAITALL) > 0) {
                    log_info(logger, "Es la CPU con ID: %d", id_cpu_recibido);
                }
                break;
            case HANDSHAKE_IO_KERNEL:
                log_info(logger, "¡Se conectó una Interfaz de Entrada/Salida (IO)!");
                break;
            default:
                log_warning(logger, "Operación desconocida en Scheduler. Código: %d", codigo_operacion);
                break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: ./bin/kernel_scheduler [Archivo Config]\n");
        return EXIT_FAILURE;
    }

    t_log *logger = log_create("kernel_scheduler.log", "KERNEL_SCHEDULER", true, LOG_LEVEL_INFO);
    t_config *config = config_create(argv[1]);

    if (config == NULL) {
        log_error(logger, "No se encontro el archivo de configuracion");
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    int servidor = iniciar_servidor(NULL, puerto_escucha, logger);

    // Usamos el nuevo cerebro del scheduler
    atender_clientes_multihilo(servidor, logger, manejar_cliente_scheduler);

    config_destroy(config);
    return 0;
}

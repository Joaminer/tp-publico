#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>

#include "utils/sockets.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"

// Variables globales para la memoria simulada
void* memoria_fisica;
int retardo_memoria;

/**
 * @brief Lógica que ejecuta cada hilo cuando una CPU se conecta al Memory Stick
 */
void* manejar_cliente_cpu(void* void_args) {
    t_args_cliente* args = (t_args_cliente*) void_args;
    int socket_cliente = args->socket_cliente;
    t_log* logger = args->logger;

    // Liberamos el struct porque ya copiamos los datos locales
    free(args); 

    // La CPU debería mandar un HANDSHAKE inicial al conectarse
    int codigo_operacion = recibir_operacion(socket_cliente);
    
    // todavía no se agregó LECTURA y ESCRITURA al enum en estructura.h, 
    // hay que agregarlo para que este switch funcione completo)
    switch (codigo_operacion) {
        /*
        case LECTURA_MEMORIA:
            // Lógica de lectura con usleep(retardo_memoria * 1000);
            break;
        case ESCRITURA_MEMORIA:
            // Lógica de escritura con usleep(retardo_memoria * 1000);
            break;
        */
        case -1:
            log_error(logger, "La CPU se desconectó inesperadamente.");
            break;
        default:
            log_warning(logger, "Operación desconocida de la CPU. Código: %d", codigo_operacion);
            break;
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    // Validamos los parámetros de entrada según el enunciado
    if (argc < 3) {
        printf("Uso: ./bin/memory_stick [Archivo Config] [Tamaño]\n");
        return EXIT_FAILURE;
    }

    char* path_config = argv[1];
    int tam_memoria = atoi(argv[2]);

    // --- PASO 1: Setup Inicial y Reserva de Memoria ---
    t_log* logger = log_create("memory_stick.log", "MEMORY_STICK", 1, LOG_LEVEL_INFO);
    t_config* config = config_create(path_config);

    if (config == NULL) {
        log_error(logger, "No se pudo leer el archivo de configuración: %s", path_config);
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    retardo_memoria = config_get_int_value(config, "MEMORY_DELAY");
    char* puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    char* ip_kernel_memory = config_get_string_value(config, "IP_KERNEL_MEMORY");
    char* puerto_kernel_memory = config_get_string_value(config, "PUERTO_KERNEL_MEMORY");

    // Simulamos el hardware: hacemos malloc del tamaño recibido por consola
    memoria_fisica = malloc(tam_memoria);
    if (memoria_fisica == NULL) {
        log_error(logger, "Error al reservar %d bytes de memoria con malloc", tam_memoria);
        config_destroy(config);
        log_destroy(logger);
        return EXIT_FAILURE;
    }
    // Llenamos la memoria de ceros por seguridad
    memset(memoria_fisica, 0, tam_memoria);
    log_info(logger, "Memoria inicializada con %d bytes", tam_memoria);


    // --- PASO 2: Sockets (Actuar como Cliente del Kernel Memory) ---
    int conexion_km = crear_conexion(ip_kernel_memory, puerto_kernel_memory);
    if (conexion_km != -1) {
        log_info(logger, "## Conectado a Kernel Memory");
        
        // Usamos la función de protocolo para enviar el Handshake
        enviar_codigo_operacion(conexion_km, HANDSHAKE_MS_MEMORIA);
        
        // Nota: Seguramente hay que mandar un paquete extra acá avisándole al Kernel Memory
        // de qué tamaño es este Memory Stick (el tam_memoria),.
    } else {
        log_error(logger, "Fallo la conexión con Kernel Memory en %s:%s", ip_kernel_memory, puerto_kernel_memory);
        free(memoria_fisica);
        config_destroy(config);
        log_destroy(logger);
        return EXIT_FAILURE;
    }


    // --- PASO 3: Sockets (Actuar como Servidor para las CPUs) ---
    int socket_servidor = iniciar_servidor(NULL, puerto_escucha, logger);
    if (socket_servidor == -1) {
        free(memoria_fisica);
        config_destroy(config);
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    log_info(logger, "Memory Stick listo escuchando a las CPUs en el puerto %s", puerto_escucha);

    // Atender CPUs (Bloquea el hilo principal y delega la gestión de hilos a la lib compartida)
    atender_clientes_multihilo(socket_servidor, logger, manejar_cliente_cpu);

    // Limpieza (solo llega acá si hay alguna señal de interrupción que rompa el loop)
    free(memoria_fisica);
    log_destroy(logger);
    config_destroy(config);
    return EXIT_SUCCESS;
}
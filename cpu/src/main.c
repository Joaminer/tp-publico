#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>

// Incluimos biblioteca compartida
#include "utils/sockets.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"

int main(int argc, char* argv[]) {
    // Validamos que nos pasen la config y el ID de la CPU
    if (argc < 3) {
        printf("Uso: ./bin/cpu [Archivo Config] [Identificador]\n");
        return EXIT_FAILURE;
    }

    char* path_config = argv[1];
    int id_cpu = atoi(argv[2]); // Convertimos el ID a número

    // Armamos un nombre de log dinámico para no pisar los logs si abren varias CPUs
    char log_name[30];
    sprintf(log_name, "cpu_%d.log", id_cpu);
    
    // Nivel de log a INFO como pide el enunciado
    t_log* logger = log_create(log_name, "CPU", 1, LOG_LEVEL_INFO);
    t_config* config = config_create(path_config);

    if (config == NULL) {
        log_error(logger, "No se pudo leer el archivo de configuración.");
        log_destroy(logger);
        return EXIT_FAILURE;
    }

    // Leemos las IPs y Puertos de los módulos a los que nos tenemos que conectar
    char* ip_kernel_scheduler = config_get_string_value(config, "IP_KERNEL_SCHEDULER");
    char* puerto_kernel_scheduler = config_get_string_value(config, "PUERTO_KERNEL_SCHEDULER");
    
    char* ip_kernel_memory = config_get_string_value(config, "IP_KERNEL_MEMORY");
    char* puerto_kernel_memory = config_get_string_value(config, "PUERTO_KERNEL_MEMORY");


    // --- 1. CONEXIÓN AL KERNEL SCHEDULER ---
    int conexion_scheduler = crear_conexion(ip_kernel_scheduler, puerto_kernel_scheduler);
    if (conexion_scheduler != -1) {
        log_info(logger, "Conectado exitosamente al Kernel Scheduler");
        // 1. Mandamos el handshake que ya está en tu estructura.h
        enviar_codigo_operacion(conexion_scheduler, HANDSHAKE_CPU_KERNEL);
        
        // 2. Mandamos nuestro ID de CPU para que el Kernel sepa quiénes somos
        send(conexion_scheduler, &id_cpu, sizeof(int), 0);
    } else {
        log_error(logger, "Fallo al conectar con Kernel Scheduler en %s:%s", ip_kernel_scheduler, puerto_kernel_scheduler);
    }


    // --- 2. CONEXIÓN AL KERNEL MEMORY ---
    int conexion_memoria = crear_conexion(ip_kernel_memory, puerto_kernel_memory);
    if (conexion_memoria != -1) {
        log_info(logger, "Conectado exitosamente al Kernel Memory");
        enviar_codigo_operacion(conexion_memoria, HANDSHAKE_CPU_MEMORIA);
    } else {
        log_error(logger, "Fallo al conectar con Kernel Memory en %s:%s", ip_kernel_memory, puerto_kernel_memory);
    }

    // --- 3. CONEXIÓN A MEMORY STICK(s) ---
    // (Falta definir cómo se descubren

    // Para evitar que el programa se cierre de golpe y cierre los sockets en esta prueba:
    printf("Presione ENTER para desconectar la CPU y cerrar el programa...\n");
    getchar();

    // Limpieza general
    if(conexion_scheduler != -1) liberar_conexion(conexion_scheduler);
    if(conexion_memoria != -1) liberar_conexion(conexion_memoria);
    log_destroy(logger);
    config_destroy(config);

    return EXIT_SUCCESS;
}
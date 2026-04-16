#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //Para el struct
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h> //Logger del commons

//Struct que le pasa datos al hilo
typedef struct {
    int socket_cliente;
    t_log* logger;
} t_args_cliente;

//Funciones para el Server (Kernel Memory, Kernel Scheduler)
int iniciar_servidor(char* ip, char* puerto, t_log* logger);
int esperar_cliente(int socket_servidor, t_log* logger);

//Funciones para el Client (CPUs, IO, etc)
int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);
void atender_clientes_multihilo(int socket_servidor, t_log* logger, void* (*handler_cliente)(void*));

void *atender_cliente_v1(void *arg);
#endif /* SOCKETS_H_ */
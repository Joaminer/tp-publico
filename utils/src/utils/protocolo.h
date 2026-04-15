#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "sockets.h"
#include "estructuras.h"

void enviar_codigo_operacion(int socket_cliente, op_code codigo);

int recibir_operacion(int socket_cliente);

void enviar_mensaje(char* mensaje, int socket_cliente);

void enviar_handshake_memory_stick(int socket_cliente, int tamaño_memoria);
#endif /* PROTOCOLO_H_ */
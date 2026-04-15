#include "protocolo.h"

/**
 * @brief Envia solamente un codigo de operacion
 */
void enviar_codigo_operacion(int socket_cliente, op_code codigo) {
    //send(socket_destino, puntero_al_dato, tamaño_del_dato, flags)
    send(socket_cliente, &codigo, sizeof(op_code), 0);
}

/**
 * @brief Se queda esperando a recibir un codigo de operacion.
 * @return El codigo recibido, o -1 si hubo un error o el cliente se desconectó.
 */
int recibir_operacion(int socket_cliente) {
    op_code codigo;
    
    //recv() devuelve la cantidad de bytes leidos (Si es 0, el cliente se desconecto)
    //Usamos MSG_WAITALL para obligar al SO a esperar que lleguen todos los bytes
    if (recv(socket_cliente, &codigo, sizeof(op_code), MSG_WAITALL) > 0) {
        return codigo;
    } else {
        close(socket_cliente);
        return -1;
    }
}

/**
 * @brief Empaqueta y envia un string dinamico por la red.
 * Estructura del paquete: [OP_CODE] + [TAMAÑO_DEL_STRING] + [STRING]
 */
void enviar_mensaje(char* mensaje, int socket_cliente) {
    op_code codigo = MENSAJE;
    //Sumamos 1 para incluir el caracter nulo "\0" (indica el fin del string)
    int tamaño_mensaje = strlen(mensaje) + 1; 
    
    //Calculamos el tamaño total del paquete que vamos a enviar
    int tamaño_paquete = sizeof(op_code) + sizeof(int) + tamaño_mensaje;

    //Reservamos la memoria exacta para el paquete
    void* stream = malloc(tamaño_paquete);
    int offset = 0; //Ubicacion del puntero

    //Copiamos el codigo de operación
    memcpy(stream + offset, &codigo, sizeof(op_code));
    offset += sizeof(op_code); //Movemos el puntero

    //Copiamos el tamaño del mensaje
    memcpy(stream + offset, &tamaño_mensaje, sizeof(int));
    offset += sizeof(int); //Movemos el puntero

    //Copiamos el mensaje real
    memcpy(stream + offset, mensaje, tamaño_mensaje);

    //Enviamos el bloque de memoria
    send(socket_cliente, stream, tamaño_paquete, 0);

    free(stream);
}

/**
 * @brief Empaqueta y envia el handshake inicial del Memory Stick al Kernel Memory.
 * Estructura del paquete: [OP_CODE] + [TAMAÑO_MEMORIA]
 */
void enviar_handshake_memory_stick(int socket_cliente, int tamaño_memoria) {
    op_code codigo = HANDSHAKE_MS_MEMORIA;
    
    // Calculamos el tamaño total: lo que pesa el enum + lo que pesa el int
    int tamaño_paquete = sizeof(op_code) + sizeof(int);

    // Reservamos la memoria exacta
    void* stream = malloc(tamaño_paquete);
    int offset = 0; // Puntero de desplazamiento

    // 1. Copiamos el código de operación
    memcpy(stream + offset, &codigo, sizeof(op_code));
    offset += sizeof(op_code); 

    // 2. Copiamos el tamaño de la memoria
    memcpy(stream + offset, &tamaño_memoria, sizeof(int));

    // Enviamos todo el paquete junto
    send(socket_cliente, stream, tamaño_paquete, 0);

    free(stream);
}
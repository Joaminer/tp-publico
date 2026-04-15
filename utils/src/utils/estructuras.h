#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

//Listado de todos los tipos de mensajes que se pueden enviar
typedef enum {
    MENSAJE,
    HANDSHAKE_KERNEL_MEMORIA,
    HANDSHAKE_CPU_MEMORIA,
    HANDSHAKE_CPU_KERNEL,
    HANDSHAKE_IO_KERNEL,
    HANDSHAKE_MS_MEMORIA, //MS = Memory Stick
    HANDSHAKE_SWAP_MEMORIA,
    //Dsp hay que agregarle mas para el resto de checkpoints

    // --- Operaciones de Memoria ---
    LECTURA_MEMORIA,
    ESCRITURA_MEMORIA,
    RESPUESTA_OK,
    RESPUESTA_ERROR
} op_code;

#endif /* ESTRUCTURAS_H_ */
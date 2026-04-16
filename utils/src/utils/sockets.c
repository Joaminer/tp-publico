#include "sockets.h"

/**
 * @brief Inicia un servidor TCP en la IP y Puerto especificados.
 * @return El File Descriptor (FD) del socket servidor, o -1 en caso de error.
 */
int iniciar_servidor(char* ip, char* puerto, t_log* logger) {
    int socket_servidor;
    struct addrinfo hints, *servinfo;

    //Inicializamos los hints para decirle al SO que conexion queremos
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     //IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;     //Usar la IP local si "ip" es NULL

    //Sacamos la info de la red del sistema
    getaddrinfo(ip, puerto, &hints, &servinfo);

    //Creamos el socket con esa info
    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    //Evita el error "Address already in use" al reiniciar el servidor
    int yes = 1;
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    //Asociamos el socket a nuestro puerto e IP local (Bind, como la Binding tool)
    if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        log_error(logger, "Error al intentar hacer bind en el puerto %s", puerto);
        freeaddrinfo(servinfo);
        return -1;
    }

    //Ponemos el socket a escuchar conexiones entrantes (Listen)
    listen(socket_servidor, SOMAXCONN); //SOMAXCONN es la cantidad máxima de conexiones en cola que permite el SO

    log_info(logger, "Servidor levantado correctamente en el puerto %s", puerto);

    //Liberamos la memoria de la estructura servinfo
    freeaddrinfo(servinfo);

    return socket_servidor;
}

/**
 * @brief Se queda bloqueado esperando a que un cliente se conecte.
 * @return El File Descriptor (FD) del socket cliente conectado.
 */
int esperar_cliente(int socket_servidor, t_log* logger) {

    //La función accept() para la ejecucion hasta que alguien se conecte
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    
    if (socket_cliente != -1) {
        log_info(logger, "¡Se conectó un nuevo cliente! (FD: %d)", socket_cliente);
    } else {
        log_error(logger, "Error al aceptar una nueva conexión de cliente");
    }

    return socket_cliente;
}

/**
 * @brief Crea una conexion hacia un server ya existente (Actua como Client).
 * @return El File Descriptor (FD) de la conexion, o -1 en caso de error.
 */
int crear_conexion(char* ip, char* puerto) {
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    //Obtenemos la info del server al que nos queremos conectar
    getaddrinfo(ip, puerto, &hints, &servinfo);

    //Creamos un socket cliente
    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    //Intentamos conectarnos al server
    if (connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        freeaddrinfo(servinfo);
        return -1; //Fallo la conexion
    }

    freeaddrinfo(servinfo);
    return socket_cliente; //Funciono la conexion
}

/**
 * @brief Cierra de manera segura el File Descriptor(FD) del socket.
 */
void liberar_conexion(int socket_cliente) {
    close(socket_cliente);
}

/**
 * @brief Escucha conexiones en un loop infinito y levanta un hilo por cada cliente.
 */
void atender_clientes_multihilo(int socket_servidor, t_log* logger, void* (*handler_cliente)(void*)) {
    while (1) {
        int socket_cliente = esperar_cliente(socket_servidor, logger);
        
        if (socket_cliente != -1) {

            //Reservamos memoria para los argumentos del hilo
            //Usar malloc para que no se pisen los datos entre hilos
            t_args_cliente* args = malloc(sizeof(t_args_cliente));
            args->socket_cliente = socket_cliente;
            args->logger = logger;

            pthread_t hilo_cliente;

            //Creamos el hilo
            pthread_create(&hilo_cliente, NULL, handler_cliente, (void*) args);
            //Usamo detach para que el SO libere los recursos del hilo al terminar
            pthread_detach(hilo_cliente); 
        }
    }
}



void *atender_cliente_v1(void *arg)
{
    t_args_cliente* args = (t_args_cliente*)arg;

    int fd = args->socket_cliente;
    t_log* logger = args->logger;


    while (1)
    {
        int opcion;

        int bytes = recv(fd, &opcion, sizeof(int), 0);

        if (bytes <= 0)
        {
            printf("cliente desconectado\n");
            close(fd);
            break;
        }
         log_info(logger, "Se recibió un mensaje del cliente");
        switch (opcion)
        {
            case 1:
                printf("OPCION-1\n");
                break;

            case 2:
                printf("OPCION-2\n");
                break;
            case 3:
                printf("OPCION-3\n");
        }
    }

    return NULL;
}

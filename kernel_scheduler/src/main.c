
#include <pthread.h>
#include <netdb.h>


typedef struct
{
    char *ip;
    char *puerto;
} info_socket;

void *atender_cliente_v1(void *arg)
{
    int fd = *(int *)arg;
    free(arg);

    while (1)
    {
        int opcion;

        int bytes = recv(fd, &opcion, sizeof(int), 0);
        printf("¡Llegó un mensaje! Código recibido: %d\n", opcion);
        if (bytes <= 0)
        {
            printf("cliente desconectado\n");
            close(fd);
            break;
        }

        switch (opcion)
        {
            case 1:
                printf("OPCION-1\n");
                break;

            case 2:
                printf("OPCION-2\n");
                break;
        }
    }

    return NULL;
}


void *crear_servidor(void *arg)
{
    info_socket *info = (info_socket *)arg;

    iniciar_server(info);

    return NULL;
}

void iniciar_server(info_socket *info)
{
    int err;
    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    err = getaddrinfo(info->ip, info->puerto, &hints, &server_info);

    if (err != 0)
    {
        printf("Error: %s\n", gai_strerror(err));
        return;
    }

    int fd_escucha = socket(server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);

    if (fd_escucha == -1)
    {
        perror("socket");
        freeaddrinfo(server_info);
        return;
    }

    int opt = 1;
    setsockopt(fd_escucha, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(fd_escucha, server_info->ai_addr, server_info->ai_addrlen);

    listen(fd_escucha, SOMAXCONN);

    freeaddrinfo(server_info);

    while (1)
    {
        int *fd_conexion_ptr = malloc(sizeof(int));
        *fd_conexion_ptr = accept(fd_escucha, NULL, NULL);

        if (*fd_conexion_ptr == -1)
        {
            perror("accept");
            free(fd_conexion_ptr);
            continue;
        }
        
    pthread_t hilo_cliente;
    pthread_create(&hilo_cliente,NULL,atender_cliente_v1,fd_conexion_ptr);




        //int tipo;

        //if (recv(*fd_conexion_ptr, &tipo, sizeof(int), 0) <= 0)
        
        //{
          //  perror("recv");
            //close(*fd_conexion_ptr);
            //free(fd_conexion_ptr);
            //continue;
        //}


        //printf("%d\n", tipo);
    }
}




int main(int argc, char *argv[]){

    info_socket *info = malloc(sizeof(info_socket));

    info->ip = strdup("127.0.0.1"); 
    info->puerto = strdup("9098");

    pthread_t hilo_servidor;
    pthread_create(&hilo_servidor, NULL, crear_servidor, info);
    pthread_join(hilo_servidor,NULL);
    return 0;
}

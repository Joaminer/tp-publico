#include <utils/sockets.h>//sospechoso....

int main(int argc, char *argv[])
{

    t_log *logger = log_create("server.log", "SERVER_PLANIFICADOR", true, LOG_LEVEL_INFO);

    char *IP = "127.0.0.1";
    char *PUERTO = "9098";

    int servidor = iniciar_servidor(IP, PUERTO, logger);

    atender_clientes_multihilo(servidor, logger, atender_cliente_v1);

    // pthread_t hilo_cliente;
    // pthread_create(&hilo_cliente, NULL, rutina_cliente, ...);

    return 0;
}

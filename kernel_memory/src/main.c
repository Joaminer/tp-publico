#include <utils/sockets.h>

int main(int argc, char* argv[]) {
    
   
    t_log *logger = log_create("server.log", "SERVER_KERNEL_MEMORY", true, LOG_LEVEL_INFO);

    char *IP = "127.0.0.1";
    char *PUERTO = "8000";

    int servidor = iniciar_servidor(IP, PUERTO, logger);

    atender_clientes_multihilo(servidor, logger, atender_cliente_v1);

    return 0;
}

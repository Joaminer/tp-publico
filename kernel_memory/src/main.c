#include <utils/sockets.h>

int main(int argc, char* argv[]) {
    
    t_log* logger = log_create("server.log", "kERNEL_MEMORY", true, LOG_LEVEL_INFO);

    log_info(logger, "Modulo KERNEL_MEMORY iniciado correctamente");

    sleep(10);

    log_destroy(logger); 

    return 0;
}

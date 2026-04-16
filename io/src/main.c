#include <utils/sockets.h>

int main(int argc, char* argv[]) {


        int conexion = crear_conexion("127.0.0.1", "9098");
  
    int numero = 0;


    while(numero != 5) {
        printf("\nEscribí un número , el es 5 para salir): ");
        scanf("%d", &numero); 

        if (numero == 5) {
            break; 
        }

        send(conexion, &numero, sizeof(int), 0);

        printf("Enviado al servidor: %d\n", numero);
    }
   
    liberar_conexion(conexion);
    printf("Conexión terminada.\n");
    
    return 0;
}

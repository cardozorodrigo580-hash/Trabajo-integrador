#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "struct.h"
#include "clientes.c"
#include "alquileres.c"
#include "movimientos.c"
#include "vehiculos.c"

// Declaracion de funciones
void submenuVeh();
void registrarVehiculo();
void listarVehiculo();
int buscar_id(void);

int main() {
    int opcion;
    do {
        printf("========================================\n");
        printf("   Sistema de Alquiler de Vehiculos\n");
        printf("========================================\n\n");
        printf("1. Clientes\n");
        printf("2. Vehiculos\n");
        printf("3. Alquileres\n");
        printf("4. Movimientos\n");
        printf("0. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuClientes();
                break;
            case 2:
                submenuVeh();
                break;
            case 3:
                menuAlquileres();
                break;
            case 4:
                menumov();
                break;
            case 0:
                printf("\nSaliendo del programa...\n");
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n\n");
        }
    } while (opcion != 0);

    return 0;
}


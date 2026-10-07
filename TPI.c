#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "struct.h"


int main() {
    int opcion;
    do {
        printf("========================================\n");
        printf("||  Sistema de Alquiler de Vehiculos  ||\n");
        printf("========================================\n");
        printf("Ingrese una opcion:\n");
        printf("1. Clientes\n");
        printf("2. Vehiculos\n");
        printf("3. Alquileres\n");
        printf("4. Movimientos\n");
        printf("0. Salir\n");
        
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n--- Gestion de Clientes ---\n\n");
                break;
            case 2:
                printf("\n--- Gestion de Vehiculos ---\n\n");
                break;
            case 3:
                printf("\n--- Gestion de Alquileres ---\n\n");
                break;
            case 4:
                printf("\n--- Gestion de Movimientos ---\n\n");
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
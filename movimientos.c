#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "struct.h"

void menuMovimientos() {
    int opt;
    do {
        printf("=======================================\n");
        printf("         Gestion de Movimientos\n");
        printf("=======================================\n");
        printf("1. Registrar un movimiento\n");
        printf("2. Consultar movimientos\n");
        printf("0. Volver al menu principal\n");
        printf("\nIngrese una opcion: ");
        scanf("%d", &opt);
        switch(opt) {
            case 0:
                printf("\nSaliendo del menu de movimientos...\n");
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                printf("Por favor ingrese una opcion valida.\n");
                break;
        }                          
    } while (opt != 0); 
}
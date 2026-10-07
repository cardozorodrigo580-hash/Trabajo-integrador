#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <conio.h>
#include "struct.h"

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
                printf("\n--- Gestion de Clientes ---\n\n");
                break;
            case 2:
                submenuVeh();
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

void submenuVeh(){
    int opcion;
    do {
        printf("\n--- Gestion de Vehiculos ---\n\n");
        printf("========================================\n");
        printf("1. Listar Vehiculo\n");
        printf("2. Registrar Vehiculo\n");
        printf("0. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                listarVehiculo();
                break;
            case 2:
                registrarVehiculo();
                break;
            case 0:
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n\n");
        }
    } while (opcion != 0);
}


int buscar_id(void) {
    FILE *arch;
    vehi_dat vehiculo;
    int ultimo_id = 0;
    
    if ((arch = fopen("vehiculos.dat", "rb")) != NULL) {
        while (fread(&vehiculo, sizeof(vehi_dat), 1, arch) == 1) {
            ultimo_id = vehiculo.idVeh;
        }
        fclose(arch);
    }
    return ultimo_id; 
}

void registrarVehiculo(){
    FILE * arch;
    int id = buscar_id(); 
    char aux[3];
    vehi_dat vehiculo;
    
    printf("\n Ingrese la patente:\t");
    scanf(" %[^\n]", vehiculo.patente);

    printf("\n Ingrese la marca:\t");
    scanf(" %[^\n]", vehiculo.marca);

    printf("\n Ingrese el modelo:\t"); 
    scanf(" %[^\n]", vehiculo.modelo);

    printf("\n Ingrese el anio:\t");
    scanf("%d", &vehiculo.anio);      

    printf("\n Ingrese la capacidad:\t");
    scanf("%d", &vehiculo.capacidad);  

    printf("\n Ingrese el estado (1: DISPONIBLE 2: ALQUILADO 3: BAJA O EN MANTENIMIENTO):\t");
    scanf("%d", &vehiculo.estado);     

    printf("\n Ingrese el tipo de vehiculo: \t");
    scanf(" %[^\n]", vehiculo.tvehiculo);

    printf("\n Ingrese el precio por dia:\t");
    scanf("%f", &vehiculo.precio);     


    if (id == 0) { 
        vehiculo.idVeh = 1; 
        strcpy(aux,"wb");  
    } else {
        vehiculo.idVeh = id + 1; 
        strcpy(aux,"ab");   
    }

    if((arch = fopen("vehiculos.dat", aux)) != NULL){
        fwrite(&vehiculo, sizeof(vehi_dat), 1, arch);
        fclose(arch);
        printf("\nVehiculo registrado con exito! ID Asignado: %d\n", vehiculo.idVeh);
    } else {
        printf("Error de apertura de archivo\n");
    }
}

void listarVehiculo(){
    FILE * arch;
    vehi_dat vehiculo; 

    if((arch = fopen("vehiculos.dat", "rb")) != NULL){
        printf("\n--- Listado de Vehiculos ---\n");

        while(fread(&vehiculo, sizeof(vehi_dat), 1, arch) == 1){ 
            printf("\n----------------------");
            printf("\nId: %d", vehiculo.idVeh);                     
            printf("\nPATENTE: %s", vehiculo.patente);              
            printf("\nMARCA: %s", vehiculo.marca);                  
            printf("\nMODELO: %s", vehiculo.modelo);               
            printf("\nANIO: %d", vehiculo.anio);                
            printf("\nCAPACIDAD: %d", vehiculo.capacidad);         
            printf("\nESTADO (1:DISP 2:ALQ 3:BAJA): %d", vehiculo.estado); 
            printf("\nTIPO DE VEHICULO: %s", vehiculo.tvehiculo);   
            printf("\nPRECIO POR DIA: $%.2f", vehiculo.precio);     
        }
        printf("\n\n----------------------\n");
        fclose(arch);
    } else {
        printf("\nNo hay vehiculos registrados todavia (archivo inexistente).\n");
    }
}
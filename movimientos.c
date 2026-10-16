#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stddef.h>
#include "struct.h"

int buscar(int nro) {
    FILE *arch = fopen("alquileres.dat", "rb");
    if (arch == NULL) {
        printf("No se pudo abrir el archivo o aun no hay alquileres registrados.\n");
        return -1;
    }
    
    alq_dat alq; 
    int pos = 0;
    int encontro = -1;
    
    while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
        if (alq.nroalquiler == nro) {
            encontro = pos;
            break;
        }
        pos++;
    }                   
    fclose(arch);
    return encontro;
}


float btotal(int pos) {
    FILE *arch = fopen("alquileres.dat", "rb");
    if (arch == NULL) {
        return -1.0f;
    }
    float total = 0;
    long offset = (pos * sizeof(alq_dat)) + offsetof(alq_dat, importetot);
    fseek(arch, offset, SEEK_SET);
    
    if (fread(&total, sizeof(float), 1, arch) == 1) {
        fclose(arch);
        return total;
    } else {
        fclose(arch);
        return -1.0f;
    }
}


float bpagado(int pos) {
    FILE *arch = fopen("alquileres.dat", "rb");
    if (arch == NULL) {
        return -1.0f;
    }
    float pagado = 0;
    long offset = (pos * sizeof(alq_dat)) + offsetof(alq_dat, pagado);
    fseek(arch, offset, SEEK_SET);
    
    if (fread(&pagado, sizeof(float), 1, arch) == 1) {
        fclose(arch);
        return pagado;
    } else {
        fclose(arch);
        return -1.0f;
    }
}

void rmov(void) {
    
}

void consultar(void) {   
    int nro;
    char opcion;
    
    printf("\n=== REGISTRO DE PAGO DE ALQUILER ===\n");
    printf("Ingrese el numero de alquiler: ");
    scanf("%d", &nro);
    
    int pos = buscar(nro);
    if (pos == -1) {
        printf("No se encontro el numero de alquiler especificado.\n");
        return;
    }
    
    float total = btotal(pos);
    float pagado = bpagado(pos);
    float saldo = total - pagado;
    
    printf("Usted tiene un saldo pendiente de: $%.2f\n", saldo);
    printf("Desea registrar un pago? (s/n): ");
    scanf(" %c", &opcion);
    
    if (opcion == 's' || opcion == 'S') {
        float monto;
        printf("Ingrese el monto a pagar: ");
        scanf("%f", &monto);
        printf("Ingrese la fecha del pago (dia mes anio): ");
        int fecha[3];
        scanf("%d %d %d", &fecha[0], &fecha[1], &fecha[2]);

        if (monto > saldo) {
            printf("El monto ingresado es mayor al saldo pendiente.\n");
            return;
        }
        
       
        FILE *arch1 = fopen("movimientos.dat", "ab"); 
        if (arch1 == NULL) {
            printf("No se pudo abrir el archivo de movimientos.\n");
            return;
        }
        
        mov_dat mov;
        mov.nroalquiler = nro;
        mov.tipo_movimiento = 1; 
        mov.monto = monto;
        mov.fecha[0] = fecha[0]; 
        mov.fecha[1] = fecha[1];
        mov.fecha[2] = fecha[2];
        
        fwrite(&mov, sizeof(mov_dat), 1, arch1);
        fclose(arch1);
        
        
        FILE *arch = fopen("alquileres.dat", "r+b");
        if (arch == NULL) {
            printf("No se pudo actualizar el archivo de alquileres.\n");
            return;
        }
        
        long offset = (pos * sizeof(alq_dat)) + offsetof(alq_dat, pagado);
        fseek(arch, offset, SEEK_SET);
        pagado += monto;
        fwrite(&pagado, sizeof(float), 1, arch);
        fclose(arch);
        
        printf("Pago registrado. Su nuevo saldo pendiente es: $%.2f\n", total - pagado);
    } else if (opcion == 'n' || opcion == 'N') {
        printf("No se registro ningun pago.\n");
    } else {
        printf("Opcion invalida. No se registro ningun pago.\n");
    }
}

void menumov(void) {
    int opt;
    do {
        printf("\n=======================================\n");
        printf("        Gestion de Movimientos\n");
        printf("=======================================\n");
        printf("1. Registrar un movimiento / pago\n");
        printf("2. Consultar movimientos\n");
        printf("0. Volver al menu principal\n");
        printf("\nIngrese una opcion: ");
        scanf("%d", &opt);
        
        switch(opt) {
            case 0:
                printf("\nVolviendo al menu principal...\n");
                break;
            case 1:
                rmov();
                break;
            case 2:
                consultar();
                break;
            default:
                printf("Por favor ingrese una opcion valida.\n");
                break;
        }                          
    } while (opt != 0); 
}
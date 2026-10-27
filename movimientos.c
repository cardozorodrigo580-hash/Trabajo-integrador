#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stddef.h>
#include "struct.h"

// Retorna el ultimo nro de movimiento para autoincrementar el ID
int obtnromov(void) {
    FILE *arch = fopen("movimientos.dat", "rb");
    if (arch == NULL) {
        return 0;
    }
    mov_dat mov;
    int ult = 0;
    while (fread(&mov, sizeof(mov_dat), 1, arch) == 1) {
        ult = mov.nromovimiento;
    }
    fclose(arch);
    return ult;
}

// Busca la posicion del alquiler y recupera el DNI y la Patente
int buscar(int nro, long *dni, char *patente) {
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
            if (dni != NULL) *dni = alq.dni;
            if (patente != NULL) strcpy(patente, alq.patente);
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

// Guarda/Actualiza el historial en movimientos_DNI.txt
void actualizartxt(mov_dat mov) {
    char nomarch[50];
    sprintf(nomarch, "movimientos_%ld.txt", mov.dni);

    FILE *archtxt = fopen(nomarch, "a");
    if (archtxt != NULL) {
        fprintf(archtxt, "Movimiento: %d | Alquiler: %d | Tipo: %d | Monto: $%.2f | Fecha: %02d/%02d/%d | Patente: %s\n",
                mov.nromovimiento, mov.nroalquiler, mov.tipo_movimiento, mov.monto,
                mov.fecha[0], mov.fecha[1], mov.fecha[2], mov.patente);
        fclose(archtxt);
    }
}

void rmov(void) {
    int nro;
    char opcion;
   
    printf("\n=== REGISTRO DE PAGO DE ALQUILER ===\n");
    printf("Ingrese el numero de alquiler: ");
    scanf("%d", &nro);
   
    long dni_cli = 0;
    char pat_veh[10] = "";
    int pos = buscar(nro, &dni_cli, pat_veh);

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
        mov.nromovimiento = obtnromov() + 1;
        mov.nroalquiler = nro;
        mov.tipo_movimiento = 1;
        mov.monto = monto;
        mov.fecha[0] = fecha[0];
        mov.fecha[1] = fecha[1];
        mov.fecha[2] = fecha[2];
        mov.dni = dni_cli;
        strcpy(mov.patente, pat_veh);
       
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

        // Actualiza el archivo de texto del cliente
        actualizartxt(mov);

    } else if (opcion == 'n' || opcion == 'N') {
        printf("No se registro ningun pago.\n");
    } else {
        printf("Opcion invalida. No se registro ningun pago.\n");
    }
}

void consultar(void) {  
    int nro;
   
    printf("\n=== CONSULTAR MOVIMIENTOS DE UN ALQUILER ===\n");
    printf("Ingrese el numero de alquiler a consultar: ");
    scanf("%d", &nro);
   
    // 1. Validar si el alquiler existe en el archivo principal ("alquileres.dat")
    int pos = buscar(nro, NULL, NULL);
    if (pos == -1) {
        printf("[ERROr] El numero de alquiler %d no existe en el sistema.\n", nro);
        return;
    }
   
    // 2. Abrir el archivo de movimientos en modo lectura
    FILE *arch = fopen("movimientos.dat", "rb");
    if (arch == NULL) {
        printf("[AVISo] El archivo de movimientos no existe todavia o no hay pagos registrados.\n");
        return;
    }
   
    mov_dat mov;
    int encontrados = 0;
   
    printf("\n--- Movimientos / Pagos para el Alquiler Nro: %d ---\n", nro);
   
    // 3. Recorrer el archivo y filtrar solo los movimientos de este alquiler
    while (fread(&mov, sizeof(mov_dat), 1, arch) == 1) {
        if (mov.nroalquiler == nro) {
            encontrados++;
            printf("  > Nro Movimiento  : %d\n", mov.nromovimiento);
            printf("  > DNI Cliente     : %ld\n", mov.dni);
            printf("  > Patente         : %s\n", mov.patente);
            printf("  > Tipo Movimiento : %d\n", mov.tipo_movimiento);
            printf("  > Monto pagado    : $%.2f\n", mov.monto);
            printf("  > Fecha del pago  : %02d/%02d/%d\n", mov.fecha[0], mov.fecha[1], mov.fecha[2]);
            printf("-----------------------------------------\n");
        }
    }
   
    // 4. Validar si el alquiler existia pero no tenia pagos registrados
    if (encontrados == 0) {
        printf("[INFo] El alquiler existe, pero todavia no registra pagos.\n");
    } else {
        printf("Total de pagos encontrados: %d\n", encontrados);
    }
   
    fclose(arch);
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
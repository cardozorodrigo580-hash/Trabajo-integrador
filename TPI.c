#include <stdio.h>
#include <stdlib.h>

typedef struct Clientes_dat {
    long dni;
    char nomApe[30];
    long tel;
    char email[20];
    int fecha[3]; // [0] Día, [1] Mes, [2] Año
    int estado;   // 1: Activo, 0: Inactivo
}client;

struct Vehiculo {
    char patente[10];
    char marca[20];
    char modelo[20];
    int anio;
    int estado;    // 1: Disponible, 2: Alquilado, 3: En mantenimiento
    int capacidad;
    float precio;
    char tvehiculo[20];
};

struct Alquiler {
    int nroalquiler;
long dni;
    char patente[10];
    int fechainicio[3];
    int estado;       // 1: Activo, 0: Inactivo
    int fechaprev[3];
    int fechaefec[3];
    int contdias;
    float importetot;
    float pagado;
};

struct Movimiento {
    int nromovimiento;
    int nroalquiler;
    int tipo_movimiento; // 1: Pago, 2: Ajuste
    float monto;
    int fecha[3];
    long dni;
    char patente[10];
};

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
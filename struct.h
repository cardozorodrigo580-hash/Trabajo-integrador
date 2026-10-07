#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


typedef struct Clientes_dat {
    long dni;
    char nomApe[30];
    long tel;
    char email[20];
    int fecha[3]; // [0] Día, [1] Mes, [2] Año
    int estado;   // 1: Activo, 0: Inactivo
}cli_dat;


typedef struct Vehiculos_dat {
    char patente[10];
    char marca[20];
    char modelo[20];
    int anio;
    int estado;    // 1: Disponible, 2: Alquilado, 3: En mantenimiento
    int capacidad;
    float precio;
    char tvehiculo[20];
}vehi_dat;


typedef struct Alquileres_dat {
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
}alq_dat;


typedef struct Movimientos_dat {
    int nromovimiento;
    int nroalquiler;
    int tipo_movimiento; // 1: Pago, 2: Ajuste
    float monto;
    int fecha[3];
    long dni;
    char patente[10];
}mov_dat;

#endif
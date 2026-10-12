#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <conio.h>
#include "struct.h"

void submenuVeh();
void registrarVehiculo();
void listarVehiculo();
int buscar_id(void);
float CalcularImporAlquiler(int dias, float precioxdia);
float CalcularSaldoPend(float importetot, float pagado);
int esBisiesto(int anio);
int CalcularDiasEntreFechas(int fechaInicio[3], int fechaFin[3]);
int buscarAlqActivoxPatente(const char patente[10]);
int buscarAlqActivoxDNI(long dni);
void registrarAlquiler();
void registrarDevolucion();
void consultarAlquileres();
void menuAlquileres();

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
                 menuAlquileres();
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
                system("cls");
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
void menuAlquileres() {
    int opt;
    do {
        printf("1. Registrar un alquiler\n");
        printf("2. Registrar una devolucion\n");
        printf("3. Consultar alquileres\n");
        printf("0. Volver al menu principal\n");
        printf("\nIngrese una opcion: ");
        scanf("%d", &opt);
        switch(opt) {
            case 0:
                printf("\nVolviendo al menu principal...\n");
                break;
            case 1:
                registrarAlquiler();
                break;
            case 2:
                registrarDevolucion();
                break;
            case 3:
                consultarAlquileres();
                break;
            default:
                printf("Por favor ingrese una opcion valida (0 a 3).\n");
                break;
        }                          
    } while (opt != 0);    
}

int buscarAlqActivoxPatente(const char patente[10]) {
    FILE *arch;
    alq_dat alq; 
    int pos = 0;
    int posenc = -1;
    if ((arch = fopen("alquileres.dat", "rb")) != NULL) {
        while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
            if (strcmp(alq.patente, patente) == 0 && alq.estado == 1) {
                posenc = pos;
                break;    
            }
            pos++;
        }    
        fclose(arch);
    }    
    return posenc;
}

int buscarAlqActivoxDNI(long dni) {
    FILE *arch;
    alq_dat alq;
    int posi = 0;
    int posienc = -1;
    if ((arch = fopen("alquileres.dat", "rb")) != NULL) {
        while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
            if (alq.dni == dni && alq.estado == 1) {
                posienc = posi;
                break;
            }
            posi++;
        }    
        fclose(arch);
    }            
    return posienc;    
}

void registrarDevolucion() {
    FILE *arch;
    alq_dat alq; 
    char patente[10];
    int pos;
    float saldoPendiente;
    
    printf("\n--- Registrar Devolucion ---\n");
    printf("Ingrese la patente del vehiculo a devolver: ");
    scanf("%9s", patente);
    pos = buscarAlqActivoxPatente(patente);
    if (pos == -1) {
        printf("\n[ERROR] No se ha encontrado un alquiler activo para la patente %s.\n", patente);
    } else {
        if ((arch = fopen("alquileres.dat", "r+b")) != NULL) {
            fseek(arch, pos * sizeof(alq_dat), SEEK_SET);
            if (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
                alq.estado = 0;
                printf("\nIngrese la fecha efectiva de la devolucion (dia mes anio): ");
                scanf("%d %d %d", &alq.fechaefec[0], &alq.fechaefec[1], &alq.fechaefec[2]);
                saldoPendiente = CalcularSaldoPend(alq.importetot, alq.pagado);
                printf("\n--- Resumen de devolucion --- \n");
                printf("Patente: %s / DNI: %ld\n", alq.patente, alq.dni);
                printf("Saldo a cobrar: $%.2f\n", saldoPendiente);
                
                fseek(arch, -((long)sizeof(alq_dat)), SEEK_CUR);
                if (fwrite(&alq, sizeof(alq_dat), 1, arch) == 1) {
                    printf("\n[EXITO] Devolucion registrada y auto liberado correctamente.\n");
                } else {
                    printf("\n[ERROR] No se pudo actualizar el archivo.\n");
                }
            }
            fclose(arch);
        } else {
            printf("\nError en la lectura del archivo.\n");
        }
    }
}

void registrarAlquiler() {
    FILE *arch;
    alq_dat alq; 
    float precioxdia;
    
    printf("\n --- Registrar un nuevo alquiler --- \n");
    printf("Ingrese el DNI del cliente a registrar: ");
    scanf("%ld", &alq.dni);
    if (buscarAlqActivoxDNI(alq.dni) != -1) {
        printf("El cliente con DNI %ld ya posee un alquiler activo.\n", alq.dni);
        return;
    } else {
        printf("Ingrese la patente del vehiculo: ");
        scanf("%9s", alq.patente);
        if (buscarAlqActivoxPatente(alq.patente) != -1) {
            printf("El vehiculo con patente %s ya se encuentra alquilado.\n", alq.patente);
            return;
        } else {
            printf("Ingrese el numero del alquiler: ");
            scanf("%d", &alq.nroalquiler);
            printf("Ingrese fecha de inicio (dia mes anio): ");
            scanf("%d %d %d", &alq.fechainicio[0], &alq.fechainicio[1], &alq.fechainicio[2]);
            printf("Ingrese fecha prevista de devolucion (dia mes anio): ");
            scanf("%d %d %d", &alq.fechaprev[0], &alq.fechaprev[1], &alq.fechaprev[2]);
            
            alq.contdias = CalcularDiasEntreFechas(alq.fechainicio, alq.fechaprev);
            
            printf("Ingrese el precio por dia del vehiculo: $");
            scanf("%f", &precioxdia);
            
            alq.importetot = CalcularImporAlquiler(alq.contdias, precioxdia);
            
            printf("Ingrese el pago inicial: $");
            scanf("%f", &alq.pagado);
            
            alq.estado = 1;
            
            if ((arch = fopen("alquileres.dat", "ab")) != NULL) {
                if (fwrite(&alq, sizeof(alq_dat), 1, arch) == 1) {
                    printf("\n[EXITO] Alquiler registrado correctamente.\n");
                    printf("Importe total: $%.2f | Saldo pendiente: $%.2f\n", 
                            alq.importetot, CalcularSaldoPend(alq.importetot, alq.pagado));
                } else {
                    printf("Error al modificar el registro del archivo.\n");
                }
                fclose(arch);
            } else {
                printf("Error en la lectura del archivo de alquileres.\n");
            }
        }                                                                      
    }
}   

int esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int CalcularDiasEntreFechas(int fechainicio[3], int fechaFin[3]) {
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d1 = fechainicio[0], m1 = fechainicio[1], a1 = fechainicio[2];
    int d2 = fechaFin[0], m2 = fechaFin[1], a2 = fechaFin[2];
    
    long dias1 = d1;
    for (int i = 1; i < m1; i++) {
        int d = diasMes[i];
        if (i == 2 && esBisiesto(a1)) d = 29;
        dias1 += d;
    }
    for (int y = 0; y < a1; y++) {
        dias1 += esBisiesto(y) ? 366 : 365;
    }
    
    long dias2 = d2;
    for (int i = 1; i < m2; i++) {
        int d = diasMes[i];
        if (i == 2 && esBisiesto(a2)) d = 29;
        dias2 += d;
    }
    for (int y = 0; y < a2; y++) {
        dias2 += esBisiesto(y) ? 366 : 365;
    }       
    
    int diferencia = (int)(dias2 - dias1);
    return (diferencia >= 0) ? diferencia : 0;
}                  

float CalcularImporAlquiler(int dias, float precioxdia) {
    return dias * precioxdia;
}

float CalcularSaldoPend(float importetot, float pagado) {
    return importetot - pagado;
}	

void consultarAlquileres() {
    FILE *arch = fopen("alquileres.dat", "rb");
    if (arch == NULL) {
        printf("No se pudo abrir el archivo o aun no hay alquileres registrados.\n");
        return;
    }
    
    alq_dat alq; 
    
    printf("\n========================================\n");
    printf("         LISTADO DE ALQUILERES          \n");
    printf("========================================\n");
    
    while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
        printf("Nro Alquiler  : %d\n", alq.nroalquiler);
        printf("DNI           : %ld\n", alq.dni);
        printf("Patente       : %s\n", alq.patente);
        printf("F. Inicio     : %02d/%02d/%d\n", alq.fechainicio[0], alq.fechainicio[1], alq.fechainicio[2]);
        printf("F. Prevista   : %02d/%02d/%d\n", alq.fechaprev[0], alq.fechaprev[1], alq.fechaprev[2]);
        printf("Dias          : %d\n", alq.contdias);
        printf("Importe Total : $%.2f\n", alq.importetot);
        printf("Pagado        : $%.2f\n", alq.pagado);
        printf("Estado        : %s\n", (alq.estado == 1) ? "Activo" : "Inactivo");
        printf("----------------------------------------\n");
    }
    
    fclose(arch);
}

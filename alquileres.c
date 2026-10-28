#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"


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
    int fechaValida = 0;
    
    printf("\n--- Registrar Devolucion ---\n");
    printf("Ingrese la patente del vehiculo a devolver: ");
    if (scanf("%9s", patente) == 1) {
        for (int i = 0; patente[i] != '\0'; i++) {
            patente[i] = toupper((unsigned char)patente[i]);
        }
    }
    
    pos = buscarAlqActivoxPatente(patente);
    if (pos == -1) {
        printf("\n[ERROR] No se ha encontrado un alquiler activo para la patente %s.\n", patente);
    } else {
        if ((arch = fopen("alquileres.dat", "r+b")) != NULL) {
            fseek(arch, pos * sizeof(alq_dat), SEEK_SET);
            if (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
                alq.estado = 0;
                
                do {
                    int d, m, a;
                    printf("\nIngrese la fecha efectiva de la devolucion (dia mes anio): ");
                    if (scanf("%d %d %d", &d, &m, &a) != 3) {
                        printf("\n[ERROR] Formato incorrecto. Debe ingresar numeros enteros.\n");
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                    } else {
                        struct tm fecha_ingresada = {0};
                        fecha_ingresada.tm_mday = d;
                        fecha_ingresada.tm_mon = m - 1;
                        fecha_ingresada.tm_year = a - 1900;

                        struct tm copia = fecha_ingresada;
                        time_t controlFecha = mktime(&copia);

                        if (controlFecha == -1 || copia.tm_mday != d || copia.tm_mon != m - 1 || copia.tm_year != a - 1900) {
                            printf("\n[ERROR] La fecha %02d/%02d/%d no es valida.\n", d, m, a);
                        } else {
                            struct tm fecha_inicio = {0};
                            fecha_inicio.tm_mday = alq.fechainicio[0];
                            fecha_inicio.tm_mon = alq.fechainicio[1] - 1;
                            fecha_inicio.tm_year = alq.fechainicio[2] - 1900;
                            time_t t_inicio = mktime(&fecha_inicio);

                            if (controlFecha < t_inicio) {
                                printf("\n[ERROR] La fecha de devolucion no puede ser anterior a la fecha de inicio del alquiler.\n");
                            } else {
                                alq.fechaefec[0] = d;
                                alq.fechaefec[1] = m;
                                alq.fechaefec[2] = a;
                                fechaValida = 1;
                            }
                        }
                    }
                } while (!fechaValida);

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
    int fechaInicioValida = 0;
    int fechaPrevValida = 0;
    
    printf("\n --- Registrar un nuevo alquiler --- \n");
    printf("Ingrese el DNI del cliente a registrar: ");
    if (scanf("%ld", &alq.dni) != 1) {
        printf("[ERROR] DNI invalido.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    
    if (buscarAlqActivoxDNI(alq.dni) != -1) {
        printf("El cliente con DNI %ld ya posee un alquiler activo.\n", alq.dni);
        return;
    } else {
        printf("Ingrese la patente del vehiculo: ");
        if (scanf("%9s", alq.patente) == 1) {
            for (int i = 0; alq.patente[i] != '\0'; i++) {
                alq.patente[i] = toupper((unsigned char)alq.patente[i]);
            }
        }
        
        if (buscarAlqActivoxPatente(alq.patente) != -1) {
            printf("El vehiculo con patente %s ya se encuentra alquilado.\n", alq.patente);
            return;
        } else {
            printf("Ingrese el numero del alquiler: ");
            scanf("%d", &alq.nroalquiler);
            
            do {
                int d, m, a;
                printf("Ingrese fecha de inicio (dia mes anio): ");
                if (scanf("%d %d %d", &d, &m, &a) != 3) {
                    printf("[ERROR] Formato incorrecto.\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                } else {
                    struct tm fecha_ing = {0};
                    fecha_ing.tm_mday = d;
                    fecha_ing.tm_mon = m - 1;
                    fecha_ing.tm_year = a - 1900;
                    struct tm copia = fecha_ing;
                    time_t t_ctrl = mktime(&copia);

                    if (t_ctrl == -1 || copia.tm_mday != d || copia.tm_mon != m - 1 || copia.tm_year != a - 1900) {
                        printf("[ERROR] La fecha %02d/%02d/%d no es valida.\n", d, m, a);
                    } else {
                        alq.fechainicio[0] = d;
                        alq.fechainicio[1] = m;
                        alq.fechainicio[2] = a;
                        fechaInicioValida = 1;
                    }
                }
            } while (!fechaInicioValida);

            do {
                int d, m, a;
                printf("Ingrese fecha prevista de devolucion (dia mes anio): ");
                if (scanf("%d %d %d", &d, &m, &a) != 3) {
                    printf("[ERROR] Formato incorrecto.\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                } else {
                    struct tm fecha_ing = {0};
                    fecha_ing.tm_mday = d;
                    fecha_ing.tm_mon = m - 1;
                    fecha_ing.tm_year = a - 1900;
                    struct tm copia = fecha_ing;
                    time_t t_ctrl = mktime(&copia);

                    if (t_ctrl == -1 || copia.tm_mday != d || copia.tm_mon != m - 1 || copia.tm_year != a - 1900) {
                        printf("[ERROR] La fecha %02d/%02d/%d no es valida.\n", d, m, a);
                    } else {
                        struct tm fecha_ini = {0};
                        fecha_ini.tm_mday = alq.fechainicio[0];
                        fecha_ini.tm_mon = alq.fechainicio[1] - 1;
                        fecha_ini.tm_year = alq.fechainicio[2] - 1900;
                        time_t t_ini = mktime(&fecha_ini);

                        if (t_ctrl < t_ini) {
                            printf("[ERROR] La fecha prevista no puede ser anterior a la de inicio.\n");
                        } else {
                            alq.fechaprev[0] = d;
                            alq.fechaprev[1] = m;
                            alq.fechaprev[2] = a;
                            fechaPrevValida = 1;
                        }
                    }
                }
            } while (!fechaPrevValida);
            
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

long convertirFechaADias(int d, int m, int a) {
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long totalDias = d;
    
    for (int i = 1; i < m; i++) {
        int diasM = diasMes[i];
        if (i == 2 && ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0))) {
            diasM = 29;
        }
        totalDias += diasM;
    }
    
    for (int y = 0; y < a; y++) {
        totalDias += ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) ? 366 : 365;
    }
    
    return totalDias;
}

int esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int CalcularDiasEntreFechas(int fechainicio[3], int fechaFin[3]) {
    long dias1 = convertirFechaADias(fechainicio[0], fechainicio[1], fechainicio[2]);
    long dias2 = convertirFechaADias(fechaFin[0], fechaFin[1], fechaFin[2]);
    
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
    int contador = 0;
    
    printf("\n========================================\n");
    printf("         LISTADO DE ALQUILERES          \n");
    printf("========================================\n");
    
    while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
        contador++;
        printf("Nro Alquiler  : %d\n", alq.nroalquiler);
        printf("DNI           : %ld\n", alq.dni);
        printf("Patente       : %s\n", alq.patente);
        printf("F. Inicio     : %02d/%02d/%d\n", alq.fechainicio[0], alq.fechainicio[1], alq.fechainicio[2]);
        printf("F. Prevista   : %02d/%02d/%d\n", alq.fechaprev[0], alq.fechaprev[1], alq.fechaprev[2]);
        
        if (alq.estado == 0) {
            printf("F. Efectiva   : %02d/%02d/%d\n", alq.fechaefec[0], alq.fechaefec[1], alq.fechaefec[2]);
        }
        
        printf("Dias          : %d\n", alq.contdias);
        printf("Importe Total : $%.2f\n", alq.importetot);
        printf("Pagado        : $%.2f\n", alq.pagado);
        printf("Saldo Pend.   : $%.2f\n", CalcularSaldoPend(alq.importetot, alq.pagado));
        printf("Estado        : %s\n", (alq.estado == 1) ? "Activo" : "Inactivo");
        printf("----------------------------------------\n");
    }
    
    printf("Total de alquileres listados: %d\n", contador);
    fclose(arch);
}
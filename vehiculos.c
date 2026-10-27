#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"

#define ARCHIVO_VEHICULOS "vehiculos.dat"
#define ARCHIVO_ALQUILERES "alquileres.dat"

// ============================================================================
// FUNCIONES AUXILIARES Y DE VALIDACION
// ============================================================================

void aMayusculas(char *cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        cadena[i] = toupper((unsigned char)cadena[i]);
        i++;
    }
}

void limpiarBufferVeh(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void quitarSaltoLinea(char *cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
            break;
        }
        i++;
    }
}

int existePatente(const char *patente) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "rb");
    if (arch == NULL) {
        return 0;
    }

    vehi_dat v;
    while (fread(&v, sizeof(vehi_dat), 1, arch) == 1) {
        if (strcmp(v.patente, patente) == 0) {
            fclose(arch);
            return 1;
        }
    }

    fclose(arch);
    return 0;
}

int buscar_id(void) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "rb");
    if (arch == NULL) {
        return 0;
    }

    vehi_dat vehiculo;
    int ultimo_id = 0;
    while (fread(&vehiculo, sizeof(vehi_dat), 1, arch) == 1) {
        ultimo_id = vehiculo.idVeh;
    }

    fclose(arch);
    return ultimo_id;
}

int buscarPosicionVehiculoPorPatente(const char *patente, vehi_dat *outVehiculo) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "rb");
    if (arch == NULL) {
        return -1;
    }

    vehi_dat v;
    int pos = 0;
    while (fread(&v, sizeof(vehi_dat), 1, arch) == 1) {
        if (strcmp(v.patente, patente) == 0) {
            if (outVehiculo != NULL) {
                *outVehiculo = v;
            }
            fclose(arch);
            return pos;
        }
        pos++;
    }

    fclose(arch);
    return -1;
}

const char* obtenerNombreEstadoVehiculo(int estado) {
    switch (estado) {
        case 1: return "Disponible";
        case 2: return "Alquilado";
        case 3: return "En Mantenimiento / Baja";
        default: return "Desconocido";
    }
}

void mostrarFichaVehiculo(const vehi_dat *v) {
    printf("----------------------------------------\n");
    printf(" ID Vehiculo  : %d\n", v->idVeh);
    printf(" Patente      : %s\n", v->patente);
    printf(" Marca/Modelo : %s %s\n", v->marca, v->modelo);
    printf(" Anio         : %d\n", v->anio);
    printf(" Capacidad    : %d personas\n", v->capacidad);
    printf(" Tipo         : %s\n", v->tvehiculo);
    printf(" Precio / Dia : $%.2f\n", v->precio);
    printf(" Estado       : %s (%d)\n", obtenerNombreEstadoVehiculo(v->estado), v->estado);
    printf("----------------------------------------\n");
}

// ============================================================================
// FUNCIONES PRINCIPALES DEL MODULO VEHICULOS
// ============================================================================

void registrarVehiculo(void) {
    vehi_dat v;
    printf("\n=== REGISTRAR NUEVO VEHICULO ===\n");

    printf("Ingrese Patente: ");
    if (scanf("%9s", v.patente) != 1) {
        printf("[ERROR] Lectura de patente invalida.\n");
        limpiarBufferVeh();
        return;
    }
    aMayusculas(v.patente);
    limpiarBufferVeh();

    if (existePatente(v.patente)) {
        printf("[ERROR] La patente '%s' ya se encuentra registrada en la flota.\n", v.patente);
        return;
    }

    printf("Ingrese Marca: ");
    fgets(v.marca, sizeof(v.marca), stdin);
    quitarSaltoLinea(v.marca);

    printf("Ingrese Modelo: ");
    fgets(v.modelo, sizeof(v.modelo), stdin);
    quitarSaltoLinea(v.modelo);

    printf("Ingrese Tipo de Vehiculo (ej: Auto, SUV, Camioneta): ");
    fgets(v.tvehiculo, sizeof(v.tvehiculo), stdin);
    quitarSaltoLinea(v.tvehiculo);

    do {
        printf("Ingrese Anio (1900 - 2026): ");
        if (scanf("%d", &v.anio) != 1 || v.anio < 1900 || v.anio > 2026) {
            printf("[ERROR] Anio fuera de rango o invalido.\n");
            limpiarBufferVeh();
        } else {
            break;
        }
    } while (1);

    do {
        printf("Ingrese Capacidad de pasajeros (mayor a 0): ");
        if (scanf("%d", &v.capacidad) != 1 || v.capacidad <= 0) {
            printf("[ERROR] La capacidad debe ser un entero positivo.\n");
            limpiarBufferVeh();
        } else {
            break;
        }
    } while (1);

    do {
        printf("Ingrese Precio de alquiler por dia ($ > 0): ");
        if (scanf("%f", &v.precio) != 1 || v.precio <= 0.0f) {
            printf("[ERROR] El precio debe ser un importe mayor a cero.\n");
            limpiarBufferVeh();
        } else {
            break;
        }
    } while (1);

    v.estado = 1;
    v.idVeh = buscar_id() + 1;

    FILE *arch = fopen(ARCHIVO_VEHICULOS, "ab");
    if (arch == NULL) {
        printf("[ERROR] No se pudo abrir el archivo '%s' para guardar.\n", ARCHIVO_VEHICULOS);
        return;
    }

    if (fwrite(&v, sizeof(vehi_dat), 1, arch) == 1) {
        printf("\n[EXITO] Vehiculo registrado correctamente. ID Asignado: %d (Estado: Disponible)\n", v.idVeh);
    } else {
        printf("\n[ERROR] No se pudo escribir en el archivo binario.\n");
    }

    fclose(arch);
}

void modificarVehiculo(void) {
    char patenteBuscada[10];
    vehi_dat v;

    printf("\n=== MODIFICAR DATOS DE VEHICULO ===\n");
    printf("Ingrese la patente del vehiculo a modificar: ");
    if (scanf("%9s", patenteBuscada) != 1) {
        limpiarBufferVeh();
        return;
    }
    aMayusculas(patenteBuscada);
    limpiarBufferVeh();

    int pos = buscarPosicionVehiculoPorPatente(patenteBuscada, &v);
    if (pos == -1) {
        printf("[ERROR] Vehiculo con patente '%s' no encontrado.\n", patenteBuscada);
        return;
    }

    int opcion;
    int modificado = 0;

    do {
        printf("\n----------------------------------------\n");
        printf(" VEHICULO SELECCIONADO: %s %s (%s)\n", v.marca, v.modelo, v.patente);
        printf("----------------------------------------\n");
        printf("1. Modificar Marca (Actual: %s)\n", v.marca);
        printf("2. Modificar Modelo (Actual: %s)\n", v.modelo);
        printf("3. Modificar Anio (Actual: %d)\n", v.anio);
        printf("4. Modificar Capacidad (Actual: %d pasajeros)\n", v.capacidad);
        printf("5. Modificar Tipo (Actual: %s)\n", v.tvehiculo);
        printf("6. Modificar Precio por Dia (Actual: $%.2f)\n", v.precio);
        printf("7. Modificar Estado (Actual: %s [%d])\n", obtenerNombreEstadoVehiculo(v.estado), v.estado);
        printf("0. Guardar cambios y Salir\n");
        printf("----------------------------------------\n");
        printf("Seleccione el campo a modificar: ");

        if (scanf("%d", &opcion) != 1) {
            printf("[ERROR] Opcion invalida.\n");
            limpiarBufferVeh();
            continue;
        }
        limpiarBufferVeh();

        switch (opcion) {
            case 1:
                printf("Ingrese nueva Marca: ");
                fgets(v.marca, sizeof(v.marca), stdin);
                quitarSaltoLinea(v.marca);
                modificado = 1;
                break;

            case 2:
                printf("Ingrese nuevo Modelo: ");
                fgets(v.modelo, sizeof(v.modelo), stdin);
                quitarSaltoLinea(v.modelo);
                modificado = 1;
                break;

            case 3:
                do {
                    printf("Ingrese nuevo Anio (1900 - 2026): ");
                    if (scanf("%d", &v.anio) == 1 && v.anio >= 1900 && v.anio <= 2026) {
                        modificado = 1;
                        break;
                    }
                    printf("[ERROR] Anio invalido.\n");
                    limpiarBufferVeh();
                } while (1);
                limpiarBufferVeh();
                break;

            case 4:
                do {
                    printf("Ingrese nueva Capacidad (> 0): ");
                    if (scanf("%d", &v.capacidad) == 1 && v.capacidad > 0) {
                        modificado = 1;
                        break;
                    }
                    printf("[ERROR] Capacidad invalida.\n");
                    limpiarBufferVeh();
                } while (1);
                limpiarBufferVeh();
                break;

            case 5:
                printf("Ingrese nuevo Tipo de Vehiculo: ");
                fgets(v.tvehiculo, sizeof(v.tvehiculo), stdin);
                quitarSaltoLinea(v.tvehiculo);
                modificado = 1;
                break;

            case 6:
                do {
                    printf("Ingrese nuevo Precio por dia ($ > 0): ");
                    if (scanf("%f", &v.precio) == 1 && v.precio > 0.0f) {
                        modificado = 1;
                        break;
                    }
                    printf("[ERROR] Precio invalido.\n");
                    limpiarBufferVeh();
                } while (1);
                limpiarBufferVeh();
                break;

            case 7:
                do {
                    printf("Ingrese nuevo Estado (1: Disponible, 2: Alquilado, 3: Baja/Mantenimiento): ");
                    if (scanf("%d", &v.estado) == 1 && v.estado >= 1 && v.estado <= 3) {
                        modificado = 1;
                        break;
                    }
                    printf("[ERROR] Estado invalido.\n");
                    limpiarBufferVeh();
                } while (1);
                limpiarBufferVeh();
                break;

            case 0:
                break;

            default:
                printf("[ERROR] Opcion no valida.\n");
        }
    } while (opcion != 0);

    if (modificado) {
        FILE *arch = fopen(ARCHIVO_VEHICULOS, "r+b");
        if (arch == NULL) {
            printf("[ERROR] No se pudo abrir el archivo para guardar los cambios.\n");
            return;
        }

        fseek(arch, pos * sizeof(vehi_dat), SEEK_SET);
        if (fwrite(&v, sizeof(vehi_dat), 1, arch) == 1) {
            printf("\n[EXITO] Registro del vehiculo actualizado correctamente.\n");
        } else {
            printf("\n[ERROR] Fallo al escribir en el archivo.\n");
        }

        fclose(arch);
    } else {
        printf("\nNo se realizaron modificaciones en el vehiculo.\n");
    }
}

void buscarVehiculo(void) {
    char patente[10];
    vehi_dat v;

    printf("\n=== BUSCAR VEHICULO POR PATENTE ===\n");
    printf("Ingrese Patente: ");
    if (scanf("%9s", patente) != 1) {
        limpiarBufferVeh();
        return;
    }
    aMayusculas(patente);
    limpiarBufferVeh();

    if (buscarPosicionVehiculoPorPatente(patente, &v) != -1) {
        printf("\n[RESULTADO ENCONTRADO]\n");
        mostrarFichaVehiculo(&v);
    } else {
        printf("[ERROR] No existe ningun vehiculo registrado con la patente '%s'.\n", patente);
    }
}

void listarVehiculo(void) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "rb");
    if (arch == NULL) {
        printf("\n[AVISO] No hay vehiculos registrados todavia o el archivo no existe.\n");
        return;
    }

    vehi_dat v;
    int total = 0;

    printf("\n========================================\n");
    printf("       LISTADO COMPLETO DE FLOTA        \n");
    printf("========================================\n");

    while (fread(&v, sizeof(vehi_dat), 1, arch) == 1) {
        mostrarFichaVehiculo(&v);
        total++;
    }

    printf("\nTotal de vehiculos registrados: %d\n", total);
    fclose(arch);
}

void listarDisponibles(void) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "rb");
    if (arch == NULL) {
        printf("\n[AVISO] No hay vehiculos registrados todavia.\n");
        return;
    }

    vehi_dat v;
    int contador = 0;

    printf("\n========================================\n");
    printf("     VEHICULOS DISPONIBLES PARA ALQUILER\n");
    printf("========================================\n");

    while (fread(&v, sizeof(vehi_dat), 1, arch) == 1) {
        if (v.estado == 1) {
            mostrarFichaVehiculo(&v);
            contador++;
        }
    }

    if (contador == 0) {
        printf("\nActualmente no hay vehiculos disponibles en la flota.\n");
    } else {
        printf("\nTotal de vehiculos disponibles: %d\n", contador);
    }

    fclose(arch);
}

void listarAlquilados(void) {
    FILE *archVeh = fopen(ARCHIVO_VEHICULOS, "rb");
    if (archVeh == NULL) {
        printf("\n[AVISO] No hay vehiculos registrados todavia.\n");
        return;
    }

    vehi_dat v;
    int contador = 0;

    printf("\n=======================================================\n");
    printf("              VEHICULOS ACTUALMENTE ALQUILADOS         \n");
    printf("=======================================================\n");

    while (fread(&v, sizeof(vehi_dat), 1, archVeh) == 1) {
        if (v.estado == 2) {
            contador++;
            printf("\n-------------------------------------------------------\n");
            printf("Patente      : %s\n", v.patente);
            printf("Marca/Modelo : %s %s\n", v.marca, v.modelo);

            FILE *archAlq = fopen(ARCHIVO_ALQUILERES, "rb");
            int alqEncontrado = 0;

            if (archAlq != NULL) {
                alq_dat alq;
                while (fread(&alq, sizeof(alq_dat), 1, archAlq) == 1) {
                    if (strcmp(alq.patente, v.patente) == 0 && alq.estado == 1) {
                        printf("DNI Cliente  : %ld\n", alq.dni);
                        printf("F. Inicio    : %02d/%02d/%d\n", alq.fechainicio[0], alq.fechainicio[1], alq.fechainicio[2]);
                        printf("F. Prevista  : %02d/%02d/%d\n", alq.fechaprev[0], alq.fechaprev[1], alq.fechaprev[2]);
                        alqEncontrado = 1;
                        break;
                    }
                }
                fclose(archAlq);
            }

            if (!alqEncontrado) {
                printf("[Info] No se encontro el registro de contrato activo asociado.\n");
            }
        }
    }

    if (contador == 0) {
        printf("\nNo hay vehiculos alquilados en este momento.\n");
    } else {
        printf("\n-------------------------------------------------------\n");
        printf("Total de vehiculos en estado Alquilado: %d\n", contador);
    }

    fclose(archVeh);
}

// ============================================================================
// SUBMENU DEL MODULO VEHICULOS
// ============================================================================

void submenuVeh(void) {
    int opcion;
    do {
        printf("\n========================================\n");
        printf("         GESTION DE VEHICULOS           \n");
        printf("========================================\n");
        printf("1. Listar Flota Completa\n");
        printf("2. Registrar Vehiculo (Alta)\n");
        printf("3. Modificar Vehiculo\n");
        printf("4. Buscar Vehiculo por Patente\n");
        printf("5. Listar Solo Vehiculos Disponibles\n");
        printf("6. Listar Solo Vehiculos Alquilados\n");
        printf("0. Volver al Menu Principal\n");
        printf("----------------------------------------\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("\n[ERROR] Entrada invalida. Por favor ingrese un numero.\n");
            limpiarBufferVeh();
            continue;
        }

        switch (opcion) {
            case 1: listarVehiculo(); break;
            case 2: registrarVehiculo(); break;
            case 3: modificarVehiculo(); break;
            case 4: buscarVehiculo(); break;
            case 5: listarDisponibles(); break;
            case 6: listarAlquilados(); break;
            case 0: break;
            default:
                printf("\n[ERROR] Opcion invalida. Elija entre 0 y 6.\n");
        }
    } while (opcion != 0);
}
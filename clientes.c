#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

// Recorre el archivo binario de clientes para verificar si existe un DNI registrado
int existeDNI(char dni[]) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    cli_dat c;
    while (fread(&c, sizeof( cli_dat), 1, arch) == 1) {
        if (strcmp(c.dni, dni) == 0) {
            fclose(arch);
            return 1;
        }
    }

    fclose(arch);
    return 0;
}

// Busca un cliente por su DNI y devuelve la estructura correspondiente por referencia
int buscarClientePorDNI(char dni[],  cli_dat *outCliente) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    cli_dat c;
    while (fread(&c, sizeof( cli_dat), 1, arch) == 1) {
        if (strcmp(c.dni, dni) == 0) {
            if (outCliente != NULL) {
                *outCliente = c;
            }
            fclose(arch);
            return 1;
        }
    }

    fclose(arch);
    return 0;
}

// Solicita los datos personales del cliente y guarda el registro en el archivo binario
void registrarCliente(void) {
    cli_dat nuevo;
    printf("\n=== REGISTRAR NUEVO CLIENTE ===\n");

    printf("Ingrese DNI: ");
    scanf("%9s", nuevo.dni);
    getchar();

    if (existeDNI(nuevo.dni)) {
        printf("[ERROR] El DNI ingresado ya se encuentra registrado.\n");
        return;
    }

    printf("Ingrese Apellido y Nombre: ");
    fgets(nuevo.apellidoNombre, sizeof(nuevo.apellidoNombre), stdin);
    nuevo.apellidoNombre[strcspn(nuevo.apellidoNombre, "\n")] = 0;

    printf("Ingrese Telefono: ");
    fgets(nuevo.telefono, sizeof(nuevo.telefono), stdin);
    nuevo.telefono[strcspn(nuevo.telefono, "\n")] = 0;

    printf("Ingrese Email: ");
    fgets(nuevo.email, sizeof(nuevo.email), stdin);
    nuevo.email[strcspn(nuevo.email, "\n")] = 0;

    printf("Ingrese Direccion: ");
    fgets(nuevo.direccion, sizeof(nuevo.direccion), stdin);
    nuevo.direccion[strcspn(nuevo.direccion, "\n")] = 0;

    printf("Fecha de alta - Dia: ");
    scanf("%d", &nuevo.diaAlta);
    printf("Fecha de alta - Mes: ");
    scanf("%d", &nuevo.mesAlta);
    printf("Fecha de alta - Anio: ");
    scanf("%d", &nuevo.anioAlta);

    nuevo.estado = 1;

    FILE *arch = fopen(ARCHIVO_CLIENTES, "ab");
    if (arch == NULL) {
        printf("[ERROR] No se pudo abrir el archivo para escritura.\n");
        return;
    }

    fwrite(&nuevo, sizeof(cli_dat), 1, arch);
    fclose(arch);

    printf("[EXITO] Cliente registrado correctamente.\n");
}

// Permite modificar los datos de contacto y estado de un cliente buscando por DNI
void modificarCliente(void) {
    char dniBuscado[10];
    printf("\n=== MODIFICAR DATOS DE CLIENTE ===\n");
    printf("Ingrese el DNI del cliente a modificar: ");
    scanf("%9s", dniBuscado);
    while (getchar() != '\n'); 

    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb+");
    if (arch == NULL) {
        printf("[ERROR] No se pudo abrir el archivo de clientes.\n");
        return;
    }

    cli_dat c;
    int encontrado = 0;
    char aux[100]; // Variable auxiliar para las modificaciones

    while (fread(&c, sizeof(cli_dat), 1, arch) == 1) {
        if (strcmp(c.dni, dniBuscado) == 0) {
            encontrado = 1;
            printf("\nCliente encontrado: %s\n", c.apellidoNombre);
            printf("(Si desea mantener el dato actual, solo presione Enter)\n\n");

            // Modificar Apellido y Nombre
            printf("Ingrese Nuevo Apellido y Nombre (actual: %s): ", c.apellidoNombre);
            fgets(aux, sizeof(aux), stdin);
            aux[strcspn(aux, "\n")] = 0;
            if (strlen(aux) > 0) {
                strcpy(c.apellidoNombre, aux);
            }

            //Modificar Teléfono
            printf("Ingrese Nuevo Telefono (actual: %s): ", c.telefono);
            fgets(aux, sizeof(aux), stdin);
            aux[strcspn(aux, "\n")] = 0;
            if (strlen(aux) > 0) {
                strcpy(c.telefono, aux);
            }

            // Modificar Email
            printf("Ingrese Nuevo Email (actual: %s): ", c.email);
            fgets(aux, sizeof(aux), stdin);
            aux[strcspn(aux, "\n")] = 0;
            if (strlen(aux) > 0) {
                strcpy(c.email, aux);
            }

            // modificar Dirección
            printf("Ingrese Nuevo Direccion (actual: %s): ", c.direccion);
            fgets(aux, sizeof(aux), stdin);
            aux[strcspn(aux, "\n")] = 0;
            if (strlen(aux) > 0) {
                strcpy(c.direccion, aux);
            }

            // modificar Estado con validación
            printf("Estado actual (%d) [1: Activo, 0: Inactivo]: ", c.estado);
            char bufferEstado[20];
            fgets(bufferEstado, sizeof(bufferEstado), stdin);
            bufferEstado[strcspn(bufferEstado, "\n")] = 0;
            
            if (strlen(bufferEstado) > 0) {
                int nuevoEstado = atoi(bufferEstado);
                while (nuevoEstado != 0 && nuevoEstado != 1) {
                    printf("[ERROR] Valor invalido. Ingrese 1 para Activo o 0 para Inactivo: ");
                    fgets(bufferEstado, sizeof(bufferEstado), stdin);
                    bufferEstado[strcspn(bufferEstado, "\n")] = 0;
                    nuevoEstado = atoi(bufferEstado);
                }
                c.estado = nuevoEstado;
            }

            //registro actual y pisarlo con la estructura modificada
            fseek(arch, -(long)sizeof(cli_dat), SEEK_CUR);
            fwrite(&c, sizeof(cli_dat), 1, arch);
            printf("\n[EXITO] Datos del cliente actualizados correctamente.\n");
            break;
        }
    }

    
    if (!encontrado) {
        printf("\n[ERROR] No se encontro un cliente con el DNI %s.\n", dniBuscado);
    }

    fclose(arch);
}

// Abre y muestra en consola el archivo individual de texto asociado al cliente
void mostrarHistorialClienteTexto(char dni[]) {
    char nombreArchivo[30];
    sprintf(nombreArchivo, "movimientos_%s.txt", dni);

    FILE *archText = fopen(nombreArchivo, "r");
    if (archText == NULL) {
        printf("No se encontro un historial de movimientos registrado para este cliente.\n");
        return;
    }

    printf("\n--- HISTORIAL DE MOVIMIENTOS ---\n");
    char linea[200];
    while (fgets(linea, sizeof(linea), archText) != NULL) {
        printf("%s", linea);
    }
    fclose(archText);
}

// Muestra la informacion del cliente y despliega su historial de movimientos
void consultarCliente(void) {
    char dniBuscado[10];
    printf("\n=== CONSULTA DE CLIENTE ===\n");
    printf("Ingrese DNI del cliente: ");
    scanf("%9s", dniBuscado);

    cli_dat c;
    if (buscarClientePorDNI(dniBuscado, &c)) {
        printf("\n----------------------------------------\n");
        printf("DNI:             %s\n", c.dni);
        printf("Nombre/Apellido: %s\n", c.apellidoNombre);
        printf("Telefono:        %s\n", c.telefono);
        printf("E-mail:          %s\n", c.email);
        printf("Direccion:       %s\n", c.direccion);
        printf("Fecha Alta:      %02d/%02d/%04d\n", c.diaAlta, c.mesAlta, c.anioAlta);
        printf("Estado:          %s\n", c.estado == 1 ? "Activo" : "Inactivo");
        printf("----------------------------------------\n");

        mostrarHistorialClienteTexto(c.dni);
    } else {
        printf("[ERROR] Cliente no encontrado.\n");
    }
}

// Despliega el menu del modulo de clientes y procesa la opcion seleccionada
void menuClientes(void) {
    int opcion;
    do {
        printf("\n--- SUBMENU CLIENTES ---\n");
        printf("1. Registrar Cliente\n");
        printf("2. Modificar Cliente\n");
        printf("3. Consultar Cliente\n");
        printf("0. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: registrarCliente(); break;
            case 2: modificarCliente(); break;
            case 3: consultarCliente(); break;
            case 0: break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
}

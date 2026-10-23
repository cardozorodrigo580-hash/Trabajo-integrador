<<<<<<< HEAD
#define ARCHIVO_CLIENTES "clientes.dat"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void menuClientes(void);
void registrarCliente(void);
void modificarCliente(void);
void consultarCliente(void);
int existeDNI(char dni[]);
int buscarClientePorDNI(char dni[], struct Cliente *outCliente);
void mostrarHistorialClienteTexto(char dni[]);

// Recorre el archivo binario de clientes para verificar si existe un DNI registrado
int existeDNI(char dni[]) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    struct Cliente c;
    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
        if (strcmp(c.dni, dni) == 0) {
            fclose(arch);
            return 1;
        }
    }

    fclose(arch);
    return 0;
}

// Busca un cliente por su DNI y devuelve la estructura correspondiente por referencia
int buscarClientePorDNI(char dni[], struct Cliente *outCliente) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    struct Cliente c;
    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
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
    struct Cliente nuevo;
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

    fwrite(&nuevo, sizeof(struct Cliente), 1, arch);
    fclose(arch);

    printf("[EXITO] Cliente registrado correctamente.\n");
}

// Permite modificar los datos de contacto y estado de un cliente buscando por DNI
void modificarCliente(void) {
    char dniBuscado[10];
    printf("\n=== MODIFICAR DATOS DE CLIENTE ===\n");
    printf("Ingrese el DNI del cliente a modificar: ");
    scanf("%9s", dniBuscado);
    getchar();

    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb+");
    if (arch == NULL) {
        printf("[ERROR] No se pudo abrir el archivo de clientes.\n");
        return;
    }

    struct Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
        if (strcmp(c.dni, dniBuscado) == 0) {
            encontrado = 1;
            printf("\nCliente encontrado: %s\n", c.apellidoNombre);

            printf("Ingrese Nuevo Telefono (actual: %s): ", c.telefono);
            fgets(c.telefono, sizeof(c.telefono), stdin);
            c.telefono[strcspn(c.telefono, "\n")] = 0;

            printf("Ingrese Nuevo Email (actual: %s): ", c.email);
            fgets(c.email, sizeof(c.email), stdin);
            c.email[strcspn(c.email, "\n")] = 0;

            printf("Ingrese Nueva Direccion (actual: %s): ", c.direccion);
            fgets(c.direccion, sizeof(c.direccion), stdin);
            c.direccion[strcspn(c.direccion, "\n")] = 0;

            printf("Estado (1: Activo, 0: Inactivo): ");
            scanf("%d", &c.estado);

            fseek(arch, -((long)sizeof(struct Cliente)), SEEK_CUR);
            fwrite(&c, sizeof(struct Cliente), 1, arch);
            printf("[EXITO] Datos del cliente actualizados correctamente.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("[ERROR] No se encontro un cliente con el DNI %s.\n", dniBuscado);
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

    struct Cliente c;
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
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

// Recorre el archivo binario de clientes para verificar si existe un DNI registrado
int existeDNI(char dni[]) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    struct Cliente c;
    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
        if (strcmp(c.dni, dni) == 0) {
            fclose(arch);
            return 1;
        }
    }

    fclose(arch);
    return 0;
}

// Busca un cliente por su DNI y devuelve la estructura correspondiente por referencia
int buscarClientePorDNI(char dni[], struct Cliente *outCliente) {
    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb");
    if (arch == NULL) {
        return 0;
    }

    struct Cliente c;
    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
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
    struct Cliente nuevo;
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

    fwrite(&nuevo, sizeof(struct Cliente), 1, arch);
    fclose(arch);

    printf("[EXITO] Cliente registrado correctamente.\n");
}

// Permite modificar los datos de contacto y estado de un cliente buscando por DNI
void modificarCliente(void) {
    char dniBuscado[10];
    printf("\n=== MODIFICAR DATOS DE CLIENTE ===\n");
    printf("Ingrese el DNI del cliente a modificar: ");
    scanf("%9s", dniBuscado);
    getchar();

    FILE *arch = fopen(ARCHIVO_CLIENTES, "rb+");
    if (arch == NULL) {
        printf("[ERROR] No se pudo abrir el archivo de clientes.\n");
        return;
    }

    struct Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(struct Cliente), 1, arch) == 1) {
        if (strcmp(c.dni, dniBuscado) == 0) {
            encontrado = 1;
            printf("\nCliente encontrado: %s\n", c.apellidoNombre);

            printf("Ingrese Nuevo Telefono (actual: %s): ", c.telefono);
            fgets(c.telefono, sizeof(c.telefono), stdin);
            c.telefono[strcspn(c.telefono, "\n")] = 0;

            printf("Ingrese Nuevo Email (actual: %s): ", c.email);
            fgets(c.email, sizeof(c.email), stdin);
            c.email[strcspn(c.email, "\n")] = 0;

            printf("Ingrese Nueva Direccion (actual: %s): ", c.direccion);
            fgets(c.direccion, sizeof(c.direccion), stdin);
            c.direccion[strcspn(c.direccion, "\n")] = 0;

            printf("Estado (1: Activo, 0: Inactivo): ");
            scanf("%d", &c.estado);

            fseek(arch, -((long)sizeof(struct Cliente)), SEEK_CUR);
            fwrite(&c, sizeof(struct Cliente), 1, arch);
            printf("[EXITO] Datos del cliente actualizados correctamente.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("[ERROR] No se encontro un cliente con el DNI %s.\n", dniBuscado);
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

    struct Cliente c;
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
>>>>>>> a267e701e1281fba9b3b9304b4c773c3423fc691

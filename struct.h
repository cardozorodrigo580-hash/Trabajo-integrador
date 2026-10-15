#ifndef STRUCT_H
#define STRUCT_H
#define ARCHIVO_CLIENTES "clientes.dat"

typedef struct Cliente_dat{
    char dni[10];
    char apellidoNombre[60];
    char telefono[20];
    char email[50];
    char direccion[60];
    int diaAlta;
    int mesAlta;
    int anioAlta;
    int estado; // 1: Activo, 0: Inactivo
}cli_dat;

// Declaracion de prototipos
void menuClientes(void);
void registrarCliente(void);
void modificarCliente(void);
void consultarCliente(void);
int existeDNI(char dni[]);
int buscarClientePorDNI(char dni[],cli_dat *outCliente);
void mostrarHistorialClienteTexto(char dni[]);

typedef struct Vehiculos_dat {
    int idVeh;
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

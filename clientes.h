#define ARCHIVO_CLIENTES "clientes.dat"

// Estructura para almacenar los datos del cliente
struct Cliente {
    char dni[10];
    char apellidoNombre[60];
    char telefono[20];
    char email[50];
    char direccion[60];
    int diaAlta;
    int mesAlta;
    int anioAlta;
    int estado; // 1: Activo, 0: Inactivo
};

// Declaracion de prototipos
void menuClientes(void);
void registrarCliente(void);
void modificarCliente(void);
void consultarCliente(void);
int existeDNI(char dni[]);
int buscarClientePorDNI(char dni[], struct Cliente *outCliente);
void mostrarHistorialClienteTexto(char dni[]);
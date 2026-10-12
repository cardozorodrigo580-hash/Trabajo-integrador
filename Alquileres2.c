//Alquileres y Devoluciones
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
//
float CalcularImporAlquiler(int dias,float precioxdia);
float CalcularSaldoPend(float importetot,float pagado);
//
int CalcularDiasEntreFechas(int fechaInicio[3],int fechaFin[3]);
int buscarAlqActivoxpatente(const char patente [10]);
int buscarAlqActivoxDNI(long dni);
//
void registrarAlquiler();
void registrarDevolucion();
void consultarAlquileres();
void menuAlquileres();
//
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
void menuAlquileres(){
                	int opt;
                		do{
							printf("1. Registrar un alquiler\n");
							printf("2. Registrar una devolucion\n");
							printf("3. Consultar alquileres\n");
							printf("0. Volver al menu principal:\n");
							printf("\nIngrese una opcion:\n");
							scanf("%d",&opt);
							switch(opt){
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
									printf("Por favor de volver a ingresar una opcion (0 a 3):\n");
									break;
							}					  	
						  }while (opt!=0);	
}
//				
int buscarAlqActivoxPatente(const char patente[10]){
	FILE *arch;
	struct Alquiler alq;
	int pos = 0;
	int posenc = -1;
		if ((arch=fopen("alquileres.dat","rb"))!=NULL){
			while (fread(&alq,sizeof(struct Alquiler),1,arch)== 1 ){
				if (strcmp(alq.patente,patente)== 0 && alq.estado == 1){
					posenc = pos;
				break;	
		}
			pos++;
			}	
		fclose(arch);
		}	
return posenc;
}
//	
int buscarAlqActivoxDNI(long dni){
	FILE *arch;
	struct Alquiler alq;
	int posi = 0;
	int posienc= -1;
		if ((arch=fopen("alquileres.dat","rb"))!=NULL){
			while (fread(&alq,sizeof(struct Alquiler),1,arch)== 1 ){
				if (alq.dni== dni && alq.estado == 1){
					posienc = posi;
				break;
			}
		posi++;
		}	
		fclose(arch);
}			
return posienc;	
}
//

//
void registrarDevolucion(){
	FILE *arch;
	struct Alquiler alq;
	char patente [10];
	int pos;
	float saldoPendiente;
			printf("\n--- Registrar Devolucion ---\n");
			printf("Ingrese la patente del vehiculo a devolver:\t");
			scanf("%9s",patente);
			pos = buscarAlqActivoxPatente(patente);
				if (pos == -1){
					printf("\n[ERROR] No se encontrado un alquiler activo para la patente %s.\n",patente);
				}else{
					if ((arch=fopen("alquileres.dat","r+b"))!=NULL){
						fseek(arch, pos * sizeof(struct Alquiler),SEEK_SET);
					if (fread(&alq,sizeof(struct Alquiler),1,arch)==1){
						alq.estado = 0;
					printf("\nIngrese la fecha efectiva de la devolucion (dia, mes, anio):\t");
					scanf("%d %d %d",&alq.fechaefec[0], &alq.fechaefec[1],&alq.fechaefec[2]);
					saldoPendiente = CalcularSaldoPend(alq.importetot, alq.pagado);
					printf("\n--- Resumen de devolución --- \n");
					printf("Patente: %s / DNI: %ld\n",alq.patente,alq.dni);
					printf("Saldo a cobrar: $%.2f\n",CalcularSaldoPend(alq.importetot, alq.pagado));
					fseek(arch, -((long)sizeof(struct Alquiler)),SEEK_CUR);
					if (fwrite(&alq,sizeof(struct Alquiler),1,arch)== 1){
						printf("\n[EXITO] Devolucion registrada  y auto liberado correctamente. \n");
                    }else {
                          printf("\n[ERROR] No se pudo actualizar el archivo. \n");
                          }
					}
					fclose(arch);
					} else {
                           printf("\nError en la lectura del archivo\n");
					}
     }
	
	
}
void registrarAlquiler(){
     FILE *arch;
     struct Alquiler alq;
     float precioxdia;
     int dias;
              printf("\n --- Registrar un nuevo alquiler --- \n");
              printf("Ingrese el DNI del cliente a registrar: ");
              scanf("%ld", &alq.dni);
              if (buscarAlqActivoxDNI(alq.dni)!= -1){
                 printf("El cliente con dni %ld ya posee un alquiler activo.\n",alq.dni);
                 return;
                 }else{
                       printf("Ingrese la patente del vehiculo: ");
                       scanf("%9s", alq.patente);
                       if (buscarAlqActivoxPatente(alq.patente)!= -1){
                          printf("El vehiculo con patente %s ya se encuentra alquilado",alq.patente);
                          return;
                          }else{
                                printf("Ingrese el numero del alquiler:");
                                scanf("%d",&alq.nroalquiler);
                                printf("Ingrese fecha de inicio (dia mes anio): ");
                                scanf("%d %d %d",&alq.fechainicio[0],&alq.fechainicio[1],&alq.fechainicio[2]);
                                printf("Ingrese fecha prevista de devolucion (dia mes anio): ");
                                scanf("%d %d %d",&alq.fechaprev[0],&alq.fechaprev[1],&alq.fechaprev[2]);
                                alq.contdias = CalcularDiasEntreFechas(alq.fechainicio,alq.fechaprev);
                                printf("Ingrese el precio por dia del vehiculo:$");
                                scanf("%f",&precioxdia);
                                alq.importetot = CalcularImporAlquiler(alq.contdias,precioxdia);
                                printf("Ingrese el pago inicial:$");
                                scanf("%f",&alq.pagado);
                                alq.estado = 1;
                                if ((arch= fopen("alquileres.dat", "ab"))!=NULL){
                                           if (fwrite(&alq,sizeof(struct Alquiler),1,arch)== 1){
                                                printf("\n[EXITO] Alquiler registrado correctamente.\n");
                                                printf("Importe total: $%.2f | Saldo pendiente: $%.2f\n", 
                                                        alq.importetot, CalcularSaldoPend(alq.importetot, alq.pagado));
                                                        }else{
                                                              printf("Error al modifivar el registro del archivo");
                                                              }
                                 fclose(arch);
                                 }else{                                        
                                       printf("Error en la lectura del archivo de alquileres.");                                       
                                       }
                                       }                                                                      
     
     
     }
}   
//
int esBisiesto(int anio){
    return (anio % 4 == 0 && anio % 100 != 0)||(anio % 400 == 0);
}
//
int CalcularDiasEntreFechas(int fechainicio[3], int fechaFin[3]){
    int diasMes[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int d1 = fechainicio[0],m1 = fechainicio[1], a1 = fechainicio[2];
    int d2 = fechaFin[0], m2 = fechaFin[1], a2 = fechaFin [2];
        long dias1 = d1;
             for (int i = 1; i < m1; i++){
                 int d = diasMes[i];
                 if (i == 2 && esBisiesto(a1)) d = 29;
                 dias1 += d;
                 }
                 for (int y = 0; y < a1; y++){
                     dias1 += esBisiesto(y) ? 366:365;
                     }
         long dias2 = d2;
              for (int i = 1; i < m2; i++){
                  int d = diasMes[i];
                  if ( i == 2 && esBisiesto(a2)) d = 29;
                  dias2 += d;
                  }
                  for (int y = 0; y < a2; y++)
                      dias2 += esBisiesto(y) ? 366:365;
                      }       
                  int diferencia = (int)(dias2-dias1);
                  return (diferencia >= 0) ? diferencia : 0;
}                  
// 
float CalcularImporAlquiler(int dias, float precioxdia){
      return dias * precioxdia;
      }
//
float CalcularSaldoPend(float importetot, float pagado){
      return importetot - pagado;
      }	
//
void consultarAlquileres() {
    FILE *arch = fopen("alquileres.dat", "rb");
    if (arch == NULL) {
        printf("No se pudo abrir el archivo o aun no hay alquileres registrados.\n");
        return;
    }
    
    struct Alquiler alq;
    
    printf("\n========================================\n");
    printf("         LISTADO DE ALQUILERES          \n");
    printf("========================================\n");
    
    // Lee el archivo registro por registro hasta llegar al final
    while (fread(&alq, sizeof(struct Alquiler), 1, arch) == 1) {
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

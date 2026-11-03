#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "struct.h"

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int esNumerico(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

int validarFormatoPatente(char *patente) {
    int j = 0;
    char limpia[10];
    for (int i = 0; patente[i] != '\0'; i++) {
        if (!isspace((unsigned char)patente[i])) {
            if (j < 9) {
                limpia[j++] = toupper((unsigned char)patente[i]);
            }
        }
    }
    limpia[j] = '\0';
    
    int len = strlen(limpia);
    if (len == 6) {
        if (isalpha((unsigned char)limpia[0]) && isalpha((unsigned char)limpia[1]) && isalpha((unsigned char)limpia[2]) &&
            isdigit((unsigned char)limpia[3]) && isdigit((unsigned char)limpia[4]) && isdigit((unsigned char)limpia[5])) {
            strcpy(patente, limpia);
            return 1;
        }
    } else if (len == 7) {
        if (isalpha((unsigned char)limpia[0]) && isalpha((unsigned char)limpia[1]) &&
            isdigit((unsigned char)limpia[2]) && isdigit((unsigned char)limpia[3]) && isdigit((unsigned char)limpia[4]) &&
            isalpha((unsigned char)limpia[5]) && isalpha((unsigned char)limpia[6])) {
            strcpy(patente, limpia);
            return 1;
        }
    }
    return 0;
}

int esFechaValida(int d, int m, int a) {
    if (a < 2020 || a > 2035) return 0;
    if (m < 1 || m > 12) return 0;
    
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0)) {
        diasMes[2] = 29;
    }
    
    if (d < 1 || d > diasMes[m]) return 0;
    return 1;
}

float CalcularImporAlquiler(int dias, float precioxdia) {
    return (dias > 0 ? dias : 1) * precioxdia;
}

float CalcularSaldoPend(float importetot, float pagado) {
    float saldo = importetot - pagado;
    return (saldo < 0) ? 0 : saldo;
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

int CalcularDiasEntreFechas(int fechainicio[3], int fechaFin[3]) {
    long dias1 = convertirFechaADias(fechainicio[0], fechainicio[1], fechainicio[2]);
    long dias2 = convertirFechaADias(fechaFin[0], fechaFin[1], fechaFin[2]);
    
    int diferencia = (int)(dias2 - dias1);
    return (diferencia >= 0) ? diferencia : 0;
}

int buscarAlqActivoxPatente(const char patente[10]) {
    FILE *arch = fopen(ARCHIVO_ALQUILERES, "rb");
    if (arch == NULL) return -1;

    alq_dat alq; 
    int pos = 0;
    int posenc = -1;
    
    while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
        if (strcmp(alq.patente, patente) == 0 && alq.estado == 1) {
            posenc = pos;
            break;    
        }
        pos++;
    }    
    fclose(arch);
    return posenc;
}

float redondearDosDecimales(float valor) {
    if (valor != valor) {
        return -1.0f;
    }
    if (valor < 0.0f || valor > 1000000000.0f) {
        return -1.0f;
    }
    return (float)((long long)(valor * 100.0f + 0.5f)) / 100.0f;
}

int validarEntradaTexto(char *destino, int maxLargo) {
    char temp[128];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return -1; 
    }
    
    char *posNewline = strchr(temp, '\n');
    if (posNewline == NULL) {
        if (feof(stdin)) {
            size_t len = strlen(temp);
            if (len == 0 || len > (size_t)maxLargo) {
                return 0;
            }
            strcpy(destino, temp);
            return 1;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return 0; 
        }
    } else {
        *posNewline = '\0';
    }
    
    if (strlen(temp) == 0) {
        return 0; 
    }
    
    if (strlen(temp) > (size_t)maxLargo) {
        return 0; 
    }
    
    strcpy(destino, temp);
    return 1;
}

int generarNuevoNroAlquiler(void) {
    errno = 0;
    FILE *arch = fopen(ARCHIVO_ALQUILERES, "rb");
    if (arch == NULL) {
        if (errno == ENOENT) {
            return 1; 
        }
        return -1; 
    }

    alq_dat alq;
    int maxId = 0;
    while (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
        if (alq.nroalquiler > maxId) {
            maxId = alq.nroalquiler;
        }
    }
    
    if (ferror(arch)) {
        fclose(arch);
        return -1; 
    }
    
    if (fclose(arch) != 0) {
        return -1; 
    }
    
    if (maxId >= INT_MAX) {
        return -1; 
    }
    
    return maxId + 1;
}

int cambiarEstadoVehiculo(const char *patente, int nuevoEstado) {
    FILE *arch = fopen(ARCHIVO_VEHICULOS, "r+b");
    if (arch == NULL) return 0;

    vehi_dat v;
    int actualizado = 0;
    while (fread(&v, sizeof(vehi_dat), 1, arch) == 1) {
        if (strcmp(v.patente, patente) == 0) {
            v.estado = nuevoEstado;
            if (fseek(arch, -(long)sizeof(vehi_dat), SEEK_CUR) != 0) {
                break;
            }
            if (fwrite(&v, sizeof(vehi_dat), 1, arch) == 1) {
                actualizado = 1;
            }
            break;
        }
    }
    
    if (ferror(arch)) {
        actualizado = 0;
    }
    
    if (fclose(arch) != 0) {
        actualizado = 0;
    }
    
    return actualizado;
}

void registrarDevolucion(void) {
    FILE *arch;
    alq_dat alq; 
    char patente[10];
    int pos = -1;
    int fechaValida = 0;
    int resLectura;
    
    printf("\n--- Registrar Devolucion ---\n");
    
    do {
        printf("Ingrese la patente del vehiculo a devolver: ");
        resLectura = validarEntradaTexto(patente, 7);
        if (resLectura == -1) {
            printf("\n[AVISO] Operacion cancelada por el usuario.\n");
            return;
        }
        if (resLectura == 0 || !validarFormatoPatente(patente)) {
            printf("[ERROR] Formato de patente invalido o excede el limite de caracteres. Intentelo nuevamente.\n");
            continue;
        }
        
        pos = buscarAlqActivoxPatente(patente);
        if (pos == -1) {
            printf("[ERROR] No se ha encontrado un alquiler activo para la patente %s. Verifique e intente nuevamente.\n", patente);
        }
    } while (pos == -1);

    if ((arch = fopen(ARCHIVO_ALQUILERES, "r+b")) != NULL) {
        if (fseek(arch, (long)(pos * sizeof(alq_dat)), SEEK_SET) != 0) {
            printf("\n[ERROR] Fallo al posicionarse en el archivo de alquileres.\n");
            fclose(arch);
            return;
        }
        
        if (fread(&alq, sizeof(alq_dat), 1, arch) == 1) {
            do {
                int d, m, a;
                printf("\nIngrese la fecha efectiva de la devolucion (dia mes anio): ");
                int scanfRes = scanf("%d %d %d", &d, &m, &a);
                if (scanfRes == EOF) {
                    printf("\n[AVISO] Operacion cancelada.\n");
                    fclose(arch);
                    return;
                }
                if (scanfRes != 3) {
                    printf("[ERROR] Formato incorrecto. Debe ingresar numeros enteros.\n");
                    limpiarBuffer();
                    continue;
                }
                limpiarBuffer();

                if (!esFechaValida(d, m, a)) {
                    printf("[ERROR] La fecha %02d/%02d/%d no es valida o esta fuera del rango permitido (2020-2035).\n", d, m, a);
                    continue;
                }

                struct tm fecha_ingresada = {0};
                fecha_ingresada.tm_mday = d;
                fecha_ingresada.tm_mon = m - 1;
                fecha_ingresada.tm_year = a - 1900;
                time_t controlFecha = mktime(&fecha_ingresada);
                if (controlFecha == (time_t)-1) {
                    printf("[ERROR] Error al procesar la fecha ingresada.\n");
                    continue;
                }

                struct tm fecha_inicio = {0};
                fecha_inicio.tm_mday = alq.fechainicio[0];
                fecha_inicio.tm_mon = alq.fechainicio[1] - 1;
                fecha_inicio.tm_year = alq.fechainicio[2] - 1900;
                time_t t_inicio = mktime(&fecha_inicio);
                if (t_inicio == (time_t)-1) {
                    printf("[ERROR] Error al procesar la fecha de inicio del alquiler.\n");
                    fclose(arch);
                    return;
                }

                if (controlFecha < t_inicio) {
                    printf("[ERROR] La fecha de devolucion no puede ser anterior a la fecha de inicio.\n");
                } else {
                    alq.fechaefec[0] = d;
                    alq.fechaefec[1] = m;
                    alq.fechaefec[2] = a;
                    fechaValida = 1;
                }
            } while (!fechaValida);

            if (alq.contdias > 0 && alq.importetot > 0) {
                float precioPorDia = alq.importetot / alq.contdias; 
                int diasReales = CalcularDiasEntreFechas(alq.fechainicio, alq.fechaefec);
                if (diasReales == 0) diasReales = 1; 

                if (diasReales != alq.contdias) {
                    alq.contdias = diasReales;
                    float importeRecalculado = CalcularImporAlquiler(alq.contdias, precioPorDia);
                    alq.importetot = redondearDosDecimales(importeRecalculado);
                    if (alq.importetot < 0.0f) {
                        printf("[ERROR] El importe recalculado excede los limites permitidos.\n");
                        fclose(arch);
                        return;
                    }
                    printf("\n[AVISO] Se recalculo el importe debido a los dias reales de uso.\n");
                }
            }

            float saldoCalc = CalcularSaldoPend(alq.importetot, alq.pagado);
            float saldoPendiente = redondearDosDecimales(saldoCalc);
            if (saldoPendiente < 0.0f) {
                printf("[ERROR] Error critico en el calculo o redondeo del saldo pendiente.\n");
                fclose(arch);
                return;
            }
            
            if (saldoPendiente > 0) {
                int deudaSaldada = 0;
                do {
                    float pagoExtraInput = 0;
                    printf("\n[ATENCION] Saldo pendiente de $%.2f.\n", saldoPendiente);
                    printf("Ingrese el monto exacto a abonar: $");
                    int scanPago = scanf("%f", &pagoExtraInput);
                    if (scanPago == EOF) {
                        printf("\n[AVISO] Operacion cancelada.\n");
                        fclose(arch);
                        return;
                    }
                    if (scanPago != 1) {
                        printf("[ERROR] Formato numerico invalido.\n");
                        limpiarBuffer();
                        continue;
                    }
                    limpiarBuffer();

                    float pagoExtraRedondeado = redondearDosDecimales(pagoExtraInput);
                    if (pagoExtraRedondeado < 0.0f) {
                        printf("[ERROR] Importe invalido o fuera de rango.\n");
                        continue;
                    }

                    if (pagoExtraRedondeado != saldoPendiente) {
                        printf("[ERROR] El monto debe ser un numero valido e igual al saldo pendiente de $%.2f.\n", saldoPendiente);
                    } else {
                        float nuevoPagado = redondearDosDecimales(alq.pagado + pagoExtraRedondeado);
                        if (nuevoPagado < 0.0f) {
                            printf("[ERROR] Desbordamiento en el calculo de pagos.\n");
                            continue;
                        }
                        alq.pagado = nuevoPagado;
                        saldoPendiente = redondearDosDecimales(CalcularSaldoPend(alq.importetot, alq.pagado));
                        if (saldoPendiente < 0.0f) {
                            printf("[ERROR] Error critico al actualizar el saldo pendiente.\n");
                            fclose(arch);
                            return;
                        }
                        deudaSaldada = 1;
                        printf("[EXITO] Deuda saldada correctamente.\n");
                    }
                } while (!deudaSaldada);
            }

            if (!cambiarEstadoVehiculo(alq.patente, 1)) {
                printf("\n[ERROR] No se pudo actualizar el estado del vehiculo a Disponible. La devolucion se ha cancelado.\n");
                fclose(arch);
                return;
            }

            alq.estado = 0;

            if (fseek(arch, (long)(pos * sizeof(alq_dat)), SEEK_SET) != 0 || fwrite(&alq, sizeof(alq_dat), 1, arch) != 1) {
                printf("\n[ERROR] No se pudo escribir la actualizacion en el archivo de alquileres. Intentando revertir estado del vehiculo...\n");
                if (!cambiarEstadoVehiculo(alq.patente, 2)) {
                    printf("[ADVERTENCIA CRITICA] Inconsistencia entre archivos: el vehiculo figura como disponible pero el alquiler no pudo cerrarse.\n");
                } else {
                    printf("[INFO] Reversion del vehiculo completada con exito.\n");
                }
            } else {
                if (fclose(arch) != 0) {
                    printf("\n[ERROR CRITICO] La escritura fue exitosa pero fallo el cierre del archivo de alquileres (fclose). No se puede garantizar la persistencia ni la consistencia exacta.\n");
                } else {
                    printf("\n[EXITO] Devolucion registrada y vehiculo liberado correctamente.\n");
                }
                return;
            }
        } else {
            if (ferror(arch)) {
                printf("\n[ERROR] Error de lectura en el archivo de alquileres.\n");
            } else {
                printf("\n[ERROR] No se encontro el registro de alquiler en la posicion indicada.\n");
            }
        }
        fclose(arch);
    } else {
        printf("\nError en la apertura del archivo de alquileres.\n");
    }
}

void registrarAlquiler(void) {
    FILE *arch;
    alq_dat alq; 
    float precioInput;
    int dniValido = 0;
    int patenteValida = 0;
    int fechaInicioValida = 0;
    int fechaPrevValida = 0;
    int resLectura;
    
    printf("\n--- Registrar Alquiler ---\n");
    
    do {
        printf("Ingrese el DNI del cliente a registrar: ");
        resLectura = validarEntradaTexto(alq.dni, 8);
        if (resLectura == -1) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (resLectura == 0 || !esNumerico(alq.dni)) {
            printf("[ERROR] DNI invalido o excede el limite de caracteres permitidos. Intentelo nuevamente.\n");
            continue;
        }

        if (!existeDNI(alq.dni)) {
            printf("[ERROR] El cliente con DNI %s no esta registrado. Intentelo nuevamente.\n", alq.dni);
        } else {
            dniValido = 1;
        }
    } while (!dniValido);

    do {
        printf("Ingrese la patente del vehiculo (Ej: ABC123 o AB123CD): ");
        resLectura = validarEntradaTexto(alq.patente, 7);
        if (resLectura == -1) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (resLectura == 0 || !validarFormatoPatente(alq.patente)) {
            printf("[ERROR] Formato de patente invalido o longitud incorrecta. Intentelo nuevamente.\n");
            continue;
        }

        vehi_dat v;
        if (buscarPosicionVehiculoPorPatente(alq.patente, &v) == -1) {
            printf("[ERROR] La patente %s no existe en la flota. Intentelo nuevamente.\n", alq.patente);
        } else if (v.estado != 1) {
            printf("[ERROR] El vehiculo %s no esta disponible.\n", alq.patente);
        } else {
            patenteValida = 1;
        }
    } while (!patenteValida);

    int nuevoId = generarNuevoNroAlquiler();
    if (nuevoId == -1) {
        printf("[ERROR] No se pudo generar el ID de alquiler debido a un error de lectura de archivo o desbordamiento.\n");
        return;
    }
    alq.nroalquiler = nuevoId;
    printf("Numero de alquiler asignado automaticamente: %d\n", alq.nroalquiler);
    
    do {
        int d, m, a;
        printf("Ingrese fecha de inicio (dia mes anio): ");
        int scanRes = scanf("%d %d %d", &d, &m, &a);
        if (scanRes == EOF) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (scanRes != 3) {
            printf("[ERROR] Formato incorrecto. Ingrese numeros enteros.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        if (!esFechaValida(d, m, a)) {
            printf("[ERROR] Fecha invalida o fuera del rango (2020-2035).\n");
            continue;
        }

        alq.fechainicio[0] = d;
        alq.fechainicio[1] = m;
        alq.fechainicio[2] = a;
        fechaInicioValida = 1;
    } while (!fechaInicioValida);

    do {
        int d, m, a;
        printf("Ingrese fecha prevista de devolucion (dia mes anio): ");
        int scanRes = scanf("%d %d %d", &d, &m, &a);
        if (scanRes == EOF) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (scanRes != 3) {
            printf("[ERROR] Formato incorrecto. Ingrese numeros enteros.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        if (!esFechaValida(d, m, a)) {
            printf("[ERROR] Fecha invalida o fuera del rango (2020-2035).\n");
            continue;
        }

        struct tm fecha_ini = {0};
        fecha_ini.tm_mday = alq.fechainicio[0];
        fecha_ini.tm_mon = alq.fechainicio[1] - 1;
        fecha_ini.tm_year = alq.fechainicio[2] - 1900;
        time_t t_ini = mktime(&fecha_ini);

        struct tm fecha_prev = {0};
        fecha_prev.tm_mday = d;
        fecha_prev.tm_mon = m - 1;
        fecha_prev.tm_year = a - 1900;
        time_t t_prev = mktime(&fecha_prev);

        if (t_ini == (time_t)-1 || t_prev == (time_t)-1) {
            printf("[ERROR] Error al procesar las fechas.\n");
            continue;
        }

        if (t_prev < t_ini) {
            printf("[ERROR] La fecha prevista no puede ser anterior a la de inicio.\n");
        } else {
            alq.fechaprev[0] = d;
            alq.fechaprev[1] = m;
            alq.fechaprev[2] = a;
            fechaPrevValida = 1;
        }
    } while (!fechaPrevValida);
    
    alq.contdias = CalcularDiasEntreFechas(alq.fechainicio, alq.fechaprev);
    if (alq.contdias == 0) alq.contdias = 1; 
    
    float precioxdia = -1.0f;
    do {
        printf("Ingrese el precio por dia del vehiculo: $");
        int scanPrecio = scanf("%f", &precioInput);
        if (scanPrecio == EOF) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (scanPrecio != 1) {
            printf("[ERROR] Formato invalido.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        precioxdia = redondearDosDecimales(precioInput);
        if (precioxdia <= 0.0f) {
            printf("[ERROR] El precio debe ser un numero valido mayor a 0 y dentro de los limites permitidos.\n");
        }
    } while (precioxdia <= 0.0f);
    
    float importeCalc = CalcularImporAlquiler(alq.contdias, precioxdia);
    alq.importetot = redondearDosDecimales(importeCalc);
    if (alq.importetot < 0.0f) {
        printf("[ERROR] El importe total calculado excede los limites permitidos.\n");
        return;
    }
    
    float pagoInicial = -1.0f;
    do {
        float pagoInput;
        printf("Ingrese el pago inicial: $");
        int scanPago = scanf("%f", &pagoInput);
        if (scanPago == EOF) {
            printf("\n[AVISO] Operacion cancelada.\n");
            return;
        }
        if (scanPago != 1) {
            printf("[ERROR] Formato invalido.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        pagoInput = redondearDosDecimales(pagoInput);
        if (pagoInput < 0.0f || pagoInput > alq.importetot) {
            printf("[ERROR] El pago no puede ser negativo ni superar el total ($%.2f).\n", alq.importetot);
            pagoInicial = -1.0f;
        } else {
            pagoInicial = pagoInput;
        }
    } while (pagoInicial < 0.0f);
    
    alq.pagado = pagoInicial;
    alq.estado = 1; 

    if (!cambiarEstadoVehiculo(alq.patente, 2)) {
        printf("\n[ERROR] No se pudo actualizar el estado del vehiculo a Alquilado. Operacion cancelada.\n");
        return;
    }
    
    if ((arch = fopen(ARCHIVO_ALQUILERES, "ab")) != NULL) {
        if (fwrite(&alq, sizeof(alq_dat), 1, arch) == 1) {
            if (fclose(arch) != 0) {
                printf("\n[ERROR CRITICO] El alquiler se escribio pero fallo el cierre del archivo (fclose). No se puede garantizar la persistencia ni la consistencia exacta.\n");
            } else {
                printf("\n[EXITO] Alquiler registrado correctamente con ID Nro: %d.\n", alq.nroalquiler);
                printf("Importe total: $%.2f | Saldo pendiente: $%.2f\n", 
                       alq.importetot, redondearDosDecimales(CalcularSaldoPend(alq.importetot, alq.pagado)));
            }
        } else {
            printf("\n[ERROR] Fallo la escritura del alquiler. Intentando revertir estado del vehiculo...\n");
            fclose(arch);
            if (!cambiarEstadoVehiculo(alq.patente, 1)) {
                printf("[ADVERTENCIA CRITICA] Inconsistencia entre archivos: el vehiculo figura como alquilado pero el registro no se guardo.\n");
            } else {
                printf("[INFO] Reversion del vehiculo completada con exito.\n");
            }
        }
    } else {
        printf("\n[ERROR] No se pudo abrir el archivo de alquileres. Intentando revertir estado del vehiculo...\n");
        if (!cambiarEstadoVehiculo(alq.patente, 1)) {
            printf("[ADVERTENCIA CRITICA] Inconsistencia entre archivos: el vehiculo figura como alquilado pero no se pudo abrir el archivo de alquileres.\n");
        } else {
            printf("[INFO] Reversion del vehiculo completada con exito.\n");
        }
    }
}

void consultarTodosLosAlquileres(void) {
    FILE *arch = fopen(ARCHIVO_ALQUILERES, "rb");
    if (arch == NULL) {
        printf("\n[AVISO] No se pudo abrir el archivo o aun no hay alquileres registrados.\n");
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
        printf("DNI           : %s\n", alq.dni);
        printf("Patente       : %s\n", alq.patente);
        printf("F. Inicio     : %02d/%02d/%d\n", alq.fechainicio[0], alq.fechainicio[1], alq.fechainicio[2]);
        printf("F. Prevista   : %02d/%02d/%d\n", alq.fechaprev[0], alq.fechaprev[1], alq.fechaprev[2]);
        
        if (alq.estado == 0) {
            printf("F. Efectiva   : %02d/%02d/%d\n", alq.fechaefec[0], alq.fechaefec[1], alq.fechaefec[2]);
        }
        
        printf("Dias          : %d\n", alq.contdias);
        printf("Importe Total : $%.2f\n", alq.importetot);
        printf("Pagado        : $%.2f\n", alq.pagado);
        printf("Saldo Pend.   : $%.2f\n", redondearDosDecimales(CalcularSaldoPend(alq.importetot, alq.pagado)));
        printf("Estado        : %s\n", (alq.estado == 1) ? "Activo" : "Inactivo");
        printf("----------------------------------------\n");
    }
    
    printf("Total de alquileres listados: %d\n", contador);
    fclose(arch);
}

void menuAlquileres(void) {
    int opt;
    do {
        printf("\n========================================\n");
        printf("         GESTION DE ALQUILERES          \n");
        printf("========================================\n");
        printf("1. Registrar un alquiler\n");
        printf("2. Registrar una devolucion\n");
        printf("3. Consultar todos los alquileres\n");
        printf("0. Volver al menu principal\n");
        printf("\nIngrese una opcion: ");
        if (scanf("%d", &opt) != 1) {
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

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
                consultarTodosLosAlquileres();
                break;
            default:
                printf("Por favor ingrese una opcion valida (0 a 3).\n");
                break;
        }                          
    } while (opt != 0);    
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HORAS_SEMANALES 40
#define RECARGO_EXTRA_DIURNO 1.5
#define RECARGO_FERIADO_DIURNO 2.0

typedef struct Empleado {
    char nombreCompleto[50];
    double salarioMensualUSD;
    int tiempoDescansoMinutos;
    int horaInicioDiurna;
    int minutoInicioDiurno;
    int horaFinDiurna;
    int minutoFinDiurno;
    int trabajoFeriadoBooleano;
    int horasExtrasDiurnas;
} Empleado;

void solicitar_datos(Empleado *empleado);
void guardar_empleado(Empleado empleado);
void mostrar_sueldo_mensual();
void limpiar_sistema();
int calcular_horas_trabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso);
double calcular_valor_hora(double salarioMensual, int tiempoDescanso);
void calcular_horas_extras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso, int *horasExtras);

int main() {
    int opcion;
    do {
        printf("\nMenu:\n");
        printf("1. Registrar empleados\n");
        printf("2. Mostrar sueldo mensual de los empleados\n");
        printf("3. Limpiar sistema\n");
        printf("4. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\nRegistro de empleado:\n");
                Empleado empleado;
                solicitar_datos(&empleado);
                guardar_empleado(empleado);
                break;
            case 2:
                printf("\nSueldo mensual de los empleados:\n");
                mostrar_sueldo_mensual();
                break;
            case 3:
                printf("\nEsta seguro de limpiar el sistema? (1 para si, 0 para no): ");
                int confirmacion;
                scanf("%d", &confirmacion);
                if (confirmacion == 1) {
                    limpiar_sistema();
                    printf("El sistema ha sido limpiado.\n");
                } else {
                    printf("Operacion cancelada.\n");
                }
                break;
            case 4:
                printf("\nSaliendo del programa.\n");
                break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}

void solicitar_datos(Empleado *empleado) {
    printf("Ingrese el nombre del empleado: ");
    scanf("%s", empleado->nombreCompleto);
    printf("Ingrese el salario mensual (USD): ");
    scanf("%lf", &empleado->salarioMensualUSD);
    printf("Ingrese el tiempo de almuerzo o descanso dentro de la jornada de trabajo (Minutos): ");
    scanf("%d", &empleado->tiempoDescansoMinutos);
    printf("Ingrese la hora de inicio de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaInicioDiurna, &empleado->minutoInicioDiurno);
    printf("Ingrese la hora de fin de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaFinDiurna, &empleado->minutoFinDiurno);
    printf("Ha trabajado en feriados? (1 para si, 0 para no): ");
    scanf("%d", &empleado->trabajoFeriadoBooleano);
    printf("Ha trabajado horas extras? (Ingrese el numero de horas extras): ");
    scanf("%d", &empleado->horasExtrasDiurnas);
}

void guardar_empleado(Empleado empleado) {
    FILE *archivo;
    archivo = fopen("empleados.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }

    fprintf(archivo, "%s %.2lf %d %d %d %d %d %d %d %d\n", empleado.nombreCompleto, empleado.salarioMensualUSD, empleado.tiempoDescansoMinutos,
            empleado.horaInicioDiurna, empleado.minutoInicioDiurno, empleado.horaFinDiurna, empleado.minutoFinDiurno, empleado.trabajoFeriadoBooleano, empleado.horasExtrasDiurnas);

    fclose(archivo);
}

void mostrar_sueldo_mensual() {
    FILE *archivo;
    archivo = fopen("empleados.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }

    printf("\n%-20s %-20s %-20s\n", "Nombre", "Sueldo Mensual", "Horas Extras");
    printf("-------------------------------------------------------------\n");

    Empleado empleado;
    while (fscanf(archivo, "%s %lf %d %d %d %d %d %d %d %d\n", empleado.nombreCompleto, &empleado.salarioMensualUSD, &empleado.tiempoDescansoMinutos,
            &empleado.horaInicioDiurna, &empleado.minutoInicioDiurno, &empleado.horaFinDiurna, &empleado.minutoFinDiurno, &empleado.trabajoFeriadoBooleano, &empleado.horasExtrasDiurnas) != EOF) {
        
        double valor_hora = calcular_valor_hora(empleado.salarioMensualUSD, empleado.tiempoDescansoMinutos);
        int horas_extras = empleado.horasExtrasDiurnas;
        
        double sueldo_mensual = empleado.salarioMensualUSD;
        if (empleado.trabajoFeriadoBooleano)
            sueldo_mensual += empleado.salarioMensualUSD / 30 * RECARGO_FERIADO_DIURNO;
        
        sueldo_mensual += horas_extras * valor_hora * RECARGO_EXTRA_DIURNO;
        
        printf("%-20s %-20.2lf %-20d\n", empleado.nombreCompleto, sueldo_mensual, horas_extras);
    }

    fclose(archivo);
}

void limpiar_sistema() {
    FILE *archivo;
    archivo = fopen("empleados.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }
    fclose(archivo);
}

int calcular_horas_trabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso) {
    int minutos_inicio_total = horaInicio * 60 + minutoInicio;
    int minutos_fin_total = horaFin * 60 + minutoFin;
    int minutos_trabajados = minutos_fin_total - minutos_inicio_total - tiempoDescanso;
    
    if (minutos_trabajados < 0) {
        minutos_trabajados += 24 * 60;
    }
    
    return minutos_trabajados / 60;
}

double calcular_valor_hora(double salarioMensual, int tiempoDescanso) {
    int horas_laborables = HORAS_SEMANALES * 4 - (tiempoDescanso / 60 * 5); 
    return salarioMensual / horas_laborables;
}

void calcular_horas_extras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso, int *horasExtras) {
    int horas_trabajadas = calcular_horas_trabajadas(horaInicio, minutoInicio, horaFin, minutoFin, tiempoDescanso);
    int horas_normales = HORAS_SEMANALES * 4 - (tiempoDescanso / 60 * 5);
    
    *horasExtras = horas_trabajadas - horas_normales;
    if (*horasExtras < 0) {
        *horasExtras = 0;
    }
}


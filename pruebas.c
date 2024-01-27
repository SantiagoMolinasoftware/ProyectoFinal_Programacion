#include <stdio.h>
#include <stdlib.h>

#define Horas_Semanales 40
#define RECARGO_EXTRA_50 1.5
#define RECARGO_FERIADO 2.0

typedef struct {
    char nombre[50];
    double salarioMensual;
    int tiempoDescanso;
    int horaInicio;
    int minutoInicio;
    int horaFin;
    int minutoFin;
    int trabajoFeriado;
    int horasExtras;
} Empleado;

void solicitarDatos(Empleado *empleado);
void mostrarDatos(Empleado empleado);
int calcularHorasTrabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso);
double calcularValorHora(double salarioMensual, int tiempoDescanso);
void calcularHorasExtras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso, double valorHora, int trabajoFeriado, int *horasExtras, double *valorExtra, double *valorSuplementario);
void guardarEmpleado(Empleado empleado);
void mostrarSueldoMensual();
void limpiarSistema();

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
                solicitarDatos(&empleado);
                guardarEmpleado(empleado);
                break;
            case 2:
                printf("\nSueldo mensual de los empleados:\n");
                mostrarSueldoMensual();
                break;
            case 3:
                printf("\n¿Está seguro de limpiar el sistema? (1 para sí, 0 para no): ");
                int confirmacion;
                scanf("%d", &confirmacion);
                if (confirmacion == 1) {
                    limpiarSistema();
                    printf("El sistema ha sido limpiado.\n");
                } else {
                    printf("Operación cancelada.\n");
                }
                break;
            case 4:
                printf("\nSaliendo del programa.\n");
                break;
            default:
                printf("\nOpción inválida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}

void solicitarDatos(Empleado *empleado) {
    printf("Ingrese el nombre del empleado: ");
    scanf("%s", empleado->nombre);
    printf("Ingrese el salario mensual (USD): ");
    scanf("%lf", &empleado->salarioMensual);
    printf("Ingrese el tiempo de almuerzo o descanso dentro de la jornada de trabajo (Minutos): ");
    scanf("%d", &empleado->tiempoDescanso);
    printf("Ingrese la hora de inicio de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaInicio, &empleado->minutoInicio);
    printf("Ingrese la hora de fin de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaFin, &empleado->minutoFin);
    printf("Ha trabajado en feriados? (1 para si, 0 para no): ");
    scanf("%d", &empleado->trabajoFeriado);
    printf("Ha trabajado horas extras? (Ingrese el numero de horas extras): ");
    scanf("%d", &empleado->horasExtras);
}

void guardarEmpleado(Empleado empleado) {
    FILE *archivo;
    archivo = fopen("empleados.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }

    fprintf(archivo, "%s %.2lf %d %d %d %d %d %d %d\n", empleado.nombre, empleado.salarioMensual, empleado.tiempoDescanso,
            empleado.horaInicio, empleado.minutoInicio, empleado.horaFin, empleado.minutoFin, empleado.trabajoFeriado, empleado.horasExtras);

    fclose(archivo);
}

void mostrarSueldoMensual() {
    FILE *archivo;
    archivo = fopen("empleados.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }

    printf("\n%-20s %-20s %-20s\n", "Nombre", "Sueldo Mensual", "Detalles");
    printf("-------------------------------------------------------------\n");

    Empleado empleado;
    while (fscanf(archivo, "%s %lf %d %d %d %d %d %d %d\n", empleado.nombre, &empleado.salarioMensual, &empleado.tiempoDescanso,
            &empleado.horaInicio, &empleado.minutoInicio, &empleado.horaFin, &empleado.minutoFin, &empleado.trabajoFeriado, &empleado.horasExtras) != EOF) {
        
        double valorHora = calcularValorHora(empleado.salarioMensual, empleado.tiempoDescanso);
        double valorExtra, valorSuplementario;
        calcularHorasExtras(empleado.horaInicio, empleado.minutoInicio, empleado.horaFin, empleado.minutoFin, empleado.tiempoDescanso,
                            valorHora, empleado.trabajoFeriado, &empleado.horasExtras, &valorExtra, &valorSuplementario);

        double sueldoMensual = empleado.salarioMensual + valorExtra + valorSuplementario;
        
        // Detalles de horas extras y trabajo en feriados
        char detalles[100];
        if (empleado.trabajoFeriado)
            sprintf(detalles, "Horas Extras: %d, Trabajo en Feriado", empleado.horasExtras);
        else
            sprintf(detalles, "Horas Extras: %d", empleado.horasExtras);

        printf("%-20s %-20.2lf %-20s\n", empleado.nombre, sueldoMensual, detalles);
    }

    fclose(archivo);
}

void limpiarSistema() {
    FILE *archivo;
    archivo = fopen("empleados.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.");
        exit(1);
    }
    fclose(archivo);
}

int calcularHorasTrabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso) {
    int minutosInicioTotal = horaInicio * 60 + minutoInicio;
    int minutosFinTotal = horaFin * 60 + minutoFin;
    int minutosTrabajados = minutosFinTotal - minutosInicioTotal - tiempoDescanso;
    
    if (minutosTrabajados < 0) {
        minutosTrabajados += 24 * 60;
    }
    
    return minutosTrabajados / 60;
}

double calcularValorHora(double salarioMensual, int tiempoDescanso) {
    int horasLaborables = Horas_Semanales * 4 - (tiempoDescanso / 60 * 5); 
    return salarioMensual / horasLaborables;
}

void calcularHorasExtras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso, double valorHora, int trabajoFeriado,
                         int *horasExtras, double *valorExtra, double *valorSuplementario) {
    int horasTrabajadas = calcularHorasTrabajadas(horaInicio, minutoInicio, horaFin, minutoFin, tiempoDescanso);
    int horasNormales = Horas_Semanales * 4 - (tiempoDescanso / 60 * 5);
    
    if (horasTrabajadas > horasNormales) {
        *horasExtras = horasTrabajadas - horasNormales;
        *valorExtra = (*horasExtras * valorHora * RECARGO_EXTRA_50) / 2;
        *valorSuplementario = (*horasExtras * valorHora * RECARGO_EXTRA_50) / 2;
    } else {
        *horasExtras = 0;
        *valorExtra = 0;
        *valorSuplementario = 0;
    }

    if (trabajoFeriado) {
        *valorExtra += horasTrabajadas * valorHora * RECARGO_FERIADO;
    }
}


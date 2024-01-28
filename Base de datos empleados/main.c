#include <stdio.h>
#define Horas_Semanales 40
#define RECARGO_EXTRA_100 2.0

// Declaramos las funciones para el calculo de los datos principales 
void solicitarDatos(double *salarioMensual, int *tiempoDescanso, int *horaInicio, int *minutoInicio, int *horaFin, int *minutoFin);
void mostrarDatos(double salarioMensual, int tiempoDescanso, int horaInicio, int minutoInicio, int horaFin, int minutoFin);
double calcularValorHora(double salarioMensual, int tiempoDescanso);//usamos la funcion double para que nos devuelva un resultado decimal grande
void calcularHorasExtras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, double valorHora, double *valorExtra, double *valorSuplementario);
double calcularHorasTrabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso);

int main() {
    double salarioMensual, valorHora, valorExtra, valorSuplementario;
    int tiempoDescanso, horaInicio, minutoInicio, horaFin, minutoFin;

    solicitarDatos(&salarioMensual, &tiempoDescanso, &horaInicio, &minutoInicio, &horaFin, &minutoFin);

    valorHora = calcularValorHora(salarioMensual, tiempoDescanso);

    calcularHorasExtras(horaInicio, minutoInicio, horaFin, minutoFin, valorHora, &valorExtra, &valorSuplementario);

    mostrarDatos(salarioMensual, tiempoDescanso, horaInicio, minutoInicio, horaFin, minutoFin);

    // Mostramos los datos finales que fueron ingresados 
    printf("\nRESULTADOS:\n");
    printf("Valor Hora: %.2lf USD\n", valorHora);
    printf("Horas Extras: %.2lf USD\n", valorExtra);
    printf("Horas Suplementarias: %.2lf USD\n", valorSuplementario);

    return 0;
}
//solicitamos el ingreso de los datos para irnlos almacenando en cada una de las variables
void solicitarDatos(double *salarioMensual, int *tiempoDescanso, int *horaInicio, int *minutoInicio, int *horaFin, int *minutoFin) {
    printf("Ingrese el salario mensual (USD): ");
    scanf("%lf", salarioMensual);
    printf("Ingrese el tiempo de almuerzo o descanso dentro de la jornada de trabajo (Minutos): ");
    scanf("%d", tiempoDescanso);
    printf("Ingrese la hora de inicio de labores (hh:mm): ");
    scanf("%d:%d", horaInicio, minutoInicio);
    printf("Ingrese la hora de fin de labores (hh:mm): ");
    scanf("%d:%d", horaFin, minutoFin);
}
//Peidmos mostrar los datos que ingresamos anteriormente
void mostrarDatos(double salarioMensual, int tiempoDescanso, int horaInicio, int minutoInicio, int horaFin, int minutoFin) {
    printf("\nDATOS INGRESADOS:\n");
    printf("Salario Mensual: %.2lf USD\n", salarioMensual);
    printf("Tiempo de Descanso: %d minutos\n", tiempoDescanso);
    printf("Hora de Inicio: %02d:%02d\n", horaInicio, minutoInicio);
    printf("Hora de Fin: %02d:%02d\n", horaFin, minutoFin);
}

double calcularValorHora(double salarioMensual, int tiempoDescanso) {
    int horasLaborables = Horas_Semanales * 4 - (tiempoDescanso / 60 * 5);
    return salarioMensual / horasLaborables;
}

void calcularHorasExtras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, double valorHora, double *valorExtra, double *valorSuplementario) {
    double horasTrabajadas = calcularHorasTrabajadas(horaInicio, minutoInicio, horaFin, minutoFin, 0);

    if (horasTrabajadas > Horas_Semanales * 4) {
        *valorExtra = (horasTrabajadas - Horas_Semanales * 4) * valorHora * RECARGO_EXTRA_100;
        *valorSuplementario = (Horas_Semanales * 4) * valorHora + *valorExtra;
    } else {
        *valorExtra = 0;
        *valorSuplementario = horasTrabajadas * valorHora;
    }
}

double calcularHorasTrabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso) {
    int minutosInicio = horaInicio * 60 + minutoInicio;
    int minutosFin = horaFin * 60 + minutoFin;
    int minutosTrabajados = minutosFin - minutosInicio - tiempoDescanso;

    return minutosTrabajados / 60.0;
}

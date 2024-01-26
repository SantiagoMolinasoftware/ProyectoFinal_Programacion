#include <stdio.h>
#define Horas_Semanales 40
#define RECARGO_EXTRA_100 2.0

void solicitarDatos(double *salarioMensual, int *tiempoDescanso, int *horaInicio, int *minutoInicio, int *horaFin, int *minutoFin);
void mostrarDatos(double salarioMensual, int tiempoDescanso, int horaInicio, int minutoInicio, int horaFin, int minutoFin);

int main() {
    double salarioMensual;
    int tiempoDescanso, horaInicio, minutoInicio, horaFin, minutoFin;

    solicitarDatos(&salarioMensual, &tiempoDescanso, &horaInicio, &minutoInicio, &horaFin, &minutoFin);
}

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

double calcularValorHora(double salarioMensual, int tiempoDescanso) {
    int horasLaborables = Horas_Semanales * 4 - (tiempoDescanso / 60 * 5); 
    return salarioMensual / horasLaborables;
}

void calcularHorasExtras(int horaInicio, int minutoInicio, int horaFin, int minutoFin, double valorHora, double *valorExtra, double *valorSuplementario) {
    
    *valorExtra = 0;
    *valorSuplementario = 0;
} 

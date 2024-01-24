#include <stdio.h>

void solicitarDatos(double *salarioMensual, int *tiempoDescanso, int *horaInicio, int *minutoInicio, int *horaFin, int *minutoFin);
void mostrarDatos(double salarioMensual, int tiempoDescanso, int horaInicio, int minutoInicio, int horaFin, int minutoFin);

int main() {
    double salarioMensual;
    int tiempoDescanso, horaInicio, minutoInicio, horaFin, minutoFin;

    solicitarDatos(&salarioMensual, &tiempoDescanso, &horaInicio, &minutoInicio, &horaFin, &minutoFin);, 

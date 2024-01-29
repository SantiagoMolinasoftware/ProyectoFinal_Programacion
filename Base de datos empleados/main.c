// Programa para calcular el sueldo mensual de un trabajador en Ecuador
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes globales
#define HORAS_SEMANALES 40 // Numero de horas semanales de trabajo
#define DIAS_MES 30 // Numero de dias en un mes
#define RECARGO_EXTRA_DIURNO 1.5 // Porcentaje de recargo por hora extra diurna
#define RECARGO_FERIADO_DIURNO 2.0 // Porcentaje de recargo por trabajo en feriado diurno
#define APORTE_IESS 0.0945 // Porcentaje de aporte al IESS

// Estructura para almacenar los datos de un empleado
typedef struct Empleado {
    char nombreCompleto[50]; // Nombre completo del empleado
    double salarioMensualUSD; // Salario mensual en dolares
    int tiempoDescansoMinutos; // Tiempo de descanso dentro de la jornada de trabajo en minutos
    int horaInicioDiurna; // Hora de inicio de labores en formato 24 horas
    int minutoInicioDiurno; // Minuto de inicio de labores
    int horaFinDiurna; // Hora de fin de labores en formato 24 horas
    int minutoFinDiurno; // Minuto de fin de labores
    int trabajoFeriadoBooleano; // Indicador de si ha trabajado en feriados (1 para si, 0 para no)
    int horasExtrasDiurnas; // Numero de horas extras diurnas trabajadas en el mes
} Empleado;

// Prototipos de las funciones
void solicitar_datos(Empleado *empleado); // Solicita los datos de un empleado por teclado
void guardar_empleado(Empleado empleado); // Guarda los datos de un empleado en un archivo
void mostrar_sueldo_mensual(); // Muestra el sueldo mensual de los empleados registrados en el archivo
void limpiar_sistema(); // Limpia el archivo de empleados
int calcular_horas_trabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso); // Calcula el numero de horas trabajadas en el mes
double calcular_valor_hora(double salarioMensual, int tiempoDescanso); // Calcula el valor de la hora ordinaria
double calcular_recargo_feriado(double salarioMensual); // Calcula el recargo por trabajo en feriado
double calcular_recargo_horas_extras(double valorHora, int horasExtras); // Calcula el recargo por horas extras
double calcular_aporte_iess(double salarioMensual); // Calcula el aporte al IESS
void validar_entrada(int *valor, int min, int max, char *mensaje); // Valida que un valor entero esté dentro de un rango

// Funcion principal
int main() {
    int opcion; // Variable para almacenar la opcion del menú
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
                Empleado empleado; // Variable para almacenar los datos de un empleado
                solicitar_datos(&empleado); // Solicitar los datos por teclado
                guardar_empleado(empleado); // Guardar los datos en el archivo
                break;
            case 2:
                printf("\nSueldo mensual de los empleados:\n");
                mostrar_sueldo_mensual(); // Mostrar el sueldo mensual de los empleados del archivo
                break;
            case 3:
                printf("\nEsta seguro de limpiar el sistema? (1 para si, 0 para no): ");
                int confirmacion; // Variable para almacenar la confirmacion del usuario
                scanf("%d", &confirmacion);
                if (confirmacion == 1) {
                    limpiar_sistema(); // Limpiar el archivo de empleados
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

// Funcion para solicitar los datos de un empleado por teclado
void solicitar_datos(Empleado *empleado) {
    printf("Ingrese el nombre del empleado: ");
    scanf(" %[^\n]", empleado->nombreCompleto); // Leer el nombre hasta el final de la linea
    printf("Ingrese el salario mensual (USD): ");
    scanf("%lf", &empleado->salarioMensualUSD);
    printf("Ingrese el tiempo de almuerzo o descanso dentro de la jornada de trabajo (Minutos): ");
    scanf("%d", &empleado->tiempoDescansoMinutos);
    printf("Ingrese la hora de inicio de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaInicioDiurna, &empleado->minutoInicioDiurno);
    validar_entrada(&empleado->horaInicioDiurna, 0, 23, "La hora de inicio debe estar entre 0 y 23."); // Validar que la hora de inicio sea valida
    validar_entrada(&empleado->minutoInicioDiurno, 0, 59, "El minuto de inicio debe estar entre 0 y 59."); // Validar que el minuto de inicio sea valido
    printf("Ingrese la hora de fin de labores (hh:mm): ");
    scanf("%d:%d", &empleado->horaFinDiurna, &empleado->minutoFinDiurno);
    validar_entrada(&empleado->horaFinDiurna, 0, 23, "La hora de fin debe estar entre 0 y 23."); // Validar que la hora de fin sea valida
    validar_entrada(&empleado->minutoFinDiurno, 0, 59, "El minuto de fin debe estar entre 0 y 59."); // Validar que el minuto de fin sea válido
    printf("Ha trabajado en feriados? (1 para si, 0 para no): ");
    scanf("%d", &empleado->trabajoFeriadoBooleano);
    validar_entrada(&empleado->trabajoFeriadoBooleano, 0, 1, "El indicador de feriado debe ser 0 o 1."); // Validar que el indicador de feriado sea valido
    printf("Ha trabajado horas extras? (Ingrese el numero de horas extras): ");
    scanf("%d", &empleado->horasExtrasDiurnas);
    validar_entrada(&empleado->horasExtrasDiurnas, 0, 24, "El número de horas extras debe estar entre 0 y 24."); // Validar que el numero de horas extras sea valido
}

// Funcion para guardar los datos de un empleado en un archivo
void guardar_empleado(Empleado empleado) {
    FILE *archivo; // Variable para manejar el archivo
    archivo = fopen("empleados.txt", "a"); // Abrir el archivo en modo de agregar
    if (archivo == NULL) { // Si el archivo no se pudo abrir
        printf("Error al abrir el archivo.");
        exit(1); // Terminar el programa con un codigo de error
    }

    // Escribir los datos del empleado en el archivo, separados por espacios
    fprintf(archivo, "%s %.2lf %d %d %d %d %d %d %d\n", empleado.nombreCompleto, empleado.salarioMensualUSD, empleado.tiempoDescansoMinutos,
            empleado.horaInicioDiurna, empleado.minutoInicioDiurno, empleado.horaFinDiurna, empleado.minutoFinDiurno, empleado.trabajoFeriadoBooleano, empleado.horasExtrasDiurnas);

    if (fclose(archivo) == EOF) { // Si el archivo no se pudo cerrar
        printf("Error al cerrar el archivo.");
        exit(1); // Terminar el programa con un codigo de error
    }
}

// Funcion para mostrar el sueldo mensual de los empleados registrados en el archivo
void mostrar_sueldo_mensual() {
    FILE *archivo; // Variable para manejar el archivo
    archivo = fopen("empleados.txt", "r"); // Abrir el archivo en modo de lectura
    if (archivo == NULL) { // Si el archivo no se pudo abrir
        printf("Error al abrir el archivo.");
        exit(1); // Terminar el programa con un codigo de error
    }

    // Imprimir los encabezados de la tabla
    printf("\n%-20s %-20s %-20s %-20s\n", "Nombre", "Sueldo Mensual", "Horas Extras", "Aporte al IESS");
    printf("--------------------------------------------------------------------------------\n");

    Empleado empleado; // Variable para almacenar los datos de un empleado
    while (fscanf(archivo, "%s %lf %d %d %d %d %d %d %d\n", empleado.nombreCompleto, &empleado.salarioMensualUSD, &empleado.tiempoDescansoMinutos,
            &empleado.horaInicioDiurna, &empleado.minutoInicioDiurno, &empleado.horaFinDiurna, &empleado.minutoFinDiurno, &empleado.trabajoFeriadoBooleano, &empleado.horasExtrasDiurnas) != EOF) {
        
        double valor_hora = calcular_valor_hora(empleado.salarioMensualUSD, empleado.tiempoDescansoMinutos); // Calcular el valor de la hora ordinaria
        int horas_extras = empleado.horasExtrasDiurnas; // Obtener el numero de horas extras diurnas
        
        double sueldo_mensual = empleado.salarioMensualUSD; // Inicializar el sueldo mensual con el salario base
        if (empleado.trabajoFeriadoBooleano) // Si ha trabajado en feriado
            sueldo_mensual += calcular_recargo_feriado(empleado.salarioMensualUSD); // Sumar el recargo por trabajo en feriado
        
        sueldo_mensual += calcular_recargo_horas_extras(valor_hora, horas_extras); // Sumar el recargo por horas extras
        
        sueldo_mensual -= calcular_aporte_iess(empleado.salarioMensualUSD); // Restar el aporte al IESS
        
        // Imprimir los datos del empleado en la tabla
        printf("%-20s %-20.2lf %-20d 9,45%\n", empleado.nombreCompleto, sueldo_mensual, horas_extras);
    }

    if (fclose(archivo) == EOF) { // Si el archivo no se pudo cerrar
        printf("Error al cerrar el archivo.");
        exit(1); // Terminar el programa con un código de error
    }
}

// Funcion para limpiar el archivo de empleados
void limpiar_sistema() {
    FILE *archivo; // Variable para manejar el archivo
    archivo = fopen("empleados.txt", "w"); // Abrir el archivo en modo de escritura
    if (archivo == NULL) { // Si el archivo no se pudo abrir
        printf("Error al abrir el archivo.");
        exit(1); // Terminar el programa con un codigo de error
    }
    if (fclose(archivo) == EOF) { // Si el archivo no se pudo cerrar
        printf("Error al cerrar el archivo.");
        exit(1); // Terminar el programa con un codigo de error
    }
}

// Funcion para calcular el numero de horas trabajadas en el mes
int calcular_horas_trabajadas(int horaInicio, int minutoInicio, int horaFin, int minutoFin, int tiempoDescanso) {
    int minutos_inicio_total = horaInicio * 60 + minutoInicio; // Convertir la hora de inicio a minutos
    int minutos_fin_total = horaFin * 60 + minutoFin; // Convertir la hora de fin a minutos
    int minutos_trabajados = minutos_fin_total - minutos_inicio_total - tiempoDescanso; // Calcular los minutos trabajados
    
    if (minutos_trabajados < 0) { // Si los minutos trabajados son negativos
        minutos_trabajados += 24 * 60; // Sumar un dia completo en minutos
    }
    
    return minutos_trabajados / 60; // Convertir los minutos trabajados a horas y retornar el resultado
}

// Funcion para calcular el valor de la hora ordinaria
double calcular_valor_hora(double salarioMensual, int tiempoDescanso) {
    int horas_laborables = HORAS_SEMANALES * 4 - (tiempoDescanso / 60 * 5); // Calcular el numero de horas laborables en el mes
    return salarioMensual / horas_laborables; // Dividir el salario mensual entre las horas laborables y retornar el resultado
}

// Funcion para calcular el recargo por trabajo en feriado
double calcular_recargo_feriado(double salarioMensual) {
    return salarioMensual / DIAS_MES * RECARGO_FERIADO_DIURNO; // Multiplicar el salario diario por el porcentaje de recargo y retornar el resultado
}

// Funcion para calcular el recargo por horas extras
double calcular_recargo_horas_extras(double valorHora, int horasExtras) {
    return horasExtras * valorHora * RECARGO_EXTRA_DIURNO; // Multiplicar el numero de horas extras por el valor de la hora y el porcentaje de recargo y retornar el resultado
}

// Funcion para calcular el aporte al IESS
double calcular_aporte_iess(double salarioMensual) {
    return salarioMensual * APORTE_IESS; // Multiplicar el salario mensual por el porcentaje de aporte y retornar el resultado
}

// Funcion para validar que un valor entero este dentro de un rango
void validar_entrada(int *valor, int min, int max, char *mensaje) {
    while (*valor < min || *valor > max) { // Mientras el valor sea menor que el minimo o mayor que el maximo
        printf("%s\n", mensaje); // Imprimir el mensaje de error
        printf("Ingrese de nuevo el valor: "); // Pedir al usuario que ingrese de nuevo el valor
        scanf("%d", valor); // Leer el valor por teclado
    }
}

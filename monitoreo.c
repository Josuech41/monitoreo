
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ZONAS 5
#define DIAS_HISTORICOS 30

typedef struct
{
    char nombre[50];
    float co2, so2, no2, pm25;
    float historicos[DIAS_HISTORICOS][4];
} Zona;

void cargarDatos(Zona zonas[], int numZonas);
void guardarDatos(Zona zonas[], int numZonas, const char *archivo);
void recuperarDatos(Zona zonas[], int numZonas, const char *archivo);
void calcularPromedios(Zona zonas[], int numZonas);
void predecirNiveles(Zona zonas[], int numZonas);
void emitirAlertas(Zona zonas[], int numZonas);
void generarRecomendaciones(Zona zonas[], int numZonas);

int main()
{
    Zona zonas[NUM_ZONAS];
    const char *archivoDatos = "datos_contaminacion.dat";
    recuperarDatos(zonas, NUM_ZONAS, archivoDatos);
    cargarDatos(zonas, NUM_ZONAS);
    calcularPromedios(zonas, NUM_ZONAS);
    predecirNiveles(zonas, NUM_ZONAS);
    emitirAlertas(zonas, NUM_ZONAS);
    generarRecomendaciones(zonas, NUM_ZONAS);
    guardarDatos(zonas, NUM_ZONAS, archivoDatos);
    return 0;
}

void cargarDatos(Zona zonas[], int numZonas)
{
    for (int i = 0; i < numZonas; i++)
    {
        printf("\nIngrese el nombre de la zona %d: ", i + 1);
        scanf("%s", zonas[i].nombre);
        printf("Ingrese los niveles actuales de CO2,SO2,NO2,PM2.5: ");
        scanf("%f%f%f%f", &zonas[i].co2, &zonas[i].so2, &zonas[i].no2, &zonas[i].pm25);
        printf("Ingrese los datos históricos para los últimos 30 días (CO2,SO2,NO2,PM2.5):\n");
        for (int j = 0; j < DIAS_HISTORICOS; j++)
        {
            printf("Día %d: ", j + 1);
            scanf("%f%f%f%f", &zonas[i].historicos[j][0], &zonas[i].historicos[j][1], &zonas[i].historicos[j][2], &zonas[i].historicos[j][3]);
        }
    }
}

void guardarDatos(Zona zonas[], int numZonas, const char *archivo)
{
    FILE *file = fopen(archivo, "w");
    if (!file)
    {
        printf("Error al guardar los datos.\n");
        return;
    }
    for (int i = 0; i < numZonas; i++)
    {
        fprintf(file, "%s\n", zonas[i].nombre);
        fprintf(file, "%.2f %.2f %.2f %.2f\n", zonas[i].co2, zonas[i].so2, zonas[i].no2, zonas[i].pm25);
        for (int j = 0; j < DIAS_HISTORICOS; j++)
        {
            fprintf(file, "%.2f %.2f %.2f %.2f\n", zonas[i].historicos[j][0], zonas[i].historicos[j][1], zonas[i].historicos[j][2], zonas[i].historicos[j][3]);
        }
    }
    fclose(file);
}

void recuperarDatos(Zona zonas[], int numZonas, const char *archivo)
{
    FILE *file = fopen(archivo, "r");
    if (!file)
    {
        printf("No se encontraron datos previos.\n");
        return;
    }
    for (int i = 0; i < numZonas; i++)
    {
        fscanf(file, "%s", zonas[i].nombre);
        fscanf(file, "%f %f %f %f", &zonas[i].co2, &zonas[i].so2, &zonas[i].no2, &zonas[i].pm25);
        for (int j = 0; j < DIAS_HISTORICOS; j++)
        {
            fscanf(file, "%f %f %f %f", &zonas[i].historicos[j][0], &zonas[i].historicos[j][1], &zonas[i].historicos[j][2], &zonas[i].historicos[j][3]);
        }
    }
    fclose(file);
}

void calcularPromedios(Zona zonas[], int numZonas)
{
    printf("\n--- Promedios Históricos ---\n");
    for (int i = 0; i < numZonas; i++)
    {
        printf("Zona: %s\n", zonas[i].nombre);
        float promedio[4] = {0};
        for (int j = 0; j < DIAS_HISTORICOS; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                promedio[k] += zonas[i].historicos[j][k];
            }
        }
        for (int k = 0; k < 4; k++)
        {
            promedio[k] /= DIAS_HISTORICOS;
        }
        printf("Promedio CO2: %.2f,SO2: %.2f,NO2: %.2f,PM2.5: %.2f\n", promedio[0], promedio[1], promedio[2], promedio[3]);
    }
}

void predecirNiveles(Zona zonas[], int numZonas)
{
    printf("\n--- Predicción de Niveles Futuros ---\n");
    for (int i = 0; i < numZonas; i++)
    {
        printf("Zona: %s\n", zonas[i].nombre);
        float prediccion[4] = {0};
        float pesos[5] = {0.4, 0.3, 0.2, 0.1, 0.0};
        for (int k = 0; k < 4; k++)
        {
            for (int j = 0; j < 5; j++)
            {
                prediccion[k] += zonas[i].historicos[DIAS_HISTORICOS - 1 - j][k] * pesos[j];
            }
        }
        printf("Predicción CO2: %.2f,SO2: %.2f,NO2: %.2f,PM2.5: %.2f\n", prediccion[0], prediccion[1], prediccion[2], prediccion[3]);
    }
}

void emitirAlertas(Zona zonas[], int numZonas)
{
    printf("\n--- Alertas Preventivas ---\n");
    const float limites[4] = {400.0, 80.0, 100.0, 25.0};
    for (int i = 0; i < numZonas; i++)
    {
        printf("Zona: %s\n", zonas[i].nombre);
        if (zonas[i].co2 > limites[0])
        {
            printf("ALERTA: CO2 supera el límite (%.2f>%.2f)\n", zonas[i].co2, limites[0]);
        }
        if (zonas[i].so2 > limites[1])
        {
            printf("ALERTA: SO2 supera el límite (%.2f>%.2f)\n", zonas[i].so2, limites[1]);
        }
        if (zonas[i].no2 > limites[2])
        {
            printf("ALERTA: NO2 supera el límite (%.2f>%.2f)\n", zonas[i].no2, limites[2]);
        }
        if (zonas[i].pm25 > limites[3])
        {
            printf("ALERTA: PM2.5 supera el límite (%.2f>%.2f)\n", zonas[i].pm25, limites[3]);
        }
    }
}

void generarRecomendaciones(Zona zonas[], int numZonas)
{
    printf("\n--- Recomendaciones ---\n");
    for (int i = 0; i < numZonas; i++)
    {
        printf("Zona: %s\n", zonas[i].nombre);
        if (zonas[i].pm25 > 25.0)
        {
            printf("- Reducir el tráfico vehicular.\n");
            printf("- Promover el transporte público y días sin automóvil.\n");
            printf("- Suspensión de actividades al aire libre.\n");
        }
        else
        {
            printf("- Niveles aceptables. Mantener buenas prácticas ambientales.\n");
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voto.h"

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void) {
    SistemaVotacion sistema;
    inicializar_sistema(&sistema);

    int opcion;
    char nombre[MAX_NOMBRE];
    int id;

    printf("=== Sistema de Voto Electr\u00f3nico ===\n");

    while (1) {
        printf("\n1. Agregar candidato\n");
        printf("2. Emitir voto\n");
        printf("3. Ver candidatos\n");
        printf("4. Ver resultados\n");
        printf("5. Salir\n");
        printf("Seleccione una opci\u00f3n: ");

        if (scanf("%d", &opcion) != 1) {
            limpiar_buffer();
            printf("Opci\u00f3n inv\u00e1lida.\n");
            continue;
        }
        limpiar_buffer();

        switch (opcion) {
        case 1:
            printf("Ingrese el nombre del candidato: ");
            if (fgets(nombre, sizeof(nombre), stdin) == NULL) {
                break;
            }
            nombre[strcspn(nombre, "\n")] = '\0';
            if (strlen(nombre) == 0) {
                printf("Nombre inv\u00e1lido.\n");
                break;
            }
            id = agregar_candidato(&sistema, nombre);
            if (id < 0) {
                printf("No se pueden agregar m\u00e1s candidatos (m\u00e1ximo %d).\n",
                       MAX_CANDIDATOS);
            } else {
                printf("Candidato '%s' registrado con ID %d.\n", nombre, id);
            }
            break;

        case 2:
            mostrar_candidatos(&sistema);
            if (sistema.total_candidatos == 0) {
                break;
            }
            printf("Ingrese el ID del candidato: ");
            if (scanf("%d", &id) != 1) {
                limpiar_buffer();
                printf("ID inv\u00e1lido.\n");
                break;
            }
            limpiar_buffer();
            if (votar(&sistema, id) < 0) {
                printf("Candidato con ID %d no encontrado.\n", id);
            } else {
                printf("Voto registrado correctamente.\n");
            }
            break;

        case 3:
            mostrar_candidatos(&sistema);
            break;

        case 4:
            mostrar_resultados(&sistema);
            break;

        case 5:
            printf("Saliendo del sistema de votaci\u00f3n.\n");
            return EXIT_SUCCESS;

        default:
            printf("Opci\u00f3n inv\u00e1lida. Intente de nuevo.\n");
            break;
        }
    }

    return EXIT_SUCCESS;
}

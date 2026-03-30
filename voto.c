#include <stdio.h>
#include <string.h>
#include "voto.h"

void inicializar_sistema(SistemaVotacion *sistema) {
    sistema->total_candidatos = 0;
    sistema->total_votos = 0;
    for (int i = 0; i < MAX_CANDIDATOS; i++) {
        sistema->candidatos[i].id = 0;
        sistema->candidatos[i].votos = 0;
        memset(sistema->candidatos[i].nombre, 0, MAX_NOMBRE);
    }
}

int agregar_candidato(SistemaVotacion *sistema, const char *nombre) {
    if (sistema->total_candidatos >= MAX_CANDIDATOS) {
        return -1;
    }
    int idx = sistema->total_candidatos;
    sistema->candidatos[idx].id = idx + 1;
    strncpy(sistema->candidatos[idx].nombre, nombre, MAX_NOMBRE - 1);
    sistema->candidatos[idx].nombre[MAX_NOMBRE - 1] = '\0';
    sistema->candidatos[idx].votos = 0;
    sistema->total_candidatos++;
    return sistema->candidatos[idx].id;
}

int buscar_candidato(const SistemaVotacion *sistema, int id) {
    for (int i = 0; i < sistema->total_candidatos; i++) {
        if (sistema->candidatos[i].id == id) {
            return i;
        }
    }
    return -1;
}

int votar(SistemaVotacion *sistema, int id_candidato) {
    int idx = buscar_candidato(sistema, id_candidato);
    if (idx < 0) {
        return -1;
    }
    sistema->candidatos[idx].votos++;
    sistema->total_votos++;
    return 0;
}

void mostrar_candidatos(const SistemaVotacion *sistema) {
    if (sistema->total_candidatos == 0) {
        printf("No hay candidatos registrados.\n");
        return;
    }
    printf("\n--- Candidatos ---\n");
    for (int i = 0; i < sistema->total_candidatos; i++) {
        printf("  ID: %d  Nombre: %s\n",
               sistema->candidatos[i].id,
               sistema->candidatos[i].nombre);
    }
}

void mostrar_resultados(const SistemaVotacion *sistema) {
    if (sistema->total_candidatos == 0) {
        printf("No hay candidatos registrados.\n");
        return;
    }
    printf("\n--- Resultados de la Votaci\u00f3n ---\n");
    printf("Total de votos emitidos: %d\n\n", sistema->total_votos);

    int ganador_idx = 0;
    int empate = 0;
    for (int i = 0; i < sistema->total_candidatos; i++) {
        double porcentaje = 0.0;
        if (sistema->total_votos > 0) {
            porcentaje = (double)sistema->candidatos[i].votos /
                         sistema->total_votos * 100.0;
        }
        printf("  %s: %d votos (%.1f%%)\n",
               sistema->candidatos[i].nombre,
               sistema->candidatos[i].votos,
               porcentaje);
        if (sistema->candidatos[i].votos > sistema->candidatos[ganador_idx].votos) {
            ganador_idx = i;
            empate = 0;
        } else if (i != ganador_idx &&
                   sistema->candidatos[i].votos == sistema->candidatos[ganador_idx].votos) {
            empate = 1;
        }
    }

    if (sistema->total_votos > 0) {
        if (empate) {
            printf("\nResultado: empate con %d votos cada uno.\n",
                   sistema->candidatos[ganador_idx].votos);
        } else {
            printf("\nGanador: %s con %d votos\n",
                   sistema->candidatos[ganador_idx].nombre,
                   sistema->candidatos[ganador_idx].votos);
        }
    }
}

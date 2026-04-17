#ifndef VOTO_H
#define VOTO_H

#define MAX_CANDIDATOS 10
#define MAX_NOMBRE 50

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    int votos;
} Candidato;

typedef struct {
    Candidato candidatos[MAX_CANDIDATOS];
    int total_candidatos;
    int total_votos;
} SistemaVotacion;

void inicializar_sistema(SistemaVotacion *sistema);
int agregar_candidato(SistemaVotacion *sistema, const char *nombre);
int votar(SistemaVotacion *sistema, int id_candidato);
void mostrar_resultados(const SistemaVotacion *sistema);
void mostrar_candidatos(const SistemaVotacion *sistema);
int buscar_candidato(const SistemaVotacion *sistema, int id);

#endif /* VOTO_H */

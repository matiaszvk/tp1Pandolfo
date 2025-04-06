#include "src/pokedex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *tipo_a_string(tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_AGUA:
		return "Agua";
	case TIPO_FUEGO:
		return "Fuego";
	case TIPO_PLANTA:
		return "Planta";
	case TIPO_ROCA:
		return "Roca";
	case TIPO_ELECTRICO:
		return "Eléctrico";
	case TIPO_NORMAL:
		return "Normal";
	case TIPO_LUCHA:
		return "Lucha";
	default:
		return "Desconocido";
	}
}
bool mostrar_pokemon(struct pokemon *pokemon, void *ctx)
{
	printf("Id: %d, Nombre: %s, Tipo: %s, Fuerza: %d, Destreza: %d, Inteligencia: %d\n",
	       pokemon->id, pokemon->nombre, tipo_a_string(pokemon->tipo),
	       pokemon->fuerza, pokemon->destreza, pokemon->inteligencia);
	return true;
}

int main(int argc, char const *argv[])
{
	if (argc < 3) {
		printf("Uso: %s <archivo.csv> <operacion> [parametro]\n",
		       argv[0]);
		printf("Operaciones:\n");
		printf("  listar_nombre\n");
		printf("  listar_id\n");
		printf("  buscar_id <id>\n");
		printf("  buscar_nombre <nombre>\n");
		return 1;
	}

	const char *archivo = argv[1];
	const char *operacion = argv[2];

	pokedex_t *pokedex = pokedex_abrir(archivo);
	if (!pokedex) {
		printf("Error al abrir archivo %s\n", archivo);
		return 1;
	}

	if (strcmp(operacion, "listar_nombre") == 0) {
		pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE,
					 mostrar_pokemon, NULL);
	} else if (strcmp(operacion, "listar_id") == 0) {
		pokedex_iterar_pokemones(pokedex, ITERAR_ID, mostrar_pokemon,
					 NULL);
	} else if (strcmp(operacion, "buscar_id") == 0) {
		if (argc < 4) {
			printf("Falta el id\n");
		} else {
			unsigned id = (unsigned)atoi(argv[3]);
			const struct pokemon *poke =
				pokedex_buscar_pokemon_id(pokedex, id);
			if (poke) {
				mostrar_pokemon((struct pokemon *)poke, NULL);
			} else {
				printf("No se encontro el Pokemon con id %u\n",
				       id);
			}
		}
	} else if (strcmp(operacion, "buscar_nombre") == 0) {
		if (argc < 4) {
			printf("Falta nombre\n");
		} else {
			const struct pokemon *poke =
				pokedex_buscar_pokemon_nombre(pokedex, argv[3]);
			if (poke) {
				mostrar_pokemon((struct pokemon *)poke, NULL);
			} else {
				printf("No se encontro un Pokemon con nombre '%s'\n",
				       argv[3]);
			}
		}
	} else {
		printf("Operacion '%s' invalida\n", operacion);
	}

	pokedex_destruir(pokedex);
	return 0;
}

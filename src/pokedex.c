#include "pokedex.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINEA 100
struct pokedex {
	struct pokemon *pokemones;
	struct pokemon **por_id;
	struct pokemon **por_nombre;
	unsigned cantidad;
};

//----------------------------------------------------FUNCION AUX
bool convertir_tipo_pokemon(const char id_letra, tipo_pokemon *tipo)
{
	switch (id_letra) {
	case 'A':
	case 'a':
		*tipo = TIPO_AGUA;
		break;
	case 'F':
	case 'f':
		*tipo = TIPO_FUEGO;
		break;
	case 'P':
	case 'p':
		*tipo = TIPO_PLANTA;
		break;
	case 'R':
	case 'r':
		*tipo = TIPO_ROCA;
		break;
	case 'E':
	case 'e':
		*tipo = TIPO_ELECTRICO;
		break;
	case 'N':
	case 'n':
		*tipo = TIPO_NORMAL;
		break;
	case 'L':
	case 'l':
		*tipo = TIPO_LUCHA;
		break;
	default:
		return false;
	}
	return true;
}

//----------------------------------------------------FUNCION AUX
char *leer_pokemon(FILE *archivo)
{
	size_t capacidad = 128;
	size_t largo = 0;
	char *linea = malloc(capacidad);
	if (!linea)
		return NULL;

	int c;
	while ((c = fgetc(archivo)) != EOF && c != '\n') {
		if (largo + 1 >= capacidad) {
			capacidad *= 2;
			char *nueva_linea = realloc(linea, capacidad);
			if (!nueva_linea) {
				free(linea);
				return NULL;
			}
			linea = nueva_linea;
		}
		linea[largo++] = (char)c;
	}

	if (c == EOF && largo == 0) {
		free(linea);
		return NULL;
	}

	linea[largo] = '\0';
	return linea;
}

//----------------------------------------------------FUNCION AUX
void ordenar_por_id(struct pokemon **pokemones, unsigned cantidad)
{
	for (int i = 1; i < cantidad; i++) {
		struct pokemon *actual = pokemones[i];
		int j = i - 1;

		while (j >= 0 && pokemones[j]->id > actual->id) {
			pokemones[j + 1] = pokemones[j];
			j--;
		}
		pokemones[j + 1] = actual;
	}
}

//----------------------------------------------------FUNCION AUX
void ordenar_por_nombre(struct pokemon **pokemones, unsigned cantidad)
{
	for (int i = 1; i < cantidad; i++) {
		struct pokemon *actual = pokemones[i];
		int j = i - 1;

		while (j >= 0 &&
		       strcmp(pokemones[j]->nombre, actual->nombre) > 0) {
			pokemones[j + 1] = pokemones[j];
			j--;
		}
		pokemones[j + 1] = actual;
	}
}

//----------------------------------------------------FUNCION AUX
bool parsear_linea_pokemon(char *linea, struct pokemon *p)
{
	char *inicio = linea;
	char *fin;

	p->nombre = NULL;

	fin = strchr(inicio, ';');
	if (!fin)
		return false;
	*fin = '\0';
	p->id = (unsigned)atoi(inicio);
	inicio = fin + 1;

	fin = strchr(inicio, ';');
	if (!fin)
		return false;
	size_t largo_nombre = (size_t)(fin - inicio);
	p->nombre = malloc(largo_nombre + 1);
	if (!p->nombre)
		return false;
	memcpy((char *)p->nombre, inicio, largo_nombre);
	((char *)p->nombre)[largo_nombre] = '\0';
	inicio = fin + 1;

	fin = strchr(inicio, ';');
	if (!fin || fin == inicio) {
		free((char *)p->nombre);
		return false;
	}
	if (!convertir_tipo_pokemon(*inicio, &p->tipo)) {
		free((char *)p->nombre);
		return false;
	}
	inicio = fin + 1;

	fin = strchr(inicio, ';');
	if (!fin) {
		free((char *)p->nombre);
		return false;
	}
	*fin = '\0';
	p->fuerza = (unsigned)atoi(inicio);
	inicio = fin + 1;

	fin = strchr(inicio, ';');
	if (!fin) {
		free((char *)p->nombre);
		return false;
	}
	*fin = '\0';
	p->destreza = (unsigned)atoi(inicio);
	inicio = fin + 1;

	p->inteligencia = (unsigned)atoi(inicio);

	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
pokedex_t *pokedex_abrir(const char *archivo)
{
	if (!archivo)
		return NULL;

	FILE *fp = fopen(archivo, "r");
	if (!fp)
		return NULL;

	pokedex_t *pokedex = calloc(1, sizeof(pokedex_t));
	if (!pokedex) {
		fclose(fp);
		return NULL;
	}

	char *linea;
	while ((linea = leer_pokemon(fp)) != NULL) {
		struct pokemon p;

		if (!parsear_linea_pokemon(linea, &p)) {
			free(linea);
			break;
		}

		struct pokemon *temp = realloc(pokedex->pokemones,
					       (pokedex->cantidad + 1) *
						       sizeof(struct pokemon));
		if (!temp) {
			free((char *)p.nombre);
			free(linea);
			break;
		}
		pokedex->pokemones = temp;
		pokedex->pokemones[pokedex->cantidad++] = p;

		free(linea);
	}

	fclose(fp);

	if (pokedex->cantidad == 0) {
		free(pokedex);
		return NULL;
	}

	pokedex->por_id = malloc(sizeof(struct pokemon *) * pokedex->cantidad);
	pokedex->por_nombre =
		malloc(sizeof(struct pokemon *) * pokedex->cantidad);
	if (!pokedex->por_id || !pokedex->por_nombre) {
		pokedex_destruir(pokedex);
		return NULL;
	}

	for (unsigned i = 0; i < pokedex->cantidad; i++) {
		pokedex->por_id[i] = &pokedex->pokemones[i];
		pokedex->por_nombre[i] = &pokedex->pokemones[i];
	}

	ordenar_por_id(pokedex->por_id, pokedex->cantidad);
	ordenar_por_nombre(pokedex->por_nombre, pokedex->cantidad);

	return pokedex;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
unsigned pokedex_cantidad_pokemones(pokedex_t *pokedex)
{
	if (!pokedex) {
		return 0;
	}
	return pokedex->cantidad;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
const struct pokemon *pokedex_buscar_pokemon_nombre(pokedex_t *pokedex,
						    const char *nombre)
{
	if (!pokedex || !nombre)
		return NULL;

	for (unsigned i = 0; i < pokedex->cantidad; i++) {
		if (strcmp(pokedex->pokemones[i].nombre, nombre) == 0)
			return &pokedex->pokemones[i];
	}
	return NULL;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
const struct pokemon *pokedex_buscar_pokemon_id(pokedex_t *pokedex, unsigned id)
{
	if (!pokedex) {
		return NULL;
	}

	for (unsigned i = 0; i < pokedex->cantidad; i++) {
		if (pokedex->pokemones[i].id == id) {
			return &pokedex->pokemones[i];
		}
	}
	return NULL;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
unsigned pokedex_iterar_pokemones(pokedex_t *pokedex, enum modo_iteracion modo,
				  bool (*funcion)(struct pokemon *, void *),
				  void *ctx)
{
	if (!pokedex || !funcion || pokedex->cantidad == 0)
		return 0;

	struct pokemon **vector = (modo == ITERAR_ID) ? pokedex->por_id :
							pokedex->por_nombre;
	unsigned iterados = 0;

	for (unsigned i = 0; i < pokedex->cantidad; i++) {
		if (!funcion(vector[i], ctx))
			return i + 1;
		iterados++;
	}

	return iterados;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++FUNCION "PRINCIPAL"
void pokedex_destruir(pokedex_t *pokedex)
{
	if (!pokedex)
		return;

	for (unsigned i = 0; i < pokedex->cantidad; i++) {
		free((char *)pokedex->pokemones[i].nombre);
	}

	free(pokedex->pokemones);
	free(pokedex->por_id);
	free(pokedex->por_nombre);
	free(pokedex);
}

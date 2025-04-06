#include "pa2m.h"
#include "src/pokedex.h"
#include <string.h>

#define ARCHIVO_PRUEBA "ejemplos/pokedex.csv"
#define ARCHIVO_INEXISTENTE "jijkkkkkk.csv"
#define ARCHIVO_MALFORMADO "ejemplos/malformado.csv"
#define ARCHIVO_TODO_INVALIDO "ejemplos/solo_invalidos.csv"

void pokedex_abrir_lee_todos_los_pokemon_y_devuelve_la_cantidad_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	pa2m_afirmar(pokedex != NULL, "La pokedex no es nula");
	unsigned cantidad = pokedex_cantidad_pokemones(pokedex);
	pa2m_afirmar(cantidad == 9,
		     "La pokedex tiene 9 pokemones (valor obtenido: %d)",
		     cantidad);
	pokedex_destruir(pokedex);
}

void pokedex_abrir_devuelve_null_si_el_archivo_no_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(pokedex == NULL,
		     "Abrir un archivo inexistente devuelve NULL");
}

void pokedex_buscar_pokemon_nombre_devuelve_el_pokemon_cuando_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *pika =
		pokedex_buscar_pokemon_nombre(pokedex, "Pikachu");
	pa2m_afirmar(strcmp(pika->nombre, "Pikachu") == 0,
		     "El pokemon encontrado es Pikachu");
	pokedex_destruir(pokedex);
}

void pokedex_deja_de_leer_ante_linea_invalida()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_MALFORMADO);
	pa2m_afirmar(pokedex != NULL,
		     "La pokedex no es NULL aunque haya una linea invalida");
	pa2m_afirmar(
		pokedex_cantidad_pokemones(pokedex) == 1,
		"Se leyeron solo los pokemones validos antes de encontrar la liena invalida");
	pokedex_destruir(pokedex);
}

void pokedex_devuelve_null_si_no_hay_pokemones_validos()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_TODO_INVALIDO);
	pa2m_afirmar(pokedex == NULL,
		     "Si no hay pokemones validos, se devuelve NULL");
}

void pokedex_cantidad_devuelve_0_con_pokedex_null()
{
	pa2m_afirmar(pokedex_cantidad_pokemones(NULL) == 0,
		     "La cantidad de pokemones es 0 si la pokedex es NULL");
}

void pokedex_cantidad_es_correcta_para_archivo_valido()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	pa2m_afirmar(
		pokedex_cantidad_pokemones(pokedex) == 9,
		"Cantidad correcta de pokemones leidos (9) desde archivo valido");
	pokedex_destruir(pokedex);
}

void pokedex_cantidad_con_malformado_es_correcta_hasta_linea_invalida()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_MALFORMADO);
	pa2m_afirmar(
		pokedex != NULL,
		"La pokedex no es NULL para archivo malformado parcialmente");
	pa2m_afirmar(
		pokedex_cantidad_pokemones(pokedex) == 1,
		"Cantidad correcta de pokemones antes de linea invalida (esperado: 1)");
	pokedex_destruir(pokedex);
}

void buscar_pokemon_por_nombre_devuelve_puntero_si_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *encontrado =
		pokedex_buscar_pokemon_nombre(pokedex, "Pikachu");
	pa2m_afirmar(encontrado != NULL,
		     "Buscar por nombre devuelve puntero si el Pokemon existe");
	pa2m_afirmar(strcmp(encontrado->nombre, "Pikachu") == 0,
		     "El nombre del Pokemon encontrado es Pikachu");
	pokedex_destruir(pokedex);
}

void buscar_pokemon_por_nombre_devuelve_null_si_no_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *no_existe =
		pokedex_buscar_pokemon_nombre(pokedex, "Manuel");
	pa2m_afirmar(no_existe == NULL,
		     "Buscar por nombre devuelve NULL si el Pokemon no existe");
	pokedex_destruir(pokedex);
}

void buscar_pokemon_por_nombre_con_pokedex_null_devuelve_null()
{
	const struct pokemon *resultado =
		pokedex_buscar_pokemon_nombre(NULL, "Pikachu");
	pa2m_afirmar(resultado == NULL,
		     "Buscar por nombre con pokedex NULL devuelve NULL");
}

void buscar_pokemon_por_id_devuelve_puntero_si_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *encontrado =
		pokedex_buscar_pokemon_id(pokedex, 25);
	pa2m_afirmar(encontrado != NULL,
		     "Buscar por ID devuelve puntero si el Pokemon existe");
	pa2m_afirmar(encontrado->id == 25,
		     "El ID del Pokemon encontrado es 25");
	pokedex_destruir(pokedex);
}

void buscar_pokemon_por_id_devuelve_null_si_no_existe()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *no_existe =
		pokedex_buscar_pokemon_id(pokedex, 999);
	pa2m_afirmar(no_existe == NULL,
		     "Buscar por ID devuelve NULL si el Pokemon no existe");
	pokedex_destruir(pokedex);
}

void buscar_pokemon_por_id_con_pokedex_null_devuelve_null()
{
	const struct pokemon *resultado = pokedex_buscar_pokemon_id(NULL, 25);
	pa2m_afirmar(resultado == NULL,
		     "Buscar por ID con pokedex NULL devuelve NULL");
}

bool contar_pokemon(struct pokemon *p, void *contexto)
{
	if (!p || !contexto)
		return false;
	(*(int *)contexto)++;
	return true;
}

bool cortar_al_tercero(struct pokemon *p, void *ctx)
{
	int *contador = ctx;
	(*contador)++;
	return (*contador < 3);
}

void iterar_todos_los_pokemon_por_id()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	int contador = 0;
	unsigned iterados = pokedex_iterar_pokemones(pokedex, ITERAR_ID,
						     contar_pokemon, &contador);
	pa2m_afirmar(iterados == 9, "Se iteran los 9 pokemones por ID");
	pa2m_afirmar(contador == 9, "La funcion se invoca 9 veces (por ID)");
	pokedex_destruir(pokedex);
}

void iterar_todos_los_pokemon_por_nombre()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	int contador = 0;
	unsigned iterados = pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE,
						     contar_pokemon, &contador);
	pa2m_afirmar(iterados == 9, "Se iteran los 9 pokemones por nombre");
	pa2m_afirmar(contador == 9,
		     "La funcion se invoca 9 veces (por nombre)");
	pokedex_destruir(pokedex);
}

void iteracion_se_corta_si_funcion_devuelve_false()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	int contador = 0;
	unsigned iterados = pokedex_iterar_pokemones(
		pokedex, ITERAR_ID, cortar_al_tercero, &contador);
	pa2m_afirmar(
		iterados == 3,
		"Se itera solo hasta el 3er Pokemon si la funcion lo corta");
	pa2m_afirmar(contador == 3, "La funcion se invoca 3 veces");
	pokedex_destruir(pokedex);
}

void iterar_con_pokedex_null_devuelve_cero()
{
	unsigned iterados =
		pokedex_iterar_pokemones(NULL, ITERAR_ID, contar_pokemon, NULL);
	pa2m_afirmar(iterados == 0, "Iterar con pokedex NULL devuelve 0");
}

void iterar_con_funcion_null_devuelve_cero()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	unsigned iterados =
		pokedex_iterar_pokemones(pokedex, ITERAR_ID, NULL, NULL);
	pa2m_afirmar(iterados == 0, "Iterar con funcion NULL devuelve 0");
	pokedex_destruir(pokedex);
}

void destruir_pokedex_valida_no_produce_errores()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	pokedex_destruir(pokedex);
	pa2m_afirmar(true, "Destruir una pokedex valida no produce errores");
}

void destruir_pokedex_null_no_produce_errores()
{
	pokedex_destruir(NULL);
	pa2m_afirmar(true, "Destruir una pokedex NULL no produce errores");
}

int main()
{
	pa2m_nuevo_grupo("Pruebas con archivo válido");
	pokedex_abrir_lee_todos_los_pokemon_y_devuelve_la_cantidad_correcta();
	//otras pruebas?????

	pa2m_nuevo_grupo("Pruebas con archivo invalido");
	pokedex_abrir_devuelve_null_si_el_archivo_no_existe();

	pa2m_nuevo_grupo("Pruebas de busqueda");
	pokedex_buscar_pokemon_nombre_devuelve_el_pokemon_cuando_existe();

	pa2m_nuevo_grupo("Pruebas de formato invalido");
	pokedex_deja_de_leer_ante_linea_invalida();
	pokedex_devuelve_null_si_no_hay_pokemones_validos();

	pa2m_nuevo_grupo("Pruebas de cantidad de pokemones");
	pokedex_cantidad_devuelve_0_con_pokedex_null();
	pokedex_cantidad_es_correcta_para_archivo_valido();
	pokedex_cantidad_con_malformado_es_correcta_hasta_linea_invalida();
	pa2m_nuevo_grupo("Pruebas de busqueda por nombre");

	buscar_pokemon_por_nombre_devuelve_puntero_si_existe();
	buscar_pokemon_por_nombre_devuelve_null_si_no_existe();
	buscar_pokemon_por_nombre_con_pokedex_null_devuelve_null();

	pa2m_nuevo_grupo("Pruebas de busqueda por ID");
	buscar_pokemon_por_id_devuelve_puntero_si_existe();
	buscar_pokemon_por_id_devuelve_null_si_no_existe();
	buscar_pokemon_por_id_con_pokedex_null_devuelve_null();

	pa2m_nuevo_grupo("Pruebas de iteracion de pokemones");
	iterar_todos_los_pokemon_por_id();
	iterar_todos_los_pokemon_por_nombre();
	iteracion_se_corta_si_funcion_devuelve_false();
	iterar_con_pokedex_null_devuelve_cero();
	iterar_con_funcion_null_devuelve_cero();

	pa2m_nuevo_grupo("Pruebas de destruccion");
	destruir_pokedex_valida_no_produce_errores();
	destruir_pokedex_null_no_produce_errores();

	return pa2m_mostrar_reporte();
}

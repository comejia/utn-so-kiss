#include "tlb.h"

static void reemplazar_entrada_tlb(t_tlb *entrada_a_agregar, char *algoritmo_reemplazo);
static void eliminar_entrada_tlb(t_tlb *entrada);
static void eliminar_pagina_swapeada(uint32_t marco);
static void print_tlb();

uint32_t indice_ultima_referencia = 0;


bool tlb_hit(uint32_t pagina) {

	bool existe_pagina(void *entrada) {
		return ((t_tlb *)entrada)->pagina == pagina;
	}
	return !list_is_empty(tlb) && list_any_satisfy(tlb, existe_pagina);
}

uint32_t tlb_marco_de_pagina(uint32_t pagina) {

	bool existe_pagina(void *entrada) {
		return ((t_tlb *)entrada)->pagina == pagina;
	}
	t_tlb *entrada = (t_tlb *)list_find(tlb, existe_pagina);
	entrada->ultima_referencia = indice_ultima_referencia;
	indice_ultima_referencia++;
	print_tlb();

	return entrada->marco;
}

void tlb_agregar_entrada(uint32_t pagina, uint32_t marco) {
	t_tlb *entrada = malloc(sizeof(t_tlb));
	entrada->pagina = pagina;
	entrada->marco = marco;
	entrada->ultima_referencia = indice_ultima_referencia;
	indice_ultima_referencia++;

	//eliminar_pagina_swapeada(marco); TODO: habilitar al implementar Memoria por completo
	if(list_size(tlb) < cpu_config->entradas_tlb) {
		list_add(tlb, entrada);
	} else {
		reemplazar_entrada_tlb(entrada, cpu_config->reemplazo_tlb);
	}
	print_tlb();
}

void tlb_limpiar_cache() {
	list_clean_and_destroy_elements(tlb, (void *)eliminar_entrada_tlb);
	indice_ultima_referencia = 0;
}


void reemplazar_entrada_tlb(t_tlb *entrada_a_agregar, char *algoritmo_reemplazo) {
	if(string_equals_ignore_case(algoritmo_reemplazo, "FIFO")) {
		list_remove_and_destroy_element(tlb, 0, (void *)eliminar_entrada_tlb);
		list_add(tlb, entrada_a_agregar);
	} else if(string_equals_ignore_case(algoritmo_reemplazo, "LRU")) {
		void *menos_usado(void *e1, void *e2) {
			return ((t_tlb *)e1)->ultima_referencia < ((t_tlb *)e2)->ultima_referencia ? e1 : e2;
		}
		t_tlb *aux = list_get_minimum(tlb, menos_usado);
		aux->pagina = entrada_a_agregar->pagina;
		aux->marco = entrada_a_agregar->marco;
		aux->ultima_referencia = entrada_a_agregar->ultima_referencia;
		eliminar_entrada_tlb(entrada_a_agregar);
	}
}

void eliminar_entrada_tlb(t_tlb *entrada) {
	free(entrada);
}

void eliminar_pagina_swapeada(uint32_t marco) {
	bool existe_marco(void *entrada) {
		return ((t_tlb *)entrada)->marco == marco;
	}
	list_remove_and_destroy_by_condition(tlb, (void *)existe_marco, (void *)eliminar_entrada_tlb);
}

void print_tlb() {
	void print(t_tlb *entrada) {
		log_info(cpu_logger, "TLB = [Page %d | Frame %d | t_uso %d]", entrada->pagina, entrada->marco, entrada->ultima_referencia);
	}
	list_iterate(tlb, (void *)print);
}


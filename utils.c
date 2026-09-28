//Stefan Alexandru Vladut 314CA
#include "utils.h"

//definirea functiei de crearea a unui dll
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t *list = malloc(sizeof(doubly_linked_list_t));
	DIE(!list, "Malloc failed()!\n");
	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

//cele 3 functii esentiale(adaugare, obtinere, eliminare) intr-un dll
dll_node_t *get_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	if (!list) {
		return NULL;
	}

	unsigned int len = list->size - 1;
	unsigned int i;
	dll_node_t *node = list->head;

	n = MIN(n, len);

	for (i = 0; i < n; ++i) {
		node = node->next;
	}

	return node;
}

void
ll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
				const void *new_data)
{
	dll_node_t *prev, *curr;
	dll_node_t *new_node;

	if (!list) {
		return;
	}

	if (n > list->size) {
		n = list->size;
	}
	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "new_node malloc");
	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "new_node->data malloc");
	memcpy(new_node->data, new_data, list->data_size);

	new_node->next = curr;
	new_node->prev = prev;
	if (!prev) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}
	if (curr) {
		curr->prev = new_node;
	}
	list->size++;
}

dll_node_t *
ll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	dll_node_t *prev, *curr;
	if (!list || !list->head) {
		return NULL;
	}

	if (n > list->size - 1) {
		n = list->size - 1;
	}

	curr = list->head;
	prev = NULL;

	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (!prev) {
		list->head = curr->next;
		if (list->head) {
			list->head->prev = NULL;
		}
	} else {
		prev->next = curr->next;
		if (curr->next) {
			curr->next->prev = prev;
		}
	}

	list->size--;
	return curr;
}

//functia de free pentru dll
void
dll_free(doubly_linked_list_t **pp_list)
{
	dll_node_t *new;
	new = (*pp_list)->head;
	for (unsigned int i = 0; i < (*pp_list)->size; i++) {
		dll_node_t *tmp = new;
		new = new->next;
		free(tmp->data);
		free(tmp);
	}
	free(*pp_list);
	*pp_list = NULL;
}

//definirea functiei de crearea a structurii de fisere
file_t *file_create(char *id, int score)
{
	file_t *file = malloc(sizeof(file_t));
	DIE(!file, "Malloc failed()!\n");
	file->id = malloc(strlen(id) + 1);
	DIE(!file->id, "Malloc failed()!\n");
	strcpy(file->id, id);
	file->score = score;
	file->capacity = 1;
	file->keywords = malloc(file->capacity * sizeof(char *));
	DIE(!file->keywords, "Malloc failed()!\n");
	file->n_keywords = 0;
	return file;
}

/*cele 4 functii esentiale pentru gestionarea sistemului de fisiere
(obtinere, verificare ca exista cheia, adaugare si stergere)*/
file_t *get_file(doubly_linked_list_t *files, char *id)
{
	if (!files) {
		return NULL;
	}
	dll_node_t *curr = files->head;
	while (curr) {
		file_t *file = *(file_t **)curr->data;
		if (strcmp(file->id, id) == 0) {
			return file;
		}
		curr = curr->next;
	}
	return NULL;
}

int file_has_key(file_t *file, char *keyword)
{
	if (!file)
		return -1;
	for (int i = 0; i < file->n_keywords; i++) {
		if (strcmp(file->keywords[i], keyword) == 0) {
			return i;
		}
	}
	return -1;
}

void file_add_key(file_t *file, char *keyword)
{
	if (!file || file_has_key(file, keyword) != -1)
		return;
	if (file->n_keywords == file->capacity) {
		file->capacity *= 2;
		char **aux = realloc(file->keywords, file->capacity * sizeof(char *));
		DIE(!aux, "Malloc failed()!\n");
		file->keywords = aux;
	}
	file->keywords[file->n_keywords] = malloc(strlen(keyword) + 1);
	DIE(!file->keywords[file->n_keywords], "Malloc failed()!\n");
	strcpy(file->keywords[file->n_keywords], keyword);
	file->n_keywords++;
}

void file_remove_key(file_t *file, char *keyword)
{
	if (!file || file_has_key(file, keyword) == -1)
		return;
	int poz = file_has_key(file, keyword);
	free(file->keywords[poz]);
	for (int i = poz; i < file->n_keywords - 1; i++) {
		file->keywords[i] = file->keywords[i + 1];
	}
	file->n_keywords--;
}

//functia de free pentru file
void file_free(file_t *file)
{
	if (!file)
		return;
	free(file->id);
	for (int i = 0; i < file->n_keywords; i++) {
		free(file->keywords[i]);
	}
	free(file->keywords);
	free(file);
	file = NULL;
}

//definirea functiilor de declarare a unui Trie si a nodului asociat
trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *new = malloc(sizeof(trie_node_t));
	DIE(!new, "Malloc failed()!\n");
	new->n_children = 0;
	new->end_of_word = 0;
	new->references = NULL;
	new->children = calloc(trie->alphabet_size, sizeof(*new->children));
	DIE(!new->children, "Malloc failed()!\n");
	trie->nNodes++;
	return new;
}

trie_t *trie_create(int data_size, int alphabet_size,
					char *alphabet, void (*free_value_cb)(void *data))
{
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, "Malloc failed()!\n");
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;
	trie->alphabet = malloc((alphabet_size + 1));
	DIE(!trie->alphabet, "Malloc failed()!\n");
	strcpy(trie->alphabet, alphabet);
	trie->free_value_cb = free_value_cb;
	trie->size = 0;
	trie->nNodes = 0;
	trie->root = trie_create_node(trie);
	DIE(!trie->root, "Malloc failed()!\n");
	return trie;
}

/*functia de insert a unui cuvant in arbore alaturi de fct helper care ajuta la
inserarea recursiva*/
void trie_insert_helper(trie_t *trie, trie_node_t *curr, char *key, void *value)
{
	if (*key == '\0') {
		if (curr->end_of_word == 1) {
			if (curr->references) {
				trie->free_value_cb(curr->references);
			}
		} else {
			curr->end_of_word = 1;
			trie->size++;
		}
		curr->references = value;
		return;
	}
	int idx = key[0] - 'a';
	if (idx < 0 || idx >= trie->alphabet_size)
		return;
	if (!curr->children[idx]) {
		curr->children[idx] = trie_create_node(trie);
		curr->n_children++;
	}
	trie_insert_helper(trie, curr->children[idx], key + 1, value);
}

void trie_insert(trie_t *trie, char *key, void *value)
{
	if (!trie || !key) {
		return;
	}
	trie_insert_helper(trie, trie->root, key, value);
}

/*functia de cautare a unui cuvant cheie in arbore alaturi de fct helper care ajuta
la cautarea recursiva a cuvantului respectiv*/
void *trie_search_helper(trie_t *trie, trie_node_t *curr, char *key)
{
	if (!curr) {
		return NULL;
	}
	if (*key == '\0') {
		if (curr->end_of_word == 1) {
			return curr->references;
		} else
			return NULL;
	} else {
		int idx = key[0] - 'a';
		if (idx < 0 || idx >= trie->alphabet_size)
			return NULL;
		return trie_search_helper(trie, curr->children[idx], key + 1);
	}
}

void *trie_search(trie_t *trie, char *key)
{
	if (!trie || !key) {
		return NULL;
	}
	return trie_search_helper(trie, trie->root, key);
}

/* functia de free alaturi de helper astfel incat sa fie eliberat atat cuvantul cheie,
cat si referinta catre fisiere*/
void trie_free_helper(trie_t *trie, trie_node_t *curr)
{
	if (!curr) {
		return;
	}
	for (int i = 0; i < trie->alphabet_size; i++) {
		trie_free_helper(trie, curr->children[i]);
	}
	if (curr->references) {
		trie->free_value_cb(curr->references);
	}
	free(curr->children);
	free(curr);
	trie->nNodes--;
}

void trie_free(trie_t **pTrie)
{
	if (!pTrie || !(*pTrie))
		return;
	trie_free_helper(*pTrie, (*pTrie)->root);
	free((*pTrie)->alphabet);
	free(*pTrie);
	*pTrie = NULL;
}


/* functia de remove impreuna cu fct helper care ajuta la stergerea recursiva
a tuturor aparitiilor cuvantului cheie */
int trie_remove_helper(trie_t *trie, trie_node_t *curr, char *key)
{
	if (!curr)
		return 0;
	if (*key == '\0') {
		if (curr->end_of_word == 0) {
			return 0;
		}
		curr->end_of_word = 0;
		trie->size--;
		if (curr->references) {
			trie->free_value_cb(curr->references);
			curr->references = NULL;
		}
		return curr->n_children == 0;
	}
	int idx = key[0] - 'a';
	if (idx < 0 || idx >= trie->alphabet_size)
		return 0;
	if (!curr->children[idx])
		return 0;
	int removed_node = trie_remove_helper(trie,
										  curr->children[idx], key + 1);
	if (removed_node) {
		trie_free_helper(trie, curr->children[idx]);
		curr->children[idx] = NULL;
		curr->n_children--;
	}
	if (curr->end_of_word == 1) {
		return 0;
	}
	return curr->n_children == 0;
}

void trie_remove(trie_t *trie, char *key)
{
	if (!trie || !key) {
		return;
	}
	trie_remove_helper(trie, trie->root, key);
}

//functia de free pentru referintele din frunzele trie-ului.
void free_references(void *data)
{
	if (!data)
		return;
	doubly_linked_list_t *references = (doubly_linked_list_t *)data;
	dll_free(&references);
}

//definirea functiei pentru crearea unui heap
heap_t *heap_create(int capacity)
{
	heap_t *heap = malloc(sizeof(heap_t));
	DIE(!heap, "Malloc failed()!\n");
	heap->capacity = capacity;
	heap->size = 0;
	heap->arr = malloc(heap->capacity * sizeof(file_t *));
	DIE(!heap->arr, "Malloc failed()!\n");
	return heap;
}

/*functie de la curs ajutatoare pt a restabili proprietatea de max-heap
prin urcarea elementului de la pos*/
void pushUp(heap_t *heap, int pos)
{
	int parent;
	file_t *vaux;
	parent = (pos - 1) / 2;
	//daca nu e top of heap si H[parent] si H[pos] nu respecta proprietatea
	//de heap le interschimbam
	while (pos > 0 && ((heap->arr[pos]->score > heap->arr[parent]->score) ||
		   (heap->arr[pos]->score == heap->arr[parent]->score && strcmp(heap->arr[pos]->id, heap->arr[parent]->id) < 0))) {
		vaux = heap->arr[parent];
		heap->arr[parent] = heap->arr[pos];
		heap->arr[pos] = vaux;

		pos = parent;
		parent = (pos - 1) / 2;
	}
}
// cele 2 functii clasice de heap_insert si heap_top implementate ca la laborator
void heap_insert(heap_t *heap, file_t *file)
{
	if (!heap || !file)
		return;
	if (heap->size == heap->capacity) {
		heap->capacity *= 2;
		file_t **aux = realloc(heap->arr, heap->capacity * sizeof(file_t *));
		DIE(!aux, "Malloc failed()!\n");
		heap->arr = aux;
	}
	heap->arr[heap->size] = file;
	heap->size++;
	pushUp(heap, heap->size - 1);
}

file_t *heap_top(heap_t *heap)
{
	if (!heap || heap->size == 0)
		return NULL;
	else
		return heap->arr[0];
}

/*functie de la curs ajutatoare pentru a restabili proprietatea de max-heap
 prin coborarea elementului radacina*/
void pushDown(heap_t *heap)
{
	int pos = 0;
	file_t *vaux;

	while (1) {
		if (2 * pos + 2 >= heap->size) {
			if (2 * pos + 1 >= heap->size) {
				//daca nu avem nici fiu drept nici fiu stang break
				break;
			/*daca nu avem fiu stang si nu e respectata proprietatea
			de heap interschimbam*/
			} else if (heap->arr[2 * pos + 1]->score > heap->arr[pos]->score ||
					   (heap->arr[2 * pos + 1]->score == heap->arr[pos]->score && strcmp(heap->arr[2 * pos + 1]->id, heap->arr[pos]->id) < 0)) {
				vaux = heap->arr[2 * pos + 1];
				heap->arr[2 * pos + 1] = heap->arr[pos];
				heap->arr[pos] = vaux;
				pos = 2 * pos + 1;
			} else
				break; //avem fiu stang si e heap order
		//avem atat fiu stang, cat si fiu drept
		} else {
			//daca fiul stang e mai mic si nu e heap order
			if (((heap->arr[2 * pos + 1]->score > heap->arr[2 * pos + 2]->score) ||
				  (heap->arr[2 * pos + 1]->score == heap->arr[2 * pos + 2]->score && strcmp(heap->arr[2 * pos + 1]->id, heap->arr[2 * pos + 2]->id) <= 0))
				  &&
				  ((heap->arr[2 * pos + 1]->score > heap->arr[pos]->score) ||
				   (heap->arr[2 * pos + 1]->score == heap->arr[pos]->score && strcmp(heap->arr[2 * pos + 1]->id, heap->arr[pos]->id) <= 0))) {
				vaux = heap->arr[2 * pos + 1];
				heap->arr[2 * pos + 1] = heap->arr[pos];
				heap->arr[pos] = vaux;
				pos = 2 * pos + 1;
			//daca fiul drept e mai mic si nu e heap order
			} else if (((heap->arr[2 * pos + 2]->score > heap->arr[2 * pos + 1]->score) ||
						(heap->arr[2 * pos + 2]->score == heap->arr[2 * pos + 1]->score && strcmp(heap->arr[2 * pos + 2]->id, heap->arr[2 * pos + 1]->id) <= 0))
						&&
						((heap->arr[2 * pos + 2]->score > heap->arr[pos]->score) ||
						 (heap->arr[2 * pos + 2]->score == heap->arr[pos]->score && strcmp(heap->arr[2 * pos + 2]->id, heap->arr[pos]->id) < 0))) {
				vaux = heap->arr[2 * pos + 2];
				heap->arr[2 * pos + 2] = heap->arr[pos];
				heap->arr[pos] = vaux;
				pos = 2 * pos + 2;
			} else
				break; //toate nodurile in heap order
		}
	}
}
// functia esentiala de heap_pop de la laborator + cea de heap_empty pt a verifica daca heap-ul e gol
void heap_pop(heap_t *heap)
{
	if (!heap || heap->size == 0) {
		return;
	}
	heap->arr[0] = heap->arr[heap->size - 1];
	heap->size--;
	if (heap->size > 0) {
		pushDown(heap);
	}
}

int heap_empty(heap_t *heap)
{
	if (!heap || heap->size == 0)
		return 1;
	else
		return 0;
}

//dealocarea heap-ului si a cuvintelor pe care le retine
void heap_free(heap_t *heap)
{
	if (!heap)
		return;
	free(heap->arr);
	free(heap);
}

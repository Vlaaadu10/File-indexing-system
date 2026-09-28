//Stefan Alexandru Vladut 314CA
#ifndef UTILS_H
#define UTILS_H
#include "declarations.h"

//declararea functiilor esentiale de la laborator pentru dll
doubly_linked_list_t *dll_create(unsigned int data_size);
dll_node_t *get_nth_node(doubly_linked_list_t *list, unsigned int n);
void ll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
					 const void *new_data);
dll_node_t *ll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);
void dll_free(doubly_linked_list_t **pp_list);

//declararea functiilor esentiale pentru organizarea fisierelor
file_t *file_create(char *id, int score);
//cauta un fisier dupa id in lista de fisiere si il returneaza
file_t *get_file(doubly_linked_list_t *files, char *id);
//verifica daca un cuvant cheie este deja continut intr-un fisier
int file_has_key(file_t *file, char *keyword);
//adauga un cuvant cheie pentru un fisier anume
void file_add_key(file_t *file, char *keyword);
//sterge un cuvant cheie pentru un fisier anume
void file_remove_key(file_t *file, char *keyword);
//dealoca memoria unui fisier si a cuvintelor pe care le contine
void file_free(file_t *file);

//declararea functiilor esentiale de la laborator pentru Trie
trie_node_t *trie_create_node(trie_t *trie);
trie_t *trie_create(int data_size, int alphabet_size, char *alphabet,
					void (*free_value_cb)(void *data));
/*functia de inserare a unei referinte catre un fisier impreuna cu fct helper
care insereaza cuvantul recursiv*/
void trie_insert_helper(trie_t *trie, trie_node_t *curr, char *key,
						void *value);
void trie_insert(trie_t *trie, char *key, void *value);
/*functia de cautare a unui cuvant in arbore si returnare a listei de fisiere asociate
impreuna cu fct helper care cauta recursiv cuvantul*/
void *trie_search_helper(trie_t *trie, trie_node_t *curr, char *key);
void *trie_search(trie_t *trie, char *key);
/*functia de dealocare impreuna cu fct helper care da free recursiv toate aparitiile
cuvantului cheie*/
void trie_free_helper(trie_t *trie, trie_node_t *curr);
void trie_free(trie_t **pTrie);
/*functia de stergere a unui cuvant cheie alaturi de fct helper care sterge recursiv
toate aparitiile*/
int trie_remove_helper(trie_t *trie, trie_node_t *curr, char *key);
void trie_remove(trie_t *trie, char *key);
//functie de dealocare a memoriei pentru referinte
void free_references(void *data);

//declararea functiilor esentiale de la laborator pentru Heap
heap_t *heap_create(int capacity);
void heap_insert(heap_t *heap, file_t *file);
file_t *heap_top(heap_t *heap);
void heap_pop(heap_t *heap);
int heap_empty(heap_t *heap);
void heap_free(heap_t *heap);
#endif

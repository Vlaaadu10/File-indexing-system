//Stefan Alexandru Vladut 314CA
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/*Macro-uri utile pentru dimensiunea alfabetului urmat de insiruirea acestuia,
aflarea minimului, respectiv cazul in care malloc nu reuseste*/
#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define MAX_LEN 101
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

/*lista de fisiere va fi reprezentata ca un dll in care fiecare nod are
urmatoarea structura*/
typedef struct dll_node_t {
	void *data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
					pointer la void. */
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t {
	dll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} doubly_linked_list_t;

//caracteristicile unui fisier
typedef struct file_t {
	char *id;
	int score;
	char **keywords;
	int n_keywords;
	int capacity;
} file_t;

//pentru arborele de chei care este reprezentat sub forma unui trie
typedef struct trie_node_t trie_node_t;
struct trie_node_t {
/*references va fi referinta (doubly_linked_list_t* cu referinta catre file_t)
pt noduri terminale, altfel NULL*/
	void *references;
	int end_of_word;
	trie_node_t **children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;
	int size;
	int data_size;
	int alphabet_size;
	char *alphabet;
	void (*free_value_cb)(void *data);
	int nNodes;
};

/*Heap modificat special pentru task-ul topk,
intrucat vor fi stocate doar referintele la fisiere*/
typedef struct {
	file_t **arr;
	int size;
	int capacity;
} heap_t;
#endif

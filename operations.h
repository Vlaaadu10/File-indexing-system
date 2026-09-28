//Stefan Alexandru Vladut 314CA
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "utils.h"

//declararea celor 7 operatii + cea de la task-ul prefix
void add(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		 char *id, int score, int n_keywords, char **keywords);

void del(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		 char *id);

void addkw(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		   char *id, char *keyword);

void delkw(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		   char *id, char *keyword);

void find(trie_t *trie, FILE *out, char *keyword);

void topk(trie_t *trie, FILE *out, char *keyword, int k);

void print(trie_t *trie, FILE *out);

void prefix(doubly_linked_list_t *files, trie_t *trie, FILE *out,
			char *prefix);
#endif

//Stefan Alexandru Vladut 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"

/*aici se regaseste centrul de comanda al programului de unde in functie de
comanda aleasa se alege ce operatie se efectueaza*/
int main(void)
{
	doubly_linked_list_t *files = dll_create(sizeof(file_t *));
	trie_t *trie = trie_create(sizeof(doubly_linked_list_t *),
							   ALPHABET_SIZE, ALPHABET, free_references);
	FILE *in = fopen("indexare.in", "r");
	DIE(!in, "Couldn't open input file");
	FILE *out = fopen("indexare.out", "w");
	DIE(!out, "Couldn't open output file");
	int Q;
	fscanf(in, "%d", &Q);
	for (int i = 0; i < Q; i++) {
		char comanda[MAX_LEN];
		fscanf(in, "%s", comanda);
		if (strcmp(comanda, "ADD") == 0) {
			char id[MAX_LEN];
			int score, n_keywords;
			fscanf(in, "%s %d %d", id, &score, &n_keywords);
			char **keywords = malloc(n_keywords * sizeof(char *));
			DIE(!keywords, "Malloc failed()!\n");
			for (int j = 0 ; j < n_keywords; j++) {
				char cuv[MAX_LEN];
				fscanf(in, "%s", cuv);
				keywords[j] = malloc(strlen(cuv) + 1);
				DIE(!keywords[j], "Malloc failed()!\n");
				strcpy(keywords[j], cuv);
			}
			add(files, trie, out, id, score, n_keywords, keywords);
			for (int j = 0; j < n_keywords; j++) {
				free(keywords[j]);
			}
			free(keywords);
		} else if (strcmp(comanda, "DEL") == 0) {
			char id[MAX_LEN];
			fscanf(in, "%s", id);
			del(files, trie, out, id);
		} else if (strcmp(comanda, "ADDKW") == 0) {
			char id[MAX_LEN];
			char keyword[MAX_LEN];
			fscanf(in, "%s %s", id, keyword);
			addkw(files, trie, out, id, keyword);
		} else if (strcmp(comanda, "DELKW") == 0) {
			char id[MAX_LEN];
			char keyword[MAX_LEN];
			fscanf(in, "%s %s", id, keyword);
			delkw(files, trie, out, id, keyword);
		} else if (strcmp(comanda, "FIND") == 0) {
			char keyword[MAX_LEN];
			fscanf(in, "%s", keyword);
			find(trie, out, keyword);
		} else if (strcmp(comanda, "TOPK") == 0) {
			int k;
			char keyword[MAX_LEN];
			fscanf(in, "%s %d", keyword, &k);
			topk(trie, out, keyword, k);
		} else if (strcmp(comanda, "PRINT") == 0) {
			print(trie, out);
		} else if (strcmp(comanda, "PREFIX") == 0) {
			char cuv[MAX_LEN];
			fscanf(in, "%s", cuv);
			prefix(files, trie, out, cuv);
		} else
			break;
	}
	//dealocarea memoriei pentru a evita memory leaks + inchidere fisiere
	trie_free(&trie);
	dll_node_t *curr = files->head;
	while (curr) {
		file_t *file = *(file_t **)curr->data;
		file_free(file);
		curr = curr->next;
	}
	dll_free(&files);
	fclose(in);
	fclose(out);
	return 0;
}

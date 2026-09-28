//Stefan Alexandru Vladut 314CA
#include "operations.h"

/* functia de adaugare a unui fisier nou si indexare a cuvintelor cheie in arbore*/
void add(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		 char *id, int score, int n_keywords, char **keywords)
{
	if (get_file(files, id)) {
		fprintf(out, "EXISTS\n");
		return;
	}
	file_t *file = file_create(id, score);
	for (int i = 0; i < n_keywords; i++) {
		// evitam duplicatele
		if (file_has_key(file, keywords[i]) == -1) {
			file_add_key(file, keywords[i]);
			doubly_linked_list_t *references = (doubly_linked_list_t *)
												trie_search(trie, keywords[i]);
			//daca nu exista in trie cuvantul cream o lista noua de referinte
			if (!references) {
				references = dll_create(sizeof(file_t *));
				trie_insert(trie, keywords[i], references);
			}
			// adaugam fisierul in lista de referinte
			ll_add_nth_node(references, references->size, &file);
		}
	}
	//adaugam fisierul in lista de fisiere
	ll_add_nth_node(files, files->size, &file);
	fprintf(out, "OK\n");
}

/* functia de stergere a unui fisier si a referintelor din arbore */
void del(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		 char *id)
{
	file_t *file = get_file(files, id);
	if (!file) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	// stergem referintele pentru fiecare cuvant cheie
	for (int i = 0; i < file->n_keywords; i++) {
		doubly_linked_list_t *references = (doubly_linked_list_t *)
											trie_search(trie,
														file->keywords[i]);
		dll_node_t *curr = references->head;
		int ok = 0, pos = 0;
		//vedem la ce pozitie se afla fisierul in lista de referinte
		while (curr && ok == 0) {
			file_t *tmp = *(file_t **)curr->data;
			if (strcmp(tmp->id, id) == 0) {
				ok = 1;
				break;
			}
			pos++;
			curr = curr->next;
		}
		// dupa ce l-am gasit stergem nodul din lista de referinte
		if (ok == 1) {
			dll_node_t *removed = ll_remove_nth_node(references, pos);
			free(removed->data);
			free(removed);
		}
		//daca lista devine vida eliminam cuvantul din arbore
		if (references->size == 0) {
			trie_remove(trie, file->keywords[i]);
		}
	}
	dll_node_t *curr = files->head;
	int pos = 0, ok = 0;
	//vdedem unde se afla fisierul in lista de fisiere
	while (curr && ok == 0) {
		file_t *tmp = *(file_t **)curr->data;
		if (strcmp(tmp->id, id) == 0) {
			ok = 1;
			break;
		}
		pos++;
		curr = curr->next;
	}
	//dupa ce l-am gasit eliminam nodul din lista de fisiere
	if (ok == 1) {
		dll_node_t *removed = ll_remove_nth_node(files, pos);
		free(removed->data);
		free(removed);
	}
	//eliberam memoria ocupata de fisier
	file_free(file);
	fprintf(out, "OK\n");
}

/* functia de adaugare a unui cuvant cheie la un fisier existent */
void addkw(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		   char *id, char *keyword)
{
	file_t *file = get_file(files, id);
	// daca nu exista fisierul se afiseaza textul "NOT FOUND"
	if (!file) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	// daca fisierul detine deja cuvantul operatia e reusita
	if (file_has_key(file, keyword) != -1) {
		fprintf(out, "OK\n");
		return;
	}
	file_add_key(file, keyword);
	doubly_linked_list_t *references = (doubly_linked_list_t *)
										trie_search(trie, keyword);
	//cream referinta intre fisier si nodul din trie
	if (!references) {
		references = dll_create(sizeof(file_t *));
		trie_insert(trie, keyword, references);
	}
	ll_add_nth_node(references, references->size, &file);
	fprintf(out, "OK\n");
}

/* sterge un cuvant cheie dintr-un fisier existent */
void delkw(doubly_linked_list_t *files, trie_t *trie, FILE *out,
		   char *id, char *keyword)
{
	file_t *file = get_file(files, id);
	if (!file) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	//verificare daca fisierul are cuvantul cheie respectiv
	if (file_has_key(file, keyword) == -1) {
		fprintf(out, "OK\n");
		return;
	}
	//stergem referinta din trie
	file_remove_key(file, keyword);
	doubly_linked_list_t *references = (doubly_linked_list_t *)
										trie_search(trie, keyword);
	if (references) {
		dll_node_t *curr = references->head;
		int ok = 0, pos = 0;
		while (curr && ok == 0) {
			file_t *tmp = *(file_t **)curr->data;
			if (strcmp(tmp->id, id) == 0) {
				ok = 1;
				break;
			}
			pos++;
			curr = curr->next;
		}
		if (ok == 1) {
			dll_node_t *removed = ll_remove_nth_node(references, pos);
			free(removed->data);
			free(removed);
		}
		// daca nodul ramane fara referinte il stergem
		if (references->size == 0) {
			trie_remove(trie, keyword);
		}
	}
	// de asemenea, daca fisierul nu mai are niciun cuvant cheie il stergem
	if (file->n_keywords == 0) {
		dll_node_t *curr = files->head;
		int pos = 0, ok = 0;
		while (curr && ok == 0) {
			file_t *tmp = *(file_t **)curr->data;
			if (strcmp(tmp->id, id) == 0) {
				ok = 1;
				break;
			}
			pos++;
			curr = curr->next;
		}
		if (ok == 1) {
			dll_node_t *removed = ll_remove_nth_node(files, pos);
			free(removed->data);
			free(removed);
		}
		file_free(file);
	}
	fprintf(out, "OK\n");
}

// cauta toate fisierele care contin un anumit cuvant cheie
void find(trie_t *trie, FILE *out, char *keyword)
{
	doubly_linked_list_t *references = (doubly_linked_list_t *)
										trie_search(trie, keyword);
	if (!references || references->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	//punem toate fisierele intr-un vector dinamic
	file_t **v = malloc(references->size * sizeof(file_t *));
	DIE(!v, "Malloc failed()!\n");
	dll_node_t *curr = references->head;
	int k = 0;
	while (curr) {
		v[k++] = *(file_t **)curr->data;
		curr = curr->next;
	}
	// sortam lexicografic
	for (int i = 0; i < k - 1; i++) {
		for (int j = i + 1; j < k; j++) {
			if (strcmp(v[i]->id, v[j]->id) > 0) {
				file_t *aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
		}
	}
	fprintf(out, "%d", references->size);
	for (int i = 0; i < k; i++) {
		fprintf(out, " %s", v[i]->id);
	}
	free(v);
	fprintf(out, "\n");
}

/* functia de afisare a primelor k fisiere relevante pentru un anumit cuvant cheie */
void topk(trie_t *trie, FILE *out, char *keyword, int k)
{
	doubly_linked_list_t *references = (doubly_linked_list_t *)
										trie_search(trie, keyword);
	if (!references || references->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	// le stocam sub forma unui max-heap
	heap_t *heap = heap_create(references->size);
	dll_node_t *curr = references->head;
	while (curr) {
		file_t *file = *(file_t **)curr->data;
		heap_insert(heap, file);
		curr = curr->next;
	}
	if (k > (int)references->size) {
		k = (int)references->size;
	}
	// afisam primele k elemente
	fprintf(out, "%d", k);
	for (int i = 0; i < k; i++) {
		file_t *file = (file_t *)heap_top(heap);
		fprintf(out, " %s", file->id);
		heap_pop(heap);
	}
	fprintf(out, "\n");
	heap_free(heap);
}

/* fct helper pt print pentru a afisa recursiv toate fisierele si cuvintele cheie asociate*/
void print_helper(trie_node_t *node, char *cuv, FILE *out, int level)
{
	if (!node) {
		return;
	}
	// finalul unui cuvant marcheaza ca trebuie sa fie afisat
	if (node->end_of_word == 1 && node->references) {
		cuv[level] = '\0';
		doubly_linked_list_t *references = (doubly_linked_list_t *)
											node->references;
		file_t **v = malloc(references->size * sizeof(file_t *));
		DIE(!v, "Malloc failed()!\n");
		dll_node_t *curr = references->head;
		int k = 0;
		while (curr) {
			v[k++] = *(file_t **)curr->data;
			curr = curr->next;
		}
		// sortam lexicografic
		for (int i = 0; i < k - 1; i++) {
			for (int j = i + 1; j < k ; j++) {
				if (strcmp(v[i]->id, v[j]->id) > 0) {
					file_t *aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
		}
		// afisam cuvantul si fisierele in care apare
		fprintf(out, "%s %d", cuv, k);
		for (int i = 0; i < k; i++) {
			fprintf(out, " %s", v[i]->id);
		}
		fprintf(out, "\n");
		free(v);
	}
	// DFS propriu zis
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i]) {
			cuv[level] = 'a' + i;
			print_helper(node->children[i], cuv, out, level + 1);
		}
	}
}

void print(trie_t *trie, FILE *out)
{
	if (!trie || !trie->root || trie->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	char cuv[MAX_LEN];
	print_helper(trie->root, cuv, out, 0);
}

//de data asta facem DFS din nodul care reprezinta ultimul caracter al prefixului
void prefix_helper(trie_node_t *node, file_t **v, int *cnt)
{
	if (!node)
		return;
	// daca marcheaza finalul unui cuvant il afisam
	if (node->references && node->end_of_word == 1) {
		doubly_linked_list_t *references = (doubly_linked_list_t *)
											node->references;
		dll_node_t *curr = references->head;
		while (curr) {
			file_t *file = *(file_t **)curr->data;
			int ok = 0;
			// verificam daca exista deja acest fisier
			for (int i = 0; i < *cnt; i++) {
				if (v[i] == file) {
					ok = 1;
				}
			}
			// daca e unic il adaugam in vectorul de rezultate
			if (ok == 0) {
				v[(*cnt)++] = file;
			}
			curr = curr->next;
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i]) {
			prefix_helper(node->children[i], v, cnt);
		}
	}
}

/* afiseaza fisierele care contin cuvinte ce contin prefixul dat */
void prefix(doubly_linked_list_t *files, trie_t *trie, FILE *out, char *prefix)
{
	if (!trie || !trie->root) {
		fprintf(out, "EMPTY\n");
		return;
	}
	// coboram in trie pana la ultimul caracter al prefixului de unde vom incepe DFS
	trie_node_t *curr = trie->root;
	for (int i = 0; i < (int)strlen(prefix); i++) {
		int pos = prefix[i] - 'a';
		if (!curr->children[pos]) {
			fprintf(out, "EMPTY\n");
			return;
		}
		curr = curr->children[pos];
	}
	/*de la nodul de unde incepe prefixul stocam intr-un vector dinamic
	fisierele*/
	file_t **v = malloc(files->size * sizeof(file_t *));
	DIE(!v, "Malloc failed()!\n");
	int cnt = 0;
	prefix_helper(curr, v, &cnt);
	if (cnt == 0) {
		fprintf(out, "EMPTY\n");
		free(v);
		return;
	}
	// sortam lexicografic
	for (int i = 0; i < cnt - 1; i++) {
		for (int j = i + 1; j < cnt; j++) {
			if (strcmp(v[i]->id, v[j]->id) > 0) {
				file_t *aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
		}
	}
	// afisam numarul total si id-urile
	fprintf(out, "%d", cnt);
	for (int i = 0; i < cnt; i++) {
		fprintf(out, " %s", v[i]->id);
	}
	fprintf(out, "\n");
	free(v);
}

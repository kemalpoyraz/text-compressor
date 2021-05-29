#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
	char* word;
	int repetition;
	int id;
	struct node* link;
	struct node* left;
	struct node* right;
}NODE;

typedef struct{
	NODE* head;
	NODE* search;
	int count;
}LIST;

typedef struct{
	NODE* root;
	NODE* search;
	int count;
}TREE;

NODE* createNode ();
LIST* createList ();
LIST* destroyList (LIST* list);
bool _addNode (LIST* list, NODE** position, char* getWord, int (*_compare_string)(NODE*, NODE*));
int _compare_string (NODE* a, NODE* b);
bool _replaceWord (LIST* list, char* string);
LIST* _insert ();
void insertWord (LIST* list, char* string);
bool _delete (LIST* list, char* keyPtr, NODE* pPos, NODE* pPre, NODE* pPost);
void deleteWord (LIST* list, char* keyString);
NODE* searchWord (LIST* list, char* string);
void printListToFile (LIST* list);

TREE* createTree( int (*compareID) (int a, int b));
bool treeInsert (TREE* tree, NODE* dataIn);
NODE* _treeInsert (TREE* tree, NODE* root, NODE* dataIn);
int compareID (int a, int b);
TREE* destroyTree (TREE* tree);
void compress (LIST* list);
void decompress (LIST* list);

int main(){
	LIST* list;
	char* string;
	int choice;
	
	list = _insert ();
	if (list){
		do{
			printf ("\n\n");
			printf ("Please enter 1 to insert a word to the text file.\n"
					"Please enter 2 to delete a word from the text file.\n"
					"Please enter 3 to search for a word from the text file.\n"
					"Please enter 4 to print dictionary to the text file.\n"
					"Please enter 5 to compress the text file.\n"
					"Please enter 6 to decompress the compressed text file.\n"
					"Enter 0 to exit.\n"
					"\n\nYour choice:");
			scanf ("%d", &choice);
			
			string = (char *)malloc((100 * sizeof(char)));
			if (choice == 1){
				printf ("\nPlease enter a word to insert to the text file: ");
				scanf ("%s", string);
				insertWord (list, string);
				printf ("\n");
			}
			else if (choice == 2){
				
				printf ("\nPlease enter a word to delete from the text file: ");
				scanf ("%s", string);
				deleteWord (list, string);
				printf("\n");
			}
			else if (choice == 3){
				printf ("\nPlease enter a word to search for: ");
				scanf ("%s", string);
				searchWord (list, string);
				printf("\n");
			}
			else if (choice == 4){
				printListToFile (list);
				printf("\n");
			}
			else if (choice == 5){
				compress (list);
				printf("\n");
			}
			else if (choice == 6){
				decompress (list);
				printf("\n");
			}
			else if (choice != 0){
				printf ("\nInvalid option. Try again.\n\n");
			}
  		}while (choice != 0 && choice);
  		free (list);
  	}
	system ("pause");
	return 0;
}

NODE* createNode (){
	NODE* node;
	node = (NODE*)malloc(sizeof(NODE));
	
	if (node == NULL){
		return NULL;
	}
	else{
		node->word = NULL;
		node->repetition = 1;
		node->link = NULL;
		node->left = NULL;
		node->right = NULL;
		return node;
	}
}

LIST* createList (){
	LIST* list;
	list = (LIST*)malloc(sizeof(LIST));
	
	if (list == NULL){
		return NULL;
	}
	else{
		list->head = NULL;
		list-> search = NULL;
		list->count = 0;
		return list;
	}
}

LIST* destroyList (LIST* pList){
	NODE* del;
	
	if (pList){
		while (pList->count > 0){
			free (pList->head->word);
			del = pList->head;
			pList->count --;
			free (del);
		}
		free (pList);
	}
	return NULL;
}

int _compare_string (NODE* a, NODE* b){
	
	return stricmp (a->word, b->word);
}

int q = 1;
bool _addNode (LIST* list, NODE** position, char* getWord, int (_compare_string)(NODE*, NODE*)){
	NODE* newNode;
	newNode = createNode ();
	if (strcmp(getWord, "") == 0){
		return false;
	}
	if (newNode == NULL){
		printf ("\nError: Memory Overflow.\n");
		return false;
	}
	else{	
		newNode->word = (char*)malloc(sizeof(char)* (strlen (getWord)+1));
		strcpy (newNode->word, getWord);
		newNode->id = q;
		q++;
		while (*position != NULL){
			if (_compare_string (*position, newNode) > 0){
				break;
			}
			else{
				position = & (* position)->link;
			}
		}
		newNode->link = *position;
		*position = newNode;
		return true;
	}
}

bool _replaceWord (LIST* list, char* string){

	NODE* check = list->head;;
	bool flag = true;
	
	strlwr(string);
	char* delimiter = strtok (string, " ,. ...:;- --_#$%&|><{}_+*?=!'/(()\\¨\"1234567890");
	while (delimiter != NULL){
		while (check != NULL){
			if (!stricmp (delimiter, check->word)){
				check->repetition++;
				check = check->link;
				flag = false;
			}
			else{
				check = check->link;
			}
		}
		if (flag){
			_addNode (list, &list->head, delimiter, _compare_string);
			list->count++;
		}
		delimiter = strtok (NULL, " ,. ...:;- --_#$%&|><{}_+*?=!'/(()\\¨\"1234567890");
	}
	if (string[0] >= '0' && string[0] <= '9' || string[strlen (string) - 1] >= '0' && string[strlen (string) -1] <= '9'){
		return false;
	}
	else{
		return true;
	}
}

LIST* _insert (){
	LIST* list;
	list = createList();
	FILE* fp;
	char* getWord;
	
	if (list != NULL){
		fp = fopen ("text.txt", "r");
		if (fp == NULL){
			printf ("\nThe text file does not exist.\n");
			return NULL;
		}
		else{
			getWord = (char*)malloc((100 * sizeof(char)));
			fscanf (fp, "%s", getWord);
			_replaceWord (list, getWord);
			list->count++;
			while (!feof (fp)){
				getWord = (char*)malloc((100 * sizeof(char)));
				fscanf (fp, "%s", getWord);
				_replaceWord (list, getWord);
			}	
			free (getWord);
			fclose (fp);
			return list;
		}
	}
}

void insertWord (LIST* list, char* string){
	
	bool check = _replaceWord (list, string);
	if (check){
		printf("\nThe word %s inserted to the text.\n", string);
	}
	else{
		printf("\nError: The word %s could not be inserted.\n", string);
	}
}

bool _delete (LIST* list, char* keyPtr, NODE* pPos, NODE* pPre, NODE* pPost){
	if (pPos == NULL || list->count <= 0){
		printf("\nError: The list has no elements.\n");
		return false;
	}
	else if (list->count == 1){
		if (stricmp (pPos->word, keyPtr)){
			destroyList(list);
			return true;
		}
		else{
			return false;
		}
	}
	else{
		bool flag = false;
		if (!stricmp (pPos->word, keyPtr)){
			pPre = pPos;
			pPos = pPost;
			pPost = pPost->link;
			flag = true;
		}
		else{
			while (pPos->link != NULL){
				if (!stricmp (pPos->word, keyPtr)){
					pPre->link = pPost;
					flag = true;
					break;
				}
				else{
					pPre = pPos;
					pPos = pPost;
					pPost = pPost->link;
				}
			}
			if (!stricmp (pPos->word, keyPtr)){
				pPre->link = pPost;
				flag = true;
			}
		}
		if (flag){
			return true;
		}
		else{
			return false;
		}
	}
}

void deleteWord (LIST* list, char* keyString){
	NODE* pPos, *pPre, *pPost;
	pPos = createNode ();
	pPre = createNode ();
	pPost = createNode ();
	bool check = true;
	
	pPos = list->head;
	pPre = NULL;
	pPost = list->head->link;
	check = _delete (list, keyString, pPos, pPre, pPost);
	
	if (check){
		printf("\nThe word %s is deleted from the text.\n", keyString);
	}
	else{
		printf("\nError: The word %s could not be deleted.\n", keyString);
	}
}

NODE* searchWord (LIST* list, char* keyString){
	bool flag = false;
	list->search = list->head;
	
	if (list != NULL){
		while (list->search != NULL){
			if (! stricmp (list->search->word, keyString)){
				flag = true;
				break;
			}
			else{
				list->search = list->search->link;
			}
		}
		if (flag){
			printf ("\nThe word %s is in the text.\n", keyString);
			return list->search;
		}
		else{
			printf ("\nThe word %s is not found.\n", keyString);
		}
	}
	else{
		printf ("\nError: The list is unreachable.\n");
	}
}

void printListToFile (LIST* list){
	FILE* fp;
	fp = fopen ("Dictionary.txt", "w+");
	list->search = list->head;
	
	while (list->search){
		fprintf (fp, "ID: %d, %s\n", list->search->id, list->search->word);
		fflush (fp);
		list->search = list->search->link;
	}
	printf ("\nThe list of words in the text is printed to Dictionary.txt\n");
}

TREE* createTree( int (*compareID) (int a, int b)){
	TREE* tree;
	tree = (TREE*)malloc (sizeof (TREE));
	
	if (tree){
		tree->root = NULL;
		tree->count = 0;
	}
	return tree;
}

bool treeInsert (TREE* tree, NODE* dataIn){
	if (dataIn == NULL){
		return false;
	}
	else{
		if (tree->count == 0){
			tree->root = dataIn;
			tree->count++;
		}
		else{
			_treeInsert (tree, tree->root, dataIn);
			tree->count++;
		}
		return true;
	}
}

NODE* _treeInsert (TREE* tree, NODE* root, NODE* dataIn){
	if (!root){
		return dataIn;
	}
	else if (_compare_string (dataIn, root) < 0){
		root->left = _treeInsert (tree, root->left, dataIn);
		return root;
	}
	else if (_compare_string (dataIn, root) > 0){
		root->right = _treeInsert (tree, root->right, dataIn);
		return root;
	}
	else{
		root->right = _treeInsert (tree, root->right, dataIn);
		return root;
	}
}

int compareID (int a, int b){
	if (a > b){
		return 1;
	}
	else if (a < b){
		return -1;
	}
	else{
		return 0;
	}
}

void _print (NODE* root){
	if (root == NULL){
		return;
	}
	else{
		_print (root->left);
		printf ("%s\n", root->word);
		_print (root->right);
	}
}

void printTree (TREE* tree){
	_print (tree->root);
}
void compress (LIST* list){
	if (list){
		TREE* tree;
		tree = createTree (compareID);
		
		if (tree){
			list->search = list->head;
			FILE* fread, *fwrite;
			char* getWord, *delimiter;
			
			while (list->search){
 				treeInsert (tree, list->search);
				list->search = list->search->link;
			}
			fread = fopen ("text.txt", "r");
			fwrite = fopen ("Compressed Text.txt", "w+");
			
			if (fread == NULL){
				printf ("\nError: Nothing to compress.\n");
				return;
			}
			else{
				while (! feof (fread)){
					tree->search = tree->root;
					getWord = (char*)malloc((100 * sizeof(char)));
					fscanf (fread, "%s", getWord);
					if (feof (fread)){
						break;
					}
					else{
						strlwr (getWord);
						delimiter = strtok (getWord, " ,. ...:;- --_#$%&|><{}_+*?=!'/(()\\¨\"1234567890");
						while (delimiter != NULL){
							getWord = delimiter;
							delimiter = strtok (NULL, " ,. ...:;- --_#$%&|><{}_+*?=!'/(()\\¨\"1234567890");
						}
						if (stricmp (getWord, tree->search->word) == 0){
							fprintf (fwrite, "%d ", tree->search->id);
							fflush (fwrite);
						}
						else{
							while (stricmp (getWord, tree->search->word) != 0){
								if (stricmp (getWord, tree->search->word) < 0){
									tree->search = tree->search->left;
								}
								else if (stricmp (getWord, tree->search->word) > 0){
									tree->search = tree->search->right;
								}
							}
							if (stricmp (getWord, tree->search->word) == 0){
								fprintf (fwrite, "%d ", tree->search->id);
								fflush (fwrite);
							}
						}
					}
				}
				printf ("\nThe compressed text file is printed to Compressed Text.txt");
			}
			fclose (fread);
			fclose (fwrite);
			free (tree);
		}
	}
}

void decompress (LIST* list){
	if (list){
		TREE* tree;
		tree = createTree (compareID);
		
		if (tree){
			list->search = list->head;
			FILE* fread, *fwrite;
			char* getID, *delimiter;
			int intID;
			
			while (list->search){
 				treeInsert (tree, list->search);
				list->search = list->search->link;
			}
			fread = fopen ("Compressed Text.txt", "r");
			fwrite = fopen ("Deompressed Text.txt", "w+");
			if (fread == NULL){
				printf ("\nError: Nothing to decompress.\n");
				return;
			}
			else{
				while (! feof (fread)){
					tree->search = tree->root;
					getID = (char*)malloc((100 * sizeof(char)));
					fscanf (fread, "%s", getID);
					intID = atoi (getID);
					if (feof (fread)){
						break;
					}
					else{
						if (compareID (intID, tree->search->id) == 0){
							fprintf (fwrite, "%s ", tree->search->word);
							fflush (fwrite);
						}
						else{
							while (compareID (intID, tree->search->id) != 0){
								if (compareID (intID, tree->search->id) < 0){
									tree->search = tree->search->left;
								}
								else if (compareID (intID, tree->search->id) > 0){
									tree->search = tree->search->right;
								}
							}
							if (compareID (intID, tree->search->id) == 0){
								fprintf (fwrite, "%s ", tree->search->word);
								fflush (fwrite);
							}
						}
					}
				}
				printf ("\nThe decompressed text file is printed to Decompressed Text.txt");
			}
			fclose (fread);
			fclose (fwrite);
			free (tree);
		}
	}
}

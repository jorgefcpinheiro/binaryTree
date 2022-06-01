#include <stdio.h>
#include <stdlib.h>

typedef struct _DEPT {
	char desig[20];
	float ganhos, despesas;
	int n_pessoas;
}DEPT;
typedef struct _BTREE_NODE {
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;
#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)
typedef BTREE_NODE* BTREE;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;
int main(int argc, char* argv[])
{
	BTREE btree;
	void* depts[15];
	char file_name[20];
	printf("Nome do ficheiro: ");
	scanf("%s", file_name);
	if (ReadFile(depts, file_name)) {
		btree = CreateBtree(depts, 0, 15);
		BtreeFree(btree);
	}
	else printf("ERRO na leitura do ficheiro\n");
	return 1;
}
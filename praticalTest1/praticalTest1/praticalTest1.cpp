#include<stdio.h>
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
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

BTREE_NODE* NewBtreeNode(void* data)
{
	BTREE_NODE* tmp_pt;
	if ((tmp_pt = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL) {
		DATA(tmp_pt) = data;
		LEFT(tmp_pt) = RIGHT(tmp_pt) = NULL;
	}
	return tmp_pt;
}

void BtreeFree(BTREE btree)
{
	if (btree != NULL) {
		BtreeFree(LEFT(btree));
		BtreeFree(RIGHT(btree));
		free(DATA(btree));
		free(btree);
	}
}
BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2)
{
	BTREE_NODE* tmp_pt = NULL;
	tmp_pt = NewBtreeNode(ptr_data);
	LEFT(tmp_pt) = node1;
	RIGHT(tmp_pt) = node2;
	return(tmp_pt);
}
STATUS ReadFile(void** depts, char* file_name)
{
	FILE* fp;
	int j, i = 0;
	void* ptr_data;
	if ((fp = fopen(file_name, "r")) != NULL) {
		while (!feof(fp)) {
			if ((ptr_data = malloc(sizeof(DEPT))) != NULL) {
				fscanf(fp, "%[^;];%d;%f;%f\n", ((DEPT*)ptr_data)->desig, &((DEPT*)ptr_data)->n_pessoas,
					&((DEPT*)ptr_data)->ganhos, &((DEPT*)ptr_data)->despesas);
				depts[i] = ptr_data;
				i++;
			}
			else {
				for (j = i; j >= 0; j--)
					free(depts[j]);
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else
		return(ERROR);
}
BTREE_NODE* CreateBtree(void** v, int i, int size)
{
	if (i >= size) return(NULL);
	else return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));
}

int btreeSize(BTREE btr) {
	int count = 0;
	if (btr != NULL) {
		count = 1 + btreeSize(LEFT(btr)) + btreeSize(RIGHT(btr));
	}
	return count;
}

void printProfit(BTREE ptr) {
	int profit = 0;
	if (ptr != NULL) {
		printProfit(LEFT(ptr));
		profit = ((DEPT*)DATA(ptr))->ganhos - ((DEPT*)DATA(ptr))->despesas;
		printf("%s:%d\n", ((DEPT*)DATA(ptr))->desig, profit);
		printProfit(RIGHT(ptr));
	}
}

void maxPeople(BTREE ptr, int* vtr) {
	if (ptr != NULL) {
		maxPeople(LEFT(ptr), vtr);
		vtr[((DEPT*)DATA(ptr))->desig[7] - 1] = ((DEPT*)DATA(ptr))->n_pessoas;
		maxPeople(RIGHT(ptr), vtr);
	}
}

void printNode(BTREE node) {
	printf("%s:%d empregados:%d ganhos:%d despesas", ((DEPT*)DATA(node))->desig, ((DEPT*)DATA(node))->n_pessoas, ((DEPT*)DATA(node))->ganhos, ((DEPT*)DATA(node))->despesas);
}

void findValue(BTREE ptr, int val) {
	if (ptr != NULL) {
		findValue(LEFT(ptr), val);
		if (((DEPT*)DATA(ptr))->n_pessoas == val) printNode(ptr);
		findValue(RIGHT(ptr), val);
	}
}

int main(int argc, char* argv[])
{
	BTREE btree;
	void* depts[8];
	int people[8];
	int index = 0;
	char file_name[20] = "torneio.txt";
	//printf("Nome do ficheiro: ");
	//scanf("%s", file_name);
	if (ReadFile(depts, file_name)) {
		btree = CreateBtree(depts, 0, 8);
		printProfit(btree);
		maxPeople(btree, people);
		for (int i = 0; i < 8; i++) {
			int max = 0;
			if (people[i] > max) index = i;
		}
		//BtreeFree(btree);
	}
	else printf("ERRO na leitura do ficheiro\n");
	return 1;
}
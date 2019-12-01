
#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TypeInsert 1
#define TypeDelete 2

typedef int ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
typedef struct OperateList *Olist;
typedef int OperateType;
struct TNode {
	ElementType Data;
	BinTree Left;
	BinTree Right;
	int flag;
};
struct OperateList {
	OperateType type;
	ElementType e;
	Position link;
};
int PreorderTraversal(BinTree BT); /* ����������ɲ���ʵ�֣�ϸ�ڲ��� */
int InorderTraversal(BinTree BT);  /* ����������ɲ���ʵ�֣�ϸ�ڲ��� */

BinTree Insert(BinTree BST, ElementType e);
BinTree Delete(BinTree BST, ElementType e);
Position Find(BinTree BST, ElementType e);
Position FindMin(BinTree BST);
Position FindMax(BinTree BST);
int* createList(int *list, int length);  /* ����һ������Ϊlength���������飬ֵΪ��Ӧ�±꣬���ɲ���������*/
int* randomSort(int *list, int length);    /*����length�γ���Ϊlength�����飬���ɲ���������*/
int printBinaryTree(int length);    /* ��ӡ�������� */
int clearFile();    /* ����ļ�  */
int main()
{
	int i;
	int* list = NULL;
	//int list[20] = { 11, 9, 17, 18, 7, 2, 15, 6, 14, 0, 10, 4, 12, 13, 8, 5, 3, 19, 16, 1 };
	int length = 60;
	BinTree bt = NULL;
	list = createList(list, length);
	list = randomSort(list, length);
	for (i = 0; i < length; i++) {
		printf("%d ", list[i]);
	}
	printf("\n");
	printf("start insert!\n");
	for (i = 0; i < length; i++) {
		bt = Insert(bt, list[i]);
	}
	printf("end insert!\n");
	clearFile();
	PreorderTraversal(bt);
	system("pause");
	InorderTraversal(bt);
	system("pause");
	printBinaryTree(length);
	printf("\n");
	system("pause");
	return 0;
}

int* createList(int *list, int length)
{
	int i = 0;
	if (list) {
		free(list);
	}
	list = (int*)malloc(sizeof(int)*length);
	for (; i < length; i++) {
		list[i] = i;
	}
	return list;
}

int* randomSort(int *list, int length)
{
	int i = 0;
	int a = 0, b = 0;
	int temp = 0;
	srand((unsigned)time(NULL));
	for (; i < length; i++) {
		a = rand();
		a = a % (length*length);
		b = a % length;
		a = (int)a / length;
		temp = list[a];
		list[a] = list[b];
		list[b] = temp;
	}
	return list;
}

BinTree CreateTNode(ElementType e)
{
	BinTree p = (BinTree)malloc(sizeof(struct TNode));
	p->Data = e;
	p->Left = NULL;
	p->Right = NULL;
	p->flag = 0;
	return p;
}
BinTree Handle(BinTree BST, ElementType e, int type)
{
	BinTree p = BST;
	BinTree pre_p = p;
	BinTree temp_p = NULL;
	ElementType temp;
	int depth = 0, index = 0;
	depth = 10; //�ֶ��������ڲ��ԣ� depth = GetDepth��Bst��
	Olist stack = (Olist)malloc(sizeof(struct OperateList)*depth * 3);
	while (p) {
		if (type == TypeInsert) {
			pre_p = p;
			//����ֱ�Ӳ�������
			if (e > p->Data && p->Right == NULL) {
				p->flag++;
				p->Right = CreateTNode(e);

				printf("%d����ɹ�\n", e);
				if (index == 0) {
					return BST;
				}
				else {
					p = stack[index - 1].link;
					e = stack[index - 1].e;
					type = stack[index - 1].type;
					index--;
					continue;
				}
			}
			if (e < p->Data && p->Left == NULL) {
				p->flag--;
				p->Left = CreateTNode(e);

				printf("%d����ɹ�\n", e);
				if (index == 0) {
					return BST;
				}
				else {
					p = stack[index - 1].link;
					e = stack[index - 1].e;
					type = stack[index - 1].type;
					index--;
					continue;
				}
			}
			//����ֱ�Ӳ��룬��ı䵱ǰ�ṹ����ֻӰ��2�����
			if (e > p->Data && p->Left == NULL) {
				p->Left = CreateTNode(p->Data);
				if (e < p->Right->Data)
					p->Data = e;
				else {
					p->Data = p->Right->Data;
					p->Right->Data = e;
				}
				p->flag = 0;

				printf("%d����ɹ�\n", e);
				if (index == 0) {
					return BST;
				}
				else {
					p = stack[index - 1].link;
					e = stack[index - 1].e;
					type = stack[index - 1].type;
					index--;
					continue;
				}
			}
			if (e < p->Data && p->Right == NULL) {
				p->Right = CreateTNode(p->Data);
				if (e > p->Left->Data)
					p->Data = e;
				else {
					p->Data = p->Left->Data;
					p->Left->Data = e;
				}
				p->flag = 0;

				printf("%d����ɹ�\n", e);
				if (index == 0) {
					return BST;
				}
				else {
					p = stack[index - 1].link;
					e = stack[index - 1].e;
					type = stack[index - 1].type;
					index--;
					continue;
				}
			}
			//����ֱ�Ӳ��룬���ǲ���ı䵱ǰ�ṹ�������ָ��ָ����һ��
			if (e > p->Data && p->flag < 1) {
				p->flag++;
				p = p->Right;
				//printf(" �� ");
				continue;
			}
			if (e < p->Data && p->flag > -1) {
				p->flag--;
				p = p->Left;
				//printf(" �� ");
				continue;
			}
			//����ֱ�Ӳ��룬��ı���ṹ
			if (e > p->Data && p->flag == 1) {
				/*
				��ʱ������ֵ�������������ұ߱���߶�������������ṹ��
				1. ����ǰ����ֵ���뵽��������ȥ��
				2���ҵ�����������Сֵ�����ֵ���Ƚϣ�
				�����ڸ�ֵ���򽫸�ֵ��ֵ����ǰ���data�������ṹ��ɣ�������ɣ�return BST;
				��С�ڸ�ִֵ�в���3��
				3.����Сֵ��ֵ����ǰ���data��ɾ������������Сֵ��㣬����ֵ���뵽�������С�
				˼·ת��:
				1����������Сֵ�����Ƚ�
				�����ڣ�ͬ��
				�����ڣ���e��p->Dataֵ������������p=p->Left;
				��С�ڣ���˳��ִ����һ����
				*/
				temp_p = FindMin(p->Right);
				if (temp_p->Data > e) {
					temp = p->Data;
					p->Data = e;
					e = temp;
					p->flag = 0;
					p = p->Left;
					continue;
				}
				if (temp_p->Data < e) {
					//��ǰ������������,����ջ�����������洢�������ʼ���Ͳ����ֵ
					stack[index].e = temp_p->Data;
					stack[index].link = p->Right;
					stack[index].type = TypeDelete;
					index++;
					stack[index].e = e;
					stack[index].link = p->Right;
					stack[index].type = TypeInsert;
					index++;

					e = p->Data;
					p->Data = temp_p->Data;
					p->flag = 0;

					p = p->Left;
					continue;
				}
			}
			if (e < p->Data && p->flag == -1) {
				/*
				˼·������һ����
				˼·ת��:
				1�����������ֵ�����Ƚ�
				��С�ڣ���e��p->Dataֵ������������p=p->Right;
				�����ڣ��������Ʋ�����
				*/
				temp_p = FindMax(p->Left);
				if (temp_p->Data < e) {
					temp = p->Data;
					p->Data = e;
					e = temp;
					p->flag = 0;
					p = p->Right;
					continue;
				}
				if (temp_p->Data > e) {
					//��ǰ������������,����ջ�����������洢�������ʼ���Ͳ����ֵ
					stack[index].e = temp_p->Data;
					stack[index].link = p->Left;
					stack[index].type = TypeDelete;
					index++;
					stack[index].e = e;
					stack[index].link = p->Left;
					stack[index].type = TypeInsert;
					index++;

					e = p->Data;
					p->Data = temp_p->Data;
					p->flag = 0;

					p = p->Right;
					continue;
				}
			}
		}
		if (type == TypeDelete) {
			//����Ҫɾ���Ľ��
			if (e == p->Data) {
				//����ֱ��ɾ��
				if (!p->Left && !p->Right) {
					if (pre_p->Left == p)
						pre_p->Left = NULL;
					else if (pre_p->Right == p)
						pre_p->Right = NULL;
					else {
						BST = NULL;
					}
					free(p);
					p = NULL;
					
					printf("%dɾ���ɹ�\n", e);
					if (index == 0) {
						return BST;
					}
					else {
						p = stack[index - 1].link;
						e = stack[index - 1].e;
						type = stack[index - 1].type;
						index--;
						continue;
					}
				}
				//����ֱ��ɾ����ͨ���򵥱任���ֱ��ɾ��
				if (!p->Left) {
					p->Data = p->Right->Data;
					free(p->Right);
					p->Right = NULL;
					p->flag = 0;
					
					printf("%dɾ���ɹ�\n", e);
					if (index == 0) {
						return BST;
					}
					else {
						p = stack[index - 1].link;
						e = stack[index - 1].e;
						type = stack[index - 1].type;
						index--;
						continue;
					}
				}
				if (!p->Right) {
					p->Data = p->Left->Data;
					free(p->Left);
					p->Left = NULL;
					p->flag = 0;

					printf("%dɾ���ɹ�\n", e);
					if (index == 0) {
						return BST;
					}
					else {
						p = stack[index - 1].link;
						e = stack[index - 1].e;
						type = stack[index - 1].type;
						index--;
						continue;
					}
				}
				//����ֱ��ɾ������һ��ֵ�����ǰֵ��Ҫɾ����ֵ�ı䣬ָ��ָ����һ��
				if (p->flag >= 0) {
					temp_p = FindMin(p->Right);
					p->Data = temp_p->Data;
					e = temp_p->Data;
					p->flag--;
					pre_p = p;
					p = p->Right;
					continue;
				}
				else {
					temp_p = FindMax(p->Left);
					p->Data = temp_p->Data;
					e = temp_p->Data;
					p->flag++;
					pre_p = p;
					p = p->Left;
					continue;
				}
			}
			//û������Ҫɾ���Ľ��
			if (e > p->Data) {
				if (p->flag != -1) {
					p->flag--;
					pre_p = p;
					p = p->Right;
					continue;
				}
				temp_p = FindMax(p->Left);

				stack[index].e = temp_p->Data;
				stack[index].link = p->Left;
				stack[index].type = TypeDelete;
				index++;
				stack[index].e = e;
				stack[index].link = p->Right;
				stack[index].type = TypeDelete;
				index++;

				type = TypeInsert;
				e = p->Data;
				p->Data = temp_p->Data;
				p->flag = 0;
				p = p->Right;
				continue;

			}
			if (e < p->Data) {
				if (p->flag != 1) {
					p->flag++;
					pre_p = p;
					p = p->Left;
					continue;
				}
				temp_p = FindMin(p->Right);

				stack[index].e = temp_p->Data;
				stack[index].link = p->Right;
				stack[index].type = TypeDelete;
				index++;
				stack[index].e = e;
				stack[index].link = p->Left;
				stack[index].type = TypeDelete;
				index++;

				type = TypeInsert;
				e = p->Data;
				p->Data = temp_p->Data;
				p->flag = 0;
				p = p->Left;
				continue;
			}
		}
	}
	return BST;
}

BinTree Insert(BinTree BST, ElementType e)
{
	BinTree p = BST;
	BinTree temp_p = NULL;
	// �����Ϊ��
	if (!p) {
		p = CreateTNode(e);
		return p;
	}
	// ����Ϊ��ʱ
	temp_p = Find(p, e);
	if (temp_p) {
		printf("��Ԫ���Ѵ���");
		return BST;
	}
	temp_p = NULL;
	p = Handle(BST, e, TypeInsert);
	return p;
}

BinTree Delete(BinTree BST, ElementType e)
{
	BinTree p = BST;
	BinTree temp_p = NULL;

	if (!p) {
		printf("û�����Ԫ��\n");
		return BST;
	}
	// ����Ϊ��ʱ
	temp_p = Find(p, e);
	if (!temp_p) {
		printf("��Ԫ�ز�����");
		return BST;
	}
	p = Handle(BST, e, TypeDelete);
	return p;
}

Position Find(BinTree BST, ElementType e)
{
	BinTree p = BST;
	while (p) {
		if (e == p->Data)
			break;
		if (e > p->Data) {
			p = p->Right;
		}
		else if (e < p->Data) {
			p = p->Left;
		}
	}
	return p;
}

Position FindMin(BinTree BST)
{
	BinTree p = BST;
	if (!p) {
		return NULL;
	}
	while (p->Left) {
		p = p->Left;
	}
	return p;
}

Position FindMax(BinTree BST) {
	BinTree p = BST;
	if (!p) {
		return NULL;
	}
	while (p->Right) {
		p = p->Right;
	}
	return p;
}

int PreorderTraversal(BinTree BT)
{
	FILE *fp;
	if (BT) {
		fopen_s(&fp, "PreorderData.txt", "a");
		if (fp == NULL) {
			printf("The file can not be opened.\n");
			return 1;
		}
		fprintf(fp, "%d ", BT->Data);
		fclose(fp);
		PreorderTraversal(BT->Left);
		PreorderTraversal(BT->Right);
	}
	return 0;
}

int InorderTraversal(BinTree BT)
{
	FILE *fp;
	if (BT) {
		InorderTraversal(BT->Left);
		fopen_s(&fp, "InorderData.txt", "a");
		if (fp == NULL) {
			printf("The file can not be opened.\n");
			return 1;
		}
		fprintf(fp, "%d ", BT->Data);
		fclose(fp);
		InorderTraversal(BT->Right);
	}
	return 0;
}

int printBinaryTree(int length)
{
	FILE *fp = NULL;
	int ch, i = 0, j = 0, k = 0, temp = 0, count = 0;				//��ʱ����
	int pos1 = 0, pos2 = 0, pos3 = 0;
	//���λ�ã�1Ϊ������Ѵ����ַ�����2Ϊ����о���ǰһ���ַ����룬3Ϊ��ǰ����ַ���list�е����

	int num = 2, sum = 0;					//num���������sum�������һ��·��
	int depth = 0, depth_t = 0;				//depth��¼�ܲ���-1��depth_t��¼��ǰ��������
	int *list = (int*)malloc(sizeof(int)*length);
	int *list1 = NULL;
	while (num - 1 <= length) {
		depth += 1;
		num = num * 2;
	}
	list1 = (int*)malloc(sizeof(int)*(depth + 2));
	fopen_s(&fp, "PreorderData.txt", "r");
	if (fp == NULL) {
		printf("The file can not be opened.\n");
		return 1;
	}
	list[0] = 0;
	do {
		ch = fgetc(fp);
		if (ch == ' ') {
			i++;
			list[i] = 0;
		}
		else {
			list[i] = list[i] * 10 + ch - 48;
		}
	} while (ch != EOF);
	for (i = 0; i < length; i++) {
		if (i == 0) {
			pos3 = i;
		}
		else if (depth_t == 0) {
			list1[0] = 0;
			depth_t = 1;
		}
		else {
			for (j = depth_t; j > 0; j--) {
				if (list1[j - 1] == 0) {
					list1[j - 1] = 1;
					break;
				}
				else {
					list1[j - 1] = 0;
				}
			}
			if (j == 0 && list1[0] == 0) {
				list1[depth_t] = 0;
				depth_t += 1;
				printf("\n");
				pos1 = 0;
			}
		}
		if (depth_t < depth) {
			for (j = 0; j < depth_t; j++) {
				if (j == 0) {
					pos3 = 0;
					num = 0;
				}
				if (list1[j] == 0) {
					pos3 += 1;;
					num += 1;
				}
				else if (list1[j] == 1) {
					pos3 = list[pos3] + num + 1;
				}
			}
		}
		else {
			for (j = 0; j < depth_t; j++) {
				if (j == 0) {
					pos3 = 0;
					num = 0;
					sum = 0;
				}
				temp = pow(2, depth - j - 1) - 1;
				if (i > pow(2, depth) - 1) {
					count = i + 1 - pow(2, depth);
				}
				if (list[pos3] <= sum + temp + count) {
					if (list1[j] == 0) {
						for (k = j + 1; k < depth_t; k++) {
							list1[k] = 0;
						}
					}
					list1[j] = 1;
					sum += temp + 1;
				}
				if (list1[j] == 0) {
					pos3 += 1;;
					num += 1;
				}
				else if (list1[j] == 1) {
					pos3 = list[pos3] + num + 1;
				}
				if (j + 2 == depth) {
					if (list[pos3] + num + 1 - pos3 == 1) {

					}
				}
			}
		}
		pos2 = list[pos3] - pos1;
		for (j = 0; j < pos2; j++)
			printf("  ");
		printf("%2d", list[pos3]);
		pos1 = pos1 + pos2 + 1;
		if (i == 0) {
			pos1 = 0;
			printf("\n");
		}
		;
	}
	printf("\n");
	fclose(fp);
	return 0;
}

int clearFile()
{
	FILE *fp = NULL;
	fopen_s(&fp, "PreorderData.txt", "w");
	if (!fp) {
		printf("�ļ���ʧ��!");
		return 1;
	}
	fprintf(fp, "");
	fclose(fp);
	fopen_s(&fp, "InorderData.txt", "w");
	if (!fp) {
		printf("�ļ���ʧ��!");
		return 1;
	}
	fprintf(fp, "");
	fclose(fp);
	return 0;
}

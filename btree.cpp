
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
int PreorderTraversal(BinTree BT); /* 先序遍历，由裁判实现，细节不表 */
int InorderTraversal(BinTree BT);  /* 中序遍历，由裁判实现，细节不表 */

BinTree Insert(BinTree BST, ElementType e);
BinTree Delete(BinTree BST, ElementType e);
Position Find(BinTree BST, ElementType e);
Position FindMin(BinTree BST);
Position FindMax(BinTree BST);
int* createList(int *list, int length);  /* 建立一个长度为length的整形数组，值为对应下标，生成测试数据用*/
int* randomSort(int *list, int length);    /*打乱length次长度为length的数组，生成测试数据用*/
int printBinaryTree(int length);    /* 打印出二叉树 */
int clearFile();    /* 清空文件  */
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
	depth = 10; //手动定义用于测试， depth = GetDepth（Bst）
	Olist stack = (Olist)malloc(sizeof(struct OperateList)*depth * 3);
	while (p) {
		if (type == TypeInsert) {
			pre_p = p;
			//可以直接插入的情况
			if (e > p->Data && p->Right == NULL) {
				p->flag++;
				p->Right = CreateTNode(e);

				printf("%d插入成功\n", e);
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

				printf("%d插入成功\n", e);
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
			//不能直接插入，会改变当前结构，但只影响2个结点
			if (e > p->Data && p->Left == NULL) {
				p->Left = CreateTNode(p->Data);
				if (e < p->Right->Data)
					p->Data = e;
				else {
					p->Data = p->Right->Data;
					p->Right->Data = e;
				}
				p->flag = 0;

				printf("%d插入成功\n", e);
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

				printf("%d插入成功\n", e);
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
			//不能直接插入，但是不会改变当前结构的情况，指针指向下一层
			if (e > p->Data && p->flag < 1) {
				p->flag++;
				p = p->Right;
				//printf(" 右 ");
				continue;
			}
			if (e < p->Data && p->flag > -1) {
				p->flag--;
				p = p->Left;
				//printf(" 左 ");
				continue;
			}
			//不能直接插入，会改变多层结构
			if (e > p->Data && p->flag == 1) {
				/*
				此时若将该值插入右子树，右边比左边多两个，需调整结构，
				1. 将当前结点的值插入到左子树中去，
				2，找到右子树的最小值，与该值做比较，
				若大于该值，则将该值赋值给当前结点data，调整结构完成，插入完成，return BST;
				若小于该值执行步骤3。
				3.将最小值赋值给当前结点data，删掉右子树中最小值结点，将该值插入到右子树中。
				思路转换:
				1、先找右最小值，作比较
				若等于，同上
				若大于，将e和p->Data值互换，并且令p=p->Left;
				若小于，则顺序执行上一步骤
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
					//当前结点插入左子树,可用栈存插入操作，存储插入的起始结点和插入的值
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
				思路类似上一个。
				思路转换:
				1、先找左最大值，作比较
				若小于，将e和p->Data值互换，并且令p=p->Right;
				若大于，则做类似操作。
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
					//当前结点插入右子树,可用栈存插入操作，存储插入的起始结点和插入的值
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
			//遇到要删除的结点
			if (e == p->Data) {
				//可以直接删除
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
					
					printf("%d删除成功\n", e);
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
				//不能直接删除，通过简单变换后可直接删除
				if (!p->Left) {
					p->Data = p->Right->Data;
					free(p->Right);
					p->Right = NULL;
					p->flag = 0;
					
					printf("%d删除成功\n", e);
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

					printf("%d删除成功\n", e);
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
				//不能直接删除，找一个值替代当前值，要删除的值改变，指针指向下一层
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
			//没有遇到要删除的结点
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
	// 如果树为空
	if (!p) {
		p = CreateTNode(e);
		return p;
	}
	// 树不为空时
	temp_p = Find(p, e);
	if (temp_p) {
		printf("该元素已存在");
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
		printf("没有这个元素\n");
		return BST;
	}
	// 树不为空时
	temp_p = Find(p, e);
	if (!temp_p) {
		printf("该元素不存在");
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
	int ch, i = 0, j = 0, k = 0, temp = 0, count = 0;				//临时变量
	int pos1 = 0, pos2 = 0, pos3 = 0;
	//存放位置，1为输出行已存在字符数，2为输出行距离前一个字符距离，3为当前输出字符在list中的序号

	int num = 2, sum = 0;					//num计算层数，sum计算最后一层路径
	int depth = 0, depth_t = 0;				//depth记录总层数-1，depth_t记录当前遍历层数
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
		printf("文件打开失败!");
		return 1;
	}
	fprintf(fp, "");
	fclose(fp);
	fopen_s(&fp, "InorderData.txt", "w");
	if (!fp) {
		printf("文件打开失败!");
		return 1;
	}
	fprintf(fp, "");
	fclose(fp);
	return 0;
}

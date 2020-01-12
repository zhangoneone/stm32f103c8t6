#include "ns_bitree.h"
#include "stdlib.h"
#include "stdio.h"
/*********************宏定义区*************************/
/*********************宏定义区end*************************/
/*********************类型定义区*************************/
/*********************类型定义区end*************************/

/*********************私有函数区*************************/
#if (BI_TREE == 1)
//访问方式:遍历时找到主键e对应的结点，则保存该结点的引用
//找到则savenode结点有了合适的值
static int VisitMode_SameReturn(TreeNode* node, TreeNode* saveNode, void* e) {
	if (node->data == *(element*)e) {
		saveNode = node; return 0;
	}
}

//访问方式:输出主键值,ShowFun是打印函数
static int VisitMode_ShowValue(TreeNode* node, TreeNode* arg1, void*args) {
	static int i = 0;
	ShowFun("中序遍历结点值:%d\n", node->data);
	return 0;
}
#endif
/*********************私有函数区end*************************/

/*********************二叉树公有操作函数区*************************/
#if (BI_TREE == 1)
//创建一个只有根节点的空树
Tree Tree_create() {
	Tree T;
	T = (Tree)malloc(sizeof(TreeNode));
	T->lchild = T->rchild = NULL;
	T->data = -1;
	return T;
}

//计算左右子树的高度，并返回树的高度
int Tree_LR_Height(Tree T, int *L_Height, int *R_Height) {
	int L = 0, R = 0;
	if (T == NULL)return 0;
	if (T->lchild == NULL && T->rchild == NULL)return 0;
	if (T->lchild != NULL)L = 1 + Tree_LR_Height(T->lchild, L_Height, R_Height);
	if (T->rchild != NULL)R = 1 + Tree_LR_Height(T->rchild, L_Height, R_Height);
	*L_Height = L;
	*R_Height = R;
	T->lchild->balance_fact = L;
	T->rchild->balance_fact = R;
	return  *L_Height > *R_Height ? *L_Height : *R_Height;
}
//返回树的高度
int Tree_LR_Height2(Tree T) {
	int L = 0, R = 0;
	if (T->lchild == NULL && T->rchild == NULL)return 0;
	if (T->lchild != NULL)L = 1 + Tree_LR_Height2(T->lchild);
	if (T->rchild != NULL)R = 1 + Tree_LR_Height2(T->rchild);
	return L > R ? L : R;
}
//计算T的左右子树的高度差,左减右的差值
int Tree_LR_Height_Deviation(Tree T) {
	int L = 0, R = 0;
	Tree_LR_Height(T, &L, &R);
	return L - R;
}
//计算二叉树的高度
int Tree_Height(Tree T) {
	int L_Height = 0;
	int R_Height = 0;
	return Tree_LR_Height(T, &L_Height, &R_Height);
}

//树的遍历,visit函数，中序遍历
int Tree_Traversal(Tree T, visit_t visit, Tree visit_prama1, void*visit_prama2) {
	if (T) {
		Tree_Traversal(T->lchild, visit, visit_prama1, visit_prama2);
		visit(T, visit_prama1, visit_prama2);
		Tree_Traversal(T->rchild, visit, visit_prama1, visit_prama2);
	}
	return 0;
}
//通过主键e找到node并返回node本身的指针,并指定访问方式
//这种方式强制遍历了整个树，不是最好的办法
TreeNode* Tree_Find_Node_Fun(Tree T, element e, visit_t visit) {
	TreeNode* node = NULL;
	Tree_Traversal(T, visit, node, (void*)&e);
	return node;
}
//通过主键e找到node并返回node本身的指针,savenode保存找到的node.传入的savenode应该是NULL
TreeNode* Tree_Find_Node(Tree T, TreeNode*savenode, element e) {
	if (T) {
		return savenode ? savenode : Tree_Find_Node(T->lchild, savenode, e);
		if (T->data == e)savenode = T;
		return savenode ? savenode : Tree_Find_Node(T->rchild, savenode, e);
	}
	return savenode;
}
//node加入T的叶子上,平衡二叉树，但不是排序树
int Tree_add(Tree *T, TreeNode *node) {
	if (*T) {//未找到插入点前
		int deviation = 0;
		deviation = Tree_LR_Height_Deviation(*T);
		if (deviation <= 0)Tree_add(&((*T)->lchild), node);
		else Tree_add(&((*T)->rchild), node);
	}
	else
		*T = node;
	return 0;
}

//remove后的结点以指针返回。并且从树上摘除。返回NULL则不存在该结点
TreeNode* Tree_remove(Tree *T, element e) {
	TreeNode*savenode = NULL;
	Tree_Find_Node(*T, savenode, e);
	if (savenode) {//找到则删除

	}
	return savenode;
}
//delete后的结点被free了内存。并且从树上摘除
//返回0代表找到并且删除并且free，否则代表没找到或者没删除或者没有free
int Tree_delete(Tree *T, element e) {
	TreeNode*savenode = NULL;
	Tree_Find_Node(*T, savenode, e);
	if (savenode) {//找到则删除

		return 0;
	}
	return -1;
}
//以元素列表生成一颗普通树，返回树根结点
Tree Tree_init(element e_list[], int len) {
	int i = 0;
	TreeNode *node = NULL;
	Tree T = Tree_create();
	if (!T)return T;
	for (i = 0; i < len; i++) {
		node = (TreeNode*)malloc(sizeof(TreeNode));
		node->data = e_list[i];
		node->lchild = node->rchild = NULL;
		Tree_add(&T, node);
	}
	Tree_Traversal(T, VisitMode_ShowValue, 0, 0);
	return T;
}

//销毁树，并free内存
int Tree_destroy(Tree T) {
	if (T) {
		Tree_destroy(T->lchild);
		Tree_destroy(T->rchild);
		if (T->lchild == NULL && T->rchild == NULL) { free(T); T = NULL; };
	}
	return 0;
}
#endif
/*********************二叉树公有操作函数区end*************************/


/*********************索引二叉树区*************************/
#if (THR_TREE==1)
//对pre和cur做中序线索化，tag为0代表保存的是线索，tag为1代表保存的是子结点
//结点如果没有左孩子，那么它指向自己的父节点，结点如果没有右孩子，那么它指向自己中序遍历的下一个结点
int do_Thr(TreeNode *pre, TreeNode *cur) {
	if (pre) {
		if (pre->rchild == NULL) {
			pre->rtag = 0;
			pre->rchild = cur;
			ShowFun("%d的后继是%d\n", pre->data, cur->data);
		}
		else pre->rtag = 1;
	}
	if (cur) {
		if (cur->lchild == NULL) {
			cur->ltag = 0;
			cur->lchild = pre;
			if (pre)ShowFun("%d的前驱是%d\n", cur->data, pre->data);
		}
		else cur->ltag = 1;
	}
	return 0;
}

//线索化树T,给出树结点数量
int Tree_Thr(Tree T, int node_num) {
	TreeNode*pre = NULL, *cur = T;
	TreeNode** stk = (TreeNode **)malloc(sizeof(TreeNode*)*node_num);
	int top = 0;
	while (top > 0 || cur) {
		if (cur) {//向左遍历，结点压栈
			stk[top++] = cur;
			cur = cur->lchild;
		}
		else {//左遍历到头了，弹栈操作
			cur = stk[--top];
			do_Thr(pre, cur);
			pre = cur;
			cur = cur->rchild;
		}
	}
	return 0;
}


//线索二叉树的遍历更加简洁，所以可以用更容易的方法

//中序下求第一个节点
TreeNode *Get_FirstNode(Tree T){
	while(T->ltag ==1)T=T->lchild;
	return T;
}
//中序下求节点p的下一个结点
TreeNode *Get_NextNode(TreeNode *p){
	if(p->rtag ==1)return Get_FirstNode(p->rchild);
	else return p->rchild;
}

//中序下，线索二叉树遍历
int Tree_Thr_Traversal(Tree T,visit_t visit,Tree visit_prama1, void*visit_prama2){
	TreeNode *p;
	for(p=Get_FirstNode(T);p!=NULL;p=Get_NextNode(p))
		visit(T,visit_prama1,visit_prama2);
}
#endif
/*********************索引二叉树区end*************************/

/*********************avl树区*************************/
#if (AVL_TREE==1)
//avl树是一颗平衡排序二叉树，它每次插入都是有序的，每次插入都要旋转操作
//因此需要重写插入，删除函数。
//使用中序，即左子树小于根，根小于右子树

//左旋两次
int Avl_Tree_LL(Tree *Ancestor,Tree *T) {

	return 0;
}

//右旋两次次
int Avl_Tree_RR(Tree *Ancestor,Tree *T) {

	return 0;
}
//左旋一次，右旋一次
int Avl_Tree_LR(Tree *Ancestor,Tree *T) {

	return 0;
}
//右旋一次，左旋一次
int Avl_Tree_RL(Tree *Ancestor,Tree *T) {

	return 0;
}

int Avl_Tree_add(Tree *T, TreeNode *node) {
	if (*T) {
		if ((*T)->data > node->data)Avl_Tree_add(&(*T)->lchild, node);
		else if((*T)->data < node->data)Avl_Tree_add(&(*T)->rchild, node);
		else return -1;
		(*T)->balance_fact = Tree_LR_Height_Deviation(*T);//计算平衡因子
		if ((*T)->balance_fact >= 2) {//左倾斜
			if ((*T)->lchild->lchild->lchild || (*T)->lchild->lchild->rchild)
				;//Avl_Tree_LL(T);
			if ((*T)->lchild->rchild->lchild || (*T)->lchild->rchild->rchild)
				;//Avl_Tree_LR(T);
		}
		else if ((*T)->balance_fact <= -2) {//右倾斜
			if ((*T)->rchild->rchild->lchild || (*T)->rchild->rchild->rchild)
				;//Avl_Tree_RR(T);
			if ((*T)->rchild->lchild->lchild || (*T)->rchild->lchild->rchild)
				;//Avl_Tree_RL(T);
		}
	}
	else {
		*T = node;
		(*T)->balance_fact = 0;
	}
	return 0;
}





#endif
/*********************avl树区end*************************/

/*********************哈夫曼树区*************************/
#if (HUFFMAN_TREE==1)

#endif
/*********************哈夫曼树区end*************************/





/**********************主函数测试区************************************/

//int main() {
//	element e_list[10] = { 5,3,6,1,7,8,4,9,2,0 };
//	Tree T = Tree_init(e_list, 10);
//	T->data = -1;
//	Tree_Thr(T, 11);
//	getchar();
//}

#ifndef NS_BITREE_H
#define NS_BITREE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_algorithm.h"

#define ShowFun(fmt,...) printf(fmt,__VA_ARGS__);
#define BI_TREE   0
#define THR_TREE	0
#define AVL_TREE	0
#define HUFFMAN_TREE	0

typedef struct TreeNode {
	char ltag;
	struct TreeNode *lchild;
	element data;
	struct TreeNode *rchild;
	char rtag;
	int node_cost;//哈夫曼树要使用的结点代价值
	int balance_fact;//avl树平衡因子
}TreeNode, *Tree;//线索二叉树的声明，兼容一般二叉树

typedef int(*visit_t)(TreeNode* node, TreeNode* OutNode, void* args);
#ifdef __cplusplus
}
#endif
#endif
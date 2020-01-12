#include "ns_hashtable.h"
#if(HASHTABLE == 1)
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int length = 800;
unsigned int m = 0x80000000;
int y0 = 568731;
int savefile(const char *name, char *s)
{
	FILE *fp = NULL;
	fp = fopen(name, "a+");
	fputs(s, fp);
	fclose(fp);
	return 0;
}
int h(int x)
{
	return x;
}
int generate_ramdom(int x[], int length)
{
	srand(time(NULL));
	for (int i = 0; i < length; i++)
	{
		x[i] = rand() % 1000;
	}
	return 0;
}
int generate_ramdom_another(int x[], int y[], int length)
{
	x[0] = 1000.0*y[0] / (m + 0.1 - 0.1) + 1;
	for (int i = 1; i < length; i++)
	{
		y[i] = (15625 * y[i - 1] + 22221) % m;
		x[i] = 1000.0*y[i] / m;
	}
	return 0;
}
int* create_hashTable(int length)
{
	int *p = (int *)(malloc(sizeof(int)*length));
	for (int i = 0; i < length; p[i] = 0, i++);
	return p;
}
//寻找k在哈希表中位置 k是键值 
int search1(int hashtable[], int k)
{
	int i = h(k);//i是拟查找位置 
	int j;
	for (j = 0; j < length&&		//表未查完 
		hashtable[(i + j) % length] != k &&	//循环查一轮表 未查到键值k 前两个条件确保了在整个表中查找键值k 
		hashtable[(i + j) % length] != 0;	//当遇到空位置，后面不用再查找了 
		j++
		);
	//执行完for循环，j要么(1)包含了表中存在了k，要么(2)查找到了空位置 
	//此时给i赋值，检测属于哪种情况 
	i = (i + j) % length;
	if (hashtable[i] == k)//属于情况1
		return i;
	return -1; //表示k不在表中 

}
int insert1(int hashtable[], int k)
{
	int i = h(k);//i是拟插入位置 
	int j;
	char s[40];
	//该位置值小于等于0，可以插入
	//该位置值大于0，需要再探测新位置 
	for (j = 0; j < length&&//确保j的范围覆盖整个表 
		hashtable[(i + j) % length]>0 &&//位置被占 
		hashtable[(i + j) % length] != k;//遍历整个表，看k是否已经被插入了 
		j++
		);
	//此时j包含了 （1）要么是已经插入的k的位置（2）要么是没有被占的位置 
	i = (i + j) % length;
	if (hashtable[i] <= 0)//属于情况2
	{
		//插入 
		hashtable[i] = k;
		sprintf(s, "%d插入了位置%d\n", k, i);
		savefile("插入记录.txt", s);
		return 0;
	}
	sprintf(s, "%d已经存在于位置%d!\n", k, i);
	savefile("冲突记录.txt", s);
	return 1;//属于情况1,k已经在表中 
}
int delete1(int hashtable[], int k)
{
	int i, j;
	i = h(k);
	char s[40];
	for (j = 0; j < length&&
		hashtable[(i + j) % length] != k &&	//没找到k
		hashtable[(i + j) % length] != 0;		//没遇到空位置
		j++);
	//此时j 要么找到k，要么找到了空位置
	i = (i + j) % length;
	if (hashtable[i] == k)
	{
		hashtable[i] = -1;
		sprintf(s, "在哈希表的位置%d删除了键值%d!\n", i, k);
		savefile("删除记录.txt", s);
		return 1;
	}
	return 0;
}

/*********拉链法实现哈希表***********/

int hashtable_test()
{
	int x[800] = { 0 };
	int y[800];
	char s[50];
	int * hashtable = create_hashTable(length);
	y[0] = 568731;
	generate_ramdom_another(x, y, length);
	for (int i = 0; i < length; i++)
	{
		//	printf("位置%d的键值是%d",i,x[i]);
		sprintf(s, "位置:%d键值:%d\n", i, x[i]);
		savefile("原始随机数.txt", s);
	}
	for (int i = 0; i < length; i++)
	{
		insert1(hashtable, x[i]);
	}
	for (int i = 0; i < length; i++)
	{
		sprintf(s, "位置%d保存了键值%d\n ", i, hashtable[i]);
		savefile("哈希表记录.txt", s);
	}
	for (int i = 0; i < length; i++)
	{
		sprintf(s, "在位置%d查找到键值%d\n ", search1(hashtable, x[i]), x[i]);
		savefile("查表记录.txt", s);
	}
	printf("OK!\n");
	while (1)
	{
		int k = 0;
		printf("输入要删除的键值:");
		scanf("%d", &k);
		if (delete1(hashtable, k))
			printf("删除成功\n");
		else
			printf("删除失败\n");
	}
	return 0;
}
#endif
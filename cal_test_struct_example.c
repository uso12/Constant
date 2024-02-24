#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 20
#define N 3
#define SIZE 3
 
//描述一个学生的姓名，年龄，三门成绩
typedef struct student
{
	char name[M];
	int age;
	float scores[N];
}Stu;
//编写一子函数，实现数组的求和
//参数1：数组的首地址 float sum[]
//参数2：数组的元素个数 int count
//返回值：三门成绩的和 float 
float sumScores(float add[],int count)
{
	int i;
	float sum = 0;
	for(i = 0; i < count; i++)
	{
		sum += add[i];
	}
	return sum;
}
//编写一子函数，实现两个结构体的交换
//参数1：结构体1的首地址 Stu *pa
//参数2：结构体2的首地址 Stu *pb
//返回值：void
void swap(Stu *pa,Stu *pb)
{
	Stu temp = {0};
	temp = *pa;
	*pa = *pb;
	*pb = temp;
}
//编写一子函数，实现结构体的输入
//参数1：结构体的首地址 Stu *pc
//参数2：结构体的个数 int count
//返回值：void
void input(Stu *pc,int count)
{
	int i,j;
	for(i = 0; i < count; i++)
	{
		printf("请输入学生的姓名，年龄，三门成绩\n");
		scanf("%s",pc->name);
		scanf("%d",&pc->age);
		for(j = 0; j < N; j++)
		{
			scanf("%f",&pc->scores[j]);
		}
		pc++;
	}
}
//编写一子函数，实现结构体的输出
//参数1：结构体的首地址 Stu *pc
//参数2：结构体的个数 int count
//返回值：void
void output(Stu *pc,int count)
{
	int i,j;
	for(i = 0; i < count; i++)
	{
		printf("%s ",pc->name);
		printf("%d ",pc->age);
		for(j = 0; j < N; j++)
		{
			printf("%.1f ",pc->scores[j]);
		}
		printf("\n");
		pc++;
	}
}
//编写一子函数，求年龄最大人的首地址
//参数1：结构体的首地址
//参数2：结构体的个数
//返回值：求得的年龄最大人的首地址 Stu *
Stu *calPmaxAge(Stu *pc,int count)
{
	int i;
	//定义一个结构体指针变量，保存年龄最大人的首地址
	Stu *pMax = NULL;
	//假设第一个为年龄最大人的首地址
	pMax = pc;
	for(i = 1; i < count; i++)
	{
		if(pMax->age < (pc+i)->age)
		{
			pMax = (pc+i);
		}
	}
	return pMax;
}
//编写一子函数，按照年龄进行排序
//参数1：结构体的首地址 Stu *pc
//参数2：结构体的个数 int count
//返回值：void
void sortByAge(Stu *pc,int count)
{
	int i,j;
	for(i = 0; i < count-1; i++)
	{
		for(j = 0; j < count-1-i; j++)
		{
			if((pc+j)->age > (pc+j+1)->age)
			{
				swap((pc+j),(pc+j+1));
			}
		}
	}
}
//编写一子函数，按照姓名进行排序
//参数1：结构体的首地址
//参数2：结构体的个数
//返回值：void
void sortByName(Stu *pc,int count)
{
	int i,j;
	for(i = 0; i < count-1; i++)
	{
		for(j = 0; j < count-1-i; j++)
		{
			if(strcmp((pc+j)->name,(pc+j+1)->name) > 0)
			{
				swap((pc+j),(pc+j+1));
			}
		}
	}
}
//编写一子函数，按照总成绩进行排序
//参数1：结构体的首地址
//参数2：结构体的个数
//返回值：void
void sortBySumScores(Stu *pc,int count)
{
	int i,j;
	for(i = 0; i < count-1; i++)
	{
		for(j = 0; j < count-1-i; j++)
		{
			if(sumScores((pc+j)->scores,N) > sumScores((pc+j+1)->scores,N))
			{
				swap((pc+j),(pc+j+1));
			}
		}
	}
}
 
int main(void)
{
	int op = 0;
	//定义一个结构体指针变量
	Stu *ps = NULL;
	Stu *pMax = NULL;
	//动态分配SIZE个Stu那么大的空间
	//void *malloc(size_size)
	//参数：要申请的空间的大小
	//返回值：成功返回申请到的空间的首地址，失败返回NULL
	ps = (Stu *)malloc(sizeof(Stu) * SIZE);
	if(NULL == ps)
	{
		perror("malloc error");
		return -1;
	}
	while(1)
	{
		printf("请输入选项:\n");
		printf("1--------------input\n");
		printf("2--------------output\n");
		printf("3--------------calPmaxAge\n");//求年龄最大人的首地址
		printf("4--------------sortByAge\n");//按照年龄排序
		printf("5--------------sortByName\n");//按照姓名排序
		printf("6--------------sortBySumScores\n");//按照总成绩进行排序
		printf("-1-------------exit\n");
		scanf("%d",&op);
		if(-1 == op)
		{
			break;
		}
		switch(op)
		{
			case 1:
				input(ps,SIZE);
				break;
			case 2:
				output(ps,SIZE);
				break;
			case 3:
				pMax = calPmaxAge(ps,SIZE);
				printf("pMax = %p\n",pMax);
				break;
			case 4:
				sortByAge(ps,SIZE);
				break;
			case 5:
				sortByName(ps,SIZE);
				break;
			case 6:
				sortBySumScores(ps,SIZE);
				break;
		}
	}
	//释放空间
	free(ps);
	ps = NULL;
 
	return 0;
}
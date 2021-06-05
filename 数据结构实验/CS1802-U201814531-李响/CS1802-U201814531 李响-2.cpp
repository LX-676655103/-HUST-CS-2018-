#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define N 100
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
/*--------------------------------------------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
/*--------------------------------------------*/
typedef int status;
typedef int ElemType; //数据元素类型定义
/*--------------------------------------------*/
typedef struct LNode{  //线性表（链式结构结点）的定义
	ElemType data;
	struct LNode *next;//存储线性表名称
}LNode,*LinkList; //结点结构名以及结点指针结构名
typedef struct{
    LinkList head;
    char name[30];
}LkList; //存储线性表的名称信息以及头指针
/*--------------------------------------------*/
status InitList(LinkList &L);//初始化线性表
status DestroyList(LinkList &L);//销毁线性表
status ClearList(LinkList L);//清空线性表
status ListEmpty(LinkList L);//判断线性表是否为空
int ListLength(LinkList L);//求表长
status GetElem(LinkList L,int i,ElemType &e);//获取元素
int LocateElem(LinkList L,ElemType e);//定位元素
status PriorElem(LinkList L,ElemType cur,ElemType &pre_e);//获取前驱
status NextElem(LinkList L,ElemType cur,ElemType &next_e);//获取后继
status ListInsert(LinkList L,int i,ElemType e);//插入元素
status ListDelete(LinkList L,int i,ElemType &e);//删除元素
status ListTrabverse(LinkList L);//遍历线性表
status OpenFile(LinkList L);//读取文件
status SetValue(LinkList L);//键盘输入数据
status SaveFile(LkList L);//保存文件
/*--------------------------------------------*/
int main(){
LkList List[N];  //定义一个头指针的数组便于管理多个线性表，线性表数组的第一个空间不使用，以便进行操作
int op=1,i=0,j,num=0,k;
ElemType e;  char name[30];
while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Linear Table On Sequence Structure \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. InitList       8. PriorElem\n");
	printf("    	  2. DestroyList    9. NextElem \n");
	printf("    	  3. ClearList      10. ListInsert\n");
	printf("    	  4. ListEmpty      11. ListDelete\n");
	printf("    	  5. ListLength     12. ListTrabverse\n");
	printf("    	  6. GetElem        13. SaveFile\n");
	printf("    	  7. LocateElem     14. ListMessage\n"); //操作14可以方便地查看所有线性表的信息，以便进行其他操作
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("      请选择你的操作[0~14]:");
	scanf("%d",&op);
	if(op>=2&&op<=13) //操作2-13初始条件为线性表存在，故提前判断
    {
        if(num)//num变量代表线性表的数量，此处判断是否有线性表
        {
            printf("      请输入您要操作的线性表的名称：");//通过名称查找线性表
            scanf("%s",name);
            for(i=1;i<=num;i++)//遍历查找
                if(strcmp(List[i].name,name)==0) break;
            if(i>num) {printf("      您查找的线性表不存在，请重新操作：\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      当前无线性表存在，请先创建线性表！\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
         if(InitList(List[++num].head)==OK)//创建线性表，线性表个数加一
         {
             printf("      线性表创建成功！请对线性表进行命名：");
             scanf("%s",List[num].name);//对线性表进行命名，以便进行后续操作
         }
         else{ printf("      线性表创建失败！\n"); num--; getchar();getchar(); break;}
         printf("      请选择：1.请对线性表进行键盘赋值\n              2.读取文件\n\n");//线性表初始赋值
         scanf("%d",&k);
         switch(k){
            case 1: if(SetValue(List[num].head)==OVERFLOW){num--; printf("      线性表赋值失败！\n");} break;//键盘赋值
            case 2: switch(OpenFile(List[num].head))//文件赋值
                    {
                       case OVERFLOW: num--; break;
                       case ERROR: num--; break;
                       case OK: break;
                    }
         }
         getchar();
		 break;
	   case 2:
		 if(DestroyList(List[i].head)==OK)
		 {
                printf("      线性表销毁成功！\n");
                for(;i<num;i++) //多线性表管理，需要将销毁的线性表删除覆盖，List[0]不存储信息
                    List[i]=List[i+1];
                num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
		 if(ClearList(List[i].head)==OK) printf("      线性表清空成功！\n");
         else printf("      线性表不存在，清空失败！\n");
		 getchar();getchar();
		 break;
	   case 4:
         if(ListEmpty(List[i].head)==FALSE) printf("      该线性表不为空表！\n");
         else printf("      该线性表为空表！\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      该线性表的长度为%d\n",ListLength(List[i].head));
		 getchar();getchar();
		 break;
	   case 6:
         if(ListEmpty(List[i].head)) {printf("      该线性表为空表，请进行其他操作！\n"); getchar();getchar();break;}
         printf("      请输入您想查找的元素的序号，注意请不要输入小于1或者大于%d的整数：",ListLength(List[i].head));
         scanf("%d",&j);
         if(GetElem(List[i].head,j,e)==ERROR) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功，第%d个元素值为%d\n",j,e);
		 getchar();getchar();
		 break;
	   case 7:
         printf("      请输入要查找的元素值：");
         scanf("%d",&e);
		 if(!LocateElem(List[i].head,e)) printf("      没有符合条件的元素存在！\n");
		 else printf("      符合条件的元素的位序为%d \n",LocateElem(List[i].head,e));
		 getchar();getchar();
		 break;
	   case 8:
         ElemType cur_e,pre_e;
		 printf("      请输入想获得前驱的元素：");
		 scanf("%d",&cur_e);
         if(PriorElem(List[i].head,cur_e,pre_e)==ERROR-1) printf("      该元素不存在前驱!\n");//若首个元素符合条件，会返回-1
         else if(PriorElem(List[i].head,cur_e,pre_e)==ERROR) printf("      该元素不存在!\n");
         else printf("      它的前驱为%d\n",pre_e);
         getchar();getchar();
         break;
	   case 9:
		 ElemType next_e;
		 printf("      请输入想获得后继的元素：");
		 scanf("%d",&cur_e);
         if(NextElem(List[i].head,cur_e,next_e)==ERROR-1) printf("      该元素不存在后继!\n");//若尾部元素符合条件，会返回-1
         else if(NextElem(List[i].head,cur_e,next_e)==ERROR) printf("      该元素不存在!\n");
         else printf("      它的后继为%d\n",next_e);
         getchar();getchar();
         break;
	   case 10:
         printf("      请输入插入的位置和插入元素值，注意请不要输入小于1或者大于%d的整数：",ListLength(List[i].head)+1);
         scanf("%d%d",&j,&e);
         if(!ListInsert(List[i].head,j,e)) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功!\n");
         getchar();getchar();
         break;
	   case 11:
		 if(ListEmpty(List[i].head)) {printf("      该线性表为空表，请进行其他操作！\n");getchar();getchar(); break;}
         printf("      请输入想删除的元素的位置，注意请不要输入小于1或者大于%d的整数：",ListLength(List[i].head));
         scanf("%d",&j);
         if(!ListDelete(List[i].head,j,e)) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功，所删除的元素为%d\n",e);
         getchar();getchar();
         break;
	   case 12:
		 if(!ListTrabverse(List[i].head)) printf("      该线性表是空表，请进行其他操作！\n");
		 getchar();getchar();
		 break;
       case 13:
         SaveFile(List[i]);
         break;
       case 14:  //操作14可以方便地查看所有线性表的信息，以便进行其他操作
         if(!num) {printf("      当先无线性表存在，请进行其他操作：\n"); getchar();getchar(); break;} //判断有无线性表
         printf("      当先线性表信息如下：\n");
         printf("      线性表个数为%d\n",num);
         printf("      线性表的名称及长度如下：\n");
         for(j=1;j<=num;j++) //遍历线性表数组，输出数据
         printf("      %20s    %d\n",List[j].name,ListLength(List[j].head));
         getchar();getchar();
	   case 0:
         getchar();getchar();
         break;
	}
  }
printf("\n欢迎下次再使用本系统！\n");
}
/*--------------------------------------------*/
status InitList(LinkList &L)
{
	L=(LinkList)malloc(sizeof(LNode)); //为头结点分配空间，头结点不参与数据保存
    if(!L) return OVERFLOW;
    L->data=0; //头结点不储存数据，赋值为零
    L->next=NULL;
	return OK;
}
/*--------------------------------------------*/
status DestroyList(LinkList &L)
{
    LinkList p=L,q; //定义两个结构指针p与q，p指向需要释放的结点空间，q指向需要释放的结点的后继，防止结点丢失
    if(!L) return ERROR;
    while(p)
    {
        q=p->next; //指向下一个结点
        free(p); //释放空间
        p=q;
    }
    L=NULL;
    return OK;
}
/*--------------------------------------------*/
status ClearList(LinkList L)
{
    LinkList p=L->next,q; //由于头结点空间不用释放，故跳过头结点
    if(!L) return ERROR;
    L->next=NULL; //将头结点的指向指向空
    while(p)
    {
        q=p->next;//与删除链表的操作一致
        free(p);
        p=q;
    }
    return OK;
}
/*--------------------------------------------*/
status ListEmpty(LinkList L)
{
    if(!L->next) return TRUE;
    else return FALSE;
}
/*--------------------------------------------*/
int ListLength(LinkList L)
{
    int length=0;
    if(!L) return ERROR;
    LinkList p=L->next;
    while(p) //遍历链表统计长度
    {
        length++;
        p=p->next;
    }
    return length;
}
/*--------------------------------------------*/
status GetElem(LinkList L,int i,ElemType &e)
{
    int j=1; //计数器，用于计数当前位置信息
    LinkList p=L->next; //由于不用改变前后指针信息，所以p和j是完全同步的
    while(p&&j<i) //仍有元素并且还未到遍历的位置
    {
        p=p->next;
        j++;
    }
    if(!p||i<1) return ERROR; //遍历的位置超过线性表的长度及查找位置为非正
    else e=p->data;
    return OK;
}
/*--------------------------------------------*/
int LocateElem(LinkList L,ElemType e)
{
    int i; //计数器，用于存储位序
    LinkList p=L->next; //由于不用改变前后指针信息，所以p和i是完全同步的
    for(i=1;p;i++)
    {
        if(p->data==e) return i;
        p=p->next;
    }
    return 0;
}
/*--------------------------------------------*/
status PriorElem(LinkList L,ElemType cur,ElemType &pre_e)
{
    LinkList p=L->next,q;
    if(cur==p->data) return -1; //首元素为需要查找前驱的元素，返回-1，以便和ERROR区分
    q=p; p=p->next; //便于返回前驱
    while(p) //遍历查找线性表
    {
        if(p->data==cur){
            pre_e=q->data;
            return OK;
        }
        q=p; p=p->next;
    }
    return ERROR;//查找元素不存在，返回ERROR
}
/*--------------------------------------------*/
status NextElem(LinkList L,ElemType cur,ElemType &next_e)
{
    LinkList p=L->next;
    while(p->next)//遍历查找线性表
    {
        if(p->data==cur){
            next_e=p->next->data;
            return OK;
        }
        p=p->next;
    }
    if(cur==p->data) return ERROR-1; //判断最后一个元素
    return ERROR;//查找元素不存在，返回ERROR
}
/*--------------------------------------------*/
status ListInsert(LinkList L,int i,ElemType e)
{
    int j=1; //计数器，用于存储位序
    LinkList p=L,pNew;  //与定位元素类似，但是由于需要改变前后指针信息，所以p需要滞后于j一个位置，便于插入元素
    while(p&&j<i) //由于比length大1的位置仍可以插入元素，故如此
    {
        p=p->next;
        j++;
    }
    if(i<1||!p) return ERROR; //不可插入元素，报错
    pNew=(LinkList)malloc(sizeof(LNode)); //创建新的结点，赋值并插入
    pNew->data=e;
    pNew->next=p->next;
    p->next=pNew;
    return OK;
}
/*--------------------------------------------*/
status ListDelete(LinkList L,int i,ElemType &e)
{
    int j=1; //计数器，用于存储位序
    LinkList p=L,q; //与定位元素类似，但是由于需要改变前后指针信息，所以p需要滞后于j一个位置，便于插入元素
    while(p->next&&j<i) //只能处理位序小于等于length的元素
    {
        p=p->next;
        j++;
    }
    if(i<1||!p->next||!L->next) return ERROR;
    q=p->next; //用q指针取出结点，便于释放空间
    p->next=q->next;
    e=q->data;
    free(q);
    return OK;
}
/*--------------------------------------------*/
status ListTrabverse(LinkList L)
{
    LinkList p=L->next;
    printf("\n----------------all elements --------------------\n");
    while(p)
    {
        printf("%d ",p->data); //遍历线性表，输出元素
        p=p->next;
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}
/*--------------------------------------------*/
status OpenFile(LinkList L)
{
    ElemType j;
    LinkList p=L,q;
    FILE* fp; char filename[30]; int i;
    printf("      请输入文件名：");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) {printf("      打开文件失败!\n"); return ERROR;} //文件赋值，打开文件
	while(fscanf(fp,"%d",&j)!=EOF) //从文件中读取数据，遇到文件尾停止
    {
        q=(LinkList)malloc(sizeof(LNode));
        q->data=j;
        p->next=q;
        q->next=NULL;
        p=p->next;
    }
    printf("      打开文件成功!\n");
    fclose(fp); //关闭文件
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SetValue(LinkList L) //键盘输入数据
{
    ElemType j;
    LinkList p=L,q;
    printf("      请输入元素值并以Ctrl+Z结束:\n");
    while(scanf("%d",&j)!=EOF)
    {
        q=(LinkList)malloc(sizeof(LNode));
        q->data=j;
        p->next=q;
        q->next=NULL;
        p=p->next;

    }
    printf("      赋值成功！\n");
    return OK;
}
/*--------------------------------------------*/
status SaveFile(LkList L)
{
    FILE* fp;
    int num;
    LinkList p=L.head->next;
    char filename[30];
    printf("      请选择：1.不删除原有的线性表，仅对文件进行追加\n              2.删除原有线性表或生成新文件\n\n");
    scanf("%d",&num);
    printf("      请输入文件名：");
    scanf("%s",filename);
    if(num==1&&(fp=fopen(filename,"a"))==NULL) //文件打开方式为追加，不对原有的数据进行修改
    {
        printf("      文件文件打开失败！\n");
        return ERROR;
    }
    else if(num==2&&(fp=fopen(filename,"w"))==NULL) //文件打开方式为更新或创建新文件，会删除原有数据
    {
        printf("      文件文件打开失败！\n");
        return ERROR;
    }
    fputs(L.name,fp);//输出线性表的名称
    fprintf(fp,"\n");//换行
    while(p) //遍历输出数据
    {
        fprintf(fp,"%d ",p->data);
        p=p->next;
    }
    fprintf(fp,"\n");
    fclose(fp);//关闭文件
    printf("      保存成功！\n");
    getchar();getchar();
    return OK;
}
/*--------------------------------------------*/











#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define N 20
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
typedef struct{  //顺序表（顺序结构）的定义
	ElemType * elem;
	int length;
	int listsize;
	char name[20];//存储线性表名称
}SqList;
/*--------------------------------------------*/
status InitList(SqList &L);//初始化线性表
status DestroyList(SqList &L);//销毁线性表
status ClearList(SqList &L);//清空线性表
status ListEmpty(SqList L);//判断线性表是否为空
int ListLength(SqList L);//求表长
status GetElem(SqList L,int i,ElemType &e);//获取元素
status LocateElem(SqList L,ElemType e);//定位元素
status PriorElem(SqList L,ElemType cur,ElemType &pre_e);//获取前驱
status NextElem(SqList L,ElemType cur,ElemType &next_e);//获取后继
status ListInsert(SqList &L,int i,ElemType e);//插入元素
status ListDelete(SqList &L,int i,ElemType &e);//删除元素
status ListTrabverse(SqList L);//遍历线性表
status OpenFile(SqList &L);//读取文件
status SetValue(SqList &L);//键盘输入数据
status SaveFile(SqList &L);//保存文件
/*--------------------------------------------*/
int main(){
SqList List[N];  //定义一个线性表数组便于管理多个线性表，线性表数组的第一个空间不使用，以便进行操作
int op=1,i=0,j,num=0,k;
ElemType e;  char name[20];
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
         if(InitList(List[++num])==OK)//创建线性表，线性表个数加一
         {
             printf("      线性表创建成功！请对线性表进行命名：");
             scanf("%s",List[num].name);//对线性表进行命名，以便进行后续操作
         }
         else{ printf("      线性表创建失败！\n"); num--; getchar();getchar(); break;}
         printf("      请选择：1.请对线性表进行键盘赋值\n              2.读取文件\n\n");//线性表初始赋值
         scanf("%d",&k);
         switch(k){
            case 1: if(SetValue(List[num])==OVERFLOW){num--; printf("      线性表赋值失败！\n");} break;//键盘赋值
            case 2: switch(OpenFile(List[num]))//文件赋值
                    {
                       case OVERFLOW: num--; break;
                       case ERROR: num--; break;
                       case OK: break;
                    }
         }
         getchar();
		 break;
	   case 2:
		 if(DestroyList(List[i])==OK)
		 {
                printf("      线性表销毁成功！\n");
                for(;i<num;i++) //多线性表管理，需要将销毁的线性表删除覆盖，List[0]不存储信息
                    List[i]=List[i+1];
                num--;
		 }
         else printf("      线性表不存在，销毁失败！\n");
		 getchar();getchar();
		 break;
	   case 3:
		 if(ClearList(List[i])==OK) printf("      线性表清空成功！\n");
         else printf("      线性表不存在，清空失败！\n");
		 getchar();getchar();
		 break;
	   case 4:
		 if(!List[i].elem) printf("      该线性表不存在！\n");
         else if(ListEmpty(List[i])==TRUE) printf("      该线性表不为空表！\n");
         else printf("      该线性表为空表！\n");
		 getchar();getchar();
		 break;
	   case 5:
		 if(!List[i].elem) printf("      该线性表不存在！\n");
         else printf("      该线性表的长度为%d\n",ListLength(List[i]));
		 getchar();getchar();
		 break;
	   case 6:
         if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n"); getchar();getchar();break;}
         else if(!List[i].length) {printf("      该线性表为空表，请进行其他操作！\n"); getchar();getchar();break;}
         printf("      请输入您想查找的元素的序号，注意请不要输入小于1或者大于%d的整数：",List[i].length);
         scanf("%d",&j);
         if(GetElem(List[i],j,e)==ERROR) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功，第%d个元素值为%d\n",j,e);
		 getchar();getchar();
		 break;
	   case 7:
         if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n"); getchar();getchar();break;}
         printf("      请输入要查找的元素值：");
         scanf("%d",&e);
		 if(!LocateElem(List[i],e)) printf("      没有符合条件的元素存在！\n");
		 else printf("      符合条件的元素的位序为%d \n",LocateElem(List[i],e));
		 getchar();getchar();
		 break;
	   case 8:
         ElemType cur_e,pre_e;
		 if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n");getchar();getchar(); break;}
		 printf("      请输入想获得前驱的元素：");
		 scanf("%d",&cur_e);
         if(PriorElem(List[i],cur_e,pre_e)==ERROR-1) printf("      该元素不存在前驱!\n");//若首个元素符合条件，会返回-1
         else if(PriorElem(List[i],cur_e,pre_e)==ERROR) printf("      该元素不存在!\n");
         else printf("      它的前驱为%d\n",pre_e);
         getchar();getchar();
         break;
	   case 9:
		 ElemType next_e;
		 if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n");getchar();getchar(); break;}
		 printf("      请输入想获得后继的元素：");
		 scanf("%d",&cur_e);
         if(NextElem(List[i],cur_e,next_e)==ERROR-1) printf("      该元素不存在后继!\n");//若尾部元素符合条件，会返回-1
         else if(NextElem(List[i],cur_e,next_e)==ERROR) printf("      该元素不存在!\n");
         else printf("      它的后继为%d\n",next_e);
         getchar();getchar();
         break;
	   case 10:
         if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n");getchar();getchar(); break;}
         printf("      请输入插入的位置和插入元素值，注意请不要输入小于1或者大于%d的整数：",List[i].length+1);
         scanf("%d%d",&j,&e);
         if(!ListInsert(List[i],j,e)) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功!\n");
         getchar();getchar();
         break;
	   case 11:
		 if(!List[i].elem) {printf("      该线性表不存在，请进行其他操作！\n");getchar();getchar(); break;}
		 else if(!List[i].length) {printf("      该线性表为空表，请进行其他操作！\n");getchar();getchar(); break;}
         printf("      请输入想删除的元素的位置，注意请不要输入小于1或者大于%d的整数：",List[i].length);
         scanf("%d",&j);
         if(!ListDelete(List[i],j,e)) printf("      您的输入错误，操作失败！\n");
         else printf("      操作成功!\n");
         getchar();getchar();
         break;
	   case 12:
		 if(!ListTrabverse(List[i])) printf("      该线性表是空表，请进行其他操作！\n");
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
         printf("      %20s    %d\n",List[j].name,List[j].length);
         getchar();getchar();
	   case 0:
         break;
	}
  }
printf("\n欢迎下次再使用本系统！\n");
}
/*--------------------------------------------*/
status InitList(SqList &L)
{
	L.elem = (ElemType *)malloc( LIST_INIT_SIZE * sizeof (ElemType));//分配空间
    if(!L.elem) return OVERFLOW;
	L.length=0;
    L.listsize=LIST_INIT_SIZE;
	return OK;
}
/*--------------------------------------------*/
status DestroyList(SqList &L)
{
    int i=0;
    if(L.elem==NULL) return ERROR;
    free(L.elem); //释放空间
    L.elem=NULL;
    L.length=0;
    L.listsize=0;
    return OK;
}
/*--------------------------------------------*/
status ClearList(SqList &L)
{
    if(L.elem==NULL) return ERROR;
    L.length=0;
    return OK;
}
/*--------------------------------------------*/
status ListEmpty(SqList L)
{
    if(!L.elem) return ERROR-1;
    if(L.length) return TRUE;
    else return FALSE;
}
/*--------------------------------------------*/
int ListLength(SqList L)
{
    if(!L.elem) return ERROR-1;
    return L.length;
}
/*--------------------------------------------*/
status GetElem(SqList L,int i,ElemType &e)
{
    if(i<1||i>L.length) return ERROR;
    e=L.elem[i-1];
    return OK;
}
/*--------------------------------------------*/
status LocateElem(SqList L,ElemType e)
{
    int i;
    for(i=0;i<L.length;i++)
        if(L.elem[i]==e) return i+1;
    return 0;
}
/*--------------------------------------------*/
status PriorElem(SqList L,ElemType cur,ElemType &pre_e)
{
    int i;
    if(cur==L.elem[0]) return ERROR-1; //首元素为需要查找前驱的元素，返回-1，以便和ERROR区分
    for(i=1;i<L.length;i++) //遍历查找线性表
        if(L.elem[i]==cur){
            pre_e=L.elem[i-1];
            return OK;
        }
    return ERROR;//查找元素不存在，返回ERROR
}
/*--------------------------------------------*/
status NextElem(SqList L,ElemType cur,ElemType &next_e)
{
    int i;
    for(i=0;i<L.length-1;i++) //遍历查找线性表
        if(L.elem[i]==cur){
            next_e=L.elem[i+1];
            return OK;
        }
    if(cur==L.elem[L.length-1]) return ERROR-1; //判断最后一个元素
    return ERROR;//查找元素不存在，返回ERROR
}
/*--------------------------------------------*/
status ListInsert(SqList &L,int i,ElemType e)
{
    int j;
    if(!L.elem||i<1||i>L.length+1) return ERROR; //线性表不存在，或者输入位置不合法，返回ERROR
    if(L.length>=L.listsize) //存储空间不足，重新开辟空间
    {
        ElemType *newbase;
        newbase=(ElemType*)realloc(L.elem,(L.listsize+LISTINCREMENT)*sizeof(ElemType));
        if(!newbase) return OVERFLOW; //新空间开辟失败
        L.elem=newbase;
        L.listsize+=LISTINCREMENT;//线性表空间扩大
    }
    for(j=L.length;j>=i;--j)//移动元素
        L.elem[j]=L.elem[j-1];
    L.elem[i-1]=e;//插入元素
    ++L.length;//表长加一
    return OK;
}
/*--------------------------------------------*/
status ListDelete(SqList &L,int i,ElemType &e)
{
    int j;
    if(!L.elem||!L.length||i<1||i>L.length) return ERROR;//线性表不存在，线性表为空，或者输入位置不合法，返回ERROR
    e=L.elem[i-1]; //取出需要删除的元素
    for(j=i;j<=L.length-1;j++) //向前移动元素
        L.elem[j-1]=L.elem[j];
    L.length--; //表长减一
    return OK;
}
/*--------------------------------------------*/
status ListTrabverse(SqList L)
{
    int i;
    if(!L.elem) return ERROR;
    printf("\n----------------all elements --------------------\n");
    for(i=0;i<L.length;i++) printf("%d ",L.elem[i]); //遍历线性表，输出元素
    printf("\n------------------ end ------------------------\n");
    return OK;
}
/*--------------------------------------------*/
status OpenFile(SqList &L)
{
    FILE* fp; char filename[30]; int i;
    printf("      请输入文件名：");
    scanf("%s",filename);
    L.length=0;
    if (!(fp=fopen(filename,"r"))) {printf("      打开文件失败!\n"); return ERROR;} //文件赋值，打开文件
	while(fscanf(fp,"%d",&L.elem[L.length])!=EOF) //从文件中读取数据，遇到文件尾停止
    {
        L.length++;
        if(L.length>=L.listsize)//空间不足时开辟空间
        {
            ElemType *newbase;
            newbase=(ElemType*)realloc(L.elem,(L.listsize+LISTINCREMENT)*sizeof(ElemType));
            if(!newbase) {printf("      存储空间不足，线性表赋值失败!\n"); return OVERFLOW;}
            L.elem=newbase;
            L.listsize+=LISTINCREMENT;
        }
    }
    printf("      打开文件成功!\n");
    fclose(fp); //关闭文件
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SetValue(SqList &L) //键盘输入数据
{
    int i;
    printf("      请输入元素值并以Ctrl+Z结束:\n");
    for(L.length=0;;L.length++)
    {
        if(L.length>=L.listsize) //空间不足时开辟空间
        {
            ElemType* newbase;
            newbase=(ElemType*)realloc(L.elem,(L.listsize+LISTINCREMENT)*sizeof(ElemType));
            if(!newbase) return OVERFLOW;
            L.elem=newbase;
            L.listsize+=LISTINCREMENT;
        }
        if((scanf("%d",&L.elem[L.length])==EOF)) break;
    }
    printf("      赋值成功！\n");
    return OK;
}
/*--------------------------------------------*/
status SaveFile(SqList &L)
{
    FILE* fp;
    int i,num;
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
    for(i=0;i<L.length;i++) //遍历输出数据
        fprintf(fp,"%d ",L.elem[i]);
    fprintf(fp,"\n");
    fclose(fp);//关闭文件
    printf("      保存成功！\n");
    getchar();getchar();
    return OK;
}
/*--------------------------------------------*/











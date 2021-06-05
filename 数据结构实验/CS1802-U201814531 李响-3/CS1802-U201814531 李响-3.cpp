/*--------------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
/*--------------------------------------------*/
#define N 100
#define LIST_INIT_SIZE 2000
/*--------------------------------------------*/
typedef int status;
typedef int Sign;  //结点关键字元素类型定义
typedef int ElemType; //数据元素类型定义
/*--------------------------------------------*/
typedef struct BiTNode{
    Sign Node_num; //结点关键字变量
    ElemType data; //结点数据变量
    struct BiTNode *lchild; //左孩子指针变量
    struct BiTNode *rchild; //右孩子指针变量
}BiTNode,*LinkBiTNode; //LinkBiTNode为结点指针变量类型
typedef struct LNode{
    LinkBiTNode Node;
    struct LNode *next;
}LNode,*LinkList; //作为存储结点的链表（在输入序列时会直接生成链表）
typedef struct{
    LinkBiTNode root;
    char name[30];
}BiTrees; //森林线性表管理
typedef struct{
    LinkBiTNode Node;
    int num; //记录结点在满二叉树中的序号，便于进行树的图形化输出
}Node; //队列线性表，用于输存储结点信息
/*--------------------------------------------*/
status ReadBiNode(LinkList &front,LinkList &rear); //读入带空子树的前序遍历序列
status ReadBiNode2(LinkBiTNode a[],int &n); //读入前序或后续遍历序列
status ReadBiNode3(Sign b[],int &n);  //读入中序遍历序列
status CreateBiTree1(LinkBiTNode &T,LinkList &front,LinkList &rear); //用带空子树的前序遍历序列生成二叉树
status CreateBiTree2(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n); //用前序+中序遍历序列生成二叉树
status CreateBiTree3(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n); //用后序+中序遍历序列生成二叉树
status DestroyBiTree(LinkBiTNode &T); //销毁二叉树
status ClearBiTree(LinkBiTNode &T);   //清空二叉树
status BiTreeEmpty(LinkBiTNode T);    //判断空二叉树
int BiTreeDepth(LinkBiTNode T);       //求二叉树深度
LinkBiTNode LocateNode(LinkBiTNode T,Sign e);        //查找结点
status Assign(LinkBiTNode T,Sign &e,ElemType value); //结点赋值
LinkBiTNode GetSibling(LinkBiTNode T,Sign e);  //获得兄弟结点
status InsertNode(LinkBiTNode T,Sign &e,int LR,LinkBiTNode c);
status DeleteNode(LinkBiTNode &T,Sign &e);
status PreOrderTraverse(LinkBiTNode T);
status InOrderTraverse(LinkBiTNode T);
status PostOrderTraverse(LinkBiTNode T);
status LevelOrderTraverse(LinkBiTNode T); //按层遍历二叉树
status OpenFile(LinkList &front,LinkList &rear); //从文件中读取序列
status SaveFile(BiTrees T); //将二叉树信息以带空子树的前序序列输出到文件中
status ShowTree(LinkBiTNode T); //图形化显示树的信息
int pow(int num,int n); //自定义平方函数，用于计算整数的平方
status Assign_name(BiTrees &T,BiTrees a[],int num);
/*--------------------------------------------*/
int main(){
BiTrees BiTList[N]; //森林（多树）线性表管理
int op=1; //菜单选项读入变量
int i=0,j,k; //计数器，用于计数二叉树的位置
int num=0,n1,n2; //二叉树的个数变量，以及前序、后续以及中序的长度
Sign e;ElemType value; char name[30];
LinkBiTNode locate;
LinkList front,rear;
LinkBiTNode a[LIST_INIT_SIZE];
Sign b[LIST_INIT_SIZE];
while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Binary Tree On Binary Linked List Structure \n");
	printf("--------------------------------------------------------------\n");
	printf("    	  1. CreateBiTree       9. InsertNode\n");
	printf("    	  2. DestroyBiTree      10. DeleteNode\n");
	printf("    	  3. ClearBiTree        11. PreOrderTraverse\n");
	printf("    	  4. BiTreeEmpty        12. InOrderTraverse\n");
	printf("    	  5. BiTreeDepth        13. PostOrderTraverse\n");
	printf("    	  6. LocateNode         14. LevelOrderTraverse\n");
	printf("    	  7. Assign             15. SaveFile\n");
	printf("    	  8. GetSibling         16. BiTreeMessage\n"); //显示所有二叉树名字以及深度信息
	printf("    	  0. Exit               17. ShowTree\n"); //用于显示图形化的
	printf("--------------------------------------------------------------\n");
	printf("      请选择你的操作[0~16]:");
	scanf("%d",&op);
	if(op>=2&&op<=15||op==17)
    {
        if(num)  //num变量代表二叉树的数量，此处判断是否有二叉树
        {
            printf("      请输入您要操作的二叉树的名称：");//通过名称查找二叉树
            scanf("%s",name);
            for(i=1;i<=num;i++) //遍历查找
                if(strcmp(BiTList[i].name,name)==0) break;
            if(i>num) {printf("      您查找的二叉树不存在，请重新操作：\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      当前无二叉树存在，请先创建二叉树！\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
        int cho,cho_1; //用于储存创建二叉树方式的计数器
        printf("      请选择创建二叉树的方式：\n");
        printf("          1.使用带空子树的二叉树前序遍历序列生成二叉树\n");
        printf("          2.使用二叉树前序遍历序列+中序遍历序列生成二叉树\n");
        printf("          3.使用二叉树后序遍历序列+中序遍历序列生成二叉树\n      ");
        scanf("%d",&cho);
        switch(cho){
            case 1:
                printf("      请选择读入序列的方式：\n");
                printf("          1.使用键盘赋值\n");
                printf("          2.使用文件赋值\n      ");
                scanf("%d",&cho_1);
                if(cho_1==1)
                {
                    printf("      请输入合带空子树的前序遍历序列，序列的每一项由关键字-数据两项组成，\n");
                    printf("      中间用空格隔开，两项之间用换行符隔开，关键字为唯一确定的正整数，\n");
                    printf("      数据为任意整数，空子树关键字用-1代替，空子树数据项请输入0，输入以Ctrl+Z结束：\n");
                    k=ReadBiNode(front,rear); //读入带空子树的前序遍历序列
                    if(k==OVERFLOW) printf("      溢出！");
                }
                if(cho_1==2)
                {
                    k=OpenFile(front,rear); //打开文件读取带空子树的前序遍历序列
                    if(k==OVERFLOW) printf("      溢出！");
                    if(k==ERROR) printf("      打开文件失败!\n");
                    if(k==OK) printf("      打开文件成功!\n");
                }
                j=CreateBiTree1(BiTList[++num].root,front,rear); //用带空子树的前序遍历序列创建二叉树
                if(j==ERROR) //当输入非法时
                {
                    while(j==ERROR)//一直显示重新输入直到输入正确
                    {
                        num--; //森林中树的个数减一
                        printf("      输入非法，请重新输入:");
                        ReadBiNode(front,rear); //重新读入序列
                        j=CreateBiTree1(BiTList[++num].root,front,rear);
                    }
                }
                if(j==OK)
                {
                    printf("      二叉树创建成功！请对二叉树进行命名：");
                    while(!Assign_name(BiTList[num],BiTList,num)) //采用命名函数来进行赋值
                        printf("      该二叉树名称已存在，请重新输入：");
                    printf("      二叉树命名成功！\n");
                }
                break;
	        case 2: //采用前序遍历序列以及中序遍历序列进行创建树（本功能仅供参考）
	            printf("      请输入合法的前序遍历序列，序列的每一项由关键字-数据两项组成，中间用空格隔开，\n");
                printf("      两项之间用换行符隔开（关键字为唯一确定的正整数，数据为任意整数）输入以Ctrl+Z结束：\n");
                k=ReadBiNode2(a,n1); //读入前序遍历序列
                printf("      请输入合法的中序遍历序列，序列的每一项只含有关键字，\n");
                printf("      两项之间用空格隔开（关键字为唯一确定的正整数）输入以Ctrl+Z结束：\n");
                k=ReadBiNode3(b,n2); //读入中序遍历序列
                if(n1!=n2) {printf("      输入非法，请进行其他操作！\n"); break;}
                j=CreateBiTree2(BiTList[++num].root,a,b,n1);
                if(j==OK)
                {
                    printf("      二叉树创建成功！请对二叉树进行命名：");
                    while(!Assign_name(BiTList[num],BiTList,num))
                        printf("      该二叉树名称已存在，请重新输入：\n");
                    printf("      二叉树命名成功！\n");
                }
                else
                {
                    printf("      输入非法，请进行其他操作！\n");
                    num--;
                }
	            break;
	        case 3:  //采用后序遍历序列以及中序遍历序列进行创建树（本功能仅供参考）
                printf("      请输入合法的后序遍历序列，序列的每一项由关键字-数据两项组成，中间用空格隔开，\n");
                printf("      两项之间用换行符隔开（关键字为唯一确定的正整数，数据为任意整数）输入以Ctrl+Z结束：\n");
                k=ReadBiNode2(a,n1); //读入后序遍历序列
                printf("      请输入合法的中序遍历序列，序列的每一项只含有关键字，\n");
                printf("      两项之间用空格隔开（关键字为唯一确定的正整数）输入以Ctrl+Z结束：\n");
                k=ReadBiNode3(b,n2); //读入中序遍历序列
                j=CreateBiTree3(BiTList[++num].root,a,b,n1);
                if(j==OK)
                {
                    printf("      二叉树创建成功！请对二叉树进行命名：");
                    while(!Assign_name(BiTList[num],BiTList,num))
                        printf("      该二叉树名称已存在，请重新输入：\n");
                    printf("      二叉树命名成功！\n");
                }
                else
                {
                    printf("      输入非法，请进行其他操作！\n");
                    num--;
                }
	            break;
        }
        getchar();getchar();
        break;
	   case 2:
         if(DestroyBiTree(BiTList[i].root)==OK)
		 {
                printf("      二叉树销毁成功！\n");
                for(;i<num;i++)  //多二叉树管理，需要将销毁的二叉树删除覆盖，BiTList[0]不存储信息
                    BiTList[i]=BiTList[i+1];
                num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
         if(ClearBiTree(BiTList[i].root)==OK)
            printf("      二叉树清空成功！\n");
         else printf("      此二叉树已经为空！\n");
		 getchar();getchar();
		 break;
	   case 4:
         if(BiTreeEmpty(BiTList[i].root)==TRUE)
            printf("      该二叉树为空！\n");
         else printf("      该二叉树不为空！\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      该二叉树的深度为%d\n",BiTreeDepth(BiTList[i].root));
		 getchar();getchar();
		 break;
	   case 6:
         printf("      请输入您想查找的结点的关键字（正整数）：");
         scanf("%d",&e);
         locate=LocateNode(BiTList[i].root,e);
         if(locate) printf("      操作成功，您查找的结点数据项为%d\n",locate->data);
         else printf("      您所查找的结点不存在，操作失败！\n");
		 getchar();getchar();
		 break;
	   case 7:
         printf("      请输入您想赋值的结点的关键字（正整数）及数据项：");
         scanf("%d %d",&e,&value);
         Assign(BiTList[i].root,e,value);
         if(e==-1) printf("      结点赋值成功！\n"); //若赋值操作正常进行，e的赋值会发生改变（详见函数）
         else printf("      您所赋值的结点不存在，操作失败！\n");
		 getchar();getchar();
		 break;
	   case 8:
         printf("      请输入您想获取兄弟结点的结点的关键字（正整数）：");
         scanf("%d",&e);
         locate=GetSibling(BiTList[i].root,e);   //locate用于存储返回的指针值
         if(locate) printf("      操作成功，您想获取的兄弟结点的关键字为%d,数据项为%d\n",locate->Node_num,locate->data);
         else if(!LocateNode(BiTList[i].root,e)) printf("      该结点不存在，操作失败！\n"); //非法情况，结点不存在
         else printf("      兄弟结点不存在，操作失败！\n"); //非法情况，兄弟结点结点不存在
         getchar();getchar();
         break;
	   case 9:
         int LR; ElemType a; Sign b;
         LinkBiTNode c; //指向生成的结点
         printf("      请输入插入结点的父节点关键字（正整数）：");
         scanf("%d",&e);
         if(!LocateNode(BiTList[i].root,e)) //判断插入位点是否存在
         {
             printf("      该插入位点不存在，操作失败！\n");
             getchar();getchar();
             break;
         }
         printf("      若插入结点为左结点，请输入0；若为右结点，则输入1：");
         scanf("%d",&LR);
         if(LR!=0&&LR!=1)
         {
             printf("      LR输入错误，操作失败！\n");
             getchar();getchar();
             break;
         }
         printf("      请输入结点信息，信息有关键字-数据两项组成，中间用空格隔开：");
         while(scanf("%d %d",&b,&a)!=EOF&&LocateNode(BiTList[i].root,b))
              printf("      该关键字已使用，请重新输入关键字与数据项:"); //判断当前关键字是否合法
         c=(LinkBiTNode)malloc(sizeof(BiTNode)); //生成结点
         c->Node_num=b;
         c->data=a;
         InsertNode(BiTList[i].root,e,LR,c);
         if(e==-1) printf("      结点插入成功！\n");
         else printf("      插入位点不存在，操作失败！\n");
         getchar();getchar();
         break;
	   case 10:
         printf("      请输入您想删除的结点的关键字（正整数）：");
         scanf("%d",&e);
         DeleteNode(BiTList[i].root,e);
         if(e==-1) printf("      结点删除成功！\n");
         else printf("      需要删除的结点不存在，操作失败！\n");
         getchar();getchar();
         break;
	   case 11:
         printf("\n----------------all elements --------------------\n");
         PreOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
	   case 12:
         printf("\n----------------all elements --------------------\n");
         InOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
		 getchar();getchar();
		 break;
       case 13:
         printf("\n----------------all elements --------------------\n");
         PostOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 14:
         printf("\n----------------all elements --------------------\n");
         LevelOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 15:
         SaveFile(BiTList[i]);
         getchar();getchar();
         break;
       case 16:
         if(!num) {printf("      当先无二叉树存在，请进行其他操作：\n"); getchar();getchar(); break;} //判断有无线性表
         printf("      当先二叉树信息如下：\n");
         printf("      二叉树个数为%d\n",num);
         printf("      二叉树的名称及深度如下：\n");
         for(j=1;j<=num;j++)    //遍历线性表数组，输出数据
         printf("      %20s    %d\n",BiTList[j].name,BiTreeDepth(BiTList[j].root));
         getchar();getchar();
         break;
       case 17:
         ShowTree(BiTList[i].root);   //图形化显示树
         getchar();getchar();
         break;
	   case 0:
         break;
	}
  }
printf("\n欢迎下次再使用本系统！\n");
}
/*--------------------------------------------*/
 //序列读入函数，读入带空子树的前序遍历序列，并生成结点串，本函数采用链表存储结点
status ReadBiNode(LinkList &front,LinkList &rear)
{
    LinkList pNew;
    ElemType Ndata;
    Sign Snum;
    front=(LinkList)malloc(sizeof(LNode));
    if(!front) return OVERFLOW;
    front->Node==NULL;  //链表结点由一个指向树的指针变量域以及一个指针域
    front->next=NULL;
    rear=front; //链表生成
    while(scanf("%d %d",&Snum,&Ndata)!=EOF) //读入序列
    {
        pNew=(LinkList)malloc(sizeof(LNode)); //生成新的链表结点
        if(!pNew) return OVERFLOW;
        pNew->Node=(LinkBiTNode)malloc(sizeof(BiTNode)); //生成新的树结点
        pNew->next=NULL;
        pNew->Node->Node_num=Snum; //对树结点进行赋值
        pNew->Node->data=Ndata;
        rear->next=pNew; //入线性表
        rear=pNew; //尾指针后移
    }
    return OK;
}
/*--------------------------------------------*/
//序列读入函数，读入普通前序遍历序列，并生成结点串，本函数采用线性表存储结点
status ReadBiNode2(LinkBiTNode a[],int &n)
{
    int i=0;
    LinkBiTNode pNew;
    ElemType Ndata;
    Sign Snum;
    while(scanf("%d %d",&Snum,&Ndata)!=EOF)
    {
        pNew=(LinkBiTNode)malloc(sizeof(BiTNode)); //生成新的结点
        if(!pNew) return OVERFLOW;
        pNew->Node_num=Snum;
        pNew->data=Ndata;
        a[i++]=pNew;    //将结点插入线性表
    }
    n=i; //变量n用于返回长度，便于判断是否合法
    return OK;
}
/*--------------------------------------------*/
status ReadBiNode3(Sign b[],int &n) //序列读入函数，读入中序遍历序列，本函数采用数组存储结点
{
    int i=0;
    while(scanf("%d",&b[i++])!=EOF);
    n=i-1; //变量n用于返回长度，便于判断是否合法
    return OK;
}
/*--------------------------------------------*/
status CreateBiTree1(LinkBiTNode &T,LinkList &front,LinkList &rear) //二叉树创建函数，使用带空子树的前序遍历序列创建二叉树
{
    int flag=1;  //判断变量，若flag==1，则插入为左结点；若flag==0，则插入为右结点
    LinkList head=front->next,top=front,temp; //head指针指向为接入树的结点串，top指针用于指向一个模拟栈
    LinkBiTNode root; //root指向需要进行子结点赋值的结点
    if(!front->next) return ERROR;
    T=head->Node;     //插入第一个结点
    top=NULL;         //栈为空
    temp=head->next;  //指向下一个需要处理的结点
    head->next=top;   //结点入栈
    top=head;         //结点入栈
    while(top&&temp)  //当栈不为空且有需要处理的结点
    {
        root=top->Node;
        if(!flag) top=top->next; //遇到空子结点，将结点出栈
        head=temp;    //便于将结点入栈
        if(head->Node->Node_num!=-1) //若插入的结点不为空
        {
            if(flag) root->lchild=head->Node; //由于只有在左结点为空时，才会操作右结点，故flag为1时一直插入在左边
            else root->rchild=head->Node;
            temp=head->next; //指向下一个需要操作的结点
            head->next=top;  //结点入栈
            top=head;        //结点入栈
            flag=1;          //下一个结点仍插入在左边
        }
        else //若插入的结点为空
        {
            if(flag) root->lchild=NULL;
            else root->rchild=NULL;
            temp=head->next;
            flag=0;         //下一个结点插入在右边
        }
    }
    //非法情况判断
    if(top&&!temp) return ERROR; //非法情况1：栈为空但仍有结点需要插入
    if(!top&&temp)               //非法情况2：栈不为空但没有结点可以插入
    {
        DestroyBiTree(T);
        return ERROR;
    }
    return OK;
}
/*--------------------------------------------*/
status CreateBiTree2(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n) //二叉树创建函数，使用前序+中序遍历序列创建二叉树
{
    int i=0;
    if(n==0){
        T=NULL;
        return OK;
    }   //若序列为空，将结点赋值为空
	for(i=0;i<n;i++)  //查找结点
    {
        if(b[i]==a[0]->Node_num)//查找成功
        {
            T=a[0]; //将结点插入
            if(CreateBiTree2(T->lchild,&a[1],b,i)==ERROR) return ERROR; //左结点查找
            if(CreateBiTree2(T->rchild,&a[1+i],&b[i+1],n-i-1)==ERROR) return ERROR; //右结点查找
            break;
        }
    }
    if(i>=n) return ERROR;
    else return OK;
}
/*--------------------------------------------*/
status CreateBiTree3(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n)
{
    int i=0;
	if(n==0) { T=NULL; return OK; }
    for(i=0;i<n;i++)
    {
        if (b[i]==a[n-1]->Node_num)
        {
            T=a[n-1];
            if(CreateBiTree3(T->lchild,a,b,i)==ERROR) return ERROR; //左结点查找
            if(CreateBiTree3(T->rchild,&a[1+i],&b[i+1],n-i-1)==ERROR) return ERROR; //右结点查找
            break;
        }
    }
	if(i>=n) return ERROR;
    else return OK;
}
/*--------------------------------------------*/
status DestroyBiTree(LinkBiTNode &T)
{
    if(!T) return OK;
    else
    {
        DestroyBiTree(T->rchild);
        DestroyBiTree(T->lchild);
        free(T); //释放空间
        T=NULL;  //结点指向空地址
        return OK;
    }
}
/*--------------------------------------------*/
status ClearBiTree(LinkBiTNode &T)
{
    if(!T) return ERROR;
    else
    {
        ClearBiTree(T->rchild);
        ClearBiTree(T->lchild);
        free(T); //释放空间
        T=NULL;  //结点指向空地址
        return OK;
    }
}
/*--------------------------------------------*/
status BiTreeEmpty(LinkBiTNode T)
{
    if(!T) return TRUE;
    else return FALSE;
}
/*--------------------------------------------*/
int BiTreeDepth(LinkBiTNode T)
{
    int ldepth,rdepth;
    if(!T) return 0; //若遇到空结点，则直接返回0
    ldepth=BiTreeDepth(T->lchild)+1;
    rdepth=BiTreeDepth(T->rchild)+1;
    if(ldepth>=rdepth) return ldepth;
    else return rdepth; //返回深度更大的数值

}
/*--------------------------------------------*/
LinkBiTNode LocateNode(LinkBiTNode T,Sign e)
{
    LinkBiTNode lp,rp;
    if(!T) return NULL; //递归出口1，找到空结点
    if(e==T->Node_num) return T; //递归出口2，找到结点
    lp=LocateNode(T->lchild,e);
    if(lp) return lp;
    rp=LocateNode(T->rchild,e);
    if(rp) return rp;
    return NULL;
}
/*--------------------------------------------*/
status Assign(LinkBiTNode T,Sign &e,ElemType value)
{
    LinkBiTNode lp,rp;
    if(!T) return OK;
    if(e==T->Node_num) //结点查找成功
    {
        T->data=value;
        e=-1;    //标记赋值
        return OK;
    }
    Assign(T->lchild,e,value); //递归查找左结点
    Assign(T->rchild,e,value); //递归查找右结点
    return OK;
}
/*--------------------------------------------*/
LinkBiTNode GetSibling(LinkBiTNode T,Sign e)
{
    LinkBiTNode lp,rp;
    if(!T||!T->lchild||!T->rchild) return NULL; //可以减少递归次数
    if(e==T->lchild->Node_num) return T->rchild;
    else if(e==T->rchild->Node_num) return T->lchild;
    lp=GetSibling(T->lchild,e);
    if(lp) return lp;
    rp=GetSibling(T->rchild,e);
    if(rp) return rp;
    return NULL;
}
/*--------------------------------------------*/
status InsertNode(LinkBiTNode T,Sign &e,int LR,LinkBiTNode c)
{
    LinkBiTNode temp;
    if(!T) return OK;
    if(e==T->Node_num) //成功查找到结点
    {
        e=-1;   //用于标记是否已经处理
        if(!LR) //LR==0则插入为左结点
        {
            temp=T->lchild;
            T->lchild=c;
        }
        else if(LR==1)  //LR==1则插入为右结点
        {
            temp=T->rchild;
            T->rchild=c;
        }
        else return ERROR;
        c->rchild=temp;
        c->lchild=NULL;
        return OK;
    }
    if(!InsertNode(T->lchild,e,LR,c)) return ERROR;
    if(!InsertNode(T->rchild,e,LR,c)) return ERROR;
    return OK;
}
/*--------------------------------------------*/
status DeleteNode(LinkBiTNode &T,Sign &e)
{
    LinkBiTNode temp,p;
    if(!T) return OK;
    if(e==T->Node_num)
    {
        e=-1;  //用于标记是否已经处理
        temp=T;
        if(temp->lchild) //需要删除的结点的左孩子存在
        {
            T=temp->lchild; //用左结点替代需要删除的结点
            if(temp->rchild) //右结点存在
            {
                p=temp->lchild;
                while(p->rchild) p=p->rchild; //查找左子树的最右结点
                p->rchild=temp->rchild;   //将子树插入
                free(temp);
            }
            else free(temp);
        }
        else
        {
            if(temp->rchild) T=temp->rchild; //只有一个结点存在情况，直接替代
            else T=NULL;
        }
        return OK;
    }
    DeleteNode(T->lchild,e);
    DeleteNode(T->rchild,e);
    return OK;
}
/*--------------------------------------------*/
status PreOrderTraverse(LinkBiTNode T) //非递归实现的前序遍历输出
 {
     int k=0;
     LinkBiTNode st[LIST_INIT_SIZE],temp=T; //使用栈来控制
     if(!T) return ERROR; //空树
     while(k||temp)   //栈不为空或者当前结点未处理
     {
         while(temp)  //循环查找左子树
         {
             st[k++]=temp; //入栈
             printf("%d ",temp->data); //输出结点
             temp=temp->lchild; //访问左子树
         }
         temp=st[--k]; //左孩子为空，将当前结点赋给需要处理的temp，出栈
         temp=temp->rchild; //指向右结点
     }
     return OK;
 }
/*--------------------------------------------*/
status InOrderTraverse(LinkBiTNode T) //非递归实现的中序遍历输出，与前序类似
{
    int k=0;
    LinkBiTNode st[LIST_INIT_SIZE],temp=T;
    if(!T) return ERROR;
    while(k||temp)
    {
        while(temp)
        {
            st[k++]=temp;
            temp=temp->lchild;
        }
        temp=st[--k];
        printf("%d ",temp->data);//结点在出栈时输出，前序遍历是入栈时输出
        temp=temp->rchild;
    }
    return OK;
}
/*--------------------------------------------*/
status PostOrderTraverse(LinkBiTNode T)
{
    if(!T) return OK;
    else
    {
        PostOrderTraverse(T->lchild); //递归访问左子树
        PostOrderTraverse(T->rchild); //递归访问右子树
        printf("%d ",T->data);        //访问根指针
    }
    return OK;
}
/*--------------------------------------------*/
status LevelOrderTraverse(LinkBiTNode T) //按层遍历，采用一个由链表模拟的队列实现
{
    LinkList front,rear,pNew; //队头指针、队尾指针以及当前指针变量
    LinkBiTNode temp=T;
    front=(LinkList)malloc(sizeof(LNode));
    front->Node=T;   //生成首个结点（本链表不包含头结点）
    front->next=NULL;
    rear=front;
    printf("%d ",T->data);
    while(front) //队非空
    {
        temp=front->Node;
        if(temp->lchild) //左结点入队
        {
            pNew=(LinkList)malloc(sizeof(LNode));
            pNew->Node=temp->lchild;
            pNew->next=NULL;
            rear->next=pNew;
            rear=pNew;
            printf("%d ",rear->Node->data);
        }
        if(temp->rchild) //右结点入队
        {
            pNew=(LinkList)malloc(sizeof(LNode));
            pNew->Node=temp->rchild;
            pNew->next=NULL;
            rear->next=pNew;
            rear=pNew;
            printf("%d ",rear->Node->data);
        }
        front=front->next; //结点出队
    }
    return OK;
}
/*--------------------------------------------*/
status OpenFile(LinkList &front,LinkList &rear)//与读入函数相似，仅改为读取文件模式
{
    LinkList pNew;
    ElemType Ndata;
    Sign Snum;
    FILE* fp; char filename[30]; int i;
    printf("      请输入文件名：");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) return ERROR;
    front=(LinkList)malloc(sizeof(LNode));
    if(!front) return OVERFLOW;
    front->Node==NULL;
    front->next=NULL;
    rear=front;
    while(fscanf(fp,"%d %d",&Snum,&Ndata)!=EOF)
    {
        pNew=(LinkList)malloc(sizeof(LNode));
        if(!pNew) return OVERFLOW;
        pNew->Node=(LinkBiTNode)malloc(sizeof(BiTNode));
        pNew->next=NULL;
        pNew->Node->Node_num=Snum;
        pNew->Node->data=Ndata;
        rear->next=pNew;
        rear=pNew;
    }
    fclose(fp); //关闭文件
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SaveFile(BiTrees T)
{
    FILE* fp;
    int num;
    int k=0;
    LinkBiTNode st[LIST_INIT_SIZE],temp=T.root;
    char filename[30];
    printf("      请选择：1.不删除原有的二叉树，仅对文件进行追加\n              2.删除原有数据或生成新文件\n\n");
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
    fputs(T.name,fp);
    fprintf(fp,"\n");//换行
    while(k||temp)
    {
        while(temp)
        {
            st[k++]=temp;
            fprintf(fp,"%d ",temp->Node_num);
            fprintf(fp,"%d\n",temp->data);
            temp=temp->lchild;
        }
        fprintf(fp,"-1 0\n");
        temp=st[--k];
        temp=temp->rchild;
    }
    fprintf(fp,"-1 0\n");
    fprintf(fp,"\n\n");
    fclose(fp);//关闭文件
    printf("      保存成功！\n");
    return OK;
}
/*--------------------------------------------*/
status ShowTree(LinkBiTNode T) //图形化显示树，本函数采用队列，可以进行文件输出
{
    FILE* fp;
    char filename[30];
    int i,front=0,front2=0,rear=0,j,k,op;
    int n=BiTreeDepth(T);
    Node num[2000];
    LinkBiTNode temp=T;
    num[rear].Node=temp;//用于存储结点
    num[rear++].num=1;  //num变量存储结点在满二叉树中的序号
    while(front<rear)
    {
        temp=num[front].Node;
        if(temp->lchild) //左孩子入队
        {
            num[rear].Node=temp->lchild;
            num[rear++].num=num[front].num*2;   //由满二叉树的性质可知，左孩子的序号为父结点序号的两倍
        }
        if(temp->rchild) //右孩子入队
        {
            num[rear].Node=temp->rchild;
            num[rear++].num=num[front].num*2+1; //由满二叉树的性质可知，右孩子的序号为父结点序号的两倍+1
        }
        front++;
    }
    printf("      请选择输出树的方式：\n");
    printf("          1.在屏幕显示（不建议显示深度超过5的树）\n");
    printf("          2.保存到文件\n");
    scanf("%d",&op);
    if(op==2)
    {
        printf("      请输入文件名：");
        scanf("%s",filename);
        if((fp=fopen(filename,"w"))==NULL) //文件打开方式为更新或创建新文件，会删除原有数据
        {
            printf("      文件文件打开失败！\n");
            return ERROR;
        }
    }
    for(i=0,front=0;i<n;i++)  //i的数值表示二叉树的层数减1，按层循环遍历
    {
        for(k=0;k<pow(2,n-i)-2;k++) //打印出每一行最开始的空格
        {
            if(op==2) fprintf(fp," ");
            else printf(" ");
        }
        for(j=pow(2,i);j<pow(2,i+1);j++) //第二层遍历，按照满二叉树的序号遍历
        {
            if(j<num[front].num) //若当前序号表示的结点为空，则打印空格
                for(k=0;k<pow(2,n-i+1);k++) //循环打印空格
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
            else if(j==num[front].num) //若当前序号表示的结点存在
            {
                if(num[front].Node->lchild)  //若左孩子存在，则打印下划线
                    for(k=0;k<pow(2,n-i)-2;k++)
                    {
                        if(op==2) fprintf(fp,"_");
                        else printf("_");
                    }
                else for(k=0;k<pow(2,n-i)-2;k++) //若左孩子不存在，则打印空格
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                if(op==2) fprintf(fp,"%04d",num[front].Node->Node_num); //打印结点
                else printf("%04d",num[front].Node->Node_num);
                if(num[front].Node->rchild) //若右孩子存在，则打印下划线
                    for(k=0;k<pow(2,n-i)-2;k++)
                    {
                        if(op==2) fprintf(fp,"_");
                        else printf("_");
                    }
                else for(k=0;k<pow(2,n-i)-2;k++)//若右孩子不存在，则打印空格
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                front++;
            }
            for(k=0;k<pow(2,n-i+1);k++) //打印两个结点之间的空格
            {
                if(op==2) fprintf(fp," ");
                else printf(" ");
            }
        }
        if(op==2) fprintf(fp,"\n"); //换行
        else printf("\n");
        for(k=0;k<pow(2,n-i)-3;k++)  //打印出每一行最开始的空格
        {
            if(op==2) fprintf(fp," ");
            else printf(" ");
        }
        for(j=pow(2,i);j<pow(2,i+1);j++)  //第二层遍历，按照满二叉树的序号遍历，输出箭头，下面格式与第一次循环输出类似
        {
            if(j<num[front2].num)
            {
                for(k=0;k<pow(2,n-i+1)+2;k++)
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
            }
            else if(j==num[front2].num)
            {
                if(num[front2].Node->lchild)
                {
                    if(op==2) fprintf(fp,"↓");
                    else printf("↓");
                }
                else
                {
                    if(op==2) fprintf(fp,"  ");
                    else printf("  ");
                }
                for(k=0;k<pow(2,n-i+1)-2;k++)
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                if(num[front2].Node->rchild)
                {
                    if(op==2) fprintf(fp,"↓");
                    else printf("↓");
                }
                else
                {
                    if(op==2) fprintf(fp,"  ");
                    else printf("  ");
                }
                front2++;
            }
            for(k=0;k<pow(2,n-i+1)-2;k++)
            {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
            }
        }
        if(op==2) fprintf(fp,"\n");
        else printf("\n");
    }
    if(op==2)
    {
        fclose(fp);//关闭文件
        printf("      保存成功！\n");
    }
    return OK;
}
/*--------------------------------------------*/
int pow(int num,int n)
{
    int i,result=1;
    for(i=0;i<n;i++)
        result=result*num;
    return result;
}
/*--------------------------------------------*/
status Assign_name(BiTrees &T,BiTrees a[],int num)
{
    int i;
    char name[30];
    scanf("%s",name);
    for(i=1;i<num;i++)
        if(strcmp(a[i].name,name)==0) break;
    if(i>=num)
    {
        strcpy(T.name,name);
        return OK;
    }
    else return ERROR;
}
/*--------------------------------------------*/


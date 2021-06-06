#include "head.h"

/*-------------------------------------------------
    函数名：ReadFile
    功能：读取CNF文件并生成可操作的CNF公式；
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_liter（int类型）    文字数，引用传递可修改；
        num_of_clause（int类型）   子句数，引用传递可修改；
        filename[]（char类型）     文件名；
    返回值：OK/ERROR；
-------------------------------------------------*/
status ReadFile(Clause* root,int &num_of_liter,int &num_of_clause,char filename[])
{
    int i,n,num;
    FILE* fp;
    char str[1024],c;
    printf("      请输入CNF文件的文件路径/文件名：");
    scanf("%s",filename);
    if(!(fp=fopen(filename,"r"))) return ERROR;  //文件打开失败，返回ERROR
    printf("      CNF文件的注释内容如下：\n");
    while(fscanf(fp,"%c",&c)!=EOF) //循环读入注释，并输出注释信息
    {
        if(c=='c')
        {
            printf("      %c",c);
            while(fscanf(fp,"%c",&c)!=EOF&&c!='\n')
                printf("%c",c);
            printf("\n");
        }
        else
        {
            fscanf(fp,"%s",str);
            fscanf(fp,"%d %d",&num_of_liter,&num_of_clause);
            printf("\n      CNF文件中的文字数为：%-8d 公式数为：%-8d\n",num_of_liter,num_of_clause);
            break;
        }
    }
    for(i=0;i<num_of_clause;i++) //循环读入子句信息
    {
        n=0;
        while(fscanf(fp,"%d",&num)!=EOF&&num!=0)
            liter[n++]=num;
        addClause(root,createClause(n,liter));  //创建并添加子句
    }
    for(i=0;i<=num_of_liter;i++) liter[i]=0;    //刷新数组，以供他用
    fclose(fp);
    return OK;
}

/*-------------------------------------------------
    函数名：createClause
    功能：创建CNF公式子句（带头结点）；
    输入参数：
        n（int类型）       子句中变元的个数；
        lit[]（int类型）   存储文字信息的数组；
    返回值：子句头指针；
-------------------------------------------------*/
Clause* createClause(int n,int lit[])
{
    int i;
    Clause* head;
    Literal* pNew;
    head=(Clause*)malloc(sizeof(Clause));
    head->first=(Literal*)malloc(sizeof(Literal)); //生成头结点
    head->first->next=NULL;
    head->next=NULL;
    for(i=0;i<n;i++) //生成表结点
    {
        pNew=(Literal*)malloc(sizeof(Literal));
        pNew->liter=lit[i];
        pNew->next=head->first->next;
        head->first->next=pNew;
    }
    return head;
}

/*-------------------------------------------------
    函数名：destroyClause
    功能：删除CNF公式子句；
    输入参数：
        p（Clause*类型）  子句头指针；
    返回值：OK/ERROR；
-------------------------------------------------*/
status destroyClause(Clause* p)
{
    Literal *pl1,*pl2;
    pl1=p->first;
    while(pl1)
    {
        pl2=pl1->next;
        free(pl1);
        pl1=pl2;
    }
    free(p);
    return OK;
}

/*-------------------------------------------------
    函数名：addClause
    功能：加入CNF公式子句；
    输入参数：
        pNew（Clause*类型）  子句头指针；
        root（Clause*类型）  CNF公式头指针；

    返回值：OK/ERROR；
-------------------------------------------------*/
status addClause(Clause* root,Clause* pNew)
{
    pNew->next=root->next;
    root->next=pNew;
    return OK;
}

/*-------------------------------------------------
    函数名：isUnitClause
    功能：判断是否为单子句
    输入参数：
        p（Clause*类型）  子句头指针；
    返回值：TRUE/FALSE；
-------------------------------------------------*/
status isUnitClause(Clause* p)
{
    if(!p->first->next) return FALSE;
    if(!p->first->next->next) return TRUE;
    return FALSE;
}

/*-------------------------------------------------
    函数名：printClause
    功能：打印CNF公式
    输入参数：
        root（Clause*类型） CNF公式头指针；
    返回值：OK/ERROR；
-------------------------------------------------*/
status printClause(Clause* root)
{
    Clause* p=root->next;
    Literal* q;
    while(p)
    {
        q=p->first->next;
        while(q)
        {
            printf("%d ",q->liter);
            q=q->next;
        }
        printf("0\n");
        p=p->next;
    }
    return OK;
}

/*-------------------------------------------------
    函数名：copyList
    功能：复制CNF公式
    输入参数：
        root（Clause*类型） CNF公式头指针；
    返回值：复制的CNF公式的头指针值；
-------------------------------------------------*/
Clause* copyList(Clause* root)
{
    Clause *pC=root->next;
    Clause *rNew=(Clause*)malloc(sizeof(Clause)),*pNew;
    Literal *pL,*pLNew;
    rNew->first=NULL;
    rNew->next=NULL;
    while(pC)
    {
        pNew=(Clause*)malloc(sizeof(Clause));
        pNew->first=(Literal*)malloc(sizeof(Literal));
        pNew->first->next=NULL;
        pNew->next=rNew->next;
        rNew->next=pNew;
        pL=pC->first->next;
        while(pL)
        {
            pLNew=(Literal*)malloc(sizeof(Literal));
            pLNew->liter=pL->liter;
            pLNew->next=pNew->first->next;
            pNew->first->next=pLNew;
            pL=pL->next;
        }
        pC=pC->next;
    }
    return rNew;
}

/*-------------------------------------------------
    函数名：destroyList
    功能：删除CNF公式
    输入参数：
        root（Clause*类型） CNF公式头指针；
    返回值：OK/ERROR；
-------------------------------------------------*/
status destroyList(Clause* root)
{
    Clause *p_pre=root,*p_cur=root->next;
    while(p_cur)
    {
        p_pre->next=p_cur->next;
        destroyClause(p_cur);
        p_cur=p_pre->next;
    }
    free(p_pre);
    return OK;
}

/*-------------------------------------------------
    函数名：addliter
    功能：向子句中加入文字
    输入参数：
        p（Clause*类型） 子句头指针；
        lit（int类型）   所需加入的文字；
    返回值：OK/ERROR；
-------------------------------------------------*/
status addliter(Clause* p,int lit)
{
    Literal *pNew;
    pNew=(Literal*)malloc(sizeof(Literal));
    pNew->liter=lit;
    pNew->next=p->first;
    p->first=pNew;
    return OK;
}

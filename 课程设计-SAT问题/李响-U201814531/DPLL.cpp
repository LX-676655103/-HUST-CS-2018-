#include "head.h"

/*-------------------------------------------------
    函数名：find_lit
    功能：非优化情况选词函数
    输入参数：
        num_of_liter（int类型）    文字数；
    返回值：文字数；
-------------------------------------------------*/
int find_lit(int num_of_liter)
{
    int i;
    for(i=1;i<=num_of_liter;i++)
        if(!liter[i]) return i;
    return 0;
}

/*-------------------------------------------------
    函数名：law_of_UnitClause
    功能：非优化情况选词函数
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        p_of_unitCla（Clause*类型）单子句头指针；
    返回值：OK/ERROR；
-------------------------------------------------*/
status law_of_UnitClause(Clause* root,Clause* p_of_unitCla,int &num_of_clause)
{
    int liter;
    Clause *p_of_curCla,*p_of_preCla;
    Literal *p_of_curLiter,*p_of_preLiter;
    p_of_preCla=root;
    p_of_curCla=root->next;
    liter=p_of_unitCla->first->next->liter;
    while(p_of_curCla)
    {
        p_of_preLiter=p_of_curCla->first;
        p_of_curLiter=p_of_preLiter->next;
        while(p_of_curLiter)
        {
            if(p_of_curLiter->liter==liter)
            {
                p_of_preCla->next=p_of_curCla->next;
                destroyClause(p_of_curCla);
                p_of_curCla=p_of_preCla;
                num_of_clause--;
                break;
            }
            else if(p_of_curLiter->liter+liter==0)
            {
                if(!p_of_curLiter->next&&p_of_preLiter==p_of_curCla->first) return ERROR;
                p_of_preLiter->next=p_of_curLiter->next;
                free(p_of_curLiter);
                p_of_curLiter=p_of_preLiter->next;
            }
            else
            {
                p_of_preLiter=p_of_curLiter;
                p_of_curLiter=p_of_curLiter->next;
            }
        }
        p_of_preCla=p_of_curCla;
        p_of_curCla=p_of_curCla->next;
    }
    return OK;
}

/*-------------------------------------------------
    函数名：preSolve
    功能：预处理
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        num_of_liter（int类型）    CNF公式文字数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status preSolve(Clause* root,int num_of_liter,int &num_of_clause)
{
    int i;
    Clause* p=root->next;
    for(i=0;i<=num_of_liter;i++) liter[i]=0;
    while(p)
        if(!p->first->next) return ERROR;
        else p=p->next;
    p=root->next;
    while(p)
    {
        if(isUnitClause(p))
        {
            liter[abs(p->first->next->liter)]=p->first->next->liter;
            if(law_of_UnitClause(root,p,num_of_clause)) p=root->next;
            else return FALSE;
        }
        else p=p->next;
    }
    return OK;
}

/*-------------------------------------------------
    函数名：Literalsum
    功能：J值统计
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_liter（int类型）    CNF公式文字数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status Literalsum(Clause *root,int num_of_liter)
{
    int i;
    Clause *p=root->next;
    Literal *q;
    for(i=0;i<num_of_liter;i++)
        a[i].liter=i+1,a[i].sum=0.0,a[i].num_of_positive=0,a[i].num_of_negative=0;
    while(p)
    {
        i=0;
        q=p->first->next;
        while(q){i++;q=q->next;}
        q=p->first->next;
        while(q)
        {
            a[abs(q->liter)-1].sum+=pow(0.5,i);
            if(q->liter>0) a[abs(q->liter)-1].num_of_positive++;
            else a[abs(q->liter)-1].num_of_negative++;
            q=q->next;
        }
        p=p->next;
    }
    return OK;
}

/*-------------------------------------------------
    函数名：Literalmax
    功能：J值排序
    输入参数：
        num_of_liter（int类型）    CNF公式文字数；
    返回值：J值最大的文字结构体；
-------------------------------------------------*/
Sum_of_Liter Literalmax(int num_of_liter)
{
    int i;
    Sum_of_Liter temp=a[0];
    for(i=1;i<num_of_liter;i++)
        if(temp.sum<a[i].sum)
            temp=a[i];
    return temp;
}

/*-------------------------------------------------
    函数名：Recovery_lit
    功能：恢复文字值
    输入参数：
        p（Clause*类型）           CNF子句头指针；
    返回值：OK/ERROR；
-------------------------------------------------*/
status Recovery_lit(Clause* p)
{
    Literal *pNew;
    pNew=p->first;
    while(pNew)
    {
        liter[abs(pNew->liter)]=0;
        pNew=pNew->next;
    }
    destroyClause(p);
    return OK;
}

/*-------------------------------------------------
    函数名：DPLL
    功能：DPLL函数
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int类型）   CNF公式子句数；
        num_of_liter（int类型）    CNF公式文字数；
    返回值：TRUE/FALSE；
-------------------------------------------------*/
status DPLL(Clause* root,int lit,int num_of_liter,int num_of_clause)
{
    int flag;
    Sum_of_Liter temp;
    Clause *p_of_UnitLiter,*p,*rNew;
    p_of_UnitLiter=(Clause*)malloc(sizeof(Clause));
    p_of_UnitLiter->first=NULL;
    rNew=copyList(root);
    liter[0]=lit;
    addClause(rNew,createClause(1,liter));
    num_of_clause++;
    p=rNew->next;
    while(p)
    if(isUnitClause(p))
    {
        if(law_of_UnitClause(rNew,p,num_of_clause))
        {
            lit=p->first->next->liter;
            addliter(p_of_UnitLiter,lit);
            liter[abs(lit)]=lit;
            p=rNew->next;
        }
        else{destroyList(rNew); Recovery_lit(p_of_UnitLiter); return FALSE;}
    }
    else p=p->next;
    if(Mark)
    {
         Literalsum(rNew,num_of_liter);
         temp=Literalmax(num_of_liter);
         lit=temp.liter;
         if(temp.num_of_positive>=temp.num_of_negative) flag=1;
         else flag=-1;
    }
    else {lit=find_lit(num_of_liter);flag=-1;}
    if(!rNew->next||DPLL(rNew,lit*flag,num_of_liter,num_of_clause)||DPLL(rNew,-lit*flag,num_of_liter,num_of_clause))
    {
        destroyList(rNew);
        return TRUE;
    }
    else{ destroyList(rNew); Recovery_lit(p_of_UnitLiter);  return FALSE; }
}

/*-------------------------------------------------
    函数名：SaveFile
    功能：保存结果文件
    输入参数：
        time（int类型）            程序运行时间
        filename[]（char类型）     文件名；
        flag（int类型）            是否可满足标志；
        num_of_liter（int类型）    CNF公式文字数；
    返回值：TRUE/FALSE；
-------------------------------------------------*/
status SaveFile(int time,char filename[],int flag,int num_of_liter)
{
    FILE *fp;
    int i=0;
    while(filename[i]!='\0') i++;
    filename[i-3]='r',filename[i-2]='e',filename[i-1]='s';
    if(!(fp=fopen(filename,"w"))) return ERROR;
    fprintf(fp,"s %d",flag);
    if(flag)
    {
        fprintf(fp,"\nv ");
        for(int i=1;i<=num_of_liter;i++)
            if(liter[i]==0) fprintf(fp,"%d ",i);
            else fprintf(fp,"%d ",liter[i]);
    }
    fprintf(fp,"\nt %d\n",time);
    fclose(fp);
    return OK;
}

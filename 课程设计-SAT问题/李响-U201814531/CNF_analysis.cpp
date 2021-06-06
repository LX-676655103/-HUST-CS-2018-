#include "head.h"

/*-------------------------------------------------
    ��������ReadFile
    ���ܣ���ȡCNF�ļ������ɿɲ�����CNF��ʽ��
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_liter��int���ͣ�    �����������ô��ݿ��޸ģ�
        num_of_clause��int���ͣ�   �Ӿ��������ô��ݿ��޸ģ�
        filename[]��char���ͣ�     �ļ�����
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status ReadFile(Clause* root,int &num_of_liter,int &num_of_clause,char filename[])
{
    int i,n,num;
    FILE* fp;
    char str[1024],c;
    printf("      ������CNF�ļ����ļ�·��/�ļ�����");
    scanf("%s",filename);
    if(!(fp=fopen(filename,"r"))) return ERROR;  //�ļ���ʧ�ܣ�����ERROR
    printf("      CNF�ļ���ע���������£�\n");
    while(fscanf(fp,"%c",&c)!=EOF) //ѭ������ע�ͣ������ע����Ϣ
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
            printf("\n      CNF�ļ��е�������Ϊ��%-8d ��ʽ��Ϊ��%-8d\n",num_of_liter,num_of_clause);
            break;
        }
    }
    for(i=0;i<num_of_clause;i++) //ѭ�������Ӿ���Ϣ
    {
        n=0;
        while(fscanf(fp,"%d",&num)!=EOF&&num!=0)
            liter[n++]=num;
        addClause(root,createClause(n,liter));  //����������Ӿ�
    }
    for(i=0;i<=num_of_liter;i++) liter[i]=0;    //ˢ�����飬�Թ�����
    fclose(fp);
    return OK;
}

/*-------------------------------------------------
    ��������createClause
    ���ܣ�����CNF��ʽ�Ӿ䣨��ͷ��㣩��
    ���������
        n��int���ͣ�       �Ӿ��б�Ԫ�ĸ�����
        lit[]��int���ͣ�   �洢������Ϣ�����飻
    ����ֵ���Ӿ�ͷָ�룻
-------------------------------------------------*/
Clause* createClause(int n,int lit[])
{
    int i;
    Clause* head;
    Literal* pNew;
    head=(Clause*)malloc(sizeof(Clause));
    head->first=(Literal*)malloc(sizeof(Literal)); //����ͷ���
    head->first->next=NULL;
    head->next=NULL;
    for(i=0;i<n;i++) //���ɱ���
    {
        pNew=(Literal*)malloc(sizeof(Literal));
        pNew->liter=lit[i];
        pNew->next=head->first->next;
        head->first->next=pNew;
    }
    return head;
}

/*-------------------------------------------------
    ��������destroyClause
    ���ܣ�ɾ��CNF��ʽ�Ӿ䣻
    ���������
        p��Clause*���ͣ�  �Ӿ�ͷָ�룻
    ����ֵ��OK/ERROR��
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
    ��������addClause
    ���ܣ�����CNF��ʽ�Ӿ䣻
    ���������
        pNew��Clause*���ͣ�  �Ӿ�ͷָ�룻
        root��Clause*���ͣ�  CNF��ʽͷָ�룻

    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status addClause(Clause* root,Clause* pNew)
{
    pNew->next=root->next;
    root->next=pNew;
    return OK;
}

/*-------------------------------------------------
    ��������isUnitClause
    ���ܣ��ж��Ƿ�Ϊ���Ӿ�
    ���������
        p��Clause*���ͣ�  �Ӿ�ͷָ�룻
    ����ֵ��TRUE/FALSE��
-------------------------------------------------*/
status isUnitClause(Clause* p)
{
    if(!p->first->next) return FALSE;
    if(!p->first->next->next) return TRUE;
    return FALSE;
}

/*-------------------------------------------------
    ��������printClause
    ���ܣ���ӡCNF��ʽ
    ���������
        root��Clause*���ͣ� CNF��ʽͷָ�룻
    ����ֵ��OK/ERROR��
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
    ��������copyList
    ���ܣ�����CNF��ʽ
    ���������
        root��Clause*���ͣ� CNF��ʽͷָ�룻
    ����ֵ�����Ƶ�CNF��ʽ��ͷָ��ֵ��
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
    ��������destroyList
    ���ܣ�ɾ��CNF��ʽ
    ���������
        root��Clause*���ͣ� CNF��ʽͷָ�룻
    ����ֵ��OK/ERROR��
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
    ��������addliter
    ���ܣ����Ӿ��м�������
    ���������
        p��Clause*���ͣ� �Ӿ�ͷָ�룻
        lit��int���ͣ�   �����������֣�
    ����ֵ��OK/ERROR��
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

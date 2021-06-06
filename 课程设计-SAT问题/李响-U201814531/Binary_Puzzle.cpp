#include "head.h"


/*-------------------------------------------------
    ��������trans_of_comliter
    ���ܣ���ͨ������Ԫ��ת��
    ���������
        i��int���ͣ�   ������
        j��int���ͣ�   ������
        n��int���ͣ�   ������
        flag��int���ͣ���־����flag==0��ʾ�������ȣ�flag==1��ʾ�������ȣ�
    ����ֵ��ת����Ĳ�����Ԫֵ��
-------------------------------------------------*/
int trans_of_comliter(int i,int j,int n,int flag)
{
    if(flag==0) return (i-1)*n+j;    //��������
    else return (j-1)*n+i;   //��������
}

/*-------------------------------------------------
    ��������trans_of_extraliter
    ���ܣ����Ӳ�����Ԫ��ת��
    ���������
        a��int���ͣ�
        b��int���ͣ�
        c��int���ͣ�
        d��int���ͣ�
        e��int���ͣ�
        n��int���ͣ�   ������
    ����ֵ��ת����Ĳ�����Ԫֵ��
-------------------------------------------------*/
int trans_of_extraliter(int a,int b,int c,int d,int e,int n)
{
    return n*n+(a-1)*n*(n-1)*n*3/2+((c-1)*(c-2)/2+b-1)*3*n+3*d+e-2;
}

/*-------------------------------------------------
    ��������firstlaw
    ���ܣ�ʹ��Լ��1����CNF��ʽ�Ӿ�
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int&���ͣ�  CNF��ʽ�Ӿ�����
        n��int���ͣ�               ������
        flag��int���ͣ�            ��־λ�����������������ȼ���
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status firstlaw(Clause* root,int &num_of_clause,int n,int flag)
{
    int i,j,k;
    for(i=1;i<=n;i++)
        for(j=1;j<=n-2;j++)
        {
            for(k=0;k<=2;k++)
                liter[k]=trans_of_comliter(i,j+k,n,flag); //��������ֵ�����
            addClause(root,createClause(3,liter));   //���ɲ�����Ӿ�
            for(k=0;k<=2;k++)
                liter[k]=-trans_of_comliter(i,j+k,n,flag); //��Ӹ����ֵ�����
            addClause(root,createClause(3,liter));   //���ɲ�����Ӿ�
            num_of_clause+=2;  //�޸��Ӿ���
        }
    return OK;
}

/*-------------------------------------------------
    ��������secondlaw_of_DEP
    ���ܣ�ʹ��Լ��2����CNF��ʽ�Ӿ䣨�ݹ麯����
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int&���ͣ�  CNF��ʽ�Ӿ�����
        layer��int���ͣ�           �ݹ������
        i��int���ͣ�               ����/������ȡ����flag1��ֵ����
        j��int���ͣ�               ����/������ȡ����flag1��ֵ����
        n��int���ͣ�               ������
        flag1��int���ͣ�           ��־λ�����������������ȼ���
        flag2��int���ͣ�           ��־λ����������������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status secondlaw_of_DEP(Clause* root,int &num_of_clause,int layer,int i,int j,int n,int flag1,int flag2)
{
    int k;
    if(layer-1==n/2+1) //�������ﵽ������һ��+1
    {
        addClause(root,createClause(layer-1,liter)); //���ɲ�����Ӿ�
        num_of_clause++;  //�޸��Ӿ���
        return OK;
    }
    for(k=j+1;k<=n/2+layer-1;k++)
    {
        liter[layer-1]=trans_of_comliter(i,k,n,flag1)*flag2; //������ֵ����飬flag2����������ֵ�����
        secondlaw_of_DEP(root,num_of_clause,layer+1,i,k,n,flag1,flag2); //�ݹ����
    }
    return OK;
}

/*-------------------------------------------------
    ��������secondlaw
    ���ܣ�ʹ��Լ��2����CNF��ʽ�Ӿ䣨�ݹ���ڣ�
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int&���ͣ�  CNF��ʽ�Ӿ�����
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status secondlaw(Clause* root,int &num_of_clause,int n)
{
    int i;
    for(i=1;i<=n;i++)
    {
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,0,1);  //��������+������
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,0,-1); //��������+������
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,1,1);  //��������+������
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,1,-1); //��������+������
    }
    return OK;
}

/*-------------------------------------------------
    ��������thirdlaw
    ���ܣ�ʹ��Լ��3����CNF��ʽ�Ӿ�
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int&���ͣ�  CNF��ʽ�Ӿ�����
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status thirdlaw(Clause* root,int &num_of_clause,int n)
{
    int a,b,c,d,e,flag;
    for(a=1;a<=2;a++)
        for(b=1;b<n;b++)
            for(c=b+1;c<=n;c++)
            {
                for(d=1;d<=n;d++)
                    liter[d-1]=-trans_of_extraliter(a,b,c,d,2,n);
                addClause(root,createClause(n,liter));
                num_of_clause++;
                for(d=1;d<=n;d++)
                {
                    for(e=0;e<=2;e++)
                    {
                        if(e==2)
                        {
                            liter[0]=trans_of_extraliter(a,b,c,d,e,n);
                            liter[1]=-trans_of_extraliter(a,b,c,d,0,n);
                            addClause(root,createClause(2,liter));
                            num_of_clause++;
                            liter[1]=-trans_of_extraliter(a,b,c,d,1,n);
                            addClause(root,createClause(2,liter));
                            num_of_clause++;
                            liter[0]=-trans_of_extraliter(a,b,c,d,e,n);
                            liter[1]=trans_of_extraliter(a,b,c,d,0,n);
                            liter[2]=trans_of_extraliter(a,b,c,d,1,n);
                            addClause(root,createClause(3,liter));
                            num_of_clause++;
                        }
                        else
                        {
                            if(e==0) flag=-1;
                            if(e==1) flag=1;
                            liter[0]=-trans_of_extraliter(a,b,c,d,e,n);
                            liter[1]=trans_of_comliter(b,d,n,a-1)*flag;
                            addClause(root,createClause(2,liter));
                            num_of_clause++;
                            liter[1]=trans_of_comliter(c,d,n,a-1)*flag;
                            addClause(root,createClause(2,liter));
                            num_of_clause++;
                            liter[0]=trans_of_extraliter(a,b,c,d,e,n);
                            liter[1]=trans_of_comliter(b,d,n,a-1)*(-flag);
                            liter[2]=trans_of_comliter(c,d,n,a-1)*(-flag);
                            addClause(root,createClause(3,liter));
                            num_of_clause++;
                        }
                    }
                }
            }
    return OK;
}

/*-------------------------------------------------
    ��������ReadPuzzle_File
    ���ܣ����ļ��ж���������������̸��
    ���������
        puzzle��int*���ͣ�         �洢���̸�֣�
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status ReadPuzzle_File(int *puzzle,int n)
{
    int i,j;
    FILE *fp;
    char str[n+1],filename[101];
    printf("      ������PUZZLE�ļ����ļ�·��/�ļ�����");
    scanf("%s",filename);
    if(!(fp=fopen(filename,"r"))) return ERROR;
    for(i=0;i<n;i++)
    {
        fscanf(fp,"%s",str);
        for(j=0;j<n;j++)
        {
            if(str[j]=='.') *(puzzle+(i*n+j))=-1;
            if(str[j]=='1') *(puzzle+(i*n+j))=1;
            if(str[j]=='0') *(puzzle+(i*n+j))=0;
        }
    }
    return OK;
}

/*-------------------------------------------------
    ��������ReadPuzzle_Buffer
    ���ܣ��ӻ������ж���������������̸��
    ���������
        puzzle��int*���ͣ�         �洢���̸�֣�
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status ReadPuzzle_Buffer(int *puzzle,int n)
{
    int i,j;
    char str[n+1];
    printf("      ������PUZZLE��\n");
    for(i=0;i<n;i++)
    {
        scanf("%s",str);
        for(j=0;j<n;j++)
        {
            if(str[j]=='.') *(puzzle+(i*n+j))=-1;
            if(str[j]=='1') *(puzzle+(i*n+j))=1;
            if(str[j]=='0') *(puzzle+(i*n+j))=0;
        }
    }
    return OK;
}

/*-------------------------------------------------
    ��������transPuzzle_to_CNF
    ���ܣ����������������̸��ת��ΪCNF��ʽ
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int&���ͣ�  CNF��ʽ�Ӿ�����
        n��int���ͣ�               ������
        a��int*���ͣ�              �洢���̸�֣�
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status transPuzzle_to_CNF(Clause* root,int &num_of_clause,int n,int *a)
{
    int i,j,temp,flag;
    for(i=1;i<=n;i++)
    for(j=1;j<=n;j++)
    {
        temp=*(a+((i-1)*n+j-1));
        if(temp==0) flag=-1;
        if(temp==1) flag=1;
        if(temp==-1) continue;
        liter[0]=flag*trans_of_comliter(i,j,n,0);
        addClause(root,createClause(1,liter));
        num_of_clause++;
    }
    firstlaw(root,num_of_clause,n,0);  //Լ��һ����������
    firstlaw(root,num_of_clause,n,1);  //Լ��һ����������
    secondlaw(root,num_of_clause,n);   //Լ����
    thirdlaw(root,num_of_clause,n);    //Լ����
    return OK;
}

/*-------------------------------------------------
    ��������SaveCNFFile
    ���ܣ�����CNF��ʽ
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int���ͣ�   CNF��ʽ�Ӿ�����
        num_of_liter��int���ͣ�    CNF��ʽ��������
        n��int���ͣ�               ������
        puzzle��int*���ͣ�         �洢���̸�֣�
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status SaveCNFFile(Clause* root,int *puzzle,int n,int num_of_liter,int num_of_clause)
{
    FILE *fp;
    int i,j,num=0;
    char filename[101];
    Clause* p=root->next;
    Literal* q;
    printf("      ��������Ҫ������ļ�����");
    scanf("%s",filename);
    if(!(fp=fopen(filename,"w"))) return ERROR;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
        if(*(puzzle+(i*n+j))==-1) num++;
    fprintf(fp,"c BinaryPuzzle, order: %d, holes: %d\nc \n",n,num);
    for(i=0;i<n;i++)
    {
        fprintf(fp,"c ");
        for(j=0;j<n;j++)
        {
            if(*(puzzle+(i*n+j))==-1) fprintf(fp,".");
            else if(*(puzzle+(i*n+j))==1) fprintf(fp,"1");
            else if(*(puzzle+(i*n+j))==0) fprintf(fp,"0");
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"c \np cnf %d %d\n",num_of_liter,num_of_clause);
    while(p)
    {
        q=p->first->next;
        while(q)
        {
            fprintf(fp,"%d ",q->liter);
            q=q->next;
        }
        fprintf(fp,"0\n");
        p=p->next;
    }
    fclose(fp);
    return OK;
}

/*-------------------------------------------------
    ��������puzzleslover
    ���ܣ����������
    ���������
        root��Clause*���ͣ�        CNF��ʽͷָ�룻
        num_of_clause��int���ͣ�   CNF��ʽ�Ӿ�������
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status puzzleslover(Clause* root,int n,int num_of_clause)
{
    int num_of_liter,lit;
    Sum_of_Liter temp;
    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);   //���ݽ������������
    if(!preSolve(root,num_of_liter,num_of_clause)) return ERROR;
    Literalsum(root,num_of_liter);  //ѡ��
    temp=Literalmax(num_of_liter);
    lit=temp.liter;
    if(!root->next||DPLL(root,lit,num_of_liter,num_of_clause)||DPLL(root,-lit,num_of_liter,num_of_clause)) //���
    {
        return OK;
    }
    else return ERROR;
}

/*-------------------------------------------------
    ��������puzzles
    ���ܣ����������
    ���������
        a��int*���ͣ�              �洢���̸�֣�
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status puzzles(int n,int *a)
{
    Clause* root;
    int num_of_clause=0;
    root=(Clause*)malloc(sizeof(Clause));
    root->next=NULL;
    transPuzzle_to_CNF(root,num_of_clause,n,a); //����CNF��ʽ
    if(puzzleslover(root,n,num_of_clause)) return OK; //���������
    else return ERROR;
}

/*-------------------------------------------------
    ��������generate_of_puzzle
    ���ܣ�����������������
    ���������
        c��int*���ͣ�              �洢���̸�֣�
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status generate_of_puzzle(int n,int *c)
{
    int i,j,puzz;
    int puzzle[n][n],*p=puzzle[0],temp[n][n];
    srand(time(NULL));
    while(1)
    {
        if(n==4)  //Ϊ��ֹ�����޷��������̵���������Ľ����̵�������
        {
            for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                puzzle[i][j]=-1;
            *(p+rand()%16*sizeof(int))=1;
            if(puzzles(n,puzzle[0])) break;
            else continue;
        }
        for(i=0;i<n;i++)
        for(j=0;j<n;j++)
        if(rand()%101>8) puzzle[i][j]=-1; //������
        else if(rand()%101>4&&rand()%101<=8)  puzzle[i][j]=0;
        else puzzle[i][j]=1;
        if(puzzles(n,puzzle[0])) break; //�������
    }
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            if(liter[(i-1)*n+j]>=0) puzzle[i-1][j-1]=1,temp[i-1][j-1]=1;
            else puzzle[i-1][j-1]=0,temp[i-1][j-1]=0;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        if(rand()%101>10) //����ڿ�
        {
            puzz=temp[i][j];
            if(temp[i][j]) temp[i][j]=0;
            else temp[i][j]=1;
            if(puzzles(n,temp[0]))
                temp[i][j]=puzz;
            else temp[i][j]=-1;
        }
    }
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
        *(c+(i*n+j))=temp[i][j];
    return OK;
}

/*-------------------------------------------------
    ��������printpuzzle
    ���ܣ���ӡ����
    ���������
        puzzle��int*���ͣ�         �洢���̸�֣�
        n��int���ͣ�               ������
    ����ֵ��OK/ERROR��
-------------------------------------------------*/
status printpuzzle(int *puzzle,int n)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        printf("      ");
        for(j=0;j<n;j++)
            if(*(puzzle+(i*n+j))==-1) printf("_ ");
            else printf("%d ",*(puzzle+(i*n+j)));
        printf("\n");
    }
    return OK;
}

#include "head.h"


/*-------------------------------------------------
    函数名：trans_of_comliter
    功能：普通布尔变元的转化
    输入参数：
        i（int类型）   行数；
        j（int类型）   列数；
        n（int类型）   阶数；
        flag（int类型）标志数，flag==0表示行序优先，flag==1表示列序优先；
    返回值：转化后的布尔变元值；
-------------------------------------------------*/
int trans_of_comliter(int i,int j,int n,int flag)
{
    if(flag==0) return (i-1)*n+j;    //行序优先
    else return (j-1)*n+i;   //列序优先
}

/*-------------------------------------------------
    函数名：trans_of_extraliter
    功能：附加布尔变元的转化
    输入参数：
        a（int类型）
        b（int类型）
        c（int类型）
        d（int类型）
        e（int类型）
        n（int类型）   阶数；
    返回值：转化后的布尔变元值；
-------------------------------------------------*/
int trans_of_extraliter(int a,int b,int c,int d,int e,int n)
{
    return n*n+(a-1)*n*(n-1)*n*3/2+((c-1)*(c-2)/2+b-1)*3*n+3*d+e-2;
}

/*-------------------------------------------------
    函数名：firstlaw
    功能：使用约束1生成CNF公式子句
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        n（int类型）               阶数；
        flag（int类型）            标志位，用于区别行列优先级；
    返回值：OK/ERROR；
-------------------------------------------------*/
status firstlaw(Clause* root,int &num_of_clause,int n,int flag)
{
    int i,j,k;
    for(i=1;i<=n;i++)
        for(j=1;j<=n-2;j++)
        {
            for(k=0;k<=2;k++)
                liter[k]=trans_of_comliter(i,j+k,n,flag); //添加正文字到数组
            addClause(root,createClause(3,liter));   //生成并添加子句
            for(k=0;k<=2;k++)
                liter[k]=-trans_of_comliter(i,j+k,n,flag); //添加负文字到数组
            addClause(root,createClause(3,liter));   //生成并添加子句
            num_of_clause+=2;  //修改子句数
        }
    return OK;
}

/*-------------------------------------------------
    函数名：secondlaw_of_DEP
    功能：使用约束2生成CNF公式子句（递归函数）
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        layer（int类型）           递归层数；
        i（int类型）               行数/列数（取决于flag1的值）；
        j（int类型）               列数/行数（取决于flag1的值）；
        n（int类型）               阶数；
        flag1（int类型）           标志位，用于区别行列优先级；
        flag2（int类型）           标志位，用于区别正负；
    返回值：OK/ERROR；
-------------------------------------------------*/
status secondlaw_of_DEP(Clause* root,int &num_of_clause,int layer,int i,int j,int n,int flag1,int flag2)
{
    int k;
    if(layer-1==n/2+1) //当层数达到阶数的一半+1
    {
        addClause(root,createClause(layer-1,liter)); //生成并添加子句
        num_of_clause++;  //修改子句数
        return OK;
    }
    for(k=j+1;k<=n/2+layer-1;k++)
    {
        liter[layer-1]=trans_of_comliter(i,k,n,flag1)*flag2; //添加文字到数组，flag2决定添加文字的正负
        secondlaw_of_DEP(root,num_of_clause,layer+1,i,k,n,flag1,flag2); //递归调用
    }
    return OK;
}

/*-------------------------------------------------
    函数名：secondlaw
    功能：使用约束2生成CNF公式子句（递归入口）
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status secondlaw(Clause* root,int &num_of_clause,int n)
{
    int i;
    for(i=1;i<=n;i++)
    {
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,0,1);  //行序优先+正文字
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,0,-1); //行序优先+负文字
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,1,1);  //列序优先+正文字
        secondlaw_of_DEP(root,num_of_clause,1,i,0,n,1,-1); //列序优先+负文字
    }
    return OK;
}

/*-------------------------------------------------
    函数名：thirdlaw
    功能：使用约束3生成CNF公式子句
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        n（int类型）               阶数；
    返回值：OK/ERROR；
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
    函数名：ReadPuzzle_File
    功能：从文件中读入二进制数独棋盘格局
    输入参数：
        puzzle（int*类型）         存储棋盘格局；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status ReadPuzzle_File(int *puzzle,int n)
{
    int i,j;
    FILE *fp;
    char str[n+1],filename[101];
    printf("      请输入PUZZLE文件的文件路径/文件名：");
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
    函数名：ReadPuzzle_Buffer
    功能：从缓冲区中读入二进制数独棋盘格局
    输入参数：
        puzzle（int*类型）         存储棋盘格局；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status ReadPuzzle_Buffer(int *puzzle,int n)
{
    int i,j;
    char str[n+1];
    printf("      请输入PUZZLE：\n");
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
    函数名：transPuzzle_to_CNF
    功能：将二进制数独棋盘格局转化为CNF公式
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int&类型）  CNF公式子句数；
        n（int类型）               阶数；
        a（int*类型）              存储棋盘格局；
    返回值：OK/ERROR；
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
    firstlaw(root,num_of_clause,n,0);  //约束一，行序优先
    firstlaw(root,num_of_clause,n,1);  //约束一，列序优先
    secondlaw(root,num_of_clause,n);   //约束二
    thirdlaw(root,num_of_clause,n);    //约束三
    return OK;
}

/*-------------------------------------------------
    函数名：SaveCNFFile
    功能：保存CNF公式
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int类型）   CNF公式子句数；
        num_of_liter（int类型）    CNF公式文字数；
        n（int类型）               阶数；
        puzzle（int*类型）         存储棋盘格局；
    返回值：OK/ERROR；
-------------------------------------------------*/
status SaveCNFFile(Clause* root,int *puzzle,int n,int num_of_liter,int num_of_clause)
{
    FILE *fp;
    int i,j,num=0;
    char filename[101];
    Clause* p=root->next;
    Literal* q;
    printf("      请输入需要保存的文件名：");
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
    函数名：puzzleslover
    功能：棋盘求解器
    输入参数：
        root（Clause*类型）        CNF公式头指针；
        num_of_clause（int类型）   CNF公式子句数；；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status puzzleslover(Clause* root,int n,int num_of_clause)
{
    int num_of_liter,lit;
    Sum_of_Liter temp;
    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);   //根据阶数求出文字数
    if(!preSolve(root,num_of_liter,num_of_clause)) return ERROR;
    Literalsum(root,num_of_liter);  //选词
    temp=Literalmax(num_of_liter);
    lit=temp.liter;
    if(!root->next||DPLL(root,lit,num_of_liter,num_of_clause)||DPLL(root,-lit,num_of_liter,num_of_clause)) //求解
    {
        return OK;
    }
    else return ERROR;
}

/*-------------------------------------------------
    函数名：puzzles
    功能：棋盘求解器
    输入参数：
        a（int*类型）              存储棋盘格局；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status puzzles(int n,int *a)
{
    Clause* root;
    int num_of_clause=0;
    root=(Clause*)malloc(sizeof(Clause));
    root->next=NULL;
    transPuzzle_to_CNF(root,num_of_clause,n,a); //生成CNF公式
    if(puzzleslover(root,n,num_of_clause)) return OK; //调用求解器
    else return ERROR;
}

/*-------------------------------------------------
    函数名：generate_of_puzzle
    功能：二进制棋盘生成器
    输入参数：
        c（int*类型）              存储棋盘格局；
        n（int类型）               阶数；
    返回值：OK/ERROR；
-------------------------------------------------*/
status generate_of_puzzle(int n,int *c)
{
    int i,j,puzz;
    int puzzle[n][n],*p=puzzle[0],temp[n][n];
    srand(time(NULL));
    while(1)
    {
        if(n==4)  //为防止出现无法生成棋盘的情况，对四阶棋盘单独处理
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
        if(rand()%101>8) puzzle[i][j]=-1; //随机填空
        else if(rand()%101>4&&rand()%101<=8)  puzzle[i][j]=0;
        else puzzle[i][j]=1;
        if(puzzles(n,puzzle[0])) break; //调用求解
    }
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            if(liter[(i-1)*n+j]>=0) puzzle[i-1][j-1]=1,temp[i-1][j-1]=1;
            else puzzle[i-1][j-1]=0,temp[i-1][j-1]=0;
    for(i=0;i<n;i++)
    for(j=0;j<n;j++)
    {
        if(rand()%101>10) //随机挖空
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
    函数名：printpuzzle
    功能：打印棋盘
    输入参数：
        puzzle（int*类型）         存储棋盘格局；
        n（int类型）               阶数；
    返回值：OK/ERROR；
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

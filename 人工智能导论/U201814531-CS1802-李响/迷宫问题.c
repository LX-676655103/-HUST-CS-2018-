#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define M 2000
int Pmaze[M][M];
/*open结构链表用以存放开放结点以及关闭结点，xy表示坐标位置；
  price是估值函数的数值，depth是搜索深度，*pfront用以回溯结点*/
struct open{
    int x, y;
    int price,depth;
    struct open *pnext;
    struct open *pfront;
};
/*迷宫行走的方向，分别为行方向和列方向*/
const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};
int scanf_map(int m,int n,int *px,int *py,int *pex,int *pey);
int find_path(int startx,int starty,int endx,int endy,int p,int q);
void fsort(struct open **phead,int n,struct open **pend);
int get_dist(struct open **phead,int m,int n);
int main()
{
    int m,n;
    struct open **phead;
    int startx,starty,*px=&startx,*py=&starty;
    int endx,endy,*pex=&endx,*pey=&endy;
    /*数据注意事项*/
    printf("本算法采用了A*算法求解迷宫路径问题;\n");
    printf("算法采用以“曼哈顿距离”作为启发信息进行启发式搜索;\n");
    printf("搜索过程为广度优先搜索（估值函数值相同时采用深度策略）\n");
    printf("+贪心策略选择开销最小的状态节点进行拓展;\n");
    printf("下面,您应该根据提示输入各种迷宫的信息;\n");
    printf("输入: 行数和列数，例如：5 3\n");
    scanf("%d%d",&m,&n);
    printf("从下一行开始输入迷宫信息，注意\"1\"代表墙壁，\"0\"代表通道，\n");
    printf("\"5\"代表起点，\"6\"代表终点，数字之间请用空格隔开，范例如下：\n");
    printf("5 0 0 0 0 0 0 0 0 0\n1 1 1 1 1 1 1 1 1 0\n1 0 0 0 0 0 0 0 1 0\n");
    printf("1 0 0 1 1 1 1 0 1 0\n1 1 0 0 0 0 1 0 1 0\n");
    printf("1 1 1 1 1 0 1 0 1 0\n6 0 1 0 1 0 1 0 1 0\n");
    printf("1 0 1 0 1 0 1 0 1 0 \n1 0 1 0 1 0 1 0 1 0\n1 0 0 0 0 0 1 0 0 0\n\n");
    scanf_map(m,n,px,py,pex,pey);
    find_path(startx,starty,endx,endy,m,n);
    return 0;
}
/*读入数据函数，用以读入迷宫信息和确定初始结束位置*/
int scanf_map(int m,int n,int *px,int *py,int *pex,int *pey)
{
    int i,j;
    for(i=0;i<m;i++)
      for(j=0;j<n;j++)
      {
        scanf("%d",&Pmaze[i][j]);
        if(Pmaze[i][j]==5) *px=i,*py=j;   /*确定起始和终点位置*/
        else if(Pmaze[i][j]==6) *pex=i,*pey=j;
      }
    return 0;
}
/*核心A*算法*/
int find_path(int startx,int starty,int endx,int endy,int p,int q)
{
    int m,n=0;
    struct open *phead,*pheadopen,*pEnd,*pNew;
    int i=startx,j=starty;
    phead=(struct open *)malloc(sizeof(struct open));
    phead->x=i;
    phead->y=j;
    phead->depth=0;
    phead->pfront=NULL;
    phead->pnext=NULL;
    pheadopen=phead;
    pEnd=phead;
    while(i!=endx||j!=endy)
    {
        for(m=0;m<4;m++)
        if((Pmaze[i+dx[m]][j+dy[m]]==0||Pmaze[i+dx[m]][j+dy[m]]==6)
           &&i+dx[m]>=0&&i+dx[m]<p&&j+dy[m]>=0&&j+dy[m]<q)
        {
            if(Pmaze[i+dx[m]][j+dy[m]]==0) Pmaze[i+dx[m]][j+dy[m]]=2;
            pNew=(struct open *)malloc(sizeof(struct open));
            pNew->x=i+dx[m];
            pNew->y=j+dy[m];
            pNew->pfront=pheadopen;
            pNew->price=abs(endx-pNew->x)+abs(endy-pNew->y);
            pNew->price=pNew->pfront->depth+pNew->price;
            pNew->depth=pNew->pfront->depth+1;
            pEnd->pnext=pNew;
            pNew->pnext=NULL;
            pNew->pfront=pheadopen;
            pEnd=pEnd->pnext;
            n++;
        }
        if(pheadopen->pnext==NULL)
        {
            printf("抱歉，无法到达出口!\n");
            return 0;
        }
        fsort(&pheadopen->pnext,n,&pEnd);
        i=pheadopen->pnext->x;
        j=pheadopen->pnext->y;
        n--;
        pheadopen=pheadopen->pnext;
    }
    get_dist(&pheadopen,p,q);
}
/*链表排序算法*/
void fsort(struct open **phead,int n,struct open **pend)
{
    int i,j;
    struct open *pNew=*phead,*pFront=*phead;
    for(i=0;i<n-1;i++)
    for(pNew=pFront=*phead,j=0;j<n-i-1;j++,pFront=pNew,pNew=pNew->pnext)
        if(pNew->price > pNew->pnext->price)
        {
            if(pNew==*phead)
            {
                *phead=pNew->pnext;
                pNew->pnext=pNew->pnext->pnext;
                (*phead)->pnext=pNew;
                pNew=*phead;
            }
            else
            {
                pFront->pnext=pNew->pnext;
                pNew->pnext=pFront->pnext->pnext;
                pFront->pnext->pnext=pNew;
                pNew=pFront->pnext;
            }
        }
        pNew=*phead;
        while(pNew)
        {
            *pend=pNew;
            pNew=pNew->pnext;
        }
}
/*路径打印算法*/
int get_dist(struct open **phead,int m,int n)
{
    int i=0,j;
    struct open *pNew=*phead;
    while(pNew->pfront)
    {
        i++;
        if(Pmaze[pNew->x][pNew->y]==2)Pmaze[pNew->x][pNew->y]=-6;
        printf("(%d,%d)<-",pNew->x+1,pNew->y+1);
        if(i%8==0) printf("\n");
        pNew=pNew->pfront;
    }
    printf("(%d,%d)\n\n",pNew->x+1,pNew->y+1);
    printf("本次寻路成功，所走步数为：%d \n\n",i);
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
            printf("%c ",Pmaze[i][j]+48);
        printf("\n");
    }
    return 0;
}

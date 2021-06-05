#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define M 2000
int Pmaze[M][M];
/*open�ṹ�������Դ�ſ��Ž���Լ��رս�㣬xy��ʾ����λ�ã�
  price�ǹ�ֵ��������ֵ��depth��������ȣ�*pfront���Ի��ݽ��*/
struct open{
    int x, y;
    int price,depth;
    struct open *pnext;
    struct open *pfront;
};
/*�Թ����ߵķ��򣬷ֱ�Ϊ�з�����з���*/
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
    /*����ע������*/
    printf("���㷨������A*�㷨����Թ�·������;\n");
    printf("�㷨�����ԡ������پ��롱��Ϊ������Ϣ��������ʽ����;\n");
    printf("��������Ϊ���������������ֵ����ֵ��ͬʱ������Ȳ��ԣ�\n");
    printf("+̰�Ĳ���ѡ������С��״̬�ڵ������չ;\n");
    printf("����,��Ӧ�ø�����ʾ��������Թ�����Ϣ;\n");
    printf("����: ���������������磺5 3\n");
    scanf("%d%d",&m,&n);
    printf("����һ�п�ʼ�����Թ���Ϣ��ע��\"1\"����ǽ�ڣ�\"0\"����ͨ����\n");
    printf("\"5\"������㣬\"6\"�����յ㣬����֮�����ÿո�������������£�\n");
    printf("5 0 0 0 0 0 0 0 0 0\n1 1 1 1 1 1 1 1 1 0\n1 0 0 0 0 0 0 0 1 0\n");
    printf("1 0 0 1 1 1 1 0 1 0\n1 1 0 0 0 0 1 0 1 0\n");
    printf("1 1 1 1 1 0 1 0 1 0\n6 0 1 0 1 0 1 0 1 0\n");
    printf("1 0 1 0 1 0 1 0 1 0 \n1 0 1 0 1 0 1 0 1 0\n1 0 0 0 0 0 1 0 0 0\n\n");
    scanf_map(m,n,px,py,pex,pey);
    find_path(startx,starty,endx,endy,m,n);
    return 0;
}
/*�������ݺ��������Զ����Թ���Ϣ��ȷ����ʼ����λ��*/
int scanf_map(int m,int n,int *px,int *py,int *pex,int *pey)
{
    int i,j;
    for(i=0;i<m;i++)
      for(j=0;j<n;j++)
      {
        scanf("%d",&Pmaze[i][j]);
        if(Pmaze[i][j]==5) *px=i,*py=j;   /*ȷ����ʼ���յ�λ��*/
        else if(Pmaze[i][j]==6) *pex=i,*pey=j;
      }
    return 0;
}
/*����A*�㷨*/
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
            printf("��Ǹ���޷��������!\n");
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
/*���������㷨*/
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
/*·����ӡ�㷨*/
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
    printf("����Ѱ·�ɹ������߲���Ϊ��%d \n\n",i);
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
            printf("%c ",Pmaze[i][j]+48);
        printf("\n");
    }
    return 0;
}

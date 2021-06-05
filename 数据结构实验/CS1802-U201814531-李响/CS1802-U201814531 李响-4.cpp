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
#define MAX_VERTEX_NUM 100
#define LIST_INIT_SIZE 2000
/*--------------------------------------------*/
typedef int status;
typedef int Sign;  //结点关键字元素类型定义
typedef int ElemType; //数据元素类型定义
int visited[MAX_VERTEX_NUM]; //访问数组，用于记录结点是否访问
/*--------------------------------------------*/
typedef struct InfoType{
    int weight; //权重（本次实验不采用权重）
}InfoType; //弧结点中，弧数据结构体
/*--------------------------------------------*/
typedef struct VRType{
    Sign Node_num1; //结点1的关键字
    Sign Node_num2; //结点2的关键字
    InfoType data;  //弧数据
}VRType;   //弧信息结构体，用于读入和存储弧信息
/*--------------------------------------------*/
typedef struct VertexType{
    Sign Node_num; //关键字
    ElemType data; //数据项
}VertexType;  //结点信息结构体，用于读入和存储结点信息
/*--------------------------------------------*/
typedef struct ArcNode{
    int adjvex;      //所指向结点的位序
    struct ArcNode *nextarc; //后继指针
    InfoType *info;  //弧信息项
}ArcNode;  //邻接表的表结点结构体
/*--------------------------------------------*/
typedef struct VNode{
    VertexType data;   //结点数据，其中包含关键字与数据项
    ArcNode *firstarc; //表头指针
}VNode,AdList[MAX_VERTEX_NUM]; //表头结点结构体
/*--------------------------------------------*/
typedef struct{
    AdList vertices;   //直接创建一个邻接表
    int vexnum,arcnum; //结点数目以及弧数目
    int kind;    //记录图的属性，1表示有向图，2表示无向图
    char name[30];     //图的名称
}ALGraph;  //多图管理结构体
/*--------------------------------------------*/
status CreateGraph(ALGraph &G,VertexType V[],VRType VR[]);
status DestroyGraph(ALGraph &G);
int LocateVex(ALGraph G,Sign u);
status PutVex(ALGraph &G,Sign u,ElemType value);
int FirstAdjVex(ALGraph G,int v);
int NextAdjVex(ALGraph G,int v,int w);
status InsertVex(ALGraph &G,VertexType v);
status DeleteVex(ALGraph &G,int v);
status InsertArc(ALGraph &G,int v,int w);
status DeleteArc(ALGraph &G,int v,int w);
status DFSTraverse(ALGraph &G);
status DFS(ALGraph &G,int u);
status BFSTraverse(ALGraph &G);
status Assign_name(ALGraph &G,ALGraph a[],int num);  //命名函数，可以查找是否重名
status OpenFile(ALGraph &G,VertexType V[],VRType VR[]); //文件赋值函数
status SaveFile(ALGraph G);  //文件保存函数
/*--------------------------------------------*/
int main()
{
    int op=1,i,j,v,w,k;
    int num=0,locate;
    Sign u;
    char name[30];
    VertexType V[MAX_VERTEX_NUM],vNode; //结点信息数组
    VRType VR[LIST_INIT_SIZE]; //弧信息数组
    ALGraph GraList[N];  //多图管理数组
    while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Graph On  Adjacency List \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. CreateGraph     8. DeleteVex\n");
	printf("    	  2. DestroyGraph    9. InsertArc \n");
	printf("    	  3. LocateVex       10. DeleteArc\n");
	printf("    	  4. PutVex          11. DFSTraverse\n");
	printf("    	  5. FirstAdjVex     12. BFSTraverse\n");
	printf("    	  6. NextAdjVex      13. SaveFile\n");     //文件保存
	printf("    	  7. InsertVex       14. GraphMessage\n"); //显示所有图的信息
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("      请选择你的操作[0~14]:");
	scanf("%d",&op);
	if(op>=2&&op<=13)  //通过图的名称进行操作
    {
        if(num)  //当前有图存在
        {
            printf("      请输入您要操作的图的名称："); //通过名称查找图
            scanf("%s",name);
            for(j=1;j<=num;j++)  //遍历查找
                if(strcmp(GraList[j].name,name)==0) break;
            if(j>num) {printf("      您查找的图不存在，请重新操作：\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      当前无图存在，请先创建图！\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
        int value;
        int n,i;
        printf("      请选择：1.进行键盘赋值\n              2.读取文件\n      ");
        scanf("%d",&k);
	    printf("      请输入需要创建的图的类型：\n");
	    printf("          1.有向图     2.无向图  \n      ");   //本程序可以实现有向图和无向图两种功能
        //printf("          3.有向网     4.无向网 \n      ");  //本次实验不实现网
	    scanf("%d",&GraList[++num].kind);
	    if(GraList[num].kind<1||GraList[num].kind>2)  //非法判断1——图的类型非法
        {
            printf("      输入非法，请重新进行操作！\n");
            break;
        }
        if(k==2) //文件赋值
        {
            if(OpenFile(GraList[num],V,VR)==ERROR)
                num--;
        }
        else
        {
            printf("      请输入图的顶点数目（请输入大于0，小于%d的整数）：",MAX_VERTEX_NUM);
	        scanf("%d",&n);
	        while(n<=0||n>=MAX_VERTEX_NUM)   //非法判断2——输入结点数非法，MAX_VERTEX_NUM为100
	        {
	            printf("      输入非法，请重新输入: ");
                scanf("%d",&n);
	        }
            GraList[num].vexnum=n;
	        printf("      请输入合法的顶点信息，每个顶点数据由关键字-数据两项组成，\n");
            printf("      中间用空格隔开，两项之间用换行符隔开，关键字为唯一确定的正整数，\n");
            printf("      数据为任意整数，请输入%d个顶点的信息：\n",n);
	        for(i=0;i<n;i++) scanf("%d %d",&V[i].Node_num,&V[i].data);
            printf("      顶点输入成功！\n");
            printf("      请输入图中弧的数目（若为无向图，请勿重复输入同一条边）：");
            scanf("%d",&GraList[num].arcnum);
            if(GraList[num].kind>=3)
            {
                printf("      请输入合法的弧边信息，每个弧边数据由顶点1关键字-顶点2关键字-权重，共三项组成，\n");
                printf("      中间用空格隔开，两项之间用换行符隔开，关键字为唯一确定的正整数\n");
                printf("      （请确保关键字存在）权重为任意正整数，请输入%d条弧边的信息：\n",GraList[num].arcnum);
                for(i=0;i<GraList[num].arcnum;i++) scanf("%d %d %d",&VR[i].Node_num1,&VR[i].Node_num2,&VR[i].data);
            }
            else
            {
                printf("      请输入合法的弧边信息，每个弧边数据由顶点1关键字-顶点2关键字，共两项组成，\n");
                printf("      中间用空格隔开，两项之间用换行符隔开，关键字为唯一确定的正整数，请输入%d条弧边的信息：\n",GraList[num].arcnum);
                for(i=0;i<GraList[num].arcnum;i++) scanf("%d %d",&VR[i].Node_num1,&VR[i].Node_num2);
            }
        }
        CreateGraph(GraList[num],V,VR);   //创建图
        printf("      图创建成功！请对图进行命名：");
        while(!Assign_name(GraList[num],GraList,num))     //采用命名函数来进行赋值，函数有判断重复的功能
            printf("      该图名称已存在，请重新输入："); //非法判断3——图的名称重复
        printf("      图命名成功！\n");
        getchar();getchar();
        break;
	   case 2:
         if(DestroyGraph(GraList[j])==OK)
		 {
		     printf("      图销毁成功！\n");
             for(;j<num;j++)  //多图管理，需要将销毁的图删除覆盖
                 GraList[j]=GraList[j+1];
             num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
         printf("      请输入您想查找的顶点的关键字（正整数）：");
         scanf("%d",&u);
         locate=LocateVex(GraList[j],u);  //locate变量存储结点的位序
         if(locate!=-1) printf("      操作成功，您查找的顶点位序为%d,数据项为%d\n",locate,GraList[j].vertices[locate].data.data);
         else printf("      您所查找的顶点不存在，操作失败！\n");
		 getchar();getchar();
		 break;
	   case 4:
         printf("      请输入您想赋值的顶点的关键字（正整数）及数据项：");
         scanf("%d %d",&u,&value);
         if(PutVex(GraList[j],u,value)) printf("      顶点赋值成功！\n"); //若赋值操作正常进行，e的赋值会发生改变（详见函数）
         else printf("      您所赋值的顶点不存在，操作失败！\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      请输入您想获取第一邻接点的顶点的位序（位序为大于等于0，小于%d的整数）：",GraList[j].vexnum);
         scanf("%d",&v);
         while(v<0||v>=GraList[j].vexnum)  //结点位序非法
         {
             printf("      位序信息非法，请重新输入：");
             scanf("%d",&v);
         }
         locate=FirstAdjVex(GraList[j],v); //locate变量存储结点的位序
         if(locate!=-1) printf("      操作成功，第一邻接点的位序为%d,数据项为%d\n",locate,GraList[j].vertices[locate].data.data);
         else printf("      第一邻接点不存在，操作失败！\n");
         getchar();getchar();
		 break;
	   case 6:
         printf("      请输入表头顶点的位序（位序为大于等于0，小于%d的整数）：",GraList[j].vexnum);
         scanf("%d",&v);
         while(v<0||v>=GraList[j].vexnum) //结点位序非法
         {
             printf("      位序信息非法，请重新输入：");
             scanf("%d",&v);
         }
         printf("      请输入想获取下一个邻接点的顶点的位序（位序为大于等于0，小于%d的整数）：",GraList[j].vexnum);
         scanf("%d",&w);
         while(w<0||w>=GraList[j].vexnum) //结点位序非法
         {
             printf("      位序信息非法，请重新输入：");
             scanf("%d",&w);
         }
         locate=NextAdjVex(GraList[j],v,w); //locate变量存储结点的位序
         if(locate>=0) printf("      操作成功，下一个邻接点的位序为%d,数据项为%d\n",locate,GraList[j].vertices[locate].data.data);
         else if(locate==-1) printf("      下一邻接点不存在，操作失败！\n");     //失败情况1
         else if(locate==-2) printf("      位序为%d的顶点不含有位序为%d的邻接点，操作失败！\n",v,w);  //失败情况2
		 getchar();getchar();
		 break;
	   case 7:
         ElemType a; Sign b;
         printf("      请输入插入顶点信息，信息有关键字-数据两项组成，中间用空格隔开：");
         while(scanf("%d %d",&b,&a)!=EOF&&LocateVex(GraList[j],b)!=-1)
              printf("      该关键字已使用，请重新输入关键字与数据项:");  //判断当前关键字是否合法
         vNode.Node_num=b; //生成结点数据项-关键字
         vNode.data=a;     //生成结点数据项-数据项
         if(InsertVex(GraList[j],vNode)) printf("      顶点插入成功！\n");
         else printf("      顶点数目超过上限，操作失败！\n");
		 getchar();getchar();
		 break;
	   case 8:
         printf("      请输入您想删除的顶点的关键字（正整数）：");
         scanf("%d",&u);
         locate=LocateVex(GraList[j],u); //locate变量存储结点的位序
         if(locate==-1)   //判断需要删除的结点是否存在
         {
             printf("      需要删除的顶点不存在，操作失败！\n");
             getchar();getchar();
             break;
         }
         if(DeleteVex(GraList[j],locate)) printf("      结点删除成功！\n");
		 getchar();getchar();
         break;
	   case 9:
         int locate1,locate2;  //locate1和locate2变量存储结点的位序
         printf("      请输入插入弧的两个顶点关键字，中间用空格隔开：");
         scanf("%d %d",&a,&b);
         while(LocateVex(GraList[j],a)==-1||LocateVex(GraList[j],b)==-1)  //判断需要结点关键字是否存在
         {
             printf("      顶点不存在，请重新输入两个顶点关键字：");
             scanf("%d %d",&a,&b);
         }
         locate1=LocateVex(GraList[j],a);
         locate2=LocateVex(GraList[j],b);
         if(GraList[j].kind==1) //有向图只需要插入一条边
         {
             if(InsertArc(GraList[j],locate1,locate2)) printf("      弧插入成功！\n");
             else printf("      弧边已存在，请勿重复插入！\n");
         }
         if(GraList[j].kind==2) //无向图需要插入两条边
         {
             if(InsertArc(GraList[j],locate1,locate2)&&InsertArc(GraList[j],locate2,locate1))
                {printf("      弧插入成功！\n"); GraList[j].arcnum--;}
             else printf("      弧边已存在，请勿重复插入！\n");
         }
         getchar();getchar();
         break;
	   case 10:
         printf("      请输入删除弧的两个顶点关键字，中间用空格隔开：");
         scanf("%d %d",&a,&b);
         while(LocateVex(GraList[j],a)==-1||LocateVex(GraList[j],b)==-1)
         {
             printf("      顶点不存在，请重新输入两个顶点关键字：");
             scanf("%d %d",&a,&b);
         }
         locate1=LocateVex(GraList[j],a); //locate1和locate2变量存储结点的位序
         locate2=LocateVex(GraList[j],b); //locate1和locate2变量存储结点的位序
         if(GraList[j].kind==1) //有向图只需要删除一条边
         {
             if(DeleteArc(GraList[j],locate1,locate2))
                printf("      弧删除成功！\n");
             else printf("      弧边不存在，删除失败！\n");
         }
         if(GraList[j].kind==2) //无向图需要删除两条边
         {
             if(DeleteArc(GraList[j],locate1,locate2)&&DeleteArc(GraList[j],locate2,locate1))
                {printf("      弧删除成功！\n");GraList[j].arcnum++;}
             else printf("      弧边不存在，删除失败！\n");
         }
         getchar();getchar();
         break;
	   case 11:
         printf("\n----------------all elements --------------------\n");
         DFSTraverse(GraList[j]);
         printf("\n------------------ end --------------------------\n");
		 getchar();getchar();
		 break;
       case 12:
         printf("\n----------------all elements --------------------\n");
         BFSTraverse(GraList[j]);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 13:
         SaveFile(GraList[j]);
         getchar();getchar();
         break;
       case 14:
         if(!num) {printf("      当先无图存在，请进行其他操作：\n"); getchar();getchar(); break;}
         printf("      当前图的信息如下：\n");
         printf("      图的个数为%d\n",num);
         printf("      图的名称、类型、顶点数以及弧边数如下：\n");
         for(j=1;j<=num;j++)    //遍历输出数据
         if(GraList[j].kind==1) printf("      %20s    有向图    %d    %d\n",GraList[j].name,GraList[j].vexnum,GraList[j].arcnum);
         else if(GraList[j].kind==2) printf("      %20s    无向图    %d    %d\n",GraList[j].name,GraList[j].vexnum,GraList[j].arcnum);
         getchar();getchar();
         break;
	   case 0:
         break;
	}
  }
printf("\n欢迎下次再使用本系统！\n");
}
/*--------------------------------------------*/
status CreateGraph(ALGraph &G,VertexType V[],VRType VR[]) //创建图函数
{

    int i,j;
    ArcNode *pNew1,*pNew2;  //pNew1指针由于指向新生成的表结点，pNew2用于无向图的生成
    for(i=0;i<G.vexnum;i++) //将结点先存储在邻接表中
    {
        G.vertices[i].data=V[i];
        G.vertices[i].firstarc=NULL;  //指针指向空地址，防止指针悬挂
    }
    for(i=0;i<G.arcnum;i++)
    {
        pNew1=(ArcNode*)malloc(sizeof(ArcNode));  //使用malloc函数生成表结点
        if(G.kind==2) pNew2=(ArcNode*)malloc(sizeof(ArcNode)); //使用malloc函数生成表结点
        for(j=0;j<G.vexnum;j++) //遍历结点数组，插入表结点
        {
            if(VR[i].Node_num1==V[j].Node_num) //当前结点为弧的起始点
            {
                pNew1->nextarc=G.vertices[j].firstarc;
                G.vertices[j].firstarc=pNew1;  //使用首插法将结点插入邻接表
            }
            if(VR[i].Node_num2==V[j].Node_num) //当前结点为弧的终点
                pNew1->adjvex=j;  //记录位序
            if(G.kind==2) //当该图为无向图时，需要同时生成两条弧
            {
                if(VR[i].Node_num2==V[j].Node_num) //当前结点为弧的起始点
                {
                    pNew2->nextarc=G.vertices[j].firstarc;
                    G.vertices[j].firstarc=pNew2;  //使用首插法将结点插入邻接表
                }
                if(VR[i].Node_num1==V[j].Node_num) //当前结点为弧的终点
                    pNew2->adjvex=j; //记录位序
            }
        }
    }
    return OK;
}
/*--------------------------------------------*/
status DestroyGraph(ALGraph &G) //删除图函数
{
    int i;
	ArcNode *p,*temp;
	for(i=0;i<G.vexnum;i++)  //遍历数组，释放所有指针值
    {
        p=G.vertices[i].firstarc;
        while(p)  //遍历释放空间
        {
            temp=p;
            p=p->nextarc;
            free(temp);
        }
        G.vertices[i].firstarc=NULL;
        G.vertices[i].data.data=0;
        G.vertices[i].data.Node_num=-1;
    }
	G.vexnum=0;
	G.arcnum=0;
	G.kind=-1;
	return OK;
}
/*--------------------------------------------*/
int LocateVex(ALGraph G,Sign u) //定位结点函数
{
    int i;
	for(i=0;i<G.vexnum;i++)  //遍历定位结点
		if(G.vertices[i].data.Node_num==u)
            return i;
	return -1;
}
/*--------------------------------------------*/
status PutVex(ALGraph &G,Sign u,ElemType value) //赋值函数
{
    int i;
	for(i=0;i<G.vexnum;i++)  //遍历定位结点
    if(G.vertices[i].data.Node_num==u)
    {
        G.vertices[i].data.data=value; //赋值
        return OK;
    }
	return ERROR;
}
/*--------------------------------------------*/
int FirstAdjVex(ALGraph G,int v) //获得第一邻接点函数
{
    if(G.vertices[v].firstarc)   //第一邻接点存在
        return G.vertices[v].firstarc->adjvex; //获得第一邻接点
	else return -1;
}
/*--------------------------------------------*/
int NextAdjVex(ALGraph G,int v,int w) //获得下一邻接点函数
{
    int i;
    ArcNode *p=G.vertices[v].firstarc;
    while(p)
    {
        if(p->adjvex==w) //当前邻接点存在
        {
            if(p->nextarc) return p->nextarc->adjvex; //下一邻接点不存在
            else  return -1;
        }
        p=p->nextarc;
    }
    return -2; //当前邻接点不存在
}
/*--------------------------------------------*/
status InsertVex(ALGraph &G,VertexType v) //插入结点函数
{
    if(G.vexnum==MAX_VERTEX_NUM)
        return ERROR;
    G.vertices[G.vexnum].data=v;
    G.vertices[G.vexnum].firstarc=NULL;
    G.vexnum++;  //结点个数增加
    return OK;
}
/*--------------------------------------------*/
status DeleteVex(ALGraph &G,int v)  //删除结点函数
{
    int i;
    ArcNode *p,*temp;
    p=G.vertices[v].firstarc;
    while(p)  //遍历删除该结点的所有弧
    {
        temp=p->nextarc;
        G.arcnum--; //弧的数目减少
        free(p);
        p=temp;
    }
    for(i=v;i<G.vexnum;i++) //所有后方结点向前移动
        G.vertices[i]=G.vertices[i+1];
    G.vexnum--;   //结点数减一
    for(i=0;i<G.vexnum;i++) //修改所有表结点的位序，凡是位序大于v的减一，等于v的删除
    {
        p=G.vertices[i].firstarc;
        if(!p) continue;   //当前结点没有表结点（没有弧从改点出发），直接进行下一个循环
        if(p->adjvex==v)   //第一个表结点指向位序为v的结点，删除当前结点
        {
            G.vertices[i].firstarc=p->nextarc;
            G.arcnum--;  //弧的数目减一
            free(p);
        }
        else if(p->adjvex>v) //第一个表结点指向的结点位序大于v，v减一
            p->adjvex--;
        temp=p->nextarc;
        while(temp) //遍历表结点
        {
            if(temp->adjvex==v) //第一个表结点指向位序为v的结点，删除当前结点
            {
                p->nextarc=temp->nextarc;
                free(temp);
                G.arcnum--; //弧的数目减一
                temp=p->nextarc;
                continue;
            }
            else if(temp->adjvex>v) //第一个表结点指向的结点位序大于v，v减一
                temp->adjvex--;
            p=temp;
            temp=temp->nextarc;
        }
    }
    return OK;
}
/*--------------------------------------------*/
status InsertArc(ALGraph &G,int v,int w) //插入弧边函数
{
    ArcNode *p=G.vertices[v].firstarc,*temp;
    while(p)  //遍历查找当前需要生成的边是否存在
    {
        if(p->adjvex==w) return ERROR; //当前需要生成的弧存在，返回ERROR
        p=p->nextarc;
    }
    p=G.vertices[v].firstarc;
    temp=(ArcNode*)malloc(sizeof(ArcNode));
    temp->adjvex=w;
    temp->nextarc=p;  //使用首插法插入弧
    G.vertices[v].firstarc=temp;
    G.arcnum++;
    return OK;
}
/*--------------------------------------------*/
status DeleteArc(ALGraph &G,int v,int w)
{
    ArcNode *p=G.vertices[v].firstarc,*temp;
    if(!p) return ERROR;  //当前结点没有弧存在，返回ERROR
    if(p->adjvex==w) //第一个表结点为需要删除的结点
    {
        G.vertices[v].firstarc=p->nextarc;
        free(p);
        G.arcnum--;
        return OK;
    }
    p=G.vertices[v].firstarc;
    temp=p->nextarc;
    while(temp)  //遍历查找
    {
        if(temp->adjvex==w) //当前表结点为需要删除的结点
        {
            p->nextarc=temp->nextarc;
            free(temp);
            G.arcnum--;
            return OK;
        }
        p=p->nextarc;
        temp=p->nextarc;
    }
    return ERROR; //当前需要删除的弧不存在，返回ERROR
}
/*--------------------------------------------*/
status DFSTraverse(ALGraph &G) //深度优先遍历函数
{
    int i;
	for(i=0;i<G.vexnum;i++) visited[i]=FALSE; //重置访问数组
	for(i=0;i<G.vexnum;i++)
		if(visited[i]==FALSE) //当前结点未输出
            DFS(G,i); //调用深度遍历函数
	return OK;
}
status DFS(ALGraph &G,int u)  //递归深度优先遍历函数
{
    int i;
    ArcNode *p=G.vertices[u].firstarc;
    visited[u]=TRUE;  //输出当前结点，访问数组当前结点位置标记为TRUE
    printf("%d ",G.vertices[u].data.Node_num);
    while(p) //遍历当前结点的弧边
    {
        i=p->adjvex;
        if(visited[i]==FALSE) //当前结点未输出
            DFS(G,i); //调用深度遍历函数
        p=p->nextarc;
    }
    return OK;
}
/*--------------------------------------------*/
status BFSTraverse(ALGraph &G) //广度优先遍历函数
{
    AdList Q;  //生成一个结点队列
    ArcNode *p;
    int i,front=0,rear=0;  //使用队列控制，front指向对头，rear指向队尾
	for(i=0;i<G.vexnum;i++) visited[i]=FALSE; //重置访问数组
    for(i=0;i<G.vexnum;i++)
    {
        if(visited[i]==FALSE) //当前结点未输出
        {
            visited[i]=TRUE; //输出当前结点，访问数组当前结点位置标记为TRUE
            Q[rear++]=G.vertices[i]; //结点入队
            while(front<rear) //队非空
            {
                printf("%d ",Q[front].data.Node_num);
                p=Q[front].firstarc;
                while(p) //将当前结点的弧指向的未访问的结点全部入队
                {
                    if(visited[p->adjvex]==FALSE)
                    {
                        Q[rear++]=G.vertices[p->adjvex];
                        visited[p->adjvex]=TRUE;
                    }
                    p=p->nextarc;
                }
                front++; //结点出队
            }
        }
    }
    return OK;
}
/*--------------------------------------------*/
status Assign_name(ALGraph &G,ALGraph a[],int num) //命名函数
{
    int i;
    char name[30];
    scanf("%s",name);
    for(i=1;i<num;i++)  //名称查重
        if(strcmp(a[i].name,name)==0) break;
    if(i>=num)
    {
        strcpy(G.name,name);
        return OK;
    }
    else return ERROR;
}
/*--------------------------------------------*/
status OpenFile(ALGraph &G,VertexType V[],VRType VR[]) //文件赋值函数
{
    FILE* fp;
    char filename[30];
    int i;
    printf("      请输入文件名：");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) {printf("      打开文件失败!\n"); return ERROR;} //文件赋值，打开文件
    fscanf(fp,"%d",&G.vexnum);
    for(i=0;i<G.vexnum;i++)
        fscanf(fp,"%d %d",&V[i].Node_num,&V[i].data);
    fscanf(fp,"%d",&G.arcnum);
    if(G.kind>=3)
        for(i=0;i<G.arcnum;i++)
            fscanf(fp,"%d %d %d",&VR[i].Node_num1,&VR[i].Node_num2,&VR[i].data);
    else
        for(i=0;i<G.arcnum;i++)
            fscanf(fp,"%d %d",&VR[i].Node_num1,&VR[i].Node_num2);
    printf("      打开文件成功!\n");
    fclose(fp); //关闭文件
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SaveFile(ALGraph G)
{
    FILE* fp;
    int num;
    int i=0;
    ArcNode *p;
    char filename[30];
    printf("      请选择：1.不删除原有的二叉树，仅对文件进行追加\n              2.删除原有数据或生成新文件\n\n");
    scanf("%d",&num);
    printf("      请输入文件名：");
    scanf("%s",filename);
    if(num==1&&(fp=fopen(filename,"a"))==NULL)   //文件打开方式为追加，不对原有的数据进行修改
    {
        printf("      文件文件打开失败！\n");
        return ERROR;
    }
    else if(num==2&&(fp=fopen(filename,"w"))==NULL) //文件打开方式为更新或创建新文件，会删除原有数据
    {
        printf("      文件文件打开失败！\n");
        return ERROR;
    }
    if(G.kind==1) fprintf(fp,"有向图\n");
    if(G.kind==2) fprintf(fp,"无向图\n");
    fputs(G.name,fp);
    fprintf(fp,"\n%d\n",G.vexnum);
    for(i=0;i<G.vexnum;i++)
        fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[i].data.data);
    fprintf(fp,"%d\n",G.arcnum);
    for(i=0;i<G.vexnum;i++)
    {
        p=G.vertices[i].firstarc;
        while(p)
        {
            if(G.kind==1)
                fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[p->adjvex].data.Node_num);
            if(G.kind==2&&p->adjvex>=i)
                fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[p->adjvex].data.Node_num);
            p=p->nextarc;
        }
    }
    fprintf(fp,"\n\n");
    fclose(fp);//关闭文件
    printf("      保存成功！\n");
    return OK;
}
/*--------------------------------------------*/






